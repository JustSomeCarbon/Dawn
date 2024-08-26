#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "solace.h"
#include "solerr.h"
#include "cleanup.h"

char* sourcefile = NULL;

/*
 * Solace Compiler: anvl
 */
int main(int argc, char* argv) {
    if (argc != 2) {
        perror("Error: invalide use of solace compiler\n");
        printf("Usage: ./anvl <sourcefilename.solc>\n\n");
    }

    // obtain the source file name
    strcpy(sourcefile, argv[1]);
    valid_solace_source_file(sourcefile);

    return 0;
}

/*
 * determine if the given source file is a valid solace source file. If not,
 * return null instead of the source file as a char*.
 * @param givenfile the source file name expected to be a solace source file
 */
void valid_solace_source_file(char* givenfile) {
    char* extension = strrchr(givenfile, '.');
    if (!extension && strcmp(extension, ".solc") != 0) {
        throwerr_file_extension(givenfile);
        end_runtime();
    }
}