#ifndef _PREPROCESSOR_H_
#define _PREPROCESSOR_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MACRO_TABLE_SIZE 10 /*represent the size of the macro table*/
#define MAX_LINE 80         /*represent the maximun characters in a line*/
#define MAX_SIZE 256        /*the length of a file name that can be provided*/
#define ON 1                /*when flag is ON we will return 1*/
#define OFF 0               /*when flag if OFF we will return 0*/
#define ERROR -1            /*when the macro contain ERROR it will return -1  */

int file_check(char as_file[], int argc);

int is_a_whitespace_only(char line[]);

void preprocessor(int argc, char *argv[]);

int capture_macro_name(char copy_buffer[], int word_count, int table_cell);

int macro_spreading(char copy_buffer[], char *token, int table_cell, char file_name[]);

int capture_macro_content(char copy_buffer[], int table_cell, int j, int content_flag);

void write_to_file(char copy_buffer[], char file_name[]);

void increasing_macro_table(int rounds);

int check_if_reserved(char *token);

/*this struct will hold the macro name and the macro content of each macro that the program will detect*/

typedef struct
{
  char macro_name[80];        /* macro name can be up to 80 characters as the max line*/
  char macro_content[10][80]; /*macro content can be up to 10 lines of 80 characters each(הוראה מהפורום)*/

} macro;

#endif