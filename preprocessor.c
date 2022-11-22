#include "preprocessor.h"

/*Declaration on global variable*/

macro *macro_table; /*declare on a pointer that will point on the macro table block memory*/

void preprocessor(int argc, char *argv[])
{

  FILE *fileptr; /*file pointer*/

  FILE *cleaner; /* file pointer used for erasing all content of a file in case of multiple runs of the file  */

  char file_name[MAX_SIZE]; /*hold the file name that provided*/

  char as_file[MAX_SIZE]; /*hold the file name that provided*/

  char *token; /*pointer on char*/

  char am_file_to_be_cleaned[MAX_SIZE]; /*hold the file name to be ereased*/

  char buffer[MAX_LINE]; /*hold the a line of characters from the provided file*/

  char copy_buffer[MAX_LINE]; /*hold the a line of characters from the provided file*/

  int content_flag = OFF; /*tells when we read the macro content*/

  int word_count; /*count the word on a line */

  int rounds = 1; /*count the number of times we increment the macro table*/

  int entered_files; /*a verbliable that help to iterate all the files that provided to the program with for loop */

  int table_cell = 0; /*counter of the macro table cells*/

  int macro_content_count = 0; /*count the line of the macro content inside the macro table*/



  /*loop all over the files that provided,one file at a time*/

  for (entered_files = 1; entered_files < argc; entered_files++)
  {

    strcpy(file_name, argv[entered_files]); /*copy the file name that provided to an array*/

    strcpy(as_file, file_name); /*copy the file name that provided to a second array*/

    strcat(as_file, ".as"); /*add .as suffix to the name of the file*/

    if (file_check(as_file, argc) == ERROR) /*check if the file name that provided if valid*/

    {
      continue; /*skip to the next file*/
    }

    fileptr = fopen(as_file, "r+");

    if (fileptr == NULL)

    {
      printf("The file %s can't be opend\n", as_file);

      continue;
    }

    macro_table = (macro *)calloc(MACRO_TABLE_SIZE, sizeof(macro)); /*allocate space to the macro table*/

    strcpy(am_file_to_be_cleaned, file_name); /*copy the file name*/

    strcat(am_file_to_be_cleaned, ".am"); /*add to the file name .am suffix */

    cleaner = fopen(am_file_to_be_cleaned, "w"); /* cleans all content of '.am' file if already exists (due to multiple runs of same file name in command line) */

    if (!cleaner)
    {

    	printf("content of %s couldn't be erased \n",am_file_to_be_cleaned);

    }

    while (fgets(buffer, MAX_LINE, fileptr) != NULL) /*read from the file one line at a time*/
    {

      word_count = 0; /*reset the word count every new line*/

      strcpy(copy_buffer, buffer); /*copy the line to an new array before we start to "cut" it into words */

      if (is_a_whitespace_only(buffer) == ON ) /*check if the line contain only white spaces*/
      {


	if (content_flag==ON) /*if the empty line is inside a macro call to a function to write the line to the macro table*/

	{
		capture_macro_content(copy_buffer, table_cell, macro_content_count, content_flag);

		macro_content_count++;
		
		continue;
	
	}


        write_to_file(copy_buffer, file_name); /*if the line contain only white spaces copy the line to the .am file*/

        continue;
      }

      token = strtok(buffer, " \t\n");

      while (token != NULL)
      {

        if (macro_spreading(copy_buffer, token, table_cell, file_name) != OFF) /*if calling to the macro was located break the
										 loop and dont wtire the name to the .am file*/
        {
          break;
        }

        if (strcmp(token, "macro") == 0) /*check if the word macro was located*/
        {

          if (capture_macro_name(copy_buffer, word_count, table_cell) != ON) /*if the macro name was unvalid copy the unvalid macro as is to the .am file*/
          {

            write_to_file(copy_buffer, file_name);

            break;
          }
          else /*if the macro is valid turn on the content_flag, break the loop and go to the next line*/
          {

            content_flag = ON;

            break;
          }
        }

        if (content_flag == ON) /*check if content_flag is ON*/
        {

          content_flag = capture_macro_content(copy_buffer, table_cell, macro_content_count, content_flag); /*call to the function that store the macro content*/

          macro_content_count++; /*increment the line counter after every cell we fill in the macro content feild in the macro table*/

          if (content_flag == OFF) /* check if the we store all the macro content and turn off the content_flag if so*/
          {

            table_cell++; /* increment the the macro_table cell by one to the next cell in the memory*/

            macro_content_count = 0; /* set to zero the macro_content line counter*/

            if (table_cell == ((MACRO_TABLE_SIZE * rounds) - 1)) /*check if the macro_table run out of space,if so, allocate more memory */
            {

              rounds++; /*add one to the macro table inceremnt round counter*/

              increasing_macro_table(rounds); /*call to a function to increment the macro table by 10*/
            }
          }

          break;
        }

        token = strtok(NULL, " \t\n");

        if (token == NULL) /*if we fhinish cutting the line into word and and we didnt locate macro definition write the line as is to the .am file*/
        {

          write_to_file(copy_buffer, file_name);
        }
      }
    }

    table_cell = 0; /*set the macro table counter to zero every new file*/

    fclose(fileptr);

    free(macro_table);
  }
}

/*** this function check if the file that the user provided is a valid file ***/

int file_check(char as_file[], int argc)
{

  FILE *check; /*file pointer */
               /* check if a file name was provided */

  if (argc == 1)
  {

    printf("\nYou did not provid any file to translate!\n");

    exit(0);
  }

  /*check if the file name that provided is exist*/

  if (!(check = fopen(as_file, "r+")))
  {

    printf("\nThe file \"%s\" is not exist!\n", as_file);

    return ERROR;
  }

  return 1;
}


/*** this function check if the line contain only white spaces ***/

int is_a_whitespace_only(char line[])
{
  int i;

  for (i = 0; i < strlen(line); i++)

  {
    if (!isspace((unsigned char)line[i])) /*  in case  character is not a whitespace return OFF*/

      return OFF;
  }

  return ON;
}


/*** this function checking if the macro name defined correctly and store it inside a the macro table ***/

int capture_macro_name(char copy_buffer[], int word_count, int table_cell)
{

  char *token;
  int name_count = 0;
  char local_buffer[MAX_LINE];
  char name[MAX_LINE];
  char last_character;

  strcpy(local_buffer, copy_buffer); /*copy the line inside the copy_buffer to a local buffer*/

  if (word_count != 0) /*check if the macro word appears in a middle of a line*/
  {
    return ERROR;
  }

  token = strtok(local_buffer, " \t\n"); /*this will capture the word "macro"*/

  token = strtok(NULL, " \t\n"); /*this will capture the name of the macro*/

  strcpy(name, token);

  if (check_if_reserved(token) == ON) /*checking if the macro name is equal to one of the reservred words*/
  {
    return ERROR;
  }

  if ((last_character = name[strlen(token) - 1]) == ':') /*checking if the macro name defined as a label*/
  {
    return ERROR;
  }

  while (token != NULL)
  {

    name_count++;

    if (name_count > 1) /*check if there is more then one name to the macro*/
    {
      return ERROR;
    }

    strcpy(macro_table[table_cell].macro_name, token); /*copy the macro name to the macro_table*/

    token = strtok(NULL, " \t\n"); /* move to the next word in the line*/
  }

  return ON;
}

/*** this function capture the content of the macro and store it inside the macro table ***/

int capture_macro_content(char copy_buffer[], int table_cell, int macro_content_count, int content_flag)
{
  char *token;

  char buffer[MAX_LINE];

  strcpy(buffer, copy_buffer); /*copy the line inside the copy_buffer to the local buffer*/

  

  if(is_a_whitespace_only(copy_buffer) == ON) /*check if line is empty*/
  {

     strcpy(macro_table[table_cell].macro_content[macro_content_count], copy_buffer); /*write the line inside the macro table*/


	return ON;
  } 
	


  token = strtok(buffer, " \t\n");

  if (strcmp(token, "endmacro") == 0) /*checking if we reach to the end of the macro*/
  {

    return OFF;
  }

  strcpy(macro_table[table_cell].macro_content[macro_content_count], copy_buffer); /*copy the content of the macro to the macro table*/

  return ON;
}

/*** this function check if the macro name not equal to one of the reserved words ***/

int check_if_reserved(char *token)
{

  int row;
  char reserved_words[30][8] = {{"r0"}, {"r1"}, {"r2"}, {"r3"}, {"r4"}, {"r5"}, {"r6"}, {"r7"}, {"mov"}, {"cmp"}, {"add"}, {"sub"}, {"not"}, {"clr"}, {"lea"}, {"inc"}, {"dec"}, {"jmp"}, {"bne"}, {"get"}, {"prn"}, {"jsr"}, {"rts"}, {"hlt"}, {".data"}, {".string"}, {".struct"}, {".entry"}, {".extern"}};

  for (row = 0; row < 30; row++) /*loop over the reserved word array and compare the reserveds words to the name of the macro*/
  {
    if (strcmp(reserved_words[row], token) == 0)
    {

      return ON;
      break;
    }
  }

  return OFF;
}

/*** this function increasing the macro table when its run out of space ***/

void increasing_macro_table(int rounds)
{

  macro *extended_macrotable_size;

  extended_macrotable_size = (macro *)realloc(macro_table, (rounds * MACRO_TABLE_SIZE) * sizeof(macro));

  if (extended_macrotable_size == NULL) /*check if the memory allocation was succsseded */
  {
    printf("memory allocation failed");

    exit(0); /* exit the program if the alocation failed */
  }

  macro_table = extended_macrotable_size; /*set the macro table to the extended macrotable block of memory address */
}

/*** This function spread the macro content that store in the macro table ***/

int macro_spreading(char copy_buffer[], char *token, int table_cell, char file_name[])
{

  int row;
  int macro_content_cell = 0;
  int local_word_count = 0;
  int found = OFF;
  char copy_file_name[MAX_SIZE];
  char local_buffer[MAX_LINE];
  char *local_token;

  strcpy(copy_file_name, file_name); /*copy the file name*/

  strcpy(local_buffer, copy_buffer); /*copy the line */

  /*loop over the macro names that stored in the macro table and check if its equal to the first word in the line*/
  for (row = 0; row < table_cell; row++)
  {

    if (strcmp(macro_table[row].macro_name, token) == 0)
    {

      found = ON;

      break;
    }
  }

  /*** if the macro name found loop over the provided line and check if the macro name found in the line alone ***/

  if (found == ON)
  {

    local_token = strtok(local_buffer, " \t,\n");

    while (local_token != NULL)
    {

      local_word_count++;

      local_token = strtok(NULL, " \t,\n");
    }

    if (local_word_count > 1) /*if there is more then the name of the macro in the line return ERROR*/
    {
      write_to_file(copy_buffer, copy_file_name);

      return ERROR;
    }

    /* this while loop run over the macro table and spread the macro contect of the macro name we found to the .am file */

    while (strlen(macro_table[row].macro_content[macro_content_cell]) != 0) /*loop til we found cell with no content*/
    {
      strcpy(copy_buffer, macro_table[row].macro_content[macro_content_cell]); /*copy the content to an array so we can pass it to the write_to_file function*/

      macro_content_cell++;

      write_to_file(copy_buffer, copy_file_name); /*write to the .am file all the macro content*/
    }

    return found; /* if a macro name found return 1*/
  }

  return OFF; /* if the passed token is not a macro name return 0*/
}

/*** this function write a given content to the .am file ***/

void write_to_file(char copy_buffer[], char file_name[])
{

  FILE *new_file;

  char am_file[MAX_SIZE];

  strcpy(am_file, file_name); /*copy the file name*/

  strcat(am_file, ".am"); /*add to the file name .am suffix */

  if ((new_file = fopen(am_file, "a+")) == NULL)
  {
    printf("The file %s can't be opend\n", am_file);

    exit(0);
  }

  fprintf(new_file, "%s", copy_buffer);

  fclose(new_file);
}