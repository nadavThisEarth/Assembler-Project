#ifndef _FIRST_PASS_COMMANDS_H
#define _FIRST_PASS_COMMANDS_H
#include "common.h"
#include "image.h"
#include <errno.h>
int firstPass_Handle_Group2(char *str, int decimal_opcode, Image *code_img);
int firstPass_Handle_Group1(char *str, int decimal_opcode, Image *code_img);
int firstPass_Handle_Group0(char *str, int decimal_opcode, Image *code_img);
int get_operand_addressing(char *operand_string);
int countCommas_in_command_arguments(char *s);
int is_struct_element(char *s);
int is_Label_operand(char *s);
int is_legal_addressings_group2(int decimal_opcode, int first_operand_addressing, int second_operand_addressing);
int group2_required_lines(int first_operand_addressing, int second_operand_addressing);
int addImmediateToCodeImage(char *str, Image *code_img);
void add_first_word(int decimal_opcode, int first_operand_addressing, int second_operand_addressing, Image *code_img);
int add_more_group2(char *first_operand, int first_operand_addressing, char *second_operand, int second_operand_addressing, Image *code_img);
int add_more_group1(char *operand, int operand_addressing, Image *code_img);
int is_legal_addressings_group1(int decimal_opcode, int operand_addressing);
int group1_required_lines(int operand_addressing);

typedef enum /* enum of errors that might occur in first pass analysis of command operands */
{
    EXTRANOUS_TEXT_FOR_HLT_OR_RTS_COMMAND = -14,
    ILLEGAL_ADDRESSING_TYPE_FOR_OPERAND = -13,
    NUMBER_OUT_RANGE = -12,
    ILLEGAL_ADDRESSING_TYPE_FOR_FIRST_OPERAND = -11,
    ILLEGAL_ADDRESSING_TYPE_FOR_SECOND_OPERAND = -10,
    NO_OPERANDS = -9,
    ILLEGAL__COMMA = -8,
    MISSING__COMMA = -7,
    MISSING_OPERAND = -6,
    ILLEGAL_IMMEDIATE_FORMAT = -5,
    ILLEGAL_STRUCT_FIELD_NUMBER = -4,
    ILLEGAL_LABEL_FORMAT_IN_OPERAND = -3,
    LABEL_TOO_LONG_IN_OPERAND = -2,
    MISSING_LABEL = -1
} CMD_ERROR;

#endif
