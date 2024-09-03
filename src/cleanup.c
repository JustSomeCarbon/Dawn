/*
 * Contains all the logic for cleaing up the compiler runtime.
 */

#include <stdio.h>
#include <stdlib.h>
#include "solerr.h"
#include "slex.h"


/*
 * Exits the compiler, cleaning up anything that needs to be cleaned up before
 * terminating the compiler.
 * @param stack the token stack used for lexing
 */
void end_runtime(struct tokenStack* stack) {
    // cleanup runtime space...
    free_token_stack(stack);

    // always ran last
    exit(errflag);
}