assembler: assembler.o preprocessor.o passes_manager.o first_pass.o first_pass_commands.o data_inst_legality.o string_inst_legality.o struct_inst_legality.o second_pass.o common.o image.o symbolTable.o
	gcc -Wall -ansi -pedantic assembler.o preprocessor.o passes_manager.o first_pass.o first_pass_commands.o data_inst_legality.o string_inst_legality.o struct_inst_legality.o second_pass.o common.o image.o symbolTable.o -o assembler

assembler.o: assembler.c
	gcc -Wall -ansi -pedantic assembler.c -c

preprocessor.o: preprocessor.c
	gcc -Wall -ansi -pedantic preprocessor.c -c

passes_manager.o: passes_manager.c
	gcc -Wall -ansi -pedantic passes_manager.c -c

first_pass.o: first_pass.c
	gcc -Wall -ansi -pedantic first_pass.c -c

first_pass_commands.o: first_pass_commands.c
	gcc -Wall -ansi -pedantic first_pass_commands.c -c

data_inst_legality.o: data_inst_legality.c
	gcc -Wall -ansi -pedantic data_inst_legality.c -c

string_inst_legality.o: string_inst_legality.c
	gcc -Wall -ansi -pedantic string_inst_legality.c -c

struct_inst_legality.o: struct_inst_legality.c
	gcc -Wall -ansi -pedantic struct_inst_legality.c -c

second_pass.o: second_pass.c
	gcc -Wall -ansi -pedantic second_pass.c -c

common.o: common.c
	gcc -Wall -ansi -pedantic common.c -c

image.o: image.c
	gcc -Wall -ansi -pedantic image.c -c

symbolTable.o: symbolTable.c
	gcc -Wall -ansi -pedantic symbolTable.c -c


clean:
	rm -rf *.o *.ob *.ent *.ext *.am
