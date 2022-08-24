%{
# include "y.tab.h"
#include <stdio.h>
#include <string.h>
%}


%option noyywrap
digit [0-9]
operator [-+*/=]
letter [a-zA-Z]


%%
{digit}+  {yylval.ivalue = atoi(yytext); printf("The digit is :%s\n", yytext); return NUM ;}


{operator} {printf("THe operator is: %c\n", yytext[0]); return yytext[0]; }


{letter}({letter}|{digit})*  {yylval.str = strdup(yytext); printf("The variable is: %s\n", yytext); return VAR; }



