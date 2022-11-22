
#include "passes_manager.h"
/**
 * manages the passes of input files.
 * initiates symbol table and memory images.
 * @param argc - number arguments counter from command line
 * @param argv - list of command line arguments
 * */
void manage_passes(int argc, char **argv)
{

    int i;
    char file_name[MAX_FILENAME_SIZE]; /*  holds file from command line input*/
    char am_file[MAX_FILENAME_SIZE];   /*  holds file from command line input with '.am' extension */
    Boolean pass1_success;             /*  Boolean to represent success on first pass*/
    Boolean pass2_success;             /*  Boolean to represent success on second pass*/
    FILE *fd;                          /*  file descriptor  */

    node *head = NULL;                      /* Creating Symbol Table*/
    Image codeImageObject, dataImageObject; /*  structure instances of memory image  */

    /**
     * Pointers to above instances of memory image.
     * code_img - holds CODE memory image.
     * data_img - holds DATA memory image.
     */
    Image *code_img = &codeImageObject;
    Image *data_img = &dataImageObject;

    for (i = 1; i < argc; i++) /* iterating over input files  */
    {
        strcpy(file_name, argv[i]); /* keep suffix-free version of file name (for example , "file1" ) */
        strcpy(am_file, file_name); /*copy the  command-line provided  file name  to a second array*/
        strcat(am_file, ".am");     /*add .am suffix to the name of the file*/

        fd = fopen(am_file, "r");
        if (!fd) /* in case current file can't be opened , continue (already handled in preproccesor phase) */
            continue;

        pass1_success = FirstPass(fd, am_file, code_img, data_img, &head);
        if (!pass1_success) /* if first pass has errors , free images and symbol table and continue to next input file */
        {
            free_image(code_img);
            free_image(data_img);
            freeSymbolTable(&head);
            continue;
        }
        else /* if first pass succeeded  */
        {
            if (fseek(fd, 0L, SEEK_SET) != 0) /*  rewind fd for use with second pass */
            {
                printf("ERROR - Can't rewind file %s\n", am_file);
                continue;
            }

            pass2_success = SecondPass(fd, am_file, code_img, &head);
        }

        if (!pass2_success) /* if second pass has errors , free memory images and symbol table and continue to next input file */
        {

            free_image(code_img);
            free_image(data_img);
            freeSymbolTable(&head);
            continue;
        }
        else /* if second pass succeeded , create outputfiles and free  memory images and symbol table */
        {

            create_output_files(&head, code_img, data_img, file_name);
            free_image(code_img);
            free_image(data_img);
            freeSymbolTable(&head);
        }
    }
    if (fd) /*  close file descriptor only if last file opening was possible*/
        fclose(fd);
}
/** creates output files due to successful second pass
 * @param head - pointer to pointer to first item in symbol table
 * @param  code_img - pointer to code image
 * @param data_img - pointer to data image
 * @param  file_name - string holding command line input file name
 * */
void create_output_files(node **head, Image *code_img, Image *data_img, char *file_name)
{
    FILE *fw; /*  file descriptor for writing to file */

    char entries_file[MAX_FILENAME_SIZE]; /*  string that holds name of entries array (shall it  be created)  */
    char externs_file[MAX_FILENAME_SIZE]; /*  string that holds name of externs mentions array ( shall  it be created)  */
    char object_file[MAX_FILENAME_SIZE];  /*  string that holds name of object file  */
    if (hasEntries(head))                 /* if input files contains symbols which are entries  , create output file*/
    {
        strcpy(entries_file, file_name); /*copy the  command-line provided  file name to a string representing entries file */
        strcat(entries_file, ".ent");    /*add .ent suffix to the name of the file*/
        fw = fopen(entries_file, "w");
        if (!fw)
        {
            printf("\n cannot open file %s for writing\n", entries_file);
            exit(-1);
        }
        output_entries(fw, head); /*  write entries to '.ent' file */
    }
    if (hasExterns(head)) /* if input files contains symbols which are externs  , create output file*/
    {
        strcpy(externs_file, file_name); /*copy the  command-line provided  file name to a string representing extern mentions file */
        strcat(externs_file, ".ext");    /*add .ext suffix to the name of the file*/
        fw = fopen(externs_file, "w");
        if (!fw)
        {
            printf("\n cannot open file %s for writing\n", externs_file);
            exit(-1);
        }
        output_externs(fw, head); /* write externs mentions to '.ext' file  */
    }
    strcpy(object_file, file_name); /* copy the  command-line provided  file name to a string representing object file */
    strcat(object_file, ".ob");     /*add .ob suffix to the name of the file*/
    fw = fopen(object_file, "w");
    if (!fw)
    {
        printf("\n cannot open file %s for writing\n", object_file);
        exit(-1);
    }
    outputToObjectFile(fw, code_img, data_img); /* write code and data image to '.ob' file */

    fclose(fw); /*  close file descriptor  */
}
