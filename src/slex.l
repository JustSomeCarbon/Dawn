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

"bool"          {return BOOLEAN;}
"int"           {return INT;}
"float"         {return FLOAT;}
"char"          {return CHAR;}
"string"        {return STRING;}
"func"          {return FUNCTION;}
":sym"          {return SYMBOL;}
"struct"        {return STRUCT;}
"use"           {return USE;}
"pack"        {return PACK;}
"Main"          {return MAINPACK;}
"main"          {return MAINFUNC;}
"head"          {return HEADVAR;}
"tail"          {return TAILVAR;}

"+"             {return ADD;}
"-"             {return SUBTRACT;}
"*"             {return MULTIPLY;}
"/"             {return DIVIDE;}
"%"             {return MODULO;}
"="             {return ASSIGNMENT;}
"+="            {return INCREMENT;}
"-="            {return DECREMENT;}
"=="            {return ISEQUALTO;}
">"             {return GREATERTHAN;}
"<"             {return LESSTHAN;}
">="            {return GREATERTHANOREQUAL;}
"<="            {return LESSTHANOREQUAL;}
"!="            {return NOTEQUALTO;}

"|"             {return BAR;}
"_"             {return DROPVAL;}
"->"            {return ARROWOP;}

"."             {return DOT;}
","             {return COMA;}
"{"             {return LBRACE;}
"}"             {return RBRACE;}
"["             {return LBRACKET;}
"]"             {return RBRACKET;}
"("             {return LPAREN;}
")"             {return RPAREN;}
":"             {return COLON;}
"::"            {return SUBSCRIPT;}
";"             {return SEMICOLON;}

[1-9][0-9]*     {return LITERALINT;}
0               {return LITERALINT;}
[0-9]+\.[0-9]+  {return LITERALFLOAT;}
(true)|(false)  {return LITERALBOOL;}
"0"[xX][0-9a-fA-F]+           {return LITERALHEX;}
\"([^"]|ESC_CHAR|\w|\W)*\"    {return LITERALSTRING;}
\'([^']|ESC_CHAR|\w|\W)?\'    {return LITERALCHAR;}
[a-zA-Z_][0-9a-zA-Z_]*        {return IDENTIFIER;}
":"[a-zA-Z_][0-9a-zA-Z_]*     {return LITERALSYMBOL;}

\n              { /* do nothing */ }
\"              {returnOnError(SYNTAX_ERROR, "syntax_error", "string error");}
\"[a-zA-Z0-9]*  {returnOnError(SYNTAX_ERROR, "syntax_error", "string error");}
\'[a-zA-Z0-9][a-zA-Z0-9]+\' {returnOnError(SYNTAX_ERROR, "syntax_error", "char error");}
\'[a-zA-Z0-9]   {returnOnError(SYNTAX_ERROR, "syntax_error", "char error");}
[a-zA-Z0-9]\'   {returnOnError(SYNTAX_ERROR, "syntax_error", "char error");}
"\\""*".        {returnOnError(SYNTAX_ERROR, "syntax_error", "comment error");}
."*""\\"        {returnOnError(SYNTAX_ERROR, "syntax_error", "comment error");}
%%