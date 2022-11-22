#include "image.h"
/** adds a binary input string to memory image.
 * @param image_p - pointer to image object
 * @param string - binary string to be added
 * */
void addStringtoImage(Image *image_p, bin_str string)
{

    char **real; /* auxillary variable for realloc function  */
    real = realloc((image_p->img), (image_p->size + 1) * sizeof(char *));
    (image_p->img) = real;
    if (!(image_p->img)) /* Null Check of realloc   */
    {
        printf("ERROR - REALLOC FAILED\n");
        exit(-1);
    }
    image_p->size++; /* update size after safe  realloc  */

    (image_p->img)[image_p->size - 1] = malloc((BINARY_LENGTH + 1) * sizeof(char));
    if (!((image_p->img)[image_p->size - 1]))
    { /* Null Check of malloc   */
        printf("ERROR - MALLOC FAILED\n");
        exit(-1);
    }
    strcpy((image_p->img)[image_p->size - 1], string);
}
/**
 * frees all lines in given memory image,  frees image itself and updates number of lines to zero.
 * @param image_p - pointer to image object
 * */
 void free_image(Image *image_p)
{
    int i;
    for (i = 0; i < (image_p)->size; i++)
    {
        free((void *)(image_p->img)[i]);
    }
    free(image_p->img);
    image_p->size = 0;
}
/**
 * replaces an exisitng dummy binary string in memory image with a binary input string
 * @param image_p - pointer to image object
 * @param index - index of string to be replaced
 * @param string - binary string to be embedded
 * */
void replaceString(Image *image_p, int index, bin_str string)
{
    strcpy((image_p->img)[index], string);
}
/**
 * writes the sizes of code image and data image + contents of those images in 32 base representation to an '.ob' file
 * @param fw file descriptor set for writing to '.ob' file
 * @param code_img pointer to code image
 * @param data_img pointer to data image
 * */
void outputToObjectFile(FILE *fw, Image *code_img, Image *data_img)
{
    int i; /*  varaible for loop iteration */

    char *code_image_size_32_base; /* represents size of code image (number of line) in base 32   */
    char *data_image_size_32_base; /* represents size of data image (number of line) in base 32   */
    char *address_in_32;           /* eventually holds 32 based string representing  address in memory image*/
    char *binary_line_in_32;       /* eventually holds 32 based string representing  binary line in memory image*/

    code_image_size_32_base = decimal_to_32(code_img->size, TRUE);
    data_image_size_32_base = decimal_to_32(data_img->size, TRUE);

    fprintf(fw, "\t%s %s", code_image_size_32_base, data_image_size_32_base); /*  write to object file the sizes of code and data memory images in 32 base */

    free(code_image_size_32_base); /* free malloced pointers */
    free(data_image_size_32_base);

    for (i = 0; i < code_img->size; i++) /*  for loop that prints to object address and content of each line in code image in 32 base */
    {
        address_in_32 = decimal_to_32(i + IC_OFFSET, FALSE);  /*  convert address number to 32 base  */
        binary_line_in_32 = binary_to_32((code_img->img)[i]); /* convert content of line in code image to base 32 */

        fprintf(fw, "\n%s\t\t%s", address_in_32, binary_line_in_32); /* print to object file   */
        free(address_in_32);                                         /*  free malloced pointers */
        free(binary_line_in_32);
    }
    for (i = 0; i < data_img->size; i++) /*  for loop that prints to object address and content of each line in data image in 32 base */
    {
        address_in_32 = decimal_to_32(i + IC_OFFSET + code_img->size, FALSE); /*  convert address number to 32 base  */
        binary_line_in_32 = binary_to_32((data_img->img)[i]);                 /* convert content of line in data image to base 32 */

        fprintf(fw, "\n%s\t\t%s", address_in_32, binary_line_in_32); /* print to object file   */
        free(address_in_32);                                         /*  free malloced pointers */
        free(binary_line_in_32);
    }
}