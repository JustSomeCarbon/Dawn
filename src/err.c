/*
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "err.h"

void check_allocation(void* mem, char* msg) {
    if (mem == NULL) {
        printf("%s", msg);
    }
    // invoke cleanup protocols
    
    // exit
    exit(-1);
}