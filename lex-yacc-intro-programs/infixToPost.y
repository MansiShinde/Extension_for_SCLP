%{
#include <stdio.h>
#include <string.h>
/*
%left '+' '-'
%left '*' '/' '%'

*/
%}

%union
{
 char chr;
 char *str;
}

%type <str> expr1
%type <chr> expr

%token <chr> CHAR



%%

expr : CHAR {printf("Character is : %c", $1); }

|

expr1 : expr '+' expr  { $$ = $1 + $3 , printf("expression is: %s", $$);  }
     | expr '-' expr  { $$ = $1 - $3 , printf("expression is: %s", $$);  }
     | expr '*' expr  { $$ = $1 * $3 , printf("expression is: %s", $$);  }
     | expr '/' expr  { $$ = $1 / $3 ,printf("expression is: %s", $$);  } 


%%
int yyerror (char *mesg)
{
	fprintf (stderr, "%s\n", mesg);
}
