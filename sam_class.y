%{
	# include <stdio.h>
	# include <string.h>
	# include <stdlib.h>
	#include "sam_class.h"


	symtab s[10];
	int s_ptr =0;
	void sym_tab(char type[]);
%}

	


%union 
{
	char *str;
	int value;	
};


%token CLASS
%token <str> CLASSNAME
%token INT
%type <value> VAR

%%

p: CLASS CLASSNAME '{'BLOCK'}'   

;
BLOCK: FIELDS 

;

FIELDS: VAR_DECL

;

VAR_DECL: INT VAR';'  {sym_tab("int");}

; 
%%

void sym_tab(char *type)
{
  int j=s_ptr -1;
  while(strcmp(s[j].type, "-1") == 0)
	strcpy(s[j--].type,type);
}




yyerror(char* s)
{
	printf("\n ERROR: ");
}

