
#include "common.h"
#include "commands.h"    /* for is_command*/
#include "registers.h"   /* for is_register  */

/**
 * trimms off all whitespace characters at edges of input string.
 * @param str input string to be trimmed off of all whitespace characters at edges.
 * @return copy of input string, with whitespaces trimmed off from edges.
 * returns empty string for either empty string or all-white string.
 * */
char *cut_whitespace(char *str) 
{
    char *last;

    /* Cut off leading space  (i.e - from the left) */
    while (isspace((unsigned char)*str))
        str++;

    if (*str == 0) /* in case of spaces-only string */
        return str;

    /* Cut off trailing spaces (i.e - from the right) */
    last = str + strlen(str) - 1;
    while (last > str && isspace((unsigned char)*last))
        last--;

    /* null terminate */
    last[1] = '\0';

    return str;
}
/**
 *@param s character to checkd whether represents a decimal integer
 *@return TRUE if input string represents an integer. False otherwise.
 */
Boolean
isInteger(char *s)
{

    int i; /*   counter for loop*/

    if (strlen(s) == 0) /* return FALSE  for empty string */
        return FALSE;
    for (i = 0; i < strlen(s); i++)
    {
        if (s[i] == '+' || s[i] == '-') /*  in case the current charachter is '+' or '-' sign */
        {
            if (i != 0 || strlen(s) == 1) /* return false in case the string consists only a sign or sign charachter is not at beggining */
                return FALSE;
        }
        else if (!isdigit(s[i])) /* in case character is not adifit , return FALSE */
            return FALSE;
    }
    return TRUE;
}
/**
 * checks whether input string consists only of whitespace characters
 * @param line - input string to be checked
 * @return - TRUE if string consists only of whitespace characters.
 *  FALSE otherwise.
 * */
Boolean is_empty(char *line)
{
    int i;
    for (i = 0; i < strlen(line); i++)
    {
        if (!isspace((unsigned char)line[i])) /*  in case  character is not a whitespace - return FALSE*/
            return FALSE;
    }
    return TRUE;
}
/**
 * checks whether input string is a comment line .
 * @param line - input string to be checked
 * @return - TRUE if string's first non-whitespace character is a semicolon(;), which signifies comment .
 *  FALSE otherwise.
 * */
Boolean is_comment(char *line)
{
    int i;
    for (i = 0; i < strlen(line); i++)
    {
        if (isspace((unsigned char)line[i])) /*  in case  character is not a whitespace - move on to next character*/
            continue;
        else if (line[i] == ';') /*  in case  character is a colon - return TRUE*/
            return TRUE;
        else /*  in case  character is not a whitespace nor colon - return FALSE*/
            return FALSE;
    }
    return FALSE;
}
/** Checks whether string varaible str is a command name .
 * @param str - potential string to be checked.
 * @return index of command in commands array. -1  Otherwise.
 */
int is_command(char *str)
{
    int i;
    for (i = 0; i < NUM_OF_COMMANDS; i++) /*   iterating over commands list */
    {
        if (strcmp(str, commands[i]) == 0)
            return i; /* return index of command in commands list */
    }
    return -1;
}
/** Checks whether string varaible s is a command name .
 * @param s - potential string to be checked.
 * @return TRUE if equals to a register name in in registers array. FALSE  Otherwise.
 */
Boolean
is_register(char *s)
{
    int i;
    for (i = 0; i < NUM_OF_REGISTERS; i++)
    {

        if (strcmp(s, registers[i]) == 0)
            return TRUE;
    }
    return FALSE;
}
/** Converts decimal long integer to a binary string in 2's complement.
 * @param num - decimal integer to be converted.
 * @return  string which represents input number in 2's complement.
 */
char *decimalToBinary(long int num)
{
    int i;
    long int right_shifted;             /*  holds right bitwise shifts of original number */
    static char bin[BINARY_LENGTH + 1]; /* will eventually hold the binary representation in 2's complement of input number */

    for (i = 0; i < BINARY_LENGTH; i++)
    {
        right_shifted = num >> i; /* right shift*/
        if (right_shifted & 1)    /*  if current LSB after shifts is 1 , store '1' at matching index from the right */
            bin[BINARY_LENGTH - 1 - i] = '1';
        else /* else store '0'  */
            bin[BINARY_LENGTH - 1 - i] = '0';
    }
    bin[BINARY_LENGTH] = '\0'; /* null terminate the returned string  */
    return bin;
}
/**  @param num number to be calculated
 * @param is_size_counter TRUE if num represents an image size (due to example given in brochure , image size shouldn't be left padded with zeros)
 *  @return string representation in 32 base of input number
 */
char *decimal_to_32(int num, Boolean is_size_counter)
{
    char *str, *re_str;
    char alphabet[] = "!@#$%^&*<>abcdefghijklmnopqrstuv";
    int rem, i = 0;
    if (num < 32) /* to allow left padded ! sign (zero) in case of input number smaller than 32*/
    {
        if (!is_size_counter) /* if input num is not a size counter, then at least two 32 based digits are supplied  */
            str = malloc(3 * sizeof(char));
        else /* if input num is a size counter, only one  32 based digits is  supplied  */
            str = malloc(2 * sizeof(char));
        if (!str)
        {
            printf("memory allocation error\n");
            exit(-1);
        }
        if (!is_size_counter) /* left pad with '0 'in 32 base which is '!' */
        {
            str[i] = '!';
            i++;
        }

        str[i] = alphabet[num];
        i++;
        str[i] = '\0';
        return str;
    }
    str = malloc(sizeof(char));
    if (!str)
    {
        printf("memory allocation error\n");
        exit(-1);
    }
    strcpy(str, "");
    while (num != 0)
    {
        rem = num % 32;
        re_str = realloc(str, strlen(str) + 2);
        if (!re_str)
        {
            printf("memory allocation error\n");
            exit(-1);
        }
        str = re_str;
        str[i] = alphabet[rem];
        str[i + 1] = '\0';
        num /= 32;
        i++;
    }
    reverse_string(str);
    return str;
}
/**
 *  reverses the input string
 * @param str input string
 */
void reverse_string(char *str)
{
    char temp;
    int i, len;
    len = strlen(str);

    for (i = 0; i < len / 2; i++)
    {
        temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}
/**  @param bin binary string to converted
 *  @return string representation in 32 base of input binary string
 */
char *binary_to_32(char *bin)
{
    char *str;
    int value;
    value = (int)strtol(bin, NULL, 2);
    str = decimal_to_32(value, FALSE);
    return str;
}