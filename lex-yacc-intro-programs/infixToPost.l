%{
# include "y.tab.h"
#include <stdio.h>
#include <string.h>
%}

%option noyywrap
letter [a-zA-Z]
operators [-+*/]


%%

{operators}    {printf("operator is : %c", yytext[0]); return yytext[0]; }

{letter}       {printf("character is : %c", yytext[0]); yylval.chr = strdup(yytext[0]); return CHAR;}

; return 0;
