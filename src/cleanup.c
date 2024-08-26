/*
 * Contains all the logic for cleaing up the compiler runtime.
 */

#include <stdio.h>
#include <stdlib.h>
#include "solerr.h"


/*
 * Exits the compiler, cleaning up anything that needs to be cleaned up before
 * terminating the compiler.
 */
void end_runtime() {
    // cleanup runtime space...

    // always ran last
    exit(errflag);
}