#ifndef MANAGER_H
#define MANAGER_H
#include "first_pass.h"
#include "second_pass.h"
void manage_passes(int argc, char **argv);

void create_output_files(node **head, Image *code_img, Image *data_img, char *file_name);


#endif