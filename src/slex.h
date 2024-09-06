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
    IF,
    ELIF,
    ElSE,
    COLON,
    SEMICOLON,
    EOL,

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
    ADD_EQUAL,
    SUBTRACT_EQUAL,
    MULTIPLY_EQUAL,
    DIVIDE_EQUAL,
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
    MATCH_HEAD,
    ARROW_OP,

    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    LEFT_BRACE,
    RIGHT_BRACE,
} categoryValues;

char reserved_words[17][10] = { // array_len, word_size
    "mod",
    "def",
    "do",
    "end",
    "let",
    "main",
    "if",
    "elseif",
    "else",
    "fn",
    "int",
    "float",
    "char",
    "string",
    "bool",
    ":sym",
    "matchon",
    };

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
struct tokenStackPtr {
    struct token tok;
    struct tokenStackPtr* next;
};

/*
 * a stack pointer to maintain our position in the stack
 */
struct tokenStack {
    struct tokenStackPtr* head;
    struct tokenStackPtr* tail;
    int stack_height;
};

/* Lexing Functions */

struct tokenStackPtr* lex_source_file(char* file_name);

/* Token functions */

struct token build_token(int category, char* string_val, int lineno, char* sourcefile);
void append_to_stack(struct tokenStack* stack, struct token new_token);
struct token pop_token(struct tokenStack* stack);
void free_token_stack(struct tokenStack* stack);
void free_token(struct token tok);

#endif