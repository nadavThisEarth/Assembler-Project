
#include "data_inst_legality.h"

/**
 * checks legality status of input to data instruction. adds words to data image based on input numbers .
 *@param str part of input line after '.data'
 @param data_img pointer to data image
 * @return upon success returns dc increment. else if error occurs , negative number representing error is returned
 *
 */
int LegalityDataInst(char *str, Image *data_img)
{
    char *s;
    int numOfCommas, numOfIntegers;
    if (!str)
        return MISSING_ARGUMENTS;

    s = cut_whitespace(str);
    if (commasAtEdges(s))
        return COMMAS_AT_EDGES; /* illegal commas at edges*/
    if (consecutiveCommas(s))
        return CONSECUTIVE_COMMAS; /* illegal consecutive commas*/
    numOfCommas = countCommas(s);
    numOfIntegers = countNumbers(s, data_img);
    if (numOfIntegers == ILLEGAL_INT_FORMAT) /*in case an error of illegal int format occured return error*/
        return ILLEGAL_INT_FORMAT;
    if (numOfIntegers == NUMBER_OUT_OF_RANGE)
        return NUMBER_OUT_OF_RANGE; /* data instruction must contain legal integers within range of long for this machine*/
    if (numOfIntegers - numOfCommas > 1)
        return MISSING_COMMAS; /* missing commas*/
    if (numOfIntegers - numOfCommas < 1)
        return TOO_MANY_COMMAS; /* too many commas*/
    else
        return numOfIntegers; /* Success - return increment of data counter */
}
/**
 * @param s input string to be checked for commas at edges
 * @return TRUE in case of commas at edges of string, FALSE otherwise.
 * */
Boolean commasAtEdges(char *s)
{
    if (s[0] == ',' || s[strlen(s) - 1] == ',')
        return TRUE;
    return FALSE;
}
/**
 * @param s input string to be checked for consecutive commas
 * @return TRUE in case of consecutive commas at edges of string, FALSE otherwise.
 * */
Boolean consecutiveCommas(char *s)
{
    int i;
    Boolean last_is_comma = FALSE;
    for (i = 0; i < strlen(s); i++)
    {
        if (isspace((unsigned char)(s[i])))
            continue;
        else if (!last_is_comma && s[i] == ',')
            last_is_comma = TRUE;
        else if (last_is_comma && s[i] == ',')
            return TRUE;
        else
            last_is_comma = FALSE;
    }
    return FALSE;
}
/**
 * @param s input string to be checked for number of commas.
 * @return number of commas in input string.
 * */
int countCommas(char *s)
{
    int i, count = 0;
    for (i = 0; i < strlen(s); i++)
    {
        if (s[i] == ',')
            count++;
    }
    return count;
}
/** counts numbers in input string and adds numbers to data image.
 * @param s input string to be checked for number of integers.
 * @return if no errors occur, number of integers in input string.Otherwise , negative number representing error is returned.
 * */
int countNumbers(char *s, Image *data_img)
{
    char *tok, *delim = ",";
    int count = 0;
    tok = strtok(s, delim);
    while (tok)
    {
        tok = cut_whitespace(tok);
        if (isInteger(tok))
        {
            if (addNumberToDataImage(tok, data_img) == NUMBER_OUT_OF_RANGE)
                return NUMBER_OUT_OF_RANGE; /* case of out of range for long int  */
            count++;
            tok = strtok(NULL, delim);
        }
        else
            return ILLEGAL_INT_FORMAT; /* NOT AN INTEGER   */
    }
    return count;
}
/** counts numbers in input string and adds numbers to data image.
 * @param str  string representing integer.
 * @return if no errors occur, 0 .Otherwise , negative number representing NUMBER_OUT_OF_RANGE error is returned.
 * */
int addNumberToDataImage(char *str, Image *data_img)
{
    long int num;
    errno = 0;
    num = strtol(str, NULL, 10);
    if (errno == ERANGE)
        return NUMBER_OUT_OF_RANGE; /* OUT OF RANGE FOR LONG INTEGER  */
    addStringtoImage(data_img, decimalToBinary(num));
    return 0;
}
