%{
#include <stdio.h>

extern char * yytext;
extern int yylineno;
%}
%token NAME INT VOID

%%
program	: decls fun_defs 
       	;

decls	: decls var_decl
	| decls fun_decl
	| 	/* empty */
	;

var_decl: data_type var_list ';'

var_list: var_list ',' NAME
	| NAME 
	;

data_type: INT
	;

/* 

The rules below are the correct rules.
See the comment after these rules.

*/

/*
   

fun_decl: data_type NAME '(' ')' ';'
	| VOID  NAME '(' ')' ';'
	;

fun_defs: fun_defs fun_def
        | fun_def
	;

fun_def	: data_type NAME '(' ')' '{' '}'
	| VOID NAME '(' ')' '{' '}'
	;
/*

/*

This part of the script causes shift reduce conflict and
a function prototype is not accepted because it always
goes to var_list. Thus the prefix "int f" of "int f();"
is read as a prefix of variable declarations.

See example program c1.c for which we get syntax error on
'(' on line number 1.

*/

fun_decl: ret_type NAME '(' ')' ';'
	;

fun_defs: fun_defs fun_def
        | fun_def
	;

fun_def	: ret_type NAME '(' ')' '{' '}'
	;

ret_type: data_type
	| VOID
	;

%%
int yyerror (char *s)
{
	fprintf (stderr, "%s for token %s on line %d\n", s, yytext, yylineno);
}

int main()
{
	return yyparse();
}
