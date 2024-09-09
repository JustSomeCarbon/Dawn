#include <stdio.h>
#include "solerr.h"

// maintains the number of errors in the source file
int errflag = 0;

/**
 * Raises an error for the given filename as it is not a solace
 * source file.
 * @param filename the name of the source file
 */
void throwerr_file_extension(char* filename) {
    fprintf(stderr, "Error:%d: given file '%s' is not a Solace source file\n\tSolace source files end with the '.solc' file extension\n\n", ERR_INVALID_FILE_TYPE, filename);
    errflag++;
    return;
}

/**
 * logs an error stating that the given file is not able to be opened.
 * @param filename the name of the source file
 */
void throwerr_file_open(char* filename) {
    fprintf(stderr, "Error:%d: given file '%s' cannot be openend\n\n", ERR_FILE_UNABLE_OPEN, filename);
    errflag++;
    return;
}

/**
 * logs an error that a string in the source file was not closed.
 * @param filename the name of the source file
 * @param str_peek a peek of the beginning of the string
 * @param lineno the line number the string begins at
 */
void throwerr_unclosed_string_lex(char* filename, char* str_peek, int lineno) {
    fprintf(stderr, "Error:%d::%d:: string '%s' not closed by end of file\n\n", ERR_EOF_LEX, lineno, str_peek);
    errflag++;
    return;
}

/**
 * logs an error that the number given is not valid
 * @param filename the name of the source file
 * @param lineno the line number or the error
 */
void throwerr_invalid_number_lex(char* filename, int lineno) {
    // might not need this???
    errflag++;
    return;
}

/**
 * logs an error that the stack height does not correspond with the
 * number of tokens within the stack.
 * @param stack_height the remaining height of the stack
 */
void throwerr_invalid_stack_height(int stack_height) {
    fprintf(stderr, "Error:%d: Invalid lexing token stack height\n\tRemaining: %d", ERR_STACK_HEIGHT, stack_height);
    errflag++;
    return;
}

/**
 * logs an error detailing that some syntax error has occured.
 * @param filename the name of the source file
 * @param str_peek the character where the error occured
 * @param description a passed description of the syntax error
 * @param lineno the line that the error occured
 */
void throwerr_invalid_syntax(char* filename, char* str_peek, char* description, int lineno) {
    fprintf(stderr, "Error:%d::%d:: invalid syntax at '%s'\n\t%s", ERR_INVALID_SYNTAX, lineno, str_peek, description);
    errflag++;
    return;
}