#ifndef SOLC_H
#define SOLC_H

extern char* sourcefile;
extern struct tokenStack* stack;

void valid_solace_source_file(char* givenfile);

#endif