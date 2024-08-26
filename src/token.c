/*
 * Token handling functions including creating new tokens, traversing tokens, freeing
 * the data stored within tokens, etc.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slex.h"

/*
 * Construct a new token and return it.
 * @param category the category integer identifier for the token
 * @param string_val the raw representation of the token
 * @param lineno the location of the token in the source file
 * @param sourcefile the solace source file the token is from
 */
struct token build_token(int category, char* string_val, int lineno, char* sourcefile) {
    char* token_string = strncpy(token_string, string_val, strlen(string_val));
    char* file = strncpy(file, sourcefile, strlen(sourcefile));

    union literalValue lit_val;
    switch (category)
    {
    case INT_LITERAL:
        lit_val.ival = atoi(token_string);
        break;
    case FLOAT_LITERAL:
        lit_val.dval = atof(token_string);
        break;
    case CHAR_LITERAL:
        lit_val.cval = string_val;
        break;
    case STRING_LITERAL:
        lit_val.sval = malloc(strlen(token_string) + 1);
        strncpy(lit_val.sval, token_string, strlen(token_string));
        break;
    default:
        lit_val.sval = NULL;
        break;
    }
    struct token new_token = (struct token){token_string, lineno, category, file, lit_val};
    return new_token;
}

/*
 * Append a new token to the token stack
 * @param stack_ptr the stack pointer
 * @param new_token the token to be appended to the stack
 */
void append_to_stack(struct tokenStackPtr* stack_ptr, struct token new_token) {
    stack_ptr->tail->next = &new_token;
    stack_ptr->stack_height += 1;
}

/*
 * pops the top token from the stack and returns it.
 */
struct token pop_token(struct tokenStackPtr* stack) {
    struct token top_token = stack->head->tok;
    stack->head = stack->head->next;
    stack->stack_height -= 1; // test stack height?
    return top_token;
}

/*
 * takes a token stack and frees all the associated tokens
 * in memory. Nothing is returned.
 * @param stack the token stack
 */
void free_token_stack(struct tokenStackPtr* stack) {
    if (stack->stack_height == 0) {
        return;
    }
    while (stack->head != stack->tail) {
        struct token tok = pop_token(stack);
        free_token(tok);
    }
    if (stack->stack_height != 0) {
        // throw an error
    }
}

/*
 * takes a token and frees any memory allocations.
 * @param tok the given token
 */
void free_token(struct token tok) {
    if (tok.category == STRING_LITERAL)
        free(tok.val.sval);
}