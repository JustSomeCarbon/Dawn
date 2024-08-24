#include <stdio.h>
#include "solerr.h"

int errflag = 0;

/*
 * Raises an error for the given filename as it is not a solace
 * source file.
 * @param filename the name of the source file
 */
void throwerr_file_extension(char* filename) {
    fprintf(stderr, "Error:%d: given file '%s' is not a Solace source file\n\tSolace source files end with the '.solc' file extension\n\n", ERR_INVALID_FILE_TYPE, filename);
    errflag = 1;
    return;
}

/*
 * logs an error stating that the given file is not able to be opened.
 * @param filename the name of the source file
 */
void throwerr_file_open(char* filename) {
    fprintf(stderr, "Error:%d: given file '%s' cannot be openend\n\n");
    errflag = 1;
    return;
}