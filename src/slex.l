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
    #include "sgram.tab.h"
    #include "tree.h"
%}

ESC_CHAR \\(n|t|\\|\'|\"|\?)

%%
"/*"([^*]|"*"+[^/*])*"*"+"/"  { /* do nothing */ }
"//".*\r?\n     { /* do nothing */ }
[ \t\r\f]+      { /* do nothing */ }

"bool"          {return allocToken(BOOLEAN);}
"int"           {return allocToken(INT);}
"float"         {return allocToken(FLOAT);}
"char"          {return allocToken(CHAR);}
"string"        {return allocToken(STRING);}
"func"          {return allocToken(FUNCTION);}
":sym"          {return allocToken(SYMBOL);}
"struct"        {return allocToken(STRUCT);}
"use"           {return allocToken(USE);}
"space"         {return allocToken(MODSPACE);}
"main"          {return allocToken(MAINFUNC);}
"head"          {return allocToken(HEADVAR);}
"tail"          {return allocToken(TAILVAR);}

"+"             {return allocToken(ADD);}
"-"             {return allocToken(SUBTRACT);}
"*"             {return allocToken(MULTIPLY);}
"/"             {return allocToken(DIVIDE);}
"%"             {return allocToken(MODULO);}
"="             {return allocToken(ASSIGNMENT);}
"=="            {return allocToken(ISEQUALTO);}
">"             {return allocToken(GREATERTHAN);}
"<"             {return allocToken(LESSTHAN);}
">="            {return allocToken(GREATERTHANOREQUAL);}
"<="            {return allocToken(LESSTHANOREQUAL);}
"!="            {return allocToken(NOTEQUALTO);}

"|"             {return allocToken(BAR);}
"_"             {return allocToken(DROPVAL);}
"->"            {return allocToken(ARROWOP);}

","             {return allocToken(COMA);}
"{"             {return allocToken(LBRACE);}
"}"             {return allocToken(RBRACE);}
"["             {return allocToken(LBRACKET);}
"]"             {return allocToken(RBRACKET);}
"("             {return allocToken(LPAREN);}
")"             {return allocToken(RPAREN);}
":"             {return allocToken(COLON);}
";"             {return allocToken(SEMICOLON);}

[1-9][0-9]*                   {return allocToken(LITERALINT);}
0                             {return allocToken(LITERALINT);}
[0-9]+\.[0-9]+                {return allocToken(LITERALFLOAT);}
(true)|(false)                {return allocToken(LITERALBOOL);}
"0"[xX][0-9a-fA-F]+           {return allocToken(LITERALHEX);}
\"([^"]|ESC_CHAR|\w|\W)*\"    {return allocToken(LITERALSTRING);}
\'([^']|ESC_CHAR|\w|\W)?\'    {return allocToken(LITERALCHAR);}
[a-zA-Z_][0-9a-zA-Z_]*        {return allocToken(IDENTIFIER);}
":"[a-zA-Z_][0-9a-zA-Z_]*":"  {return allocToken(LITERALSYMBOL);}

\n              { /* do nothing */ }
\"              {returnOnError(SYNTAX_ERROR, "syntax_error", "string error");}
\"[a-zA-Z0-9]*  {returnOnError(SYNTAX_ERROR, "syntax_error", "string error");}
\'[a-zA-Z0-9][a-zA-Z0-9]+\' {returnOnError(SYNTAX_ERROR, "syntax_error", "char error");}
\'[a-zA-Z0-9]   {returnOnError(SYNTAX_ERROR, "syntax_error", "char error");}
[a-zA-Z0-9]\'   {returnOnError(SYNTAX_ERROR, "syntax_error", "char error");}
"\\""*".        {returnOnError(SYNTAX_ERROR, "syntax_error", "comment error");}
."*""\\"        {returnOnError(SYNTAX_ERROR, "syntax_error", "comment error");}
%%