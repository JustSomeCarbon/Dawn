#ifndef SOLERR_H
#define SOLERR_H
/*
 * The error handling environment used by the solace compiler to keep track of different
 * errors that are raised throughout the runtime of the compiler.
 */

extern int errflag;
extern char* raised_errors;

typedef enum {
    NO_ERR = 1001,
    ERR_FILE_NOT_FOUND = 1002,
    ERR_INVALID_FILE_TYPE = 1003,
    ERR_FILE_UNABLE_OPEN = 1004,
    ERR_EOF_LEX = 1005,
    ERR_INVALID_NUM_LEX = 1006,
} err_code;

void throwerr_file_extension(char* filename);
void throwerr_file_open(char* filename);
void throwerr_unclosed_string_lex(char* filename, char peek, int lineno);
void throwerr_invalid_number_lex(char* filename, int lineno);
void throwerr_invalid_stack_height(int stack_height);


#endif