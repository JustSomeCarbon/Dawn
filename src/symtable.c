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
#include "sgram.tab.h"
#include "tree.h"
#include "symtable.h"

extern struct sym_table* root_symtable;

int hash(SymbolTable table, char* val);
char* alloc(int n);


/*
 *
 */
SymbolTable build_symtable(struct tree* ast)
{
    // initialize the root of the symbol table
    if (ast->kids[0]->nkids != 1) {
        /* throw an error, complex module space defined */
        /* ex: space:Foo:Bar */
    }

    // call the population symtable function and return the global symbol table
    return root_symtable;
}


/*
 * Traverse through the given abstract syntax tree and populate the symbol tables for each
 * nested scope in the given tree. returns the overarching symbol table of the compiled program.
 */
SymbolTable populate_symboltable(struct tree* ast)
{
    // look through the ast and create new symbol tables
    if (ast == NULL)
    {
        // end of the abstract syntax tree
        return;
    }

    switch (ast->prodrule)
    {
    case FILE_ROOT:
        /* define the domule space tree */
        break;

    default:
        break;
    }

    // return the root symbol table
}

/*
 * Takes a new scope name as a character pointer and enters into a new scope
 * by creating a new symbol table and adding it as an entry to the current symbol
 * table. Nothing is returned.
 */
SymbolTable enter_new_scope(SymbolTable current_scope, char* scope_name)
{
    int buckets = B_SIZE;
    // create a new entry and create a new scope
    SymbolTable new_table = generate_symboltable(buckets, scope_name);
    insert_symbol_entry(current_scope, scope_name);
    SymbolEntry new_entry = lookup_symbol_entry(current_scope, scope_name);
    new_entry->table = new_table;

    // set the new current scope to
    current_scope = new_table;
    return current_scope;
}

/*
 * generates a new symbol table with a given ammount of buckets as an integer value
 * and a name for the new symbol table as a character pointer. A pointer to the new
 * symbol table is returned.
 */
SymbolTable generate_symboltable(int buckets, char* name)
{
    SymbolTable new_table = (SymbolTable)alloc(sizeof(struct sym_table));

    new_table->nbuckets = buckets;
    new_table->symtable = 0;
    new_table->symtable_name = strdup(name);
    new_table->symtable = (SymbolTable*)alloc((unsigned int)(buckets * sizeof(SymbolEntry)));

    return new_table;
}

/*
 * takes a new string and generates a new symbol table entry with the given string as
 * the name.
 */
SymbolEntry generate_new_entry(char* name)
{
    SymbolEntry new_entry = (SymbolEntry)alloc(sizeof(struct sym_entry));
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

/*
 * takes a symbol table and a symbol table name as a string and checks to
 * try and find the symbol table. If the table is not found, NULL is returned.
 */
SymbolTable lookup_symboltable(SymbolTable table, char* name)
{
    return NULL;
}

/*
 * takes a symbol table and an entry and tries to look for the entry within the given
 * symbol table. If the entry is not found, NULL is returned.
 */
SymbolEntry lookup_symbol_entry(SymbolTable table, char* name)
{
    int index = hash(table, name);
    SymbolEntry entry = table->symtable[index];
    while(strcmp(entry->name, name) != 0) {
        entry = entry->next;
        if (entry == NULL) {
            return NULL;
        }
    }
    return entry;
}

/*
 * Takes a symbol table and frees all successive symbol tables bellow it.
 * Nothing is returned.
 */
void free_symboltable(SymbolTable table)
{
    // free all chained entries in the hash table
    for (int i = 0; i < table->nbuckets; i++) {
        free_symbolentry(table->symtable[i]);
    }
    // free the symbol table on memory
    free(table->symtable);
    free(table);
}

/*
 * Takes an entry to a symbol table and frees the allocated memory associated
 with the symbol value. Nothing is returned.
 */
void free_symbolentry(SymbolEntry entry)
{
    // if there is no entry, return
    if (entry == NULL) {
        return;
    }
    if (entry->table != NULL) {
        free_symboltable(entry->table);
    }
    if (entry->next != NULL) {
        free_symbolentry(entry->next);
    }
    // free the entry
    free(entry->name);
    free(entry);
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
 * Takes a given size for an object and allocates memory on the heap and checks that
 * the memory allocated correctly, if not throws an error and exits the compiler
 */
char* alloc(int n)
{
    char* new_allocation = calloc(n, sizeof(char));
    if (!new_allocation) {
        // throw an error
        printf("\nError: no more memory to allocate\n\tfailed on %d bytes\n\n", n);
        exit(-1);
    }

    return new_allocation;
}