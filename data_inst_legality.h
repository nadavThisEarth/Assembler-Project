#ifndef DATA_INSTRUTION_LEGALITY_H
#define DATA_INSTRUTION_LEGALITY_H
#include "common.h"
#include "image.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
typedef enum /*numbers represent errors found in data sentence analysis*/
{
    NUMBER_OUT_OF_RANGE = -7,
    MISSING_ARGUMENTS = -6,
    TOO_MANY_COMMAS = -5,
    MISSING_COMMAS = -4,
    ILLEGAL_INT_FORMAT = -3,
    CONSECUTIVE_COMMAS = -2,
    COMMAS_AT_EDGES = -1
} DataInstStatus;

int LegalityDataInst(char *str, Image *data_img);
 Boolean commasAtEdges(char *s);
 Boolean consecutiveCommas(char *s);
 int countCommas(char *s);
 int countNumbers(char *s, Image *data_img);
 int addNumberToDataImage(char *str, Image *data_img);

#endif