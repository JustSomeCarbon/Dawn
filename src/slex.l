/* - Flex lexer file - */

/*
 * Solace lexer source file. Built using flex.
 */

%option noyywrap
%option noinput
%option nounput
%option yylineno

%{
    // Includes
    #include "tree.h"
    #include "solace.h"
%}

ESC_CHAR \\(n|t|\\|\'|\"|\?)

%%
"/*"([^*]|"*"+[^/*])*"*"+"/"  { /* do nothing */ }
"//".*\r?\n     { /* do nothing */ }
[ \t\r\f]+      { /* do nothing */ }

"bool"          {return alloc_token(BOOLEAN);}
"int"           {return alloc_token(INT);}
"float"         {return alloc_token(FLOAT);}
"char"          {return alloc_token(CHAR);}
"string"        {return alloc_token(STRING);}
"func"          {return alloc_token(FUNCTION);}
":sym"          {return alloc_token(SYMBOL);}
"tpl"           {return alloc_token(TUPPLE);}
"struct"        {return alloc_token(STRUCT);}
"module"        {return alloc_token(MODULE);}
"Main"          {return alloc_token(MAINMOD);}
"main"          {return alloc_token(MAINFUNC);}

"+"             {return alloc_token(ADD);}
"-"             {return alloc_token(SUBTRACT);}
"*"             {return alloc_token(MULTIPLY);}
"/"             {return alloc_token(DIVIDE);}
"%"             {return alloc_token(MODULO);}
"="             {return alloc_token(ASSIGNMENT);}
"=="            {return alloc_token(COMPARISON);}
">"             {return alloc_token(COMPARISON);}
"<"             {return alloc_token(COMPARISON);}
">="            {return alloc_token(COMPARISON);}
"<="            {return alloc_token(COMPARISON);}
"!="            {return alloc_token(NOTEQUAL);}

"|"             {return alloc_token(CONCATENATION);}
"_"             {return alloc_token(DROPVAL);}
"->"            {return alloc_token(ARROWOP);}

","             {return alloc_token(COMA);}
"{"             {return alloc_token(LBRACE);}
"}"             {return alloc_token(RBRACE);}
"["             {return alloc_token(LBRACKET);}
"]"             {return alloc_token(RBRACKET);}
"("             {return alloc_token(LPAREN);}
")"             {return alloc_token(RPAREN);}
":"             {return alloc_token(COLON);}
";"             {return alloc_token(SEMICOLON);}

[1-9][0-9]*     {return alloc_token(LITERALINT);}
0               {return alloc_token(LITERALINT);}
[0-9]+\.[0-9]+  {return alloc_token(LITERALFLOAT));}
(true)|(false)  {return alloc_token(LITERALBOOL);}
"0"[xX][0-9a-fA-F]+     {return alloc_token(LITERALHEX);}
\"([^"|ESC_CHAR])*\"    {return alloc_token(LITERALSTRING);}
\'([^'|ESC_CHAR])?\'    {return alloc_token(LITERALCHAR);}
[a-zA-Z_][0-9a-zA-Z_]*  {return alloc_token(IDENTIFIER);}

\"              {return alloc_token(STRINGERR);}
\"[a-zA-Z0-9]*  {return alloc_token(STRINGERR);}
\'[a-zA-Z0-9][a-zA-Z0-9]+\' {return alloc_token(CHARERR);}
\'[a-zA-Z0-9]   {return alloc_token(CHARERR);}
[a-zA-Z0-9]\'   {return alloc_token(CHARERR);}
\\"*".          {return alloc_token(COMMENTERR);}
%%