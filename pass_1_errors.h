#ifndef _PASS1_ERRORS_H
#define _PASS1_ERRORS_H

static const char *first_pass_label_errors[] = {
    "Label Already Exists",
    "Label is a reserved word",
    "Illegal characters in label",
    "Label too long",
    "Empty label",
    "Label starts with non alphabet character",
    "Empty definition for label",
    "Entry or Extern sentence cannot be saved to label",
    "literal is neither command nor instruction nor label definition",
    "Empty input after instruction or command"};

static const char *first_pass_command_errors[] =
    {" MISSING LABEL IN OPERAND", " LABEL IS TOO LONG IN OPERAND", " ILLEGAL LABEL FORMAT IN OPERAND",
     " ILLEGAL STRUCT FIELD NUMBER", " ILLEGAL IMMEDIATE FORMAT ",
     " MISSING OPERAND", " MISSING COMMA", " ILLEGAL COMMA", " NO OPERANDS",
     " ILLEGAL ADDRESSING TYPE FOR SECOND OPERAND",
     " ILLEGAL ADDRESSING TYPE FOR FIRST OPERAND",
     " NUMBER OUT OF RANGE FOR LONG INTEGER",
     "ILLEGAL ADDRESSING TYPE FOR OPERAND",
     "EXTRANOUS TEXT FOR HLT OR RTS COMMAND"};

static const char *first_pass_data_errors[] = {
    " Illegal comma/commas at edges of input to data",
    " Consecutive commas",
    " Illegal int format",
    " Missing comma/commas",
    " Too many commas",
    "Missing arguments for data instruction",
    "Number out of range for long integer on this machine"};

static const char *first_pass_string_errors[] = {
    "Extranous characters at edges of input to instruction",
    "Missing double quotes",
    " Character inside string literal is not an ASCII"};

static const char *first_pass_struct_errors[] = {
    "Illegal integer format For first operand of .struct sentence",
    "Illegal text between comma and string literal",
    "Missing comma",
    "Illegal comma",
    "instruction ends with an illegal character",
    "Missing double quotes",
    "No arguments given",
    " Character inside string literal is not an ASCII",
    "Number out of range for long integer on this machine"};

#endif 
