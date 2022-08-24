%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
%}
%union 
{
	int integer_value;
	double double_value;
	char *string_value;

};

%token CLASS PRIVATE PUBLIC


%token <integer_value> INTEGER_NUMBER
%token <double_value> DOUBLE_NUMBER
%token <string_value> NAME 
%token <string_value> STRING_CONSTANT
%token INTEGER FLOAT STRING BOOL
%token ASSIGN VOID
%token IF WHILE DO ELSE
%token WRITE READ
%token RETURN
%token AT

%right '?' ':'
%left OR
%left AND
%left NE EQ
%left LT LE GT GE
%left '+' '-'
%left '*' '/'
%right NOT
%right UMINUS
%nonassoc '(' ')'
%left ELSE


%start program

%%
program 
	: global_decl_statement 
	  func_def_list 
		{ 	printf("global_decl_statement func_def_list ");
		}
	| func_def_list 
		{ printf("\nfunc_def_list ");
                }
;

global_decl_statement
	: global_decl_statement 
	  func_decl 
		{ printf("\nglobal_decl_statement func_decl");
		}  
	| global_decl_statement 
	  var_decl  
	  	{ 	printf("\nglobal_decl_statement var_decl");
  		}  
	| var_decl  
		{   	printf("\nvar_decl");
 		}
	| func_decl 
		{   	printf("\nfunc_decl");
 		}
	| class_declaration_stat
		{	printf("\nclass_declaration_stat");
		}

;

class_declaration_stat: CLASS NAME '{' block '}'';'
		{ printf("\nCLASS");
		}
;

block: block
	|access_specifier_type':'
	global_decl_statement
;

access_specifier_type: PUBLIC
			|PRIVATE
;


func_decl 
	: type NAME '(' dummy_formal_param_list ')' ';'  
		{  	printf("\ntype NAME '(' dummy_formal_param_list ')' ';'  ");
		}
	| type NAME '('')' ';' 
		{ printf("\ntype NAME '('')' ';' ");
  			
		}
	| type NAME '(' formal_param_list ')' ';' 
		{ 	printf("\ntype NAME '(' formal_param_list ')' ';'");
		}
;

dummy_formal_param_list
	: dummy_formal_param_list ',' dummy_formal_param 
		{ printf("\ndummy_formal_param_list ',' dummy_formal_param ");	
		}
	| dummy_formal_param 
		{ printf("\ndummy_formal_param"); }
;

dummy_formal_param 
	: param_type 
		{ printf("\nparam_type");	
		}
;

func_def_list 
	: func_def_list func_def 
		{ printf("\nfunc_def_list func_def");
		}
	| func_def
		{ printf("\nfunc_def");
		} 
;

func_def  
	: type NAME '(' formal_param_list')'  
	  '{' '}' 
		{ printf("\n  type NAME '(' formal_param_list')'  '{' '}'");
		} 

	| type NAME '(' formal_param_list')'  
	  '{' var_decl_list  
	  statement_list '}'
		{ printf("\n type NAME '(' formal_param_list')'  '{' var_decl_list  statement_list '}'");
		} 

	| type NAME '(' ')'  
	  '{'  '}'  
		{ printf("\n type NAME '(' ')'  '{'  '}'  ");
		} 

		
	| type NAME '(' ')'  
	  '{' var_decl_list
	  statement_list '}' 
		{ printf("\n type NAME '(' ')' '{' var_decl_list statement_list '}'   ");
		}  

	| type NAME '(' formal_param_list')' 
	  '{' var_decl_list '}'
		{ printf("\ntype NAME '(' formal_param_list')' '{' var_decl_list '}'");
		}

	| type NAME '(' ')' 
	  '{' var_decl_list '}'
		{ printf("\ntype NAME '(' ')' '{' var_decl_list '}'");
		}

	| type NAME '(' formal_param_list')'  
	  '{'  
	  statement_list '}'
		{ printf("\ntype NAME '(' formal_param_list')' '{' statement_list '}'");
		}

	| type NAME '(' ')'  
	  '{'   
	  statement_list '}'
		{ printf("\ntype NAME '(' ')' '{' statement_list '}'");
		}
		
;

formal_param_list 
	: formal_param_list ',' formal_param 
		{ printf("\nformal_param_list ',' formal_param ");
		}
	| formal_param 
		{ printf("\nformal_param");
		}
;

formal_param
	: param_type NAME 
		{ printf("\nparam_type NAME ");
		}
;

param_type 
	: INTEGER 
		{ printf("\nINTEGER ");	
		}
	| FLOAT   
		{ printf("\nFLOAT");
		}
	| BOOL   
		{ printf("\nBOOL");
		}
;

statement_list
	: statement_list statement 
		{ 	printf("\nstatement_list statement ");
  		}
	| statement
	{ printf("\nstatement");
	}
;

statement
	: assignment_statement  
		{ 	printf("\nstatement");
           	}
	| if_statement  
		{ 	printf("\nstatement");
           	} 
	| do_while_statement 
		{ 	printf("\nstatement");
           	}
	| while_statement   
		{ 	printf("\nstatement");
           	} 
	| compound_statement 
		{ 	printf("\nstatement");
           	} 
	| print_statement 
		{ 	printf("\nstatement");
           	} 
	| read_statement 
		{ 	printf("\nstatement");
           	} 
	| call_statement 
		{ 	printf("\nstatement");
               	} 
	| return_statement 
		{ 	printf("\nstatement");
                }
; 

call_statement
	: func_call ';' 
		{ 	printf("\nfunc_call ';'");
		}
;

func_call
	: NAME '(' non_empty_arg_list  ')'
 	{ printf("\nNAME '(' non_empty_arg_list  ')'");
	}	
	| NAME '(' ')'
	{ printf("\n NAME '(' ')'");
	}
;


non_empty_arg_list
	: non_empty_arg_list ',' actual_arg 
		{ printf("\nnon_empty_arg_list ',' actual_arg ");	
		}
	| actual_arg    
		{ 	printf("\nactual_arg ");
		}
;

actual_arg
	: arith_expression 
		{ 	printf("\narith_expression");
  		}
;

return_statement
	: RETURN ';' 
		{ printf("\nRETURN ';'");
		}

	| RETURN arith_expression ';'
		{ 	printf("\n RETURN arith_expression ';'");
		}
;

var_decl_list
	: var_decl 
		{ printf("\nvar_decl ");	
  		}
	| var_decl_list var_decl 
		{ 	printf("\nvar_decl_list var_decl ");
		}
;

var_decl
	: type variables_list ';'
         	{ printf("\ntype variables_list ';'");	
		}
;

type
	: INTEGER 
		{ printf("\nINTERGER");
		}
	| FLOAT   
		{ 	printf("\nFLOAT");
		}
	| VOID    
		{ 	printf("\nVOID");
		}
	| STRING    
		{ 	printf("\nSTRING");
		}
	| BOOL    
		{ 	printf("\nBOOL");
		}
	| NAME
		{ printf("\nNAME");
		}
;

variables_list
	: NAME  
		{ 	
	  	}
	| variables_list ',' NAME  
		{ 	
  		}
;

assignment_statement
	: variable ASSIGN arith_expression ';' 
		{ 	
		}
	| variable ASSIGN func_call ';' 
		{ 	
		}
	| variable ASSIGN logical_expression ';'
		{ 	
		}
;

if_statement
	: IF '(' logical_expression ')' statement ELSE statement 
		{ 	
  		}
	| IF '(' logical_expression ')' statement  
		{ 	
  		}
;

do_while_statement
	: DO statement WHILE '(' logical_expression ')' ';' 
		{ 	
		}
;

while_statement
	: WHILE '(' logical_expression ')' statement 
		{ 	
  		}
;

compound_statement
	: '{' statement_list '}' 
		{ 	
  		} 

	| '{' '}'
	{
	}
; 

print_statement
	: WRITE variable ';' 
		{ 	
		}
	| WRITE STRING_CONSTANT ';'
		{ 	
		}
;

read_statement
	: READ variable ';' 
		{ 	
		}

;

arith_expression
	: operand '+' operand  
		{ 	
  		}
	| operand '-' operand  
		{ 	
  		}
	| operand '*' operand  
		{ 	
  		}
	| operand '/' operand    
		{ 	
  		}
	| '-' operand %prec UMINUS  
		{ 	
  		}
	| '(' operand ')' 
		{  	
  		}
	| logical_expression '?' operand ':' operand
  		{ 	
  		}
	| expression_term  
		{ 	
  		}
;

operand
	: arith_expression  
		{ 	
  		}
;

expression_term
	: variable  
		{ 	
  		}
	| constant  
		{ 	
  		}
;

rel_expression
	: operand LT operand 
		{ 	
  		}
	| operand LE operand 
		{ 	
  		}
	| operand GT operand 
		{ 	
  		}
	| operand GE operand 
		{ 	
  		}
	| operand NE operand 
		{ 	
  		}
	| operand EQ operand 
		{ 	
  		}
;

logical_expression
	: logical_expression AND logical_expression 
		{ 	
  		}
	| logical_expression OR logical_expression 
		{ 	
		}
	| NOT logical_expression   
		{ 	
		}
	| '(' logical_expression ')'  
		{ 	
  		}
	| rel_expression                
		{ 	
  		}
	|  AT variable
		{ 	
  		}
;


variable
	: NAME   
		{ 	
  		}

;


constant
	: INTEGER_NUMBER  
		{ 	
  		}
	| DOUBLE_NUMBER 
		{ 	
  		}
	| STRING_CONSTANT
		{ 	
  		}
;
