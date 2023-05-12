# Assembler-Project
A colaboration between a fellow student (Aviv Itzhak) and myself (Nadav Kopit), this project was a university (Open University of Israel) heavyweight assignment,
 for the course ***System Programming Lab*** during summer 2022.
It had been written in ANSI C  (C 90) , and compiled on UBUNTU 20.04  .

## Purpose:
The program simulates an assembler. It recieves custom-made assembly language input files (with .as extension) and outputs 32-based alphabet code files (with .ob ,  .ext, .ent extensions)
which represent commands and data-storage instructions found in input files.

## Procedure:
***At first*** , 'macros' in the input files are handled. For this project's purposes, a macro **definition**  is essentially a block of assembly code lines in an input file. It strats with the word "macro"
and ends with "endmacro".
A macro name mention is an execution of that same macro.
Handling of macros is done in **"preprocessor.c"** (***written by Aviv***) and consists of spreading a macro block instead of every corresponding macro name-mention , 
and later deleting the macro definitions. All of which is done in a new file with same name (with extension ".am")  which is afterward sent to the Passes Manager for further processing.

***Passes Manager***  ( ***written by Nadav in "passes_manager.c"*** ) initiates variables **symbol table** and **memory images** and runs 2 sub-procedures (namely - **"passes"**)  on the ".am" file it recieves.   

Symbol table ( ***written by Nadav in "symbolTable.c"*** )  is a linked list which eventually holds all symbols (labels) int the ".am" file.
Memory images ( ***written by Nadav in "image.c"*** ) are dynamically-allocated arrays of strings which in the end contain the 10-bit representation of lines in ".am" file, according to rules set by the assignment.

***First Pass*** ( ***written by Nadav in "first_pass.c"*** ) processes the ".am" file and checks legality of each line.  In case of an error at a specific line, it will printed out to the screen, mentioning line number of occurrence.
Code for checks is spread upon files: *"first_pass_commands.c"*, *"data_inst_legality.c"* , *"string_inst_legality.c"* and *"struct_inst_legality.c"* - ***All written by Nadav***.

If there's an error (at least one), Passes Manger frees memory images and symbol-table and proceeds to next input file. Otherwise, the input file is sent to second pass.

***Second Pass*** ( ***written by Nadav in "second_pass.c"*** )  analyses the ".am" file line after line in a similar manner to First Pass, albeit targeting **entry** type symbols and **symbols in command type sentences** (both unhandled during First Pass).

If second pass has errors - they are printed out to the screen. Otherwise , Passes Manager creates output files whose lines are bespoke 32 -based alphabet representation of  symbol table and memory images.




