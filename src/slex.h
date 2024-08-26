#ifndef SLEX_H
#define SLEX_H

/*
 * categories of values for tokens
 */
typedef enum {
    MOD = 1101,
    DEFINE,
    DO,
    END,
    LET,
    MAIN,
    SEMICOLON,

    FUNCTION,
    INT_LITERAL,
    FLOAT_LITERAL,
    CHAR_LITERAL,
    STRING_LITERAL,
    BOOL_LITERAL,
    SYMBOL_LITERAL,
    TUPPLE_LITERAL,

    ADD,
    SUBTRACT,
    DIVIDE,
    MULTIPLY,
    DOT,
    AND,
    OR,
    NOT,

    EQUALS,
    IS_EQUAL_TO,
    IS_NOT_EQUAL_TO,
    MATCH_EQUALS,
    GREATER_THAN,
    LESS_THAN,
    GREATER_THAN_OR_EQ,
    LESS_THAN_OR_EQ,

    BAR,
    MATCHON,
    MATCH_ARM,
    ARROW_OP,
} categoryValues;

/*
 * The token structure used to retain the tokenized source file
 */
struct token {
    char* literal;
    int lineno;
    int category;
    char* fileorigin;
    union literalValue val;
};

/*
 * The true value of the allocated token
 */
union literalValue {
    int ival;
    double dval;
    char cval;
    char* sval;
};

/*
 * The stack that contains all the tokens that have
 * been lexed
 */
struct tokenStack {
    struct token tok;
    struct tokenStack* next;
};

/*
 * a stack pointer to maintain our position in the stack
 */
struct tokenStackPtr {
    struct tokenStack* head;
    struct tokenStack* tail;
    int stack_height;
};

/* Lexing Functions */
struct tokenStack* lex_source_file(char* file_name);

/* Token functions */
struct token build_token(int category, char* string_val, int lineno, char* sourcefile);
void append_to_stack(struct tokenStackPtr* stack, struct token new_token);
struct token pop_token(struct tokenStackPtr* stack);
void free_token_stack(struct tokenStackPtr* stack);
void free_token(struct token tok);

#endif