%{
# include "y.tab.h"
#include "sam_class.h"
%}


%option noyywrap



%%

class    {return(CLASS);}
[a-zA-Z][a-zA-Z_0-9]*  {yylval.str = strdup(yytext); printf("The class name is: %s\n", yytext); return NAME; }
"{"      {return('{');}
"}"      {return('}');}


