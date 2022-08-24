%{
# include "y.tab.h"
%}

%option noyywrap
digit [0-9]
operator [-+*/]

%%
{digit}+    { yylval = atoi(yytext); return NUM; }
{operator}  { return yytext[0]; }

