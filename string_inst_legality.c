
#include "string_inst_legality.h"
/**
 * checks legality status of input to .string  instruction. adds words to data image based on characters in input.
 *@param str part of input line after '.data'
 @param data_img pointer to data image
 * @return upon success returns dc increment. else if error occurs , negative number representing error is returned
 *
 */
int LegalityStringInst(char *str, Image *data_img)
{
    char *s;
    int numOfDoubleQuotes, dc_increment;
    Boolean hasExtranousCharsAtEdges;
    if (!str)
        return MISSING_DOUBLE_QUOTES;

    s = cut_whitespace(str);

    numOfDoubleQuotes = countDoubleQuotes(s);
    if (numOfDoubleQuotes < 2)
        return MISSING_DOUBLE_QUOTES; /* missing double quotes*/

    hasExtranousCharsAtEdges = ExtranousCharsAtEdges(s);
    if (hasExtranousCharsAtEdges)
        return EXTRANOUS_CHARACTERS_AT_EDGES; /* extranous characters at edges of */

    dc_increment = addStringToDataImage(s, data_img); /* if Successful - returns dc increment due to actions. else returns NON_ASCII ERROR  */
    return dc_increment;
}
/**
 * @param s string literal after '.string' instruction
 * @return TRUE if s contains extranous chars(i.e NOT double quotes) at edges .False otherwise.
 * */
Boolean ExtranousCharsAtEdges(char *s)
{
    if (s[0] != '\"' || s[strlen(s) - 1] != '\"')
        return TRUE;
    return FALSE;
}
/**
 * @param s string literal after '.string' instruction
 * @return number of double quotes in s
 * */
int countDoubleQuotes(char *s)
{
    int i, count = 0;
    for (i = 0; i < strlen(s); i++)
    {
        if (s[i] == '\"')
            count++;
    }
    return count;
}
/** add string characters to data image
 * @param s string literal whose characters are supposed  to be added to data image
 * @return dc increment if no errors occur. else return negative number representing error.
 * */
int addStringToDataImage(char *s, Image *data_img)
{
    int i;
    for (i = 1; i < strlen(s) - 1; i++)
    {
        if ((unsigned char)s[i] > ASCII_MAX) /*  in case there is a non ASCII character  in string literal*/
            return NON_ASCII;
        addStringtoImage(data_img, decimalToBinary(s[i]));
    }
    addStringtoImage(data_img, decimalToBinary(0)); /* adding NULL TERMINATING CHARACTER */
    return i;
}