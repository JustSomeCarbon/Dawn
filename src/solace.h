#include <stdio.h>

extern FILE* yyin;

// lexical value enumeration
enum lexcode {
    BOOLEAN=258,
    INT,
    DOUBLE,         // 300
    CHAR,
    STRING,
    SYMBOL,
    FUNCTION,
    TUPPLE,         // 305
    ARRAY,
    STRUCT,
    ADD,
    SUBTRACT,
    MULTIPLY,       // 310
    DIVIDE,
    MODULO,
    ASSIGNMENT,
    CONCATENATION,
    PATTERNMATCH,   // 315
    CONDSTATEMENT,
    ARROWOP,
    RETURN,
    EQUALTO,
    NOTEQUAL,       // 320
    LOGICALCOMPARISON,
    LBRACE,
    RBRACE,
    LPAREN,
    RPAREN,         // 325
    COMA,
    COLON,
    SEMICOLON,
    IDENTIFIER,
    DROPVAL,        // 330
    UNSUPPORTEDOP,
    UNSUPPORTEDKEY,
    STRINGERR,
    CHARERR,
    COMMENTERR,     // 335
    ESCAPE
};


struct tokenlist {
    struct token* t;
    struct tokenlist* next;
};

struct token {
    int category;   // integer return code from yylex
    char* text;     // raw string pattern
    int lineno;     // token line number location
    char* filename; // source file name
    int ival;       // used for integer values
    double dval;    // used for floating point values
    char* sval;     // used for character and string values
};

// LEXER PROTOTYPES
int yylex();
int yylex_destroy();

void printTokenList(struct tokenlist* tl);