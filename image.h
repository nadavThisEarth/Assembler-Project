#ifndef IMAGE_H
#define IMAGE_H
#include "common.h"
typedef char bin_str[BINARY_LENGTH + 1]; /* holds a line  whose length is a memory image's line length  */

/*
     structure that represents memory image (either code image or data image)
*/
typedef struct memory_image
{
    char **img; /* - holds the lines that construct the memory image (dynamically allocated and extended)  */
    int size;   /*   - holds the number of lines */
} Image;
#include <string.h> /* needed for strcpy and strlen */
#include <stdio.h>
#include <stdlib.h>
void addStringtoImage(Image *image_p, bin_str string);
void free_image(Image *image_p);
void replaceString(Image *image_p, int index, bin_str string);
void outputToObjectFile(FILE *fw, Image *code_img, Image *data_img)
;
#endif /* End of IMAGE_H*/
