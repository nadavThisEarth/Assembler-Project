#ifndef FIRST_PASS_H_
#define FIRST_PASS_H_
#include "symbolTable.h"
#include "image.h"

Boolean FirstPass(FILE *fd, char *am_file, Image *code_img, Image *data_img, node **head);

int is_valid_lable(char *string);
Boolean is_reserved_word(char *str);
int is_instruction(char *str);
int is_modifier(char *str);
int is_label_legal(char *token, node **head);

typedef enum /*  enum of negative numbers representing errors concerning LABEL checks  */
{
    ILLEGAL_EMPTY_INPUT_ = -10,
    LITERAL_IS_NEITHER_COMMAND_NOR_INSTRUCTION = -9,
    ENTRY_OR_EXTERN_SENTENCE_CANNOT_BE_SAVED_TO_LABEL = -8,
    EMPTY_DEFINITION_FOR_LABEL = -7,
    LABEL_STARTS_WITH_NON_ALPHABET = -6,
    EMPTY_LABEL = -5,
    LABEL_TOO_LONG_ = -4,
    ILLEGAL_CHARACTERS_IN_LABEL = -3,
    LABEL_IS_A_RESERVED_WORD = -2,
    LABEL_ALREADY_EXISTS = -1
} FIRST_PASS_LABEL_ERROR_CODE;

#endif