#ifndef _SECOND_PASS_H
#define _SECOND_PASS_H

#include "symbolTable.h"
#include "image.h"
Boolean SecondPass(FILE *fd, char *am_file, Image *code_img, node **head);
int second_Pass_Handle_Group2(char *operand, Image *code_img, node **head, int current_ic);
int second_Pass_Handle_Group1(char *operand, Image *code_img, node **head, int current_ic);
int second_pass_get_addressing(char *str);
int updateEntry(node **head, Label input_label);

typedef enum
{
    OPERAND_STRUCT_NAME_DOES_NOT_EXIST_AS_STRUCT_IN_SYMBOL_TABLE = -4,

    OPERAND_SYMBOL_DOES_NOT_EXIST_IN_SYMBOL_TABLE = -3
} SECOND_PASS_ERROR;

#endif