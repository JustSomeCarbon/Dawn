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
void walk_number(FILE* sourcefile, struct tokenStack* stack, char first_num);
void walk_word(FILE* sourcefile, struct tokenStack* stack, char first_char);
void walk_symbol(FILE* sourcefile, struct tokenStack* stack);
int is_reserved_word(struct tokenStack* stack, char* word);
void walk_special_token(FILE* sourcefile,struct tokenStack* stack, char current_char);
void walk_line_comment(FILE* sourcefile);
void walk_multiline_comment(FILE* sourcefile);

int is_alpha_or_num(char character);
int is_alpha(char character);
int is_num(char character);

// line number of current position
int lineno = 1;
char* sourcefilename;

/**
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
            // create an EOL token
            append_to_stack(stack, build_token(EOL, "eol", lineno, sourcefilename));
            lineno += 1;
        } else if (current_char == 34) { // "
            // walk through a string
            walk_string(sourcefile, stack);
        } else if (current_char >= 48 && current_char <= 57) { // [0-9]
            // walk through number
            walk_number(sourcefile, stack, current_char);
        } else if (is_alpha(current_char) || (current_char == 95)) { // [A-Za-z_]
            // walk through defined word
            walk_word(sourcefile, stack, current_char);
        } else if (current_char == 32) {// space
            continue;
        } else {
            walk_special_token(sourcefile, stack, current_char);
        }
    }
    }

    return stack;
}

/**
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

/**
 * walk a string literal. when the closing double quote is found, create a token
 * of the string literal and add it to the stackptr.
 * @param sourcfile the solace source file
 * @param stackptr the pointer to the token stack
 */
void walk_string(FILE* sourcefile, struct tokenStack* stack) {
    int start_line = lineno;
    char* string_token = malloc(sizeof(char));
    int length = 1;
    *string_token = "\"";
    
    char next_character;
    while(next_character = walk(sourcefile) != EOF) {
        if (next_character == 34) { // "
            // end of string
            string_token = realloc(string_token, strlen(string_token) + 2);
            string_token[length] = '\"';
            string_token[length+1] = '\0';
            append_to_stack(stack, build_token(STRING_LITERAL, string_token, lineno, sourcefilename));
            return;
        }
        if (next_character == '\n') {
            lineno += 1;
        }
        string_token = realloc(string_token, length + 1);
        string_token[length] = next_character;
        length += 1;
    }
    if (next_character == EOF) {
        // end of the file
        free(string_token);
        char str_peek[15];
        strncpy(str_peek, string_token, 15);
        throwerr_unclosed_string_lex(sourcefile, str_peek, start_line);
        end_runtime(stack); // need to free stack
    }
}

/**
 * walks a number value until its completion. The number can be either an integer
 * or a floating point number. create a token of the integer literal and
 * append it to the stack.
 * @param sourcefile the solace source file
 * @param stack the token stack
 * @param first_num the first number character found
 */
void walk_number(FILE* sourcefile, struct tokenStack* stack, char first_num) {
    char* number_literal = (char*)malloc(sizeof(char));
    number_literal = first_num;
    int length = 1;
    int category = INT_LITERAL;

    char next_character;
    while ((next_character = walk(sourcefile)) != EOF) {
        if (is_alpha(next_character)) {
            throwerr_invalid_number_lex(sourcefilename, next_character, lineno);
        }
        if ((next_character == 32) || !(is_num(next_character)) || !(next_character == 46)) {
            // is a space, is not a number, or is not a dot
            // end of number
            number_literal = realloc(number_literal, sizeof(char) * (length + 1));
            number_literal[length] = '\0';
            append_to_stack(stack, build_token(category, number_literal, lineno, sourcefilename));
            break;
        } else if (next_character == 46) { // is a dot
            category = FLOAT_LITERAL;
        }
        number_literal = realloc(number_literal, sizeof(char) * (length + 1));
        number_literal[length] = next_character;
        length += 1;
    }
    if (next_character == EOF) {
        number_literal = realloc(number_literal, sizeof(char) * (length + 1));
        number_literal[length] = '\0';
        append_to_stack(stack, build_token(category, number_literal, lineno, sourcefilename));
    }
    // put the next character back
    ungetc(next_character, sourcefile);
}

/**
 * walks a word to its completion. Words are either reserved words or user defined
 * words. Creates a token with the completed word and appends it to the stack.
 * @param sourcefile the solace source file
 * @param stack the token stack
 * @param first_char the first character of the word
 */
void walk_word(FILE* sourcefile, struct tokenStack* stack, char first_char) {
    char* new_word = (char*)malloc(sizeof(char));
    new_word = first_char;
    char next_character;
    int length = strlen(new_word);

    while((next_character = walk(sourcefile)) != EOF) {
        // build a new word
        if (is_alpha_or_num(next_character) || (next_character == 95)) {
            new_word = realloc(new_word, sizeof(char) * (length + 1));
            new_word[length] = next_character;
            length += 1;
        } else {
            // end of the word
            new_word = realloc(new_word, sizeof(char) * (length + 1));
            new_word[length] = '/0';
            if (!(is_reserved_word(stack, new_word))) {
                append_to_stack(stack, build_token(USER_SYMBOL, new_word, lineno, sourcefilename));
            }
            break;
        }
    }
    // put the next character back
    ungetc(next_character, sourcefile);
}

void walk_symbol(FILE* sourcefile, struct tokenStack* stack) {
    char* new_symbol = (char*)malloc(sizeof(char));
    new_symbol = ':';
    char next_character;
    int length = 1;

    while((next_character = walk(sourcefile)) != EOF) {
        if (is_alpha(next_character)) {
            // build symbol
            new_symbol = realloc(new_symbol, sizeof(char) * (length+1));
            new_symbol[length] = next_character;
            length += 1;
        } else {
            // end symbol
            new_symbol = realloc(new_symbol, sizeof(char) * (length+1));
            new_symbol[length] = '\0';
            if (strcmp(new_symbol, reserved_words[16]) == 0) {
                append_to_stack(stack, build_token(SYMBOL_TYPE, new_symbol, lineno, sourcefilename));
            } else {
                append_to_stack(stack, build_token(SYMBOL_TYPE, new_symbol, lineno, sourcefilename));
            }
            break;
        }
    }
    ungetc(next_character, sourcefile);
}

/**
 * Takes a word and determines if it is a reserved word. If it is, the
 * reserved word category value is returned, if not 0.
 * @param word the given word to test
 */
int is_reserved_word(struct tokenStack* stack, char* word) {
    int length = sizeof(reserved_words) / sizeof(reserved_words[0]);
    for (int i = 0; i < length; i++) {
        if (strcmp(word, reserved_words[i]) == 0) {
            append_to_stack(stack, build_token((1101+i), word, lineno, sourcefilename));
            return 1;
        }
    }
    return 0;
}

/**
 * builds a token when encountering a special character. Unknown characters
 * throw an error and continue lexing.
 * @param sourcefile the solace source file
 * @param current_char the current special token found
 */
void walk_special_token(FILE* sourcefile, struct tokenStack* stack, char current_char) {
    // create special character token
    switch (current_char) {
        case 40: // (
            append_to_stack(stack, build_token(LEFT_PAREN, '(', lineno, sourcefilename));
            break;
        case 41: // )
            append_to_stack(stack, build_token(RIGHT_PAREN, ')', lineno, sourcefilename));
            break;
        case 46: // .
            append_to_stack(stack, build_token(DOT, '.', lineno, sourcefilename));
            break;
        case 58: // :
            char next = peek(sourcefile);
            if (next == 58) {
                // build a colon token
                append_to_stack(stack, build_token(COLON, "::", lineno, sourcefilename));
            } else {
                if (is_alpha(next)) {
                    // build a symbol token
                    walk_symbol(sourcefile, stack);
                } else {
                    // throw an error
                    throwerr_invalid_syntax(sourcefilename, next, "Invalid symbol definition", lineno);
                }
            }
            append_to_stack(stack, build_token(COLON, ':', lineno, sourcefilename));
            break;
        case 59: // ;
            append_to_stack(stack, build_token(SEMICOLON, ';', lineno, sourcefilename));
            break;
        case 91: // [
            append_to_stack(stack, build_token(LEFT_BRACKET, '[', lineno, sourcefilename));
            break;
        case 93: // ]
            append_to_stack(stack, build_token(RIGHT_BRACKET, ']', lineno, sourcefilename));
            break;
        case 123: // {
            append_to_stack(stack, build_token(LEFT_BRACE, '{', lineno, sourcefilename));
            break;
        case 125: // }
            append_to_stack(stack, build_token(RIGHT_BRACE, '}', lineno, sourcefilename));
            break;
        case 126: // ~
            if (peek(sourcefile) == 61) {
                append_to_stack(stack, build_token(MATCH_EQUALS, "~=", lineno, sourcefilename));
                current_char = walk(sourcefile); // =
            } else {
                throwerr_invalid_syntax(sourcefilename, current_char, "Expected an = character",  lineno);
            }
            break;
        case 124: // |
            if (peek(sourcefile) == 124) {
                append_to_stack(stack, build_token(OR, "||", lineno, sourcefilename));
            } else {
                append_to_stack(stack, build_token(BAR, '|', lineno, sourcefilename));
            }
            break;
        case 33: // !
            append_to_stack(stack, build_token(NOT, '!', lineno, sourcefilename));;
            break;
        case 38: // &
            char next = peek(sourcefile);
            // check if logical and, if not throw error
            if (next == 38) {
                append_to_stack(stack, build_token(AND, "&&", lineno, sourcefilename));
            } else {
                throwerr_invalid_syntax(sourcefilename, next, "Expected an & charcter", lineno);
            }
            break;
        case 42: // *
            if (peek(sourcefile) == 61) {
                append_to_stack(stack, build_token(MULTIPLY_EQUAL, "*=", lineno, sourcefilename));
            } else {
                append_to_stack(stack, build_token(MULTIPLY, '*', lineno, sourcefilename));
            }
            break;
        case 43: // +
            if (peek(sourcefile) == 61) {
                append_to_stack(stack, build_token(ADD_EQUAL, "+=", lineno, sourcefilename));
            } else {
                append_to_stack(stack, build_token(ADD, '+', lineno, sourcefilename));
            }
            break;
        case 45: // -
            if (peek(sourcefile) == 61) {
                append_to_stack(stack, build_token(SUBTRACT_EQUAL, "-=", lineno, sourcefilename));
            } else {
                append_to_stack(stack, build_token(SUBTRACT, '-', lineno, sourcefilename));
            }
            break;
        case 47: // /
            if (peek(sourcefile) == 47) { // /
                walk_line_comment(sourcefile);
            } else if (peek(sourcefile) == 42) { // *
                walk_multiline_comment(sourcefile);
            } else if (peek(sourcefile) == 61) {
                append_to_stack(stack, build_token(DIVIDE_EQUAL, "/=", lineno, sourcefilename));
            } else {
                append_to_stack(stack, build_token(DIVIDE, '/', lineno, sourcefilename));
            }
            break;
        case 60: // <
            if (peek(sourcefile) == 61) {
                append_to_stack(stack, build_token(LESS_THAN_OR_EQ, "<=", lineno, sourcefilename));
            } else {
                append_to_stack(stack, build_token(LESS_THAN, '<', lineno, sourcefilename));
            }
            break;
        case 61: // =
            if (peek(sourcefile) == 61) {
                append_to_stack(stack, build_token(IS_EQUAL_TO, "==", lineno, sourcefilename));
            } else {
                append_to_stack(stack, build_token(EQUALS, '=', lineno, sourcefilename));
            }
            break;
        case 62: // >
            if (peek(sourcefile) == 61) {
                append_to_stack(stack, build_token(GREATER_THAN_OR_EQ, ">=", lineno, sourcefilename));
            } else {
                append_to_stack(stack, build_token(GREATER_THAN, '>', lineno, sourcefilename));
            }
            break;
        case 63: // ?
            char next = peek(sourcefile);
            if (next == 62) {
                append_to_stack(stack, build_token(MATCH_HEAD, "?>", lineno, sourcefilename));
            } else {
                throwerr_invalid_syntax(sourcefilename, next, "Expected a > character", lineno);
            }
            break;
        default:
            // invalid character in syntax
            throwerr_invalid_syntax(sourcefilename, current_char, "Invalid character found", lineno);
            break;
    }
}

/**
 * walk the remainder of the line and discard everything
 * @param sourcefile the source file
 */
void walk_line_comment(FILE* sourcefile) {
    while (walk(sourcefile) != 10) {
        continue;
    }
    lineno += 1;
}

/**
 * walk the remainder of the line until a close multiline comment
 * symbol is found. Discard everything.
 * @param sourcefile the source file
 */
void walk_multiline_comment(FILE* sourcefile) {
    // consume the * character
    walk(sourcefile);
    // consume the first character in the comment
    char current_char = walk(sourcefile);
    while (!(current_char == '*' && peek(sourcefile) == 47)) {
        current_char = walk(sourcefile);
        if (current_char == 10) {
            lineno += 1;
        }
    }
    // consume the end of the multiline comment
    walk(sourcefile);
}

/**
 * checks the given character and determines if the character is an
 * alphabet character, or a number character. If not 0 is returned.
 * @param character the given character to test
 */
int is_alpha_or_num(char character) {
    if ((character >= 48 && character <= 57) || (character >= 65 && character <= 90) || (character >= 97 && character <= 122)) {
        return 1;
    }
    return 0;
}

/**
 * checks if the given character is an alphabet character. Returns 0 if not.
 * @param character the given character to test
 */
int is_alpha(char character) {
    // [A-Za-z]
    if ((character >= 65 && character <= 90) || (character >= 97 && character <= 122)) {
        return 1;
    }
    return 0;
}

/**
 * checks if the given character is a number character. Returns 0 if not.
 * @param character the given character to test
 */
int is_num(char character) {
    if (character >= 65 && character <= 90) {
        return 1;
    }
    return 0;
}