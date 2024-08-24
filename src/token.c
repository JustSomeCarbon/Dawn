/*
 * Token handling functions including creating new tokens, traversing tokens, freeing
 * the data stored within tokens, etc.
 */

#include <stdio.h>
#include <stdlib.h>
#include "slex.h"

/*
 * Construct a new token and return it.
 * @param category the category integer identifier for the token
 * @param literal_val the raw representation of the token
 * @param lineno the location of the token in the source file
 * @param sourcefile the solace source file the token is from
 */
struct token build_token(int category, char* literal_val, int lineno, char* sourcefile) {
    struct token new_token = (struct token){}; // initialize
    return new_token;
}