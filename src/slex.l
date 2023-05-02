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
%}

ESC_CHAR \\(n|t|\\|\'|\"|\?)

%%
"/*"([^*]|"*"+[^/*])*"*"+"/"  { /* do nothing */ }
"//".*\r?\n     { /* do nothing */ }
[ \t\r\f]+      { /* do nothing */ }

"bool" {}
"int"
"float"
"char"
"string"
"func"
":sym"
"tpl"
"struct"
"module"

"+"
"-"
"*"
"/"
"%"
"="
"=="
">"
"<"
">="
"<="
"!="

"|"
"_"
"->"

","
"{"
"}"
"["
"]"
"("
")"
":"
";"

[1-9][0-9]*
0
[0-9]+\.[0-9]+
(true)|(false)
"0"[xX][0-9a-fA-F]+
\"([^"|ESC_CHAR])*\"
\'([^'|ESC_CHAR])?\'
[a-zA-Z_][0-9a-zA-Z_]*
"_"

\"
\"[a-zA-Z0-9]*
\'[a-zA-Z0-9][a-zA-Z0-9]+\'
\'[a-zA-Z0-9]
[a-zA-Z0-9]\'
\\"*".
%%