#ifndef STRING_INSTRUCTION_LEGALITY_H
#define STRING_INSTRUCTION_LEGALITY_H
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "image.h"
#include <string.h>
#include "common.h"
typedef enum
{
    NON_ASCII = -3,
    MISSING_DOUBLE_QUOTES = -2,
    EXTRANOUS_CHARACTERS_AT_EDGES = -1
} StringInstStatus;
Boolean ExtranousCharsAtEdges(char *s);
 int countDoubleQuotes(char *s);
 int addStringToDataImage(char *s, Image *data_img);
int LegalityStringInst(char *str, Image *data_img);
#endif