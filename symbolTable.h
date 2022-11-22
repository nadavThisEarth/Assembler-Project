#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "common.h"
typedef char Label[MAX_LABEL_LENGTH + 1]; /* defining label type with max length*/
typedef enum
{
    DATA,
    EXTERNAL,
    CODE,
    ENTRY
} LabelType;

typedef struct node_tagName /* defining node with a field of string , and a field of pointer to next node in linked list*/
{
    Label label;
    LabelType type;
    int value; /* eventually holds address in total memory image of CPU (except for .EXTERN DEFINITIONS whose values are set to zero)*/
    Boolean is_Struct;
    struct node_tagName *next;
} node;

typedef enum
{

    DUPLICATE_DECLARATION_OF_SYMBOL_AS_ENTRY = -2,
    SYMBOL_DECLARED_BOTH__AS_EXTERN_AND_ENTRY = -1
} SYMBOL_TABLE_ERRORS;

void add_to_Symbol_table(node **head, Label input_label, LabelType input_type, int input_value, Boolean input_is_struct);
void freeSymbolTable(node **head);
int is_exists_Symbol(char *str, Boolean input_is_a_struct, node **head);
Boolean is_label_already_exists(node *head, char *tested_label);
void updateLabelsValues(node **head, int final_ic);
Boolean hasEntries(node **head);
Boolean hasExterns(node **head);
void output_entries(FILE *fw, node **head);
void output_externs(FILE *fw, node **head);

#endif