#include <stdio.h>

extern FILE* yyin;

// lexical value enumeration
enum lexcode {
    BOOLEAN=258,
    INT,
    FLOAT,         // 300
    CHAR,
    STRING,
    SYMBOL,
    LITERALBOOL,
    LITERALINT,     // 305
    LITERALHEX,
    LITERALFLOAT,
    LITERALCHAR,
    LITERALSTRING,
    LITERALSYMBOL,  // 310
    FUNCTION,
    TUPPLE,
    ARRAY,
    STRUCT,
    ADD,            // 315
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    MODULO,
    ASSIGNMENT,     // 320
    CONCATENATION,
    PATTERNMATCH,
    CONDSTATEMENT,
    ARROWOP,
    RETURN,         // 325
    EQUALTO,
    NOTEQUAL,
    COMPARISON,
    LBRACE,
    RBRACE,         // 330
    LPAREN,
    RPAREN,
    LBRACKET,
    RBRACKET,
    COMA,           // 335
    COLON,
    SEMICOLON,
    MODULE,
    MAINMOD,
    MAINFUNC,       // 340
    IDENTIFIER,
    DROPVAL,
    UNSUPPORTEDOP,
    UNSUPPORTEDKEY,
    STRINGERR,      // 345
    CHARERR,
    COMMENTERR,
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