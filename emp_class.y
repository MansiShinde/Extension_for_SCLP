%{
	# include <stdio.h>
	# include <string.h>
	# include <stdlib.h>

%}

%union 
{
	char *str;	
};


%token CLASS
%token <str> NAME

%%

p: CLASS NAME '{''}'   {printf("Empty class is defined!!");}


%%

yyerror(char* s)
{
	printf("\n ERROR: ");
}




