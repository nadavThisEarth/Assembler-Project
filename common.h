/* this header contains commonly used functions in both passes  */

#ifndef _COMMON_H
#define _COMMON_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> /* for isdigit */
#define MAX_LABEL_LENGTH 30
#define MAX_FILENAME_SIZE 255
#define IC_OFFSET 100
#define LINE_LENGTH 80
#define ASCII_MAX 127
#define BINARY_LENGTH 10 /*  length of every binary line in memory image   */

typedef enum /* enum representing boolean . i.e the set {0,1}  */
{
    FALSE,
    TRUE
} Boolean;
typedef enum /* enum representing addressing method . */
{
    IMMEDIATE,
    DIRECT,
    STRUCT,
    REGISTER
} ADDRESSING;

Boolean isInteger(char *s);
char *cut_whitespace(char *str);
Boolean is_empty(char *line);
Boolean is_comment(char *line);
int is_command(char *str);
Boolean is_register(char *s);
char *decimalToBinary(long int num);
char *decimal_to_32(int num, Boolean is_size_counter);
void reverse_string(char *str);
char *binary_to_32(char *bin);

#endif