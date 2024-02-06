/*
 * File: symtable.h
 * Author: Noe Garcia
 * Date: January 30, 2024
 * Description: header file for the symbol table definition and prototypes. Structure
 *   definitions are defined first, followed by prototype function definitions that
 *   are implemented in the symtable.c file located in the same directory.
 */

#include "tree.h"

#ifndef SYMT_H
#define SYMT_H

// standardize the number of buckets to use.
#define B_SIZE 25;

extern SymbolTable root_symtable;
extern SymbolTable current_symtable;

/* STRUCTURE DEFINITIONS */

typedef struct sym_table
{
    int nbuckets; // number of buckets
    int nentries; // number of entries in the table
    char* symtable_name; // the name of the current symbol table

    struct sym_table* parent;   // parent of current table
    struct sym_entry** symtable; // entries of the table
} *SymbolTable;

typedef struct sym_entry
{
    SymbolTable table; // the table the entry belongs to
    char* name;        // the name of the symbol

    // The next entry in the table
    struct sym_entry* next;
} *SymbolEntry;


/* FUNCTION PROTOTYPES */

void populate_symboltable(struct tree* ast);
void enter_new_scope(char* scope_name);
SymbolTable generate_symboltable(int buckets, char* name);
SymbolEntry generate_new_entry(char* name);
int insert_symbol_entry(SymbolTable table, char* name);
SymbolTable lookup_symboltable(SymbolTable table, char* name);
SymbolEntry lookup_symbol_entry(SymbolTable table, char* name);
void free_symboltable(SymbolTable table);
void delete_symboltable(SymbolTable table);

#endif