#include <stdio.h>

// !! TODO: remove file after bison conversion.

extern FILE* yyin;
extern char* yytext; // points to lexer pattern
//extern size_t yyleng; // CONFLICTING TYPES ON DIFFERENT PLATFORMS
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
    BAR,
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
    PACK,
    MAINMOD,
    MAINFUNC,       // 300
    IDENTIFIER,
    USE,
    DROPVAL,
    UNSUPPORTEDOP,
    UNSUPPORTEDKEY, // 305
    STRINGERR,
    CHARERR,
    COMMENTERR,
    ESCAPE
};

/* -- Legacy: moved to tree.h -- */
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
/* -- Legacy -- */

// LEXER PROTOTYPES
int yylex();
int yylex_destroy();

struct tokenlist* allocateToken();
void printTokenList(struct tokenlist* tl);