#include "first_pass.h"
#include "common.h"
#include "first_pass_commands.h"
#include "data_inst_legality.h"
#include "struct_inst_legality.h"
#include "string_inst_legality.h"
#include "instructions.h"
#include "modifiers.h"
#include "commands.h"
#include "registers.h"
#include "pass_1_errors.h"

/** passes over input file line-by-line and adds elements to memory image and symbol table based on legality checks.
 * also updates addresses of elemnets in symbol table at the end;
 * @param fd file descriptor - set to read an input file
 * @param am_file string of command line input file with '.am' extension
 * @param code_image code image that stores binary lines representing elements of command type line
 * @param data_img data image that stores binary lines representing elements of data instructions type line
 * @param head pointer to pointer to first item in symbol table
 * @return TRUE if succeeds without errors. FALSE otherwise.
 */
Boolean FirstPass(FILE *fd, char *am_file, Image *code_img, Image *data_img, node **head)
{

    Boolean Success = TRUE; /* equals TRUE if no errors found in file . FALSE otherwise */

    /* hasLabel indicates whether line contains a label that stands all tests .
     */
    Boolean hasLabel;

    /* label_legal_result - indicates lagelity of label definition.
        analyze_result - holds required increment for memory image counters.
        sentence_result - holds index of instruction/command in corresponding  array , -1 otherwise.
        line_number - holds current line number.
        ic - holds current number of total binary lines in code image
        dc- holds current number of total binary lines in data image
      */
    int label_legal_result, analyze_result, sentence_result, line_number = 0, ic = 0, dc = 0;
    Label label;

    char line[LINE_LENGTH + 1]; /* stores an input line from a file */
    char *token;                /* stores sliced part of the input line when strtok is used*/

    code_img->size = 0; /*  initiallizing  memory images of both code and data */
    data_img->size = 0;
    code_img->img = malloc(sizeof(char *));
    if (code_img->img == NULL) /*NULL CHECKS*/
    {
        printf("memory allocation failed\n");
        exit(-1);
    }
    data_img->img = malloc(sizeof(char *));
    if (data_img->img == NULL) /*NULL CHECKS*/
    {
        printf("memory allocation failed\n");
        exit(-1);
    }

    while (fgets(line, LINE_LENGTH + 1, fd)) /*  iterate .am file line by line and put string of input line in variable 'line' */
    {
        hasLabel = FALSE;                 /*  set hasLABEL to false at beggining of line reading*/
        line_number++;                    /* update line number */
        line[strcspn(line, "\n")] = '\0'; /*  null terminate  */

        if (is_empty(line)) /*  check whether line is empty - if so, analyze next input line */
            continue;
        if (is_comment(line)) /*  check whether first non-white character is ';' (semicolon) - if so, analyze next input line */
            continue;
        token = strtok(line, " \t\n"); /* store first literal of line in token */
        if (!token)
            continue;

        token = cut_whitespace(token); /* cut off all whitespaces from edges of token */

        if (token[strlen(token) - 1] == ':') /*  perform checks for potential label definition */
        {
            token[strlen(token) - 1] = '\0'; /* replace colon by null terminator  */
            label_legal_result = is_label_legal(token, head);

            if (label_legal_result < 0) /*  in case of error , print error to stdout */
            {
                Success = FALSE;
                printf("error in file  %s  at line %d : %s\n", am_file, line_number, first_pass_label_errors[(label_legal_result * -1) - 1]);
                continue;
            }
            else
            {                         /* in case label is appropriate and stands all tests above */
                hasLabel = TRUE;      /* change  hasLabel indicator to TRUE */
                strcpy(label, token); /* storing label literal in label varaible  */

                token = strtok(NULL, " \t\n"); /* storing SECOND literal of line in token */
                if (!token || is_empty(token)) /* in case part after label is null or empty  */
                {
                    Success = FALSE;
                    printf("error in file:  %s  at line %d : %s\n", am_file, line_number, first_pass_label_errors[(EMPTY_DEFINITION_FOR_LABEL * -1) - 1]);
                    continue;
                }
                token = cut_whitespace(token);
                if (is_modifier(token) >= 0) /* in case part after label definition is .entry or .extern sentence  */
                {
                    Success = FALSE;
                    printf("error in file  %s  at line %d : %s\n", am_file, line_number, first_pass_label_errors[(ENTRY_OR_EXTERN_SENTENCE_CANNOT_BE_SAVED_TO_LABEL * -1) - 1]);
                    continue;
                }
            }
        } /**  end of label checks **/

        sentence_result = is_instruction(token); /*sentence_result will hold index of instruction in instructions array if there is a match, -1 otherwise   */
        if (sentence_result >= 0)                /*  in case token is a valid instruction name  */
        {

            token = strtok(NULL, ""); /*  store rest of line in token */
            if (sentence_result == 0) /*  case of .data instruction */
            {
                analyze_result = LegalityDataInst(token, data_img);

                if (analyze_result < 0) /* case of error returned  */
                {
                    Success = FALSE;
                    printf("error in file   %s  at line %d : %s\n", am_file, line_number, first_pass_data_errors[(analyze_result * -1) - 1]);
                    continue;
                }
                else /* analysis of line succeeded */
                {
                    if (hasLabel)
                    {
                        add_to_Symbol_table(head, label, DATA, dc, FALSE); /* adding label containing .data definition to symbol table  */
                    }
                    dc += analyze_result; /* update dc according to increment due to line analysis  */
                }
            }

            else if (sentence_result == 1) /*  case of .struct instruction */
            {

                analyze_result = LegalityStructInst(token, data_img);

                if (analyze_result < 0) /* case of error returned  */
                {
                    Success = FALSE;
                    printf("error in file  %s  at line %d : %s\n", am_file, line_number, first_pass_struct_errors[(analyze_result * -1) - 1]);
                    continue;
                }
                else /* analysis of line succeeded */
                {
                    if (hasLabel)
                    {
                        add_to_Symbol_table(head, label, DATA, dc, TRUE); /* adding label containing .struct definition to symbol table  */
                    }

                    dc += analyze_result; /* update dc according to increment due to line analysis  */
                }
            }
            else /*  case of .string instruction */
            {
                analyze_result = LegalityStringInst(token, data_img);
                if (analyze_result < 0) /* case of error returned  */
                {
                    Success = FALSE;
                    printf("error in file  %s  at line %d : %s\n", am_file, line_number, first_pass_string_errors[(analyze_result * -1) - 1]);
                    continue;
                }
                else /* analysis of line succeeded */
                {
                    if (hasLabel)
                    {
                        add_to_Symbol_table(head, label, DATA, dc, FALSE); /* adding label containing .string definition to symbol table  */
                    }
                    dc += analyze_result; /* update dc according to increment due to line analysis  */
                }
            }
        }
        else /* check whether token is a COMMAND or a MODIFIER ( .entry/ .extern ) */
        {
            sentence_result = is_command(token); /*sentence_result will hold index of command in commands array if there is a match, -1 otherwise   */
            if (sentence_result >= 0)
            {
                token = strtok(NULL, ""); /*  store rest of line in token */

                if ((sentence_result >= 0 && sentence_result <= 3) || sentence_result == 6) /* case of command with 2 operands */
                {
                    analyze_result = firstPass_Handle_Group2(token, sentence_result, code_img);

                    if (analyze_result < 0) /* case of error returned  */
                    {
                        Success = FALSE;
                        printf("error in file  %s  at line %d : %s\n", am_file, line_number, first_pass_command_errors[(analyze_result * -1) - 1]);
                        continue;
                    }
                    else /* analysis of line succeeded */
                    {
                        if (hasLabel)
                        {
                            add_to_Symbol_table(head, label, CODE, ic, FALSE); /* adding label containing command to symbol table  */
                        }
                        ic += analyze_result; /* update ic according to increment due to line analysis  */
                    }
                }
                else if ((sentence_result >= 4 && sentence_result <= 5) || (sentence_result >= 7 && sentence_result <= 13)) /* case of commands with 1 operand */
                {
                    analyze_result = firstPass_Handle_Group1(token, sentence_result, code_img);
                    if (analyze_result < 0) /* case of error returned  */
                    {
                        Success = FALSE;
                        printf("error in file  %s   at line %d : %s\n", am_file, line_number, first_pass_command_errors[(analyze_result * -1) - 1]);
                        continue;
                    }
                    else
                    {
                        if (hasLabel)
                        {
                            add_to_Symbol_table(head, label, CODE, ic, FALSE); /* adding label containing command to symbol table  */
                        }
                        ic += analyze_result; /* update ic according to increment due to line analysis  */
                    }
                }
                else /* case of commands without operands   */
                {
                    analyze_result = firstPass_Handle_Group0(token, sentence_result, code_img);
                    if (analyze_result < 0) /* case of error returned  */
                    {
                        Success = FALSE;
                        printf("error in file  %s  at line %d : %s\n", am_file, line_number, first_pass_command_errors[(analyze_result * -1) - 1]);
                        continue;
                    }
                    else /* analysis of line succeeded */
                    {
                        if (hasLabel)
                        {
                            add_to_Symbol_table(head, label, CODE, ic, FALSE); /* adding label containing command to symbol table  */
                        }
                        ic += analyze_result; /* update ic according to increment due to line analysis  */
                    }
                }
            }
            else if (is_modifier(token) >= 0) /* if token equals '.entry' or '.extern'  */
            {
                sentence_result = is_modifier(token);
                token = strtok(NULL, ""); /* get rest of line */
                if (!token || is_empty(token))
                {
                    Success = FALSE;
                    printf("error in file  %s  at line %d : %s\n", am_file, line_number, first_pass_label_errors[(ILLEGAL_EMPTY_INPUT_ * -1) - 1]);
                    continue;
                }

                token = cut_whitespace(token);
                label_legal_result = is_label_legal(token, head);

                /*  Throw error and set success to FALSE for all illegality BESIDES entry already exists */
                if ((label_legal_result < 0 && sentence_result == 1) || (label_legal_result < LABEL_ALREADY_EXISTS && sentence_result == 0))
                {
                    Success = FALSE;
                    printf("error in file  %s  at line %d : %s\n", am_file, line_number, first_pass_label_errors[(label_legal_result * -1) - 1]);
                    continue;
                }
                else
                {
                    if (sentence_result == 1) /* add only extern varaible (entry varaible will be added in second pass) */
                        add_to_Symbol_table(head, token, EXTERNAL, 0, FALSE);
                }
            }
            else /* in case of illegal literal command name or instruction name or modifier name */
            {
                Success = FALSE;
                printf("error in file %s  at line %d : %s %s\n", am_file, line_number, hasLabel ? "second" : "first", first_pass_label_errors[(LITERAL_IS_NEITHER_COMMAND_NOR_INSTRUCTION * -1) - 1]);
                continue;
            }
        }
    } /*  end of while loop*/

    if (Success)
    {

        updateLabelsValues(head, ic); /* update values of symbols in symbol table */
    }

    return Success;
}

/** Checks whether length is between 1 and 30, whether initial characher is alphabetic
 *  and whether rest of characters are alphanumeric.
 * @param str - potential label to be verified.
 * @return TRUE if label is valid. Else , returns a negative number representing the error
 */
int is_valid_lable(char *str)
{
    int i;
    for (i = 0; i < strlen(str); i++)
    {
        if (i == 0 && !isalpha(str[i]))
            return LABEL_STARTS_WITH_NON_ALPHABET; /*error - illegal first letter of label */
        else if (!isalnum(str[i]))
            return ILLEGAL_CHARACTERS_IN_LABEL; /*error - non numeric or alphabetic label charchter */
    }
    if (i > MAX_LABEL_LENGTH)
        return LABEL_TOO_LONG_; /*error - lable length exceeds limit */
    else if (i == 0)
        return EMPTY_LABEL; /*error - lable length is 0 */

    return TRUE;
}

/** Checks whether label is a reserved word -  may it be register name , command name instruction name or modifier (entry/extern).
 * @param str - potential label to be verified.
 * @return TRUE if label is a reserved word. False Otherwise.
 */
Boolean is_reserved_word(char *str)
{
    int i;
    for (i = 0; i < NUM_OF_COMMANDS; i++) /*   iterating over commands list */
    {
        if (strcmp(str, commands[i]) == 0)
            return TRUE;
    }
    for (i = 0; i < NUM_OF_REGISTERS; i++)
    {
        if (strcmp(str, registers[i]) == 0)
            return TRUE;
    }
    for (i = 0; i < NUM_OF_INSTRUCTIONS; i++)
    {
        if (strcmp(str, instructions[i] + 1) == 0)
            return TRUE;
    }
    for (i = 0; i < NUM_OF_MODIFIERS; i++)
    {
        if (strcmp(str, modifiers[i] + 1) == 0)
            return TRUE;
    }
    return FALSE;
}
/** Checks whether string variable str is an instruction name .
 * @param str - potential string to be checked.
 * @return index of instruction in instructions array. -1  Otherwise.
 */
int is_instruction(char *str)
{
    int i;
    for (i = 0; i < NUM_OF_INSTRUCTIONS; i++) /*   iterating over instructions list */
    {
        if (strcmp(str, instructions[i]) == 0)
            return i; /* return index of instruction in instructions list */
    }
    return -1;
}

/** Checks whether string varaible str is an modifier name (.entry/.extern) .
 * @param str - potential string to be checked.
 * @return index of modifier in modifiers array. -1  Otherwise.
 */
int is_modifier(char *str)
{
    int i;
    for (i = 0; i < NUM_OF_MODIFIERS; i++) /*   iterating over modifiers list */
    {
        if (strcmp(str, modifiers[i]) == 0)
            return i; /* return index of modifier in modifiers list */
    }
    return -1;
}
/** Checks whether string varaible token is legal label .
 * Contains calls to multiple functions.
 * @param token - potential string to be checked.
 *  @param head - pointer to pointer to Symbol table - Needed for call to is_label_already_exists() .
 * @return True if label is legal. Else , returns a negative number representing the error.
 */
int is_label_legal(char *token, node **head)
{
    int valid_label;
    valid_label = is_valid_lable(token);

    if (valid_label < 0)
        return valid_label;
    else if (is_label_already_exists(*head, token))
    {
        return LABEL_ALREADY_EXISTS;
    }
    else if (is_reserved_word(token))
    {
        return LABEL_IS_A_RESERVED_WORD;
    }
    return TRUE;
}
