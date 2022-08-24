%{
#include <stdio.h>
#include <string.h>
/*
%left '+'
*/
%}


%union 
{
	char *str;
	int ivalue;
};

%token <ivalue> NUM
%token <str> VAR
%type <ivalue> E

%%



A : VAR '=' E   { printf(" expression is : %s = %d\n", $1, $3); } 

|

E : NUM    { printf("found a number : %d \n ", $1);}

  | E '+' E  { $$ = $1 + $3 , printf("addition is : %d\n", $1 + $3);  }

  | E '-' E  { $$ = $1 - $3 , printf("subtraction is : %d\n", $1 - $3);  }

  | E '*' E  { $$ = $1 * $3 , printf("multiplication is : %d\n", $1 * $3);  }

  | E '/' E  { $$ = $1 / $3 , printf("division is : %d\n", $1 / $3);  }

%%
int yyerror (char *mesg)
{
	fprintf (stderr, "%s\n", mesg);
}
