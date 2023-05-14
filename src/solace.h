#include <stdio.h>

extern FILE* yyin;
extern char* yytext; // points to lexer pattern
extern int yyleng;
extern int yylineno; // holds source file line number

// lexical value enumeration
enum lexcode {
    BOOLEAN=258,
    INT,
    FLOAT,         // 260
    CHAR,
    STRING,
    SYMBOL,
    LITERALBOOL,
    LITERALINT,     // 265
    LITERALHEX,
    LITERALFLOAT,
    LITERALCHAR,
    LITERALSTRING,
    LITERALSYMBOL,  // 270
    FUNCTION,
    TUPPLE,
    ARRAY,
    STRUCT,
    ADD,            // 275
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    MODULO,
    ASSIGNMENT,     // 280
    CONCATENATION,
    PATTERNMATCH,
    CONDSTATEMENT,
    ARROWOP,
    RETURN,         // 285
    EQUALTO,
    NOTEQUAL,
    COMPARISON,
    LBRACE,
    RBRACE,         // 290
    LPAREN,
    RPAREN,
    LBRACKET,
    RBRACKET,
    COMA,           // 295
    COLON,
    SEMICOLON,
    MODULE,
    MAINMOD,
    MAINFUNC,       // 300
    IDENTIFIER,
    DROPVAL,
    UNSUPPORTEDOP,
    UNSUPPORTEDKEY,
    STRINGERR,      // 305
    CHARERR,
    COMMENTERR,
    BOOLERR,
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

struct tokenlist* allocateToken();
void printTokenList(struct tokenlist* tl);