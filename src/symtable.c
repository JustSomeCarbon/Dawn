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
 * initializes a symbol table and calls the populate symbol table
 * function to walk through the ast and populate the table
 * Returns
 */
SymbolTable build_symtable(struct tree* ast)
{
    // create a symbol table to use
    char* name = obtain_name(ast->kids[0]);
    int buckets = B_SIZE;
    SymbolTable symtable = generate_symboltable(buckets, name);
    free(name);
    populate_symboltable(ast, symtable);

    return symtable;
}


/*
 * Traverse through the given abstract syntax tree and populate the symbol tables for
 * each nested scope in the given tree. returns the overarching symbol table of the
 * compiled program.
 * All of the heavy lifting involved in moving through the abstract syntax tree and
 * building the symbol table is handled by this function.
 */
void populate_symboltable(struct tree* ast, SymbolTable current_table)
{
    // look through the ast and create new symbol tables
    if (ast == NULL)
    {
        // end of the abstract syntax tree
        return;
    }

    /*
    Assess production rules for the given abstract syntax tree given
    */
    switch (ast->prodrule)
    {
    case FILE_ROOT:
        if (ast->nkids == 1) {
            return;
        }
        populate_symboltable(ast->kids[1], current_table);
        break;

    case FILE_DEFINITIONS:
        if (ast->nkids == 2) {
            // step into right of ast
            populate_symboltable(ast->kids[1], current_table);
            // recurse on the left of ast
            populate_symboltable(ast->kids[0], current_table);
        } else {
            populate_symboltable(ast->kids[0], current_table);
        }
        break;
    
    case STRUCT_DEFINITION:
        char* name = obtain_name(ast->kids[1]);
        SymbolTable new_scope = enter_new_scope(current_table, name);
        free(name);
        // recurse for all structure parameters in struct delcaration
        populate_symboltable(ast->kids[3], new_scope);
        break;
    
    case STRUCT_PARAMS:
        // recurse through all parameters
        if (ast->nkids == 2) {
            populate_symboltable(ast->kids[1], current_table);
            populate_symboltable(ast->kids[0], current_table);
        } else {
            populate_symboltable(ast->kids[0], current_table);
        }
        // this returns and moves back to the parent scope
        break;
    case STRUCT_PARAM:
        // create a new symbol for the structure parameter
        char* name = obtain_name(ast->kids[0]);
        int index = insert_symbol_entry(current_table, name);
        free(name);
        return current_table;
        break;

    case FUNC_DEFINITION:
        // break apart into function header and function body
        SymbolTable new_scope = populate_symboltable(ast->kids[0], current_table);
        break;

    case FUNC_HEADER:
        // if the function name is not main
        if (ast->kids[1]->prodrule == NAME) {
            char* name = obtain_name(ast->kids[1]);
            SymbolTable new_scope = enter_new_scope();
            free(name);
        } else if (ast->kids[1]->prodrule == MAINFUNC) {
            //
        } else {
            throw_err("Function header not defined correctly", 1);
        }
        break;

    default:
        break;
    }

    // return the root symbol table
    return;
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
 * generates a new symbol table with a given ammount of buckets as an integer
 * value and a name for the new symbol table as a character pointer. A
 * pointer to the new symbol table is returned.
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