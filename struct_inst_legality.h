#ifndef STRUCT_LEGALITY_H
#define STRUCT_LEGALITY_H
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "image.h"
#include <errno.h>
#include "common.h"
int LegalityStructInst(char *str, Image *data_img);
Boolean IllegalCharAtEnd(char *s);
int firstDoubleQuotes(char *s);
int lastDoubleQuotes(char *s);
int countCommas_in_struct(char *s, int firstDQ_index);
Boolean ExtranousCharAfterComma(char *s, int firstDQ_index);
int addNumberPartToDataImage(char *str, Image *data_img);
int addStringPartToDataImage(char *s, Image *data_img);

typedef enum
{
    INTEGER_OUT_OF_RANGE = -9,
    NON__ASCII = -8,
    NO_ARGUMENTS_GIVEN = -7,
    MISSING__DOUBLE_QUOTES = -6,
    ENDS_WITH_ILLEGAL_CHAR = -5,
    ILLEGAL_COMMA = -4,
    MISSING_COMMA = -3,
    ILLEGAL_TEXT_BETWEEN_COMMA_AND_STRING = -2,
    ILLEGAL_INTEGER_FORMAT = -1
} StructInstStatus;
#endif
