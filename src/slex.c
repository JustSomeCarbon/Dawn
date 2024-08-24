/*
 * The source lexing file used to tokenize source solace files.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slex.h"
#include "solerr.h"

char walk(FILE* sourcefile);
char peek(FILE* sourcefile);
void walk_string(FILE* sourcefile, struct tokenStack* stackptr);
void walk_number(FILE* sourcefile, struct tokenStack* stackptr);

// line number of current position
int lineno = 1;

/*
 * Takes the source file name and walks through the file to create
 * individual tokens that are stored into a stack that is
 * returned once the whole file is lexed.
 * @param filename the name of the solace source file
 */
struct tokenStack* lex_source_file(char* file_name) {
    FILE* sourcefile = fopen(file_name, "r");
    if (!sourcefile) {
        // throw an error and exit
        throwerr_file_open(file_name);
        end_runtime();
    }

    struct tokenStack* stackptr = NULL;
    struct tokenStack* head = stackptr;

    // walk the file and create tokens
    char current_char = fgetc(sourcefile);

    if (current_char != EOF) {
        if (current_char == 10) { // character: /n
            lineno += 1;
            // create an EOL token
        } else if (current_char == 34) { // character: \"
            // walk through a string
        } else if (current_char >= 48 && current_char <= 57) { // character: 0-9
            // walk through number
        }
    }

    return head;
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
void walk_string(FILE* sourcefile, struct tokenStack* stackptr) {
    char* string_token = malloc(sizeof(char));
    *string_token = "\"";
    
    char next_character;
    while(next_character = walk(sourcefile)) {
        if (next_character == EOF) {
            // end of the file
            free(string_token);
            char peek[10];
            strncpy(peek, string_token, 10);
            throwerr_unclosed_string_lex(sourcefile, peek, lineno);
        } else if (next_character == 34) {
            // end of string
            // create token
            break;
        }
        string_token = realloc(string_token, strlen(string_token) + 1);
        string_token[strlen(string_token)] = next_character;
    }
}