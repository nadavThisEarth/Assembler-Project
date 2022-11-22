#include "struct_inst_legality.h"
/**
 * checks legality status of input to .struct  instruction. adds words to data image based on number and characters .
 *@param str part of input line after '.struct'
 @param data_img pointer to data image
 * @return upon success returns dc increment. else if error occurs , negative number representing error is returned
 *
 */
int LegalityStructInst(char *str, Image *data_img)
{
    char *s; /*  holds the string after  whitespace at edges are trimmed */

    /* firstDQ_index - first index of double quotes,
        lastDQ_index - last index of double quotes */
    int firstDQ_index, lastDQ_index, comma_count, dc_increment, addition_returned_value;
    Boolean hasIllegalCharAtEnd;

    if (!str)
        return NO_ARGUMENTS_GIVEN; /* NO ARGUMENTS GIVEN */
    s = cut_whitespace(str);

    firstDQ_index = firstDoubleQuotes(s);
    lastDQ_index = lastDoubleQuotes(s);

    if (firstDQ_index == -1 || lastDQ_index == -1 || firstDQ_index >= lastDQ_index)
        return MISSING__DOUBLE_QUOTES; /* missing double quotes in struct definition's second operand */

    hasIllegalCharAtEnd = IllegalCharAtEnd(s);
    if (hasIllegalCharAtEnd)
        return ENDS_WITH_ILLEGAL_CHAR; /* struct instruction must end with \" */

    comma_count = countCommas_in_struct(s, firstDQ_index);

    if (comma_count > 1)
        return ILLEGAL_COMMA; /* Illegal comma */

    if (comma_count < 1)
        return MISSING_COMMA; /* missing comma */

    if (ExtranousCharAfterComma(s, firstDQ_index))
        return ILLEGAL_TEXT_BETWEEN_COMMA_AND_STRING; /* Illegal text BETWEEN Comma and string literal*/

    s = strtok(s, ",");
    s = cut_whitespace(s);

    if (!isInteger(s))
        return ILLEGAL_INTEGER_FORMAT; /* first operand of struct must have a valid integer form */
    else if (addNumberPartToDataImage(s, data_img) == INTEGER_OUT_OF_RANGE)
        return INTEGER_OUT_OF_RANGE; /* case of out of range for long int on this machine*/
    dc_increment = 1;                /*  increment due to integer inserted to data image */
    s = strtok(NULL, "");
    s = cut_whitespace(s);

    addition_returned_value = addStringPartToDataImage(s, data_img);
    if (addition_returned_value == NON__ASCII) /*  checking non_ASCII characters in string literal */
        return NON__ASCII;
    dc_increment += addition_returned_value; /*  add  increment due to string addition actions */
    return dc_increment;                     /* Success - return dc increment due to additions */
}
/**
 * @param s part of line after '.struct'
 * @return TRUE if last non whitespace character is not double quotes. FALSE otherwise
 * */
Boolean IllegalCharAtEnd(char *s)
{
    if (s[strlen(s) - 1] != '\"')
        return TRUE;
    return FALSE;
}
/**
 * @param s part of line after '.struct'
 * @return index of first double quotes in s . -1 if does not contain double quotes
 * */
int firstDoubleQuotes(char *s)
{
    int i;
    for (i = 0; i < strlen(s); i++)
    {
        if (s[i] == '\"')
            return i;
    }
    return -1;
}
/**
 * @param s part of line after '.struct'
 * @return index of last double quotes in s . -1 if does not contain double quotes
 * */
int lastDoubleQuotes(char *s)
{
    int i;
    for (i = strlen(s) - 1; i >= 0; i--)
    {
        if (s[i] == '\"')
            return i;
    }
    return -1;
}
/**
 * @param s part of line after '.struct'
 * @param firstDQ_index index of first double quotes in s.
 * @return number of commas before first double quotes in s .
 * */
int countCommas_in_struct(char *s, int firstDQ_index)
{
    int i, count = 0;
    for (i = 0; i < firstDQ_index; i++)
    {
        if (s[i] == ',')
            count++;
    }
    return count;
}
/**
 * @param s part of line after '.struct'
 * @param firstDQ_index index of first double quotes in s.
 * @return TRUE if exists a non whitespace character between first comma and first double quotes. FALSE otherwise
 * */
Boolean ExtranousCharAfterComma(char *s, int firstDQ_index)
{
    int i, flag = 0;
    for (i = 0; i < firstDQ_index; i++)
    {
        if (isspace((unsigned char)(s[i])))
            continue;
        else if (s[i] == ',')
            flag = 1;
        else if (flag == 0)
            continue;
        else
            return TRUE;
    }
    return FALSE;
}
/** adds word due to  number part of '.struct' instruction to data image
 * @param str part of line after '.struct'
 * @param data_img pointer to data image
 * @return 0 if succeeds without errors. negative integer representing INTEGER_OUT_OF_RANGE error otherwise.
 * */
int addNumberPartToDataImage(char *str, Image *data_img)
{
    long int num;
    errno = 0;
    num = strtol(str, NULL, 10);
    if (errno == ERANGE)
        return INTEGER_OUT_OF_RANGE; /* OUT OF RANGE FOR LONG INTEGER  */
    addStringtoImage(data_img, decimalToBinary(num));
    return 0;
}
/** adds words due to  string literal  part of '.struct' instruction to data image
 * @param str part of line after '.struct'
 * @param data_img pointer to data image
 * @return dc increment due to addition if succeeds without errors. negative integer representing NON__ASCII error otherwise.
 * */
int addStringPartToDataImage(char *s, Image *data_img)
{
    int i;
    for (i = 1; i < strlen(s) - 1; i++)
    {
        if ((unsigned char)s[i] > ASCII_MAX) /*  in case there is a non ASCII character  in string literal*/
            return NON__ASCII;
        addStringtoImage(data_img, decimalToBinary(s[i]));
    }
    addStringtoImage(data_img, decimalToBinary(0)); /* adding NULL TERMINATING CHARACTER */
    return i;
}