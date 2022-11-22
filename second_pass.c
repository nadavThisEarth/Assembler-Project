#include "second_pass.h"
#include "common.h"
#include "pass_2_errors.h"

/**
 *  analyses input file line by line. updates entry type symbols in symbol table. adds symbols in command type sentences to code image.
 * @param fd file descriptor set to read mode of command line input file
 * @param am_file string of command line input file with '.am' extension
 * @param code_img code image that stores binary lines elicited due to analysis of command type sentences
 * @param head pointer to pointer to first item of symbol table
 * @return TRUE if succeeds without errors. FALSE otherwise.
 */
Boolean SecondPass(FILE *fd, char *am_file, Image *code_img, node **head)
{

    Boolean success = TRUE; /* equals TRUE if no errors found in file . FALSE otherwise */

    int analyze_result, sentence_result, line_number = 0, ic = 0;
    char line[LINE_LENGTH + 1]; /* stores an input line from a file */
    char *token;                /* stores sliced part of the input line when strtok is used*/

    while (fgets(line, LINE_LENGTH + 1, fd)) /* iterate over '.am' file line by line and store current line in variable 'line' */
    {
        line_number++;
        line[strcspn(line, "\n")] = '\0'; /* NULL terminate input string */

        if (is_empty(line)) /*  check whether line is empty - if so, analyze next input line */
            continue;

        if (is_comment(line)) /*  check whether first non-white character is ';' (semicolon) - if so, analyze next input line */
            continue;

        token = strtok(line, " \t\n"); /* store first literal of line in token */
        if (!token)
            continue;

        token = cut_whitespace(token); /* cut off all whitespaces from edges of token */

        if (strcmp(token, ".entry") == 0) /* if first literal of input string is '.entry'   */
        {
            token = strtok(NULL, ""); /*  get rest of string*/
            token = cut_whitespace(token);

            sentence_result = updateEntry(head, token); /*  attempt updating all matching symbols to entry */

            if (sentence_result < 0) /* if error occurs , print a message to stdout and set success to False */
            {

                success = FALSE;
                printf("error in file  %s   at line %d : %s\n", am_file, line_number, pass_2_errors[(sentence_result * -1) - 1]);
                continue;
            }
        }
        else
        {
            if (strcmp(token, ".extern") == 0) /* if first literal of input string is '.extern' , skip to next line in file  */
                continue;

            if (token[strlen(token) - 1] == ':')
            { /*  skip label defintion (it's already been handled in pass 1) */
                token = strtok(NULL, " \t\n");
                token = cut_whitespace(token);
            }

            sentence_result = is_command(token);
            if (sentence_result < 0) /*  if literal is not a command name , skip to to next input line */
                continue;
            else
            {
                token = strtok(NULL, ""); /*  store rest of line in token */

                if ((sentence_result >= 0 && sentence_result <= 3) || sentence_result == 6) /* case of command with 2 operands */
                {
                    analyze_result = second_Pass_Handle_Group2(token, code_img, head, ic);
                    if (analyze_result < 0) /* case of error returned  */
                    {
                        success = FALSE;
                        printf("error in file  %s  at line %d : %s\n", am_file, line_number, pass_2_errors[(analyze_result * -1) - 1]);
                        continue;
                    }
                    else
                    {
                        ic += analyze_result;
                    }
                }
                else if ((sentence_result >= 4 && sentence_result <= 5) || (sentence_result >= 7 && sentence_result <= 13)) /* case of commands with 1 operand */
                {
                    analyze_result = second_Pass_Handle_Group1(token, code_img, head, ic);
                    if (analyze_result < 0) /* case of error returned  */
                    {
                        success = FALSE;
                        printf("error in file  %s  at line %d : %s\n", am_file, line_number, pass_2_errors[(analyze_result * -1) - 1]);
                        continue;
                    }
                    else
                    {
                        ic += analyze_result;
                    }
                }
                else /* case of commands without operands   */
                {
                    ic += 1; /*easy case for hlt and rts  */
                }
            }
        }
    } /*  end of while loop */
    return success;
}

/** analyzes command sentences of commands with 2 operands. updates lines in dode image for symbols.
 * @param str string after command name to be analyzed
 * @param code_img code image
 * @param head pointer to pointer to first symbol in symbol table
 * @param current_ic current ic (before call to function)
 * @return ic increment due to analysis if succeeds without errors. else, negative number representing error is returned.
 * */
int second_Pass_Handle_Group2(char *str, Image *code_img, node **head, int current_ic)
{
    /*mention address - address in code image of mention of a symbol*/
    int first_operand_addressing, second_operand_addressing, ic_increment = 1, exists_symbol, first_available_index, mention_address;
    char *first_operand, *second_operand, *struct_name;
    bin_str word; /* binary word variable  */

    first_operand = strtok(str, ",");              /*  getting first operand   */
    second_operand = strtok(NULL, "");             /*   getting second operand */
    first_operand = cut_whitespace(first_operand); /*trim leading and trailing whitespaces */
    second_operand = cut_whitespace(second_operand);

    first_operand_addressing = second_pass_get_addressing(first_operand);
    second_operand_addressing = second_pass_get_addressing(second_operand);

    if (first_operand_addressing == REGISTER && second_operand_addressing == REGISTER) /*case of both operands are registers*/
    {
        ic_increment += 1;
        return ic_increment;
    }
    else
    {
        if (first_operand_addressing == IMMEDIATE || first_operand_addressing == REGISTER) /* case where first operand is immediate or register (but second operand isn't register)*/
        {
            ic_increment += 1;
        }
        else if (first_operand_addressing == DIRECT) /*  handling case of first operand addressing being direct*/
        {
            first_available_index = ic_increment + current_ic;            /*first_available_index equals address of first line in code image to be updated  */
            exists_symbol = is_exists_Symbol(first_operand, FALSE, head); /*  setting exists_symbol to value of symbol in symbol table*/
            if (exists_symbol < 0)                                        /* if symbol does not exist in symbol table, return error */
            {
                return OPERAND_SYMBOL_DOES_NOT_EXIST_IN_SYMBOL_TABLE;
            }
            else /* if symbol exists in symbol table*/
            {

                strcpy(word, decimalToBinary(exists_symbol << 2)); /*  store binary representation of 8 MSB bits of symbol value in variable word */
                if (exists_symbol == 0)                            /* case of extern - set ARE bits to 01 */
                {
                    word[8] = '0';
                    word[9] = '1';

                    mention_address = IC_OFFSET + first_available_index; /*  set mention address to number of line in code image where symbol is mentioned  */

                    add_to_Symbol_table(head, first_operand, EXTERNAL, mention_address, FALSE);
                }
                else /* case of non extern - set ARE bits to 10 */
                {
                    word[8] = '1';
                    word[9] = '0';
                }

                replaceString(code_img, first_available_index, word); /*  replacing existing dummy string in code image with appropriate word representing symbol */

                ic_increment += 1;
            }
        }
        else /*case of struct field access*/
        {
            first_available_index = ic_increment + current_ic; /*first_available_index equals address of first line in code image to be updated  */
            struct_name = strtok(first_operand, ".");          /* get struct name  */
            exists_symbol = is_exists_Symbol(struct_name, TRUE, head);
            if (exists_symbol < 0)
            {
                return OPERAND_STRUCT_NAME_DOES_NOT_EXIST_AS_STRUCT_IN_SYMBOL_TABLE;
            }
            else /* if symbol exists , handle updates  to symbol table and code image  */
            {
                strcpy(word, decimalToBinary(exists_symbol << 2)); /*  store binary representation of 8 MSB bits of symbol value in variable word */
                if (exists_symbol == 0)                            /* case of extern - set ARE bits to 01 */
                {
                    word[8] = '0';
                    word[9] = '1';

                    mention_address = IC_OFFSET + first_available_index; /*  set mention address to number of line in code image where symbol is mentioned  */

                    add_to_Symbol_table(head, struct_name, EXTERNAL, mention_address, FALSE);
                }
                else /* case of NON extern - set ARE bits to 10 */
                {
                    word[8] = '1';
                    word[9] = '0';
                }

                replaceString(code_img, first_available_index, word);

                ic_increment += 2; /*due to field number already inserted in first pass*/
            }
        }
        /*now handle second operand */
        if (second_operand_addressing == IMMEDIATE || second_operand_addressing == REGISTER) /* case where second operand is immediate or register*/
        {
            ic_increment += 1;
        }
        else if (second_operand_addressing == DIRECT)
        {
            first_available_index = ic_increment + current_ic; /*first_available_index equals address of first line in code image to be updated  */
            exists_symbol = is_exists_Symbol(second_operand, FALSE, head);
            if (exists_symbol < 0) /* if symbol does not exists in symbol table - return error */
            {
                return OPERAND_SYMBOL_DOES_NOT_EXIST_IN_SYMBOL_TABLE;
            }
            else
            {

                strcpy(word, decimalToBinary(exists_symbol << 2)); /*  store binary representation of 8 MSB bits of symbol value in variable word */
                if (exists_symbol == 0)                            /* case of extern - set ARE bits to 01 */
                {
                    word[8] = '0';
                    word[9] = '1';

                    mention_address = IC_OFFSET + first_available_index; /*  set mention address to number of line in code image where symbol is mentioned  */

                    add_to_Symbol_table(head, second_operand, EXTERNAL, mention_address, FALSE);
                }
                else /* case of NON extern - set ARE bits to 10 */
                {
                    word[8] = '1';
                    word[9] = '0';
                }

                replaceString(code_img, first_available_index, word);

                ic_increment += 1;
            }
        }
        else /*case of struct field access*/
        {
            first_available_index = ic_increment + current_ic; /*first_available_index equals address of first line in code image to be updated  */
            struct_name = strtok(second_operand, ".");         /*get struct name*/
            exists_symbol = is_exists_Symbol(struct_name, TRUE, head);
            if (exists_symbol < 0)
            {
                return OPERAND_STRUCT_NAME_DOES_NOT_EXIST_AS_STRUCT_IN_SYMBOL_TABLE;
            }
            else
            {
                strcpy(word, decimalToBinary(exists_symbol << 2)); /*  store binary representation of 8 MSB bits of symbol value in variable word */
                if (exists_symbol == 0)                            /* case of extern - set ARE bits to 01 */
                {
                    word[8] = '0';
                    word[9] = '1';

                    mention_address = IC_OFFSET + first_available_index; /*  set mention address to number of line in code image where symbol is mentioned  */

                    add_to_Symbol_table(head, struct_name, EXTERNAL, mention_address, FALSE);
                }
                else /* case of NON extern - set ARE bits to 10 */
                {
                    word[8] = '1';
                    word[9] = '0';
                }

                replaceString(code_img, first_available_index, word);

                ic_increment += 2; /*due to field number already inserted in first pass*/
            }
        }
    }
    return ic_increment;
}
/** analyzes command sentences of commands with 1 operand. updates lines in dode image for symbols.
 * @param operand string of operand after command name to be analyzed
 * @param code_img code image
 * @param head pointer to pointer to first symbol in symbol table
 * @param current_ic current ic (before call to function)
 * @return ic increment due to analysis if succeeds without errors. else, negative number representing error is returned.
 * */
int second_Pass_Handle_Group1(char *operand, Image *code_img, node **head, int current_ic)
{
    int operand_addressing, ic_increment = 1, exists_symbol, first_available_index, mention_address;
    char *struct_name;
    bin_str word;

    operand = cut_whitespace(operand); /*trim leading and trailing whitespaces */

    operand_addressing = second_pass_get_addressing(operand);

    if (operand_addressing == IMMEDIATE || operand_addressing == REGISTER)
    {
        ic_increment += 1;
    }
    else if (operand_addressing == DIRECT)
    {
        first_available_index = ic_increment + current_ic;
        exists_symbol = is_exists_Symbol(operand, FALSE, head);
        if (exists_symbol < 0)
        {
            return OPERAND_SYMBOL_DOES_NOT_EXIST_IN_SYMBOL_TABLE;
        }
        else
        {

            strcpy(word, decimalToBinary(exists_symbol << 2)); /*  store binary representation of 8 MSB bits of symbol value in variable word */
            if (exists_symbol == 0)                            /* case of extern - set ARE bits to 01 */
            {
                word[8] = '0';
                word[9] = '1';

                mention_address = IC_OFFSET + first_available_index; /*  set mention address to number of line in code image where symbol is mentioned  */

                add_to_Symbol_table(head, operand, EXTERNAL, mention_address, FALSE);
            }
            else /* case of NON extern - set ARE bits to 10 */
            {
                word[8] = '1';
                word[9] = '0';
            }

            replaceString(code_img, first_available_index, word);

            ic_increment += 1;
        }
    }
    else /*case of struct field access*/
    {
        first_available_index = ic_increment + current_ic;
        struct_name = strtok(operand, ".");
        exists_symbol = is_exists_Symbol(struct_name, TRUE, head);
        if (exists_symbol < 0)
        {
            return OPERAND_STRUCT_NAME_DOES_NOT_EXIST_AS_STRUCT_IN_SYMBOL_TABLE;
        }
        else
        {
            strcpy(word, decimalToBinary(exists_symbol << 2)); /*  store binary representation of 8 MSB bits of symbol value in variable word */
            if (exists_symbol == 0)                            /* case of extern - set ARE bits to 01 */
            {
                word[8] = '0';
                word[9] = '1';

                mention_address = IC_OFFSET + first_available_index; /*  set mention address to number of line in code image where symbol is mentioned  */

                add_to_Symbol_table(head, struct_name, EXTERNAL, mention_address, FALSE);
            }
            else /* case of NON extern - set ARE bits to 10 */
            {
                word[8] = '1';
                word[9] = '0';
            }

            replaceString(code_img, first_available_index, word);

            ic_increment += 2; /*due to field number already inserted in first pass*/
        }
    }
    return ic_increment;
}

/**
 * @param str operand to be checked
 * @return integer representing addressing of operand
 * */
int second_pass_get_addressing(char *str)
{
    if (str[0] == '#')
        return IMMEDIATE;
    else if (is_register(str))
        return REGISTER;
    else if (strrchr(str, '.'))
        return STRUCT;
    else
        return DIRECT;
}
