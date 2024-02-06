/*
 * File: symtable.c
 * Author: Noe Garcia
 * Date: January 30, 2024
 * Description: Contains all the function implementations for the symbol table functionality
 *   of the Solace compiler.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "symtable.h"

int hash(SymbolTable table, char* val);
char* alloc(int n);


/*
 * Traverse through the given abstract syntax tree and populate the symbol tables for each
 * nested scope in the given tree.
 */
void populate_symboltable(struct tree* ast)
{
    //
}

/*
 * Takes a new scope name as a character pointer and enters into a new scope
 * by creating a new symbol table and adding it as an entry to the current symbol
 * table. Nothing is returned.
 */
void enter_new_scope(char* scope_name)
{
    //
}

/*
 * generates a new symbol table with a given ammount of buckets as an integer value
 * and a name for the new symbol table as a character pointer. A pointer to the new
 * symbol table is returned.
 */
SymbolTable generate_symboltable(int buckets, char* name)
{
    SymbolTable new_table = (SymbolTable)calloc(1, sizeof(struct sym_table));
    new_table->nbuckets = buckets;
    new_table->symtable = 0;
    new_table->symtable_name = strdup(name);
    new_table->symtable = (SymbolTable*)calloc(buckets, sizeof(struct sym_entry));

    return new_table;
}

SymbolEntry generate_new_entry(char* name)
{
    SymbolEntry new_entry = (SymbolEntry)calloc(1, sizeof(struct sym_entry));
    new_entry->name = strdup(name);

    return new_entry;
}

/*
 * Takes a symbol table and the name of the new symbol entry and generates a new symbol
 * table entry for the given symbol table. returns the index value for the location of the new
 * symbol entry.
 */
int insert_symbol_entry(SymbolTable table, char* name)
{
    SymbolEntry entry = generate_new_entry(name);
    int index = hash(table, name);

    SymbolEntry insert_chain = table->symtable[index];
    if (insert_chain == NULL) {
        table->symtable[index] = entry;
        return index;
    }
    while (insert_chain->next != NULL) {
        insert_chain = insert_chain->next;
    }
    insert_chain->next = entry;

    return index;
}

SymbolTable lookup_symboltable(SymbolTable table, char* name)
{
    //
}

SymbolEntry lookup_symbol_entry(SymbolTable table, char* name)
{
    //
}

/*
 * Takes a symbol table and frees all successive symbol tables bellow it.
 * Nothing is returned.
 */
void free_symboltable(SymbolTable table)
{
    //
}

/*
 * Takes an entry to a symbol table and frees the allocated memory associated
 with the symbol value. Nothing is returned.
 */
void free_symbolentry(SymbolEntry entry)
{
    //
}

/*
 * Takes a symbol table and frees its data from memory. Nothing gets returned.
 */
void delete_symboltable(SymbolTable table)
{
    //
}

/*
 * Takes a symbol table and a character pointer as an input value and determines
 * the index of the given value to the hash table of the symbol table.
 */
int hash(SymbolTable table, char* val)
{
    register char current_char;
    register int sum = 0;
    while (current_char = *val++)
    {
        sum += current_char & 0377;
        sum *= 13;
    }
    if (sum < 0) sum = -sum;
    return sum % table->nbuckets;
}

/*
 * takes a given string size and allocates the memory on the heap as zeroed our values
 * and returns the allocated pointer to that memory.
 */
char* alloc(int n)
{
    //
}