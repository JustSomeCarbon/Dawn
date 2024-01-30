/*
 * File: symtable.h
 * Author: Noe Garcia
 * Date: January 30, 2024
 * Description: header file for the symbol table definition and prototypes. Structure
 *   definitions are defined first, followed by prototype function definitions that
 *   are implemented in the symtable.c file located in the same directory.
 */

#ifndef SYMT_H
#define SYMT_H

// standardize the number of buckets to use.
#define B_SIZE 48;


/* STRUCTURE DEFINITIONS */

typedef struct sym_table
{
    int nbuckets; // number of buckets
    int nentries; // number of entries in the table

    struct sym_table* parent;   // parent of current table
    struct sym_entry** entries; // entries of the table
} *SymbolTable;

typedef struct sym_entry
{
    SymbolTable table; // the table the entry belongs to
    char* name;        // the name of the symbol

    // The next entry in the table
    struct sym_entry* next;
} *SymbolEntry;


/* FUNCTION PROTOTYPES */

SymbolTable generate_symboltable(int buckets, char* name);
void delete_symboltable(SymbolTable table);
int insert_symbol_entry(SymbolTable table, char* name);
SymbolTable lookup_symboltable(SymbolTable table, char* name);
SymbolEntry lookup_symbol_entry(SymbolTable table, char* name);

#endif