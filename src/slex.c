/*
 * The source lexing file used to tokenize source solace files.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slex.h"
#include "solerr.h"
#include "cleanup.h"

char walk(FILE* sourcefile);
char peek(FILE* sourcefile);
void walk_string(FILE* sourcefile, struct tokenStack* stack);
void walk_number(FILE* sourcefile, struct tokenStack* stack);

// line number of current position
int lineno = 1;
char* sourcefilename;

/*
 * Takes the source file name and walks through the file to create
 * individual tokens that are stored into a stack that is
 * returned once the whole file is lexed.
 * @param filename the name of the solace source file
 */
struct tokenStackPtr* lex_source_file(char* file_name) {
    sourcefilename = strndup(file_name, strlen(file_name));
    FILE* sourcefile = fopen(file_name, "r");
    if (!sourcefile) {
        // throw an error and exit
        throwerr_file_open(file_name);
        end_runtime(NULL);
    }

    // maintains our position within the stack
    struct tokenStackPtr* stackptr = NULL;
    // maintains the head, tail, and height of the stack
    struct tokenStack* stack = (struct tokenStack*)malloc(sizeof(struct tokenStackPtr));
    stack->head = stackptr;
    stack->tail = stackptr;
    stack->stack_height = 0;

    // current character in source file
    char current_char;

    // walk through the source file
    while ((current_char = walk(sourcefile)) != EOF) {
    if (current_char != EOF) {
        if (current_char == 10) { // /n
            lineno += 1;
            // create an EOL token
        } else if (current_char == 34) { // \"
            // walk through a string
        } else if (current_char >= 48 && current_char <= 57) { // [0-9]
            // walk through number
        } else if ((current_char >= 65 && current_char <= 90) || (current_char >= 97 && current_char <= 122) || (current_char == 95)) { // [A-Za-z_]
            // walk through defined word
        } else {
            build_special_token(sourcefile, current_char);
        }
    }
    }

    return stack;
}

/*
 * walk the source file and consume a character, return the character.
 * @param sourcefile the solace source file
 */
char walk(FILE* sourcefile) {
    return fgetc(sourcefile);
}

/*
 * peeks at the next character in the source file and returns it.
 * @param sourcefile the solace source file
 */
char peek(FILE* sourcefile) {
    char next_char = fgetc(sourcefile);
    if (next_char != EOF) {
        ungetc(next_char, sourcefile);
    }
    return next_char;
}

/*
 * walk a string literal. when the closing double quote is found, create a token
 * of the string literal and add it to the stackptr.
 * @param sourcfile the solace source file
 * @param stackptr the pointer to the token stack
 */
void walk_string(FILE* sourcefile, struct tokenStack* stack) {
    char* string_token = malloc(sizeof(char));
    *string_token = "\"";
    
    char next_character;
    while(next_character = walk(sourcefile) != EOF) {
        if (next_character == 34) {
            // end of string
            // create token
            string_token = realloc(string_token, strlen(string_token) + 1);
            string_token[strlen(string_token)] = '\0';
            append_to_stack(stack, build_token(STRING_LITERAL, string_token, lineno, sourcefilename));
            break;
        }
        string_token = realloc(string_token, strlen(string_token) + 1);
        string_token[strlen(string_token)] = next_character;
    }
    if (next_character == EOF) {
        // end of the file
        free(string_token);
        char peek[10];
        strncpy(peek, string_token, 10);
        throwerr_unclosed_string_lex(sourcefile, peek, lineno);
        end_runtime(stack); // need to free stack
    }
}

void build_special_token(FILE* sourcefile, char current_char) {
    // create special character token
    switch (current_char) {
        case 40: // (
            break;
        case 41: // )
            break;
        case 91: // [
            break;
        case 93: // ]
            break;
        case 123: // {
            break;
        case 125: // }
            break;
        case 126: // ~
            break;
        case 124: // |
            break;
        default:
            break;
    }
}