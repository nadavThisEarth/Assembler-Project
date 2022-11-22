/*  this file implements symbolTable.h  */
#include "symbolTable.h"

/** adds label to symbol table
 * frees all node of symbol table
 *  @param head - pointer to pointer to Symbol table .
 */
void freeSymbolTable(node **head) /* freeing the memory of the Symbol table - node by node   */
{
    node *temp;
    while (*head != NULL)
    {
        temp = (*head);
        *head = (*head)->next;
        free(temp);
    }
}
/** adds label to symbol table
 * @param input_label - label string to be added.
 * @param input_type - refers to one of the following: DATA, COMMAND,EXTERNAL,ENTRY
 * @param input_value - refers to address in memory image
 * @param input_is_struct - dictates whether label is a struct name (will be crucial for second pass)
 *  @param head - pointer to pointer to Symbol table .
 */
void add_to_Symbol_table(node **head, Label input_label, LabelType input_type, int input_value, Boolean input_is_struct)
{

    node *newNode = (node *)malloc(sizeof(node)); /* newNode points to a node that we want to add   */
    if (!newNode)                                 /*  memory allocation null check */
    {
        printf("memory allocation failed\n");
        exit(-1);
    }
    strcpy(newNode->label, input_label);  /* copying input string from text to node line field */
    newNode->type = input_type;           /* setting node type */
    newNode->value = input_value;         /* setting value */
    newNode->is_Struct = input_is_struct; /* setting whether label is struct name */
    newNode->next = NULL;                 /* setting node next to null , since we add it at the end of the table */

    if (*head == NULL) /* in case of empty table - node becomes the head*/
        *head = newNode;
    else /* otherwise place node at the end of the non-empty table   */
    {
        node *temp = *head;

        while (temp->next != NULL) /* find last node in table - which applies for next = NULL */
        {
            temp = temp->next;
        }

        temp->next = newNode; /* newNode added at the end of the symbol table*/
    }
}
/** needed specifically for second pass
 * @param str symbol mentiones in command sentence
 * @param  input_is_a_struct boolean indicating whether variable str is a struct or not
 * @param head pointer to pointer to first item in symbol table
 * @return  value of symbol in symbol table if exists. -1 otherwise.
 */
int is_exists_Symbol(char *str, Boolean input_is_a_struct, node **head)
{
    node *temp = *head;
    while (temp)
    {
        if (input_is_a_struct) /*  check for a struct symbol */
        {
            if (strcmp(temp->label, str) == 0 && (temp->is_Struct == input_is_a_struct || temp->type == EXTERNAL))
            {
                if (temp->type == EXTERNAL) /* for external , always return 0 (due to coding requirements) */
                    return 0;
                return temp->value;
            }
        }
        else /* check generically for a symbol with same label name  */
        {
            if (strcmp(temp->label, str) == 0)
                return temp->value;
        }

        temp = temp->next;
    }
    return -1;
}
/**
 * @param tested_label - potential input label to be checked.
 * @param head - pointer to pointer to first item in Symbol Table.
 * @return TRUE if label already exists in Symbol Table. Else , returns FALSE.
 */
Boolean is_label_already_exists(node *head, char *tested_label)
{
    node *temp = head;

    /* iterate over the entire linked list*/
    while (temp != NULL)
    {
        /* tested_label found return True.*/
        if (strcmp(temp->label, tested_label) == 0)
            return TRUE;
        temp = temp->next;
    }
    /* tested_label not found*/
    return FALSE;
}
/** adds final value of ic to DATA labels in symbol table (plus IC_OFFSET )
 * adds IC_OFFSET to value of CODE labels in symbol table
 * @param final_ic - holds final value of ic
 *  @param head - pointer to pointer to first item in Symbol table .
 */
void updateLabelsValues(node **head, int final_ic)
{
    node *temp = *head;
    while (temp)
    {
        if (temp->type == DATA)
        {
            temp->value += (final_ic + IC_OFFSET);
        }
        else if (temp->type == CODE)
        {
            temp->value += IC_OFFSET;
        }
        temp = temp->next;
    }
}

/**
 * update type to entry for symbols in symbol_table with identical label of given entry definition.
 *
 * @param  head  - pointer to pointer to symbol table
 * @param input_label - entry declaration label
 *
 * @return TRUE if succeeds. negative number representing error otherwise.
 * */
int updateEntry(node **head, Label input_label)
{
    node *temp = *head;
    while (temp)
    {
        if (strcmp(temp->label, input_label) == 0)
        {
            if (temp->type == EXTERNAL) /* if exists identical symbol whose type is extern, return error*/
            {
                return SYMBOL_DECLARED_BOTH__AS_EXTERN_AND_ENTRY;
            }
            else if (temp->type == ENTRY) /* case of duplicate .entry sentence for same symbol*/
            {
                return DUPLICATE_DECLARATION_OF_SYMBOL_AS_ENTRY;
            }
            else
            {
                temp->type = ENTRY; /* set type of symbol to ENTRY */
            }
        }

        temp = temp->next;
    }
    return TRUE;
}
/**
 * @param head - pointer to pointer to symbol table
 * @return TRUE if symbol table has entry symbols. False otherwise
 */
Boolean hasEntries(node **head)
{
    node *temp = *head;
    while (temp)
    {
        if (temp->type == ENTRY)
        {
            return TRUE;
        }
        temp = temp->next;
    }
    return FALSE;
}
/**
 * @param head - pointer to pointer to symbol table
 * @return TRUE if symbol table has extern symbols. False otherwise
 */
Boolean hasExterns(node **head)
{
    node *temp = *head;
    while (temp)
    {
        if (temp->type == EXTERNAL && temp->value > 0)
        {
            return TRUE;
        }
        temp = temp->next;
    }
    return FALSE;
}
/**
 *  * writes entry symbols and their addresses in 32 base  to output file
 * @param head - pointer to pointer to symbol table
 * @param fw file descriptor set to write mode at '.ent' ended  file
 */
void output_entries(FILE *fw, node **head)
{
    char *entry_value_in_32base;
    node *temp = *head;
    Boolean first = TRUE; /* indicates first line to be printed (needed due to line break purposes) */

    while (temp)
    {
        if (temp->type == ENTRY)
        {
            entry_value_in_32base = decimal_to_32(temp->value, FALSE);
            if (first)
            {
                fprintf(fw, "%-32s   %8s", temp->label, entry_value_in_32base);
                first = FALSE;
            }
            else
            {
                fprintf(fw, "\n%-32s   %8s", temp->label, entry_value_in_32base);
            }
            free(entry_value_in_32base); /*  free malloced pointer */
        }
        temp = temp->next;
    }
}
/**
 *  * writes extern symbols and their addresses in 32 base  to output file
 * @param head - pointer to pointer to symbol table
 * @param fw file descriptor set to write mode at '.ext' ended  file
 */
void output_externs(FILE *fw, node **head)
{
    char *extern_value_in_32base;
    node *temp = *head;
    Boolean first = TRUE; /* indicates first line to be printed (needed due to line break purposes) */

    while (temp)
    {
        if (temp->type == EXTERNAL && temp->value > 0) /*  0 values are skipped because they only  represent .entry definitions , not mentions of symbol in commands */
        {
            extern_value_in_32base = decimal_to_32(temp->value, FALSE);
            if (first)
            {
                fprintf(fw, "%-32s   %8s", temp->label, extern_value_in_32base);
                first = FALSE;
            }
            else
            {
                fprintf(fw, "\n%-32s   %8s", temp->label, extern_value_in_32base);
            }
            free(extern_value_in_32base); /* free malloced pointer*/
        }
        temp = temp->next;
    }
}