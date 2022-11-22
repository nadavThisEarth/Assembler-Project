#include "first_pass_commands.h"

/** analyzes operands of commands with 2 operands and adds binary lines to code image.
 * @param str  string after command name to be analyzed.
 * @param decimal_opcode decimal number of opcode of command
 * @param code_img code image.
 * @return if no errors occur, ic increment .Otherwise , negative number representing error is returned.
 * */
int firstPass_Handle_Group2(char *str, int decimal_opcode, Image *code_img)
{
  int first_operand_addressing, second_operand_addressing,
      legal_addressings_result, add_more_result, lines_needed;
  char *first_operand, *second_operand;

  if (!str) /*null check of operands part */
    return NO_OPERANDS;
  if (countCommas_in_command_arguments(str) > 1) /*in case of more than 1 comma */
    return ILLEGAL__COMMA;
  if (countCommas_in_command_arguments(str) < 1) /*in case of less than 1 comma */
    return MISSING__COMMA;

  first_operand = strtok(str, ","); /*null check of  first operand */
  if (!first_operand)
  {
    return MISSING_OPERAND;
  }

  second_operand = strtok(NULL, ""); /*null check of  second operand */
  if (!second_operand)
  {
    return MISSING_OPERAND;
  }

  first_operand = cut_whitespace(first_operand); /*trim leading and trailing whitespaces */
  second_operand = cut_whitespace(second_operand);

  first_operand_addressing = get_operand_addressing(first_operand);
  if (first_operand_addressing < 0) /*  case of error */
    return first_operand_addressing;

  second_operand_addressing = get_operand_addressing(second_operand);
  if (second_operand_addressing < 0) /*  case of error */
    return second_operand_addressing;

  legal_addressings_result =
      is_legal_addressings_group2(decimal_opcode, first_operand_addressing,
                                  second_operand_addressing);
  if (legal_addressings_result < 0) /*  case of error */
    return legal_addressings_result;

  add_first_word(decimal_opcode, first_operand_addressing,
                 second_operand_addressing, code_img);
  add_more_result = add_more_group2(first_operand, first_operand_addressing, second_operand,
                                    second_operand_addressing, code_img);
  if (add_more_result < 0) /*  case of error */
    return add_more_result;

  lines_needed =
      group2_required_lines(first_operand_addressing,
                            second_operand_addressing);

  return lines_needed; /* return ic increment due to analysis of line*/
}

/** analyzes operands of commands with 1 operand and adds binary lines to code image.
 * @param str  string after command name to be analyzed.
 * @param decimal_opcode decimal number of opcode of command
 * @param code_img code image.
 * @return if no errors occur, ic increment .Otherwise , negative number representing error is returned.
 * */
int firstPass_Handle_Group1(char *str, int decimal_opcode, Image *code_img)
{
  int operand_addressing,
      legal_addressings_result, add_more_result, lines_needed;
  char *operand;

  if (!str) /*null check of operand part */
    return MISSING_OPERAND;
  operand = cut_whitespace(str); /*trim leading and trailing whitespaces */

  operand_addressing = get_operand_addressing(operand);
  if (operand_addressing < 0) /*  case of error */
    return operand_addressing;

  legal_addressings_result =
      is_legal_addressings_group1(decimal_opcode, operand_addressing);
  if (legal_addressings_result == ILLEGAL_ADDRESSING_TYPE_FOR_OPERAND) /*  case of error */
    return ILLEGAL_ADDRESSING_TYPE_FOR_OPERAND;

  add_first_word(decimal_opcode, 0, operand_addressing, code_img);
  lines_needed = group1_required_lines(operand_addressing);
  add_more_result = add_more_group1(operand, operand_addressing, code_img);
  if (add_more_result < 0) /*  case of error */
    return add_more_result;
  return lines_needed; /*  return ic increment due to analysis of line*/
}
/** analyzes operands of commands with 0 operands and adds binary lines to code image.
 * @param str  string after command name to be analyzed.
 * @param decimal_opcode decimal number of opcode of command
 * @param code_img code image.
 * @return if no errors occur, ic increment .Otherwise , negative number representing error is returned.
 * */
int firstPass_Handle_Group0(char *str, int decimal_opcode, Image *code_img)
{

  if (str)
  {
    str = cut_whitespace(str);
    if (strlen(str) != 0) /*in case there are non whitespace characters after .hlt or .rts , return error*/
      return EXTRANOUS_TEXT_FOR_HLT_OR_RTS_COMMAND;
  }
  add_first_word(decimal_opcode, 0, 0, code_img);
  return 1; /*1 binary line needed*/
}
/**
 * @param s string after command name to be scanned for number of commas
 * @return  number of commas
 * */
int countCommas_in_command_arguments(char *s)
{
  int i, count = 0;
  for (i = 0; i < strlen(s); i++)
  {
    if (s[i] == ',')
      count++;
  }
  return count;
}
/**
 * @param str string after command name to checked whether represents a struct access literal.
 * @return TRUE if string is a struct access literal .0 if not a struct access literal but a potential label.
 * Negative number represeting error otherwise.
 * */
int is_struct_element(char *str)
{
  int i;
  char *stop;
  char *includes_dot = strrchr(str, '.');
  if (!includes_dot)
    return 0; /* doesn't contain a dot - move to lable check function */
  if (strlen(includes_dot) != 2)
    return ILLEGAL_STRUCT_FIELD_NUMBER; /*"error - illegal struct field number */
  stop = strstr(includes_dot, ".1");
  if (!stop)
    stop = strstr(includes_dot, ".2");
  if (!stop)
    return ILLEGAL_STRUCT_FIELD_NUMBER; /*"error - illegal struct field number */

  for (i = 0; &str[i] < stop; i++) /* iterate from beggining of string  until last dot sign  */
  {
    if (i == 0 && !isalpha(str[i]))
      return ILLEGAL_LABEL_FORMAT_IN_OPERAND; /*error - illegal struct label format */
    else if (!isalnum(str[i]))
      return ILLEGAL_LABEL_FORMAT_IN_OPERAND; /*error - illegal struct label format */
  }
  if (i > MAX_LABEL_LENGTH)
    return LABEL_TOO_LONG_IN_OPERAND; /*error - lable length exceeds limit */
  else if (i == 0)
    return MISSING_LABEL; /*error - lable length is 0 */

  return TRUE;
}
/**
 * @param s string after command name to be checked whether represents a label literal
 * @return TRUE if string is a label literal
 * Negative number represeting error otherwise.
 * */
int is_Label_operand(char *s)
{
  int i;
  if (strlen(s) > MAX_LABEL_LENGTH) /*error -  label length exceeds limit */
    return LABEL_TOO_LONG_IN_OPERAND;
  if (strlen(s) == 0)
    return MISSING_LABEL;
  for (i = 0; i < strlen(s); i++)
  {
    if (i == 0 && !isalpha(s[i]))
      return ILLEGAL_LABEL_FORMAT_IN_OPERAND; /*error - illegal label format */
    else if (!isalnum(s[i]))
      return ILLEGAL_LABEL_FORMAT_IN_OPERAND; /*error - illegal label format */
  }
  return TRUE;
}
/** checks whether addressing of operands are legal for given command.
 * @param decimal_opcode decimal number of opcode of command
 * @param first_operand_addressing - first operand addressing
 * @param second_operand_addressing - second operand addressing
 * @return TRUE if addressing types are legal for this command.
 * Negative number represeting error otherwise.
 * */
int is_legal_addressings_group2(int decimal_opcode, int first_operand_addressing,
                                int second_operand_addressing)
{
  if (second_operand_addressing == IMMEDIATE)
  {
    if (decimal_opcode != 1) /*  if command is not 'cmp' ,return error*/
      return ILLEGAL_ADDRESSING_TYPE_FOR_SECOND_OPERAND;
  }
  if (first_operand_addressing == IMMEDIATE || first_operand_addressing == REGISTER)
  {
    if (decimal_opcode == 6) /*  if command is  'lea' ,return error*/
      return ILLEGAL_ADDRESSING_TYPE_FOR_FIRST_OPERAND;
  }
  return TRUE;
}
/** checks whether addressing of operand is legal for given command.
 * @param decimal_opcode decimal number of opcode of command
 * @param operand_addressing -  operand addressing
 * @return TRUE if addressing type is legal for this command.
 * Negative number represeting error otherwise.
 * */
int is_legal_addressings_group1(int decimal_opcode, int operand_addressing)
{
  if (decimal_opcode != 12 && operand_addressing == IMMEDIATE) /*if command is NOT 'prn ' and operand addressing is immediate ,return error*/
    return ILLEGAL_ADDRESSING_TYPE_FOR_OPERAND;
  return TRUE;
}
/** computes required binary lines in code image due to analysis of command arguments (commands with 2 operands)
 * @param first_operand_addressing - first operand addressing
 * @param second_operand_addressing - second operand addressing
 * @return required binary lines in code image due to analysis of lines (equal to dc increment)
 * */
int group2_required_lines(int first_operand_addressing,
                          int second_operand_addressing)
{
  int required_binary_lines = 1; /*  due to first word */
  if (first_operand_addressing == REGISTER && second_operand_addressing == REGISTER)
    required_binary_lines = 2;
  else
  {
    if (first_operand_addressing == REGISTER || first_operand_addressing == IMMEDIATE || first_operand_addressing == DIRECT)
      required_binary_lines++;
    else
      required_binary_lines += 2;

    if (second_operand_addressing == REGISTER || second_operand_addressing == IMMEDIATE || second_operand_addressing == DIRECT)
      required_binary_lines++;
    else
      required_binary_lines += 2;
  }

  return required_binary_lines;
}
/** computes required binary lines in code image due to analysis of command arguments (commands with 1 operand)
 * @param operand_addressing -  operand addressing
 * @return required binary lines in code image due to analysis of lines (equal to dc increment)
 * */
int group1_required_lines(int operand_addressing)
{
  int required_binary_lines = 1;
  if (operand_addressing == STRUCT)
    required_binary_lines += 2;
  else
    required_binary_lines++;

  return required_binary_lines;
}
/** adds operand of immediate type to code image.
 * @param str -  string representing immediate operand to be added to code image
 * @param code_img code image
 * @return 0 upon success, negative number representing error otherwise.
 * */
int addImmediateToCodeImage(char *str, Image *code_img)
{
  long int num;
  errno = 0;
  num = strtol(str, NULL, 10);
  if (errno == ERANGE)
    return NUMBER_OUT_RANGE; /* OUT OF RANGE FOR LONG INTEGER  */
  num <<= 2;                 /*shift left by 2 for setting ARE bits to 0 */
  addStringtoImage(code_img, decimalToBinary(num));
  return 0;
}
/** adds first word due to analysis of input line  to the code image  .
 * @param decimal_opcode decimal number of opcode of command
 * @param first_operand_addressing - first operand addressing
 * @param second_operand_addressing - second operand addressing (in case of commands with 1 arguments, this variable should be set to 0)
 * @param code_img code image
 * */
void add_first_word(int decimal_opcode, int first_operand_addressing,
                    int second_operand_addressing, Image *code_img)
{
  bin_str first_word, temp; /*  first word eventually holds binary representation of word to be added to code image. temp is an auxiliary array */
  int i;                    /*  iterator for loops */

  strcpy(temp, decimalToBinary(decimal_opcode)); /*  handling opcode representaion */
  for (i = 0; i < 4; i++)
  {
    first_word[i] = temp[6 + i];
  }
  strcpy(temp, decimalToBinary(first_operand_addressing)); /*  handling first operand addressing representaion */
  for (i = 0; i < 2; i++)
  {
    first_word[i + 4] = temp[8 + i];
  }
  strcpy(temp, decimalToBinary(second_operand_addressing)); /*  handling second operand addressing representaion */
  for (i = 0; i < 2; i++)
  {
    first_word[i + 6] = temp[8 + i];
  }
  first_word[8] = '0'; /*in first word, 2 righmost bits are set to 0*/
  first_word[9] = '0';
  first_word[BINARY_LENGTH] = '\0';       /* NULL terminate string */
  addStringtoImage(code_img, first_word); /* add binary string to code image */
}
/** adds additional produced words due to analysis of input line  to the code image  .
 * @param first_operand_addressing - first operand addressing
 * @param second_operand_addressing - second operand addressing
 * @param first_operand string representing first operand
 * @param second_operand string representing second operand
 * @param code_img code image
 * */
int add_more_group2(char *first_operand, int first_operand_addressing,
                    char *second_operand, int second_operand_addressing,
                    Image *code_img)
{
  /*word eventually holds binary representation of word to be added to code image.
   temp is an auxiliary array.
   dummy is a string representing a dummy line (for symbols)  */
  bin_str word, temp, dummy;
  char *struct_field; /* pointer to dot sign in struct access operand */
  int i, immediate_success;
  strcpy(dummy, "uuuuuuuuuu");                                                       /* dummy is set to a dummy string for use with symbols  */
  if (first_operand_addressing == REGISTER && second_operand_addressing == REGISTER) /* handling case of both operands being registers */
  {
    strcpy(temp,
           decimalToBinary(first_operand[strlen(first_operand) - 1] - '0')); /* get register number */

    for (i = 0; i < 4; i++)
    {
      word[i] = temp[6 + i];
    }
    strcpy(temp,
           decimalToBinary(second_operand[strlen(second_operand) - 1] - '0')); /* get register number */

    for (i = 0; i < 4; i++)
    {
      word[i + 4] = temp[6 + i];
    }
    word[8] = '0';
    word[9] = '0';
    word[BINARY_LENGTH] = '\0';
    addStringtoImage(code_img, word);
  }
  else
  {
    if (first_operand_addressing == IMMEDIATE) /* handling immediate for first operand */
    {
      immediate_success = addImmediateToCodeImage(first_operand + 1, code_img);
      if (immediate_success == NUMBER_OUT_RANGE)
        return NUMBER_OUT_RANGE;
    }

    else if (first_operand_addressing == DIRECT)   /* handling direct addressing for first  operand */
      addStringtoImage(code_img, dummy);           /* adding dummy to code image */
    else if (first_operand_addressing == REGISTER) /* handling register addressing for first operand */
    {
      strcpy(temp, decimalToBinary(first_operand[strlen(first_operand) - 1] - '0')); /* get register number */
      for (i = 0; i < 4; i++)
      {
        word[i] = temp[6 + i];
      }
      for (i = 4; i < BINARY_LENGTH; i++)
      {
        word[i] = '0';
      }
      word[BINARY_LENGTH] = '\0';
      addStringtoImage(code_img, word);
    }
    else /* handling struct access addressing */
    {
      addStringtoImage(code_img, dummy); /* adding dummy to code image */
      struct_field = strrchr(first_operand, '.');
      immediate_success = addImmediateToCodeImage(struct_field + 1, code_img); /* adding field number to code image */
      if (immediate_success == NUMBER_OUT_RANGE)                               /*  in case of error ,return error*/
        return NUMBER_OUT_RANGE;
    }

    if (second_operand_addressing == IMMEDIATE) /* handling immediate for second operand */
    {
      immediate_success = addImmediateToCodeImage(second_operand + 1, code_img);
      if (immediate_success == NUMBER_OUT_RANGE)
        return NUMBER_OUT_RANGE;
    }

    else if (second_operand_addressing == DIRECT)   /* handling direct addressing for second  operand */
      addStringtoImage(code_img, dummy);            /* adding dummy to code image */
    else if (second_operand_addressing == REGISTER) /* handling register addressing for second  operand */
    {
      strcpy(temp,
             decimalToBinary(second_operand[strlen(second_operand) - 1] - '0')); /* get register number */
      for (i = 0; i < 4; i++)
      {
        word[i + 4] = temp[6 + i];
      }
      for (i = 0; i < 4; i++)
      {
        word[i] = '0';
      }
      word[8] = '0';
      word[9] = '0';
      word[BINARY_LENGTH] = '\0';
      addStringtoImage(code_img, word);
    }
    else /* handling struct access addressing */
    {
      addStringtoImage(code_img, dummy); /* adding dummy to code image */
      struct_field = strrchr(second_operand, '.');
      immediate_success = addImmediateToCodeImage(struct_field + 1, code_img); /* adding field number to code image */
      if (immediate_success == NUMBER_OUT_RANGE)                               /*  in case of error ,return error*/
        return NUMBER_OUT_RANGE;
    }
  }
  return TRUE;
}
/** adds additional produced words due to analysis of input line  to the code image  .
 * @param operand_addressing -  operand addressing
 * @param operand string representing  operand
 * @param code_img code image
 * */
int add_more_group1(char *operand, int operand_addressing, Image *code_img)
{
  /*word eventually holds binary representation of word to be added to code image.
   temp is an auxiliary array.
   dummy is a string representing a dummy line (for symbols)  */
  bin_str word, temp, dummy;
  char *struct_field; /* pointer to dot sign in struct access operand */
  int i, immediate_success;
  strcpy(dummy, "uuuuuuuuuu");         /* dummy is set to a dummy string for use with symbols  */
  if (operand_addressing == IMMEDIATE) /* handling case of immediate addressing */
  {

    immediate_success = addImmediateToCodeImage(operand + 1, code_img);
    if (immediate_success == NUMBER_OUT_RANGE) /* in case of error , return error */
      return NUMBER_OUT_RANGE;
  }

  else if (operand_addressing == DIRECT) /*  handling case of direct addressing */
    addStringtoImage(code_img, dummy);   /*  add dummy string to code image */
  else if (operand_addressing == REGISTER)/*  handling case of register addressing */
  {
    strcpy(temp, decimalToBinary(operand[strlen(operand) - 1] - '0')); /* get register number */
    for (i = 0; i < 4; i++)
    {
      word[i] = temp[6 + i];
    }
    for (i = 4; i < BINARY_LENGTH; i++)
    {
      word[i] = '0';
    }
    word[BINARY_LENGTH] = '\0';
    addStringtoImage(code_img, word);
  }
  else /* handling cas of struct access addressing type  */
  {
    addStringtoImage(code_img, dummy); /* adding dummy string to code image */
    struct_field = strrchr(operand, '.');
    immediate_success = addImmediateToCodeImage(struct_field + 1, code_img); /* adding field number to code image */
    if (immediate_success == NUMBER_OUT_RANGE) /*  in case of error return error */
      return NUMBER_OUT_RANGE;
  }
  return TRUE;
}
/** determines addressing of given operand.
 * @param operand_string string containing operand
 * @return NON NEGATIVE number representing Addressing in case of no failures; NEGATIVE number representing error otherwise.
 * */
int get_operand_addressing(char *operand_string)
{
  int is_struct_result, is_label_result;
  if (strlen(operand_string) == 0) /* case of blank  operand */
    return MISSING_OPERAND;
  else if (operand_string[0] == '#') /* check whether   operand is an immediate type */
  {
    if (!isInteger(operand_string + 1))
      return ILLEGAL_IMMEDIATE_FORMAT;
    else
      return IMMEDIATE; /* in this case , addressing type returned set to IMMEDIATE  */
  }
  else if (is_register(operand_string)) /* check whether  operand  is a register type */
  {
    return REGISTER;
  }
  else
  {
    is_struct_result = is_struct_element(operand_string); /* check whether  first operand is a struct field type */
    if (is_struct_result < 0)
      return is_struct_result; /* in this case , error is returned */
    else if (is_struct_result > 0)
      return STRUCT; /* in this case , addressing type is set to STRUCT (field) */
    else             /*   last option is label */
    {
      is_label_result = is_Label_operand(operand_string); /* check whether  first operand is a label */
      if (is_label_result > 0)
        return DIRECT; /* in this case , addressing type is set to DIRECT  */
      else
        return is_label_result; /* in this case , error is returned */
    }
  }
}
