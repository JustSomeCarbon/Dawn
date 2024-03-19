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

// global number of buckets to use
#define B_SIZE 25;

extern struct sym_table* root_symtable;
extern struct sym_table* current_symtable;

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

SymbolTable build_symtable(struct tree* ast);
void populate_symboltable(struct tree* ast, SymbolTable current_table);
SymbolTable enter_new_scope(SymbolTable current_scope, char* scope_name);
SymbolTable generate_symboltable(int buckets, char* name);
SymbolEntry generate_new_entry(char* name);
int insert_symbol_entry(SymbolTable table, char* name);
SymbolTable lookup_symboltable(SymbolTable table, char* name);
SymbolEntry lookup_symbol_entry(SymbolTable table, char* name);
void print_symtable(SymbolTable table);
void free_symboltable(SymbolTable table);


char* obtain_name(struct tree* ast);
void func_body_walkthrough(struct tree* ast, SymbolTable current_symtable);


#endif