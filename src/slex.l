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

"bool"          {return alloc_token(1);}
"int"           {return alloc_token(2);}
"float"         {return alloc_token(3);}
"char"          {return alloc_token(4);}
"string"        {return alloc_token(5);}
"func"          {return alloc_token(6);}
":sym"          {return alloc_token(7);}
"tpl"           {return alloc_token(8);}
"struct"        {return alloc_token(9);}
"module"        {return alloc_token(10);}

"+"             {return alloc_token(11);}
"-"             {return alloc_token(12);}
"*"             {return alloc_token(13);}
"/"             {return alloc_token(14);}
"%"             {return alloc_token(15);}
"="             {return alloc_token(16);}
"=="            {return alloc_token(17);}
">"             {return alloc_token(18);}
"<"             {return alloc_token(19);}
">="            {return alloc_token(10);}
"<="            {return alloc_token(21);}
"!="            {return alloc_token(22);}

"|"             {return alloc_token(23);}
"_"             {return alloc_token(24);}
"->"            {return alloc_token(25);}

","             {return alloc_token(26);}
"{"             {return alloc_token(27);}
"}"             {return alloc_token(28);}
"["             {return alloc_token(29);}
"]"             {return alloc_token(30);}
"("             {return alloc_token(31);}
")"             {return alloc_token(32);}
":"             {return alloc_token(33);}
";"             {return alloc_token(34);}

[1-9][0-9]*     {return alloc_token(35);}
0               {return alloc_token(36);}
[0-9]+\.[0-9]+  {return alloc_token(37);}
(true)|(false)  {return alloc_token(38);}
"0"[xX][0-9a-fA-F]+     {return alloc_token(39);}
\"([^"|ESC_CHAR])*\"    {return alloc_token(40);}
\'([^'|ESC_CHAR])?\'    {return alloc_token(41);}
[a-zA-Z_][0-9a-zA-Z_]*  {return alloc_token(42);}

\"              {return alloc_token(-1);}
\"[a-zA-Z0-9]*  {return alloc_token(-1);}
\'[a-zA-Z0-9][a-zA-Z0-9]+\' {return alloc_token(-1);}
\'[a-zA-Z0-9]   {return alloc_token(-1);}
[a-zA-Z0-9]\'   {return alloc_token(-1);}
\\"*".          {return alloc_token(-1);}
%%