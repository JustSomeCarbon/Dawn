#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "solace.h"
#include "slex.h"
#include "solerr.h"
#include "cleanup.h"

char* sourcefile = NULL;
struct tokenStack* stack = NULL;

void print_stack(struct tokenStack* stack);

/*
 * Solace Compiler: anvl
 */
int main(int argc, char* argv[]) {
    if (argc != 2) {
        perror("Error: invalide use of solace compiler\n");
        printf("Usage: ./anvl <sourcefilename.solc>\n\n");
    }

    // obtain the source file name
    strcpy(sourcefile, argv[1]);
    valid_solace_source_file(sourcefile);

    // lex the given source file
    stack = lex_source_file(sourcefile);
    
    // free the token stack
    free_token_stack(stack);

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
        end_runtime(stack);
    }
}

/**
 * pretty print the stack.
 * remove when done testing
 * @param stack the token stack
 */
void print_stack(struct tokenStack* stack) {
    struct tokenStackPtr* ptr = stack->head;
    while (ptr) {
        if (ptr->tok.category == EOL) {
            printf("Token<EOL>\n");
        } else {
            printf("Token<%d:%s>  ", ptr->tok.category, ptr->tok.literal);
        }
        ptr = ptr->next;
    }
    printf("Source file: %s\n", sourcefile);
    printf("Stack height: %d\n", stack->stack_height);
}