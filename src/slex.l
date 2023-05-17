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

"bool"          {return BOOLEAN;}
"int"           {return INT;}
"float"         {return FLOAT;}
"char"          {return CHAR;}
"string"        {return STRING;}
"func"          {return FUNCTION;}
":sym"          {return SYMBOL;}
"tpl"           {return TUPPLE;}
"struct"        {return STRUCT;}
"module"        {return MODULE;}
"Main"          {return MAINMOD;}
"main"          {return MAINFUNC;}

"+"             {return ADD;}
"-"             {return SUBTRACT;}
"*"             {return MULTIPLY;}
"/"             {return DIVIDE;}
"%"             {return MODULO;}
"="             {return ASSIGNMENT;}
"=="            {return COMPARISON;}
">"             {return COMPARISON;}
"<"             {return COMPARISON;}
">="            {return COMPARISON;}
"<="            {return COMPARISON;}
"!="            {return NOTEQUAL;}

"|"             {return BAR;}
"_"             {return DROPVAL;}
"->"            {return ARROWOP;}

","             {return COMA;}
"{"             {return LBRACE;}
"}"             {return RBRACE;}
"["             {return LBRACKET;}
"]"             {return RBRACKET;}
"("             {return LPAREN;}
")"             {return RPAREN;}
":"             {return COLON;}
";"             {return SEMICOLON;}

[1-9][0-9]*     {return LITERALINT;}
0               {return LITERALINT;}
[0-9]+\.[0-9]+  {return LITERALFLOAT;}
(true)|(false)  {return LITERALBOOL;}
"0"[xX][0-9a-fA-F]+     {return LITERALHEX;}
\"([^"]|ESC_CHAR|\w|\W)*\"    {return LITERALSTRING;}
\'([^']|ESC_CHAR|\w|\W)?\'    {return LITERALCHAR;}
[a-zA-Z_][0-9a-zA-Z_]*  {return IDENTIFIER;}

\"              {return STRINGERR;}
\"[a-zA-Z0-9]*  {return STRINGERR;}
\'[a-zA-Z0-9][a-zA-Z0-9]+\' {return CHARERR;}
\'[a-zA-Z0-9]   {return CHARERR;}
[a-zA-Z0-9]\'   {return CHARERR;}
\\"*".          {return COMMENTERR;}
%%