
/*********************************************************************************************

                                sclp : A C Language Processor
                                --------------------------------

           About:

           Implemented   by  Tanu  Kanvar (tanu@cse.iitb.ac.in) and Uday
           Khedker    (http://www.cse.iitb.ac.in/~uday)  for the courses
           cs302+cs316: Language  Processors  (theory and  lab)  at  IIT
           Bombay.

           Initial release  date  Jan  15, 2013.  

           Copyrights reserved  by Uday Khedker. This  implemenation has
           been made available purely  for academic purposes without any
           warranty of any kind.


***********************************************************************************************/

%{
extern "C" void yyerror(char *s);
extern int yylineno;
extern int yylex(void);

#include "common-classes.hh"
#include "error-display.hh"
#include "user-options.hh"
#include "local-environment.hh"
#include "symbol-table.hh"
#include "ast.hh"
#include "basic-block.hh"
#include "procedure.hh"
#include "program.hh"

#include <iostream>
Procedure * current_procedure;	
Data_Type type;
int dummy_no=0;

%}

%union 
{
	int integer_value;
	double double_value;
	std::string * string_value;
	list<Ast *> * ast_list;
	Ast * ast;
	Symbol_Table * symbol_table;
	Symbol_Table_Entry * symbol_entry;
	Basic_Block * basic_block;
	Procedure * procedure;
  Data_Type type;
};

%token <integer_value> INTEGER_NUMBER
%token <double_value> DOUBLE_NUMBER
%token <string_value> NAME
%token  INTEGER FLOAT
%token ASSIGN VOID
%token IF WHILE DO ELSE
%token PRINT
%token RETURN

%right '?' ':'
%left OR
%left AND
%left NE EQ
%left LT LE GT GE
%left '+' '-'
%left '*' '/'
%right NOT
%right UMINUS
%nonassoc '('


%type <symbol_table> global_decl_statement
%type <symbol_table> optional_local_var_decl_list
%type <symbol_table> variable_declaration
%type <symbol_table> variable_declaration_list
%type <symbol_table> variables_list
%type <symbol_table> var_decl
%type <symbol_table> declaration
%type <ast_list> statement_list
%type <ast> statement
%type <ast> assignment_statement
%type <ast> operand
%type <ast> variable
%type <ast> constant
%type <ast> arith_expression
%type <ast> expression_term
%type <ast>  rel_expression
%type <ast> logical_expression
%type <ast> if_else_statement
%type <ast> do_while_statement
%type <ast> while_statement
%type <ast> compound_statement
%type <ast> print_statement 
%type <ast> call_statement
%type <ast> return_statement
%type <ast> func_call
%type <type> param_type
%type <symbol_table> param
%type <symbol_table> param_list
%type <ast_list> act_arg_list
%type <ast_list> arg_list
%type <ast> arg
%type <symbol_table> dummy_param
%type <symbol_table> dummy_param_list
%type <symbol_table> dummy_decl_param_list


%start program

%%
program: 
	global_decl_statement 

	func_definitions 
	{ 
		if (command_options.not_only_parse)
  		{
         		bool main_absent = !((program_object.get_main_procedure(cout) == NULL || 
			(program_object.get_main_procedure(cout))->get_return_type() != void_data_type));
		CHECK_INVARIANT(main_absent, "Main should be defined and it's type should be void")
   		}
		
	}
	| func_definitions 
	{
	}


;

global_decl_statement: 
	global_decl_statement 
	func_decl 
	{
    		if (command_options.not_only_parse)
  		{
         		$$=$1;
   		}
	}  
	| global_decl_statement 
	  var_decl  
	  {
     		if (command_options.not_only_parse)
  		{
    			// if declaration is local then no need to check in global list
    			// if declaration is global then this list is global list

    			Symbol_Table * decl_list_1 = $1;
    			Symbol_Table * decl_list_2 = $2;

          		if(decl_list_1!=NULL && decl_list_2!=NULL) 
			{    
        			decl_list_1->append_list(*decl_list_2,yylineno);
             			$$=decl_list_1;
        		}
        
          		else 
			{
                  		if(decl_list_1!=NULL)
                         		$$ = decl_list_1;
                  		else if(decl_list_2!=NULL)
                          		$$= decl_list_2;
                   		else  $$=NULL;
          		}

          		Symbol_Table * global_table = $$;

               		if(global_table !=NULL)
        		program_object.set_global_table(*global_table);
    
  		}
  	}  
| var_decl  {   if (command_options.not_only_parse)
  {
         $$=$1;
                   Symbol_Table * global_table = $$;

               if(global_table !=NULL)
        program_object.set_global_table(*global_table);

   }

 }
| func_decl  {}    
;
var_decl:  variable_declaration {  

  if (command_options.not_only_parse)
  {
         $$=$1;
   }

  }
;
func_decl: type NAME '(' dummy_decl_param_list ')' ';'  {  


  if (command_options.not_only_parse)
  {

  	 CHECK_INVARIANT(($2 != NULL), "Procedure name cannot be null");

  	 string proc_name = *$2;


  	 CHECK_INPUT ((program_object.variable_in_symbol_list_check(proc_name) == false), 
			"Procedure name cannot be same as global variable", yylineno);

  

   
   	Procedure *pdecl = new Procedure(type, proc_name, yylineno);
    Symbol_Table * formal_param_table = $4;

    pdecl->set_formal_param_list(*formal_param_table);
    program_object.set_proc_to_map(proc_name,pdecl);
    dummy_no=0;


  }

}
| type NAME '('')' ';' {

   if (command_options.not_only_parse)
  {
    CHECK_INVARIANT(($2 != NULL), "Procedure name cannot be null");

    string proc_name = *$2;

    Procedure *pdecl = new Procedure(type, proc_name, yylineno);

    program_object.set_proc_to_map(proc_name, pdecl);

     
  }

}
| type NAME '(' param_list ')' ';' {

   if (command_options.not_only_parse)
  {
    CHECK_INVARIANT(($2 != NULL), "Procedure name cannot be null");

    string proc_name = *$2;

    Procedure *pdecl = new Procedure(type, proc_name, yylineno);

    Symbol_Table * formal_param_table = $4;

    pdecl->set_formal_param_list(*formal_param_table);
    program_object.set_proc_to_map(proc_name,pdecl);
    
  }

}
;
dummy_decl_param_list:  dummy_param_list {
	if (command_options.not_only_parse)
 	{
		$$=$1;
	}

}
;
dummy_param_list: dummy_param_list ',' dummy_param {
	if (command_options.not_only_parse)
 	{
	
	    Symbol_Table * decl_list_1 = $1;
	    Symbol_Table * decl_list_2 = $3;
	    decl_list_1->append_list(*decl_list_2,yylineno);
	    $$=decl_list_1;
	}


}
|  dummy_param {

	  $$=$1;
	 

}
;
dummy_param: param_type { 

 if (command_options.not_only_parse)
  {
  	  dummy_no++;
	  string name = "dummy"+to_string(dummy_no);
      Symbol_Table_Entry * decl_entry = new Symbol_Table_Entry(name, $1, yylineno);
      Symbol_Table * decl_list = new Symbol_Table();
      decl_list->push_symbol(decl_entry);
      $$ = decl_list;

  }


}
;
func_definitions: func_definitions func_def {}
| func_def {}
;
func_def:  type NAME '(' param_list')'  { 

     if (command_options.not_only_parse)
  {
    CHECK_INVARIANT(($2 != NULL), "Procedure name cannot be null");

    string proc_name = *$2;

    if(program_object.is_procedure_exists(proc_name)){
    	current_procedure=program_object.get_procedure_prototype(proc_name);
    	
    	CHECK_INVARIANT(current_procedure->is_proc_defined()==false, "Current procedure "+proc_name+" already defined");

    	Symbol_Table * new_formal_param_table = $4;
    	Symbol_Table * old_formal_param_table = &current_procedure->get_formal_param_list();

    	
    	CHECK_INVARIANT(old_formal_param_table->get_size()==new_formal_param_table->get_size(), "definition and declaration should have same number of params");
    	
    	bool types_matched=true;
    	for(int i=0;i<new_formal_param_table->get_size();i++){
	    	CHECK_INVARIANT(new_formal_param_table->get_symbol_table_entry_by_index(i).get_data_type()==
	    		old_formal_param_table->get_symbol_table_entry_by_index(i).get_data_type(),
	    		"param types in declaration and definition not matching")

    	}

    	CHECK_INVARIANT(current_procedure->get_return_type()==type,"return type in declaration and definition not matching")

      	current_procedure->set_proc_is_defined();

      	current_procedure->set_formal_param_list(*new_formal_param_table);
    }

    else{

    	current_procedure = new Procedure(type, proc_name, yylineno);
    	Symbol_Table * formal_param_table = $4;

      	current_procedure->set_proc_is_defined();

      	current_procedure->set_formal_param_list(*formal_param_table);


      	program_object.set_proc_to_map(proc_name, current_procedure);


    }

  }

  }
	     '{' optional_local_var_decl_list    { 

       if (command_options.not_only_parse)
  {

    CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null");

    Symbol_Table * local_table = $8;

    if (local_table == NULL)
      local_table = new Symbol_Table();

    current_procedure->set_local_list(*local_table);
  }

  }
  statement_list '}'  {

  if (command_options.not_only_parse)
  {
    CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null"); 
  
        list<Ast *> * stmt_list = NULL;

          if($10!=NULL)
               stmt_list = $10;
          else
               stmt_list = new list<Ast *>;

         current_procedure->set_ast_list(*stmt_list);     
        
  }
    string proc_name = *$2;
    // program_object.set_proc_to_map(proc_name, current_procedure);
  }
|  type NAME '(' ')'  { 

     if (command_options.not_only_parse)
  {
    CHECK_INVARIANT(($2 != NULL), "Procedure name cannot be null");

    string proc_name = *$2;

     if(program_object.is_procedure_exists(proc_name)){

        

    	current_procedure=program_object.get_procedure_prototype(proc_name);

    	CHECK_INVARIANT(current_procedure->is_proc_defined()==false, "Current procedure already defined");
    	CHECK_INVARIANT(current_procedure->get_return_type()==type,"return type in declaration and definition not matching")


    	

      	current_procedure->set_proc_is_defined();

      	
	    }

	    else{

	    	current_procedure = new Procedure(type, proc_name, yylineno);
	    	
	      	current_procedure->set_proc_is_defined();

	      	


	      	program_object.set_proc_to_map(proc_name, current_procedure);


	    }


  }

  }
       '{' optional_local_var_decl_list    { 

       if (command_options.not_only_parse)
  {

    CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null");

    Symbol_Table * local_table = $7;

    if (local_table == NULL)
      local_table = new Symbol_Table();

    current_procedure->set_local_list(*local_table);
  }

  }
  statement_list '}'  {

  if (command_options.not_only_parse)
  {
    CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null"); 
  
        list<Ast *> * stmt_list = NULL;

          if($9!=NULL)
               stmt_list = $9;
          else
               stmt_list = new list<Ast *>;

         current_procedure->set_ast_list(*stmt_list);     
        
  }
    string proc_name = *$2;
    // program_object.set_proc_to_map(proc_name, current_procedure);
  }
;
param_list: param_list ',' param { 
    Symbol_Table * decl_list_1 = $1;
    Symbol_Table * decl_list_2 = $3;
    decl_list_1->append_list(*decl_list_2,yylineno);
    $$=decl_list_1;

}
| param { $$ = $1;}
;
param: param_type NAME {
      string name = *$2;
      Symbol_Table_Entry * decl_entry = new Symbol_Table_Entry(name, $1, yylineno);
      Symbol_Table * decl_list = new Symbol_Table();
      decl_list->push_symbol(decl_entry);
      $$ = decl_list;
}
;
param_type : INTEGER {
  $$ = int_data_type;
}
| FLOAT {
  $$ = double_data_type;
}
;
statement_list:  statement_list statement { 

         if(command_options.not_only_parse)
  {
    list<Ast *> * statement_list = $1;
    Ast * stmt = $2;
    list<Ast *> * statement_list_new;

    if (statement_list == NULL)
      statement_list_new = new list<Ast *>;

    else
      statement_list_new = statement_list;
   
           if(stmt!=NULL)
    statement_list_new->push_back(stmt);

    $$ = statement_list_new;
  }

  }
|  {  

     if (command_options.not_only_parse)
  {
            $$ = NULL;
  }

  }
;
statement:   assignment_statement  { 

           if (command_options.not_only_parse) {
             $$ = $1;
           }
           }
| if_else_statement  {

               if (command_options.not_only_parse) {
             $$ = $1;
           } 

        }
| do_while_statement { 

               if (command_options.not_only_parse) {
             $$ = $1;
           }

           }
|  while_statement   {
       if (command_options.not_only_parse) {
             $$ = $1;
           } 
           }
|  compound_statement {

           if (command_options.not_only_parse) {
             $$ = $1;
           } 
           }
|  print_statement {
	   
	   if (command_options.not_only_parse) {
             $$ = $1;
           } 

   }
| call_statement { 

          if (command_options.not_only_parse) {
             $$ = $1;
               } 
            }
|  return_statement { if (command_options.not_only_parse) {
             $$ = $1;
               } 
                }
; 
call_statement: func_call ';' {
     if (command_options.not_only_parse)
    {
  $$=$1;
}

}
;
func_call: NAME '(' act_arg_list ')' {
   if (command_options.not_only_parse)
    {
  Call_Ast* func_call = new Call_Ast(*$1,yylineno);
        if($3!=NULL)
   func_call->set_actual_param_list(*$3);

      func_call->check_actual_formal_param(((program_object.get_procedure_prototype(*$1))->get_formal_param_list()));
   $$=func_call;
}


}
;
act_arg_list : arg_list {
   if (command_options.not_only_parse)
    {
  $$=$1;
}

}
|  { 
 if (command_options.not_only_parse)
    {
  $$=NULL;
}
   }
;
arg_list: arg_list ',' arg {
 if (command_options.not_only_parse)
    {
        list<Ast *> * arg_list = $1;
        arg_list->push_back($3);
        $$= arg_list;
  }

}
| arg    {
  if (command_options.not_only_parse)
    {
        list<Ast *> * arg_list = new list<Ast *>;
        arg_list->push_back($1);
        $$= arg_list;
  }

}
;
arg: arith_expression {
if (command_options.not_only_parse)
    {
  $$= $1;
  }

}
;
return_statement:  RETURN ';' {

if (command_options.not_only_parse)
    {
      Return_Ast* return_stmt = new Return_Ast(NULL, current_procedure->get_proc_name(), yylineno);
      $$ = return_stmt;
  }

}

| RETURN arith_expression ';'{

  if (command_options.not_only_parse)
    {
      Return_Ast* return_stmt = new Return_Ast($2, current_procedure->get_proc_name(), yylineno);
      $$ = return_stmt;
      
  }

}
;
optional_local_var_decl_list:  { 
      if (command_options.not_only_parse)
    {
        $$ = NULL;
    }  

  }
|   variable_declaration_list  { 
  
      if (command_options.not_only_parse)
  {
         if($1!=NULL)
           $$ = $1;
  }  

  }
;
variable_declaration_list:  variable_declaration {

       if (command_options.not_only_parse)
      {
         $$ = $1;
       }
  }
| variable_declaration_list variable_declaration {
  
   if (command_options.not_only_parse)
  {
    // if declaration is local then no need to check in global list
    // if declaration is global then this list is global list

    Symbol_Table * decl_list_1 = $1;
    Symbol_Table * decl_list_2 = $2;

          if(decl_list_1!=NULL && decl_list_2!=NULL) {    
        decl_list_1->append_list(*decl_list_2,yylineno);
             $$=decl_list_1;
        }
        
          else {
                  if(decl_list_1!=NULL)
                         $$ = decl_list_1;
                  else if(decl_list_2!=NULL)
                          $$= decl_list_2;
                   else  $$=NULL;

          }

  }
}
;
variable_declaration:  declaration ';' { 

  if (command_options.not_only_parse)
  {
    $$ = $1;
  }

  }
;
declaration:   type variables_list { 
    if (command_options.not_only_parse)
  {
  $$=$2;
   }

}
;
type: INTEGER {  type = int_data_type; }
| FLOAT  { type = double_data_type;}
|  VOID  { type = void_data_type; }
;
variables_list: NAME  { 

     if (command_options.not_only_parse)
  {
        string name = *$1;

      Symbol_Table_Entry * decl_entry = new Symbol_Table_Entry(name, type, yylineno);

      Symbol_Table * decl_list = new Symbol_Table();

    decl_list->push_symbol(decl_entry);

    $$=decl_list;
        
  }

  }
|    variables_list ',' NAME  {

     if (command_options.not_only_parse)
  {
        string name = *$3;

        Symbol_Table * decl_list = $1;

        CHECK_INPUT((decl_list->variable_in_symbol_list_check(name) == false), 
        "Variable is declared twice", yylineno);

        Symbol_Table_Entry * decl_entry = new Symbol_Table_Entry(name, type, yylineno);

        decl_list->push_symbol(decl_entry);

         $$=decl_list;
  }

  }
;
assignment_statement:  variable ASSIGN arith_expression ';' {
   if (command_options.not_only_parse)
  {
    CHECK_INVARIANT(($1 && $3), "Lhs/Rhs of assignment expression cannot be null");
    $$ = new Assignment_Ast($1, $3, yylineno);
    $$->check_ast();
  }

}
| variable ASSIGN func_call ';' {
   if (command_options.not_only_parse)
  {
    CHECK_INVARIANT(($1 && $3), "Lhs/Rhs of assignment expression cannot be null");
    $$ = new Assignment_Ast($1, $3, yylineno);
    $$->check_ast();
  }

}
;
if_else_statement:  IF '(' logical_expression ')' statement ELSE statement {

         if (command_options.not_only_parse)
  {
    $$ = new Selection_Statement_Ast($3,$5,$7,yylineno);
    
  }

  }
|     IF '(' logical_expression ')' statement  {

    if (command_options.not_only_parse)
  {
    $$ = new Selection_Statement_Ast($3,$5,NULL,yylineno);
    
  }

  }
;
do_while_statement: DO statement WHILE '(' logical_expression ')' ';' {
  
      if (command_options.not_only_parse)
  {
    $$ = new Iteration_Statement_Ast($5,$2,yylineno,true);
    
  }

}
;
while_statement: WHILE '(' logical_expression ')' statement {
  
   if (command_options.not_only_parse)
  {
    $$ = new Iteration_Statement_Ast($3,$5,yylineno,false);
    
  }

}
;
compound_statement: '{' statement_list '}' { 

   if (command_options.not_only_parse)
  {

       CHECK_INVARIANT(($2 != NULL), " Block of statements cannot be empty");
      Sequence_Ast *sast = new Sequence_Ast(yylineno);

              list<Ast *>::iterator t;

      for (t = $2->begin(); t != $2->end(); ++t)
             sast->ast_push_back(*t);
             $$=sast;
  }

  } 
; 
print_statement:  PRINT variable ';' {
	
if (command_options.not_only_parse)
  {

    CHECK_INVARIANT($2 , "An variable cannot be null in print");

    $$ = new Print_Ast($2,yylineno);

   // $$->set_data_type($2->get_data_type());
  }
}

;
arith_expression:  operand '+' operand  { 

    if (command_options.not_only_parse)
  {

    CHECK_INVARIANT(($1 && $3), "An arith_operand cannot have null entries");

    $$ = new Plus_Ast($1, $3, yylineno);

    $$->set_data_type($1->get_data_type());
    $$->check_ast();
  }

  }
|  operand '-' operand  { 
      if (command_options.not_only_parse)
  {
    CHECK_INVARIANT(($1 && $3), "An arith_operand cannot have null entries");

    $$ = new Minus_Ast($1, $3, yylineno);
  
    $$->set_data_type($1->get_data_type());
    $$->check_ast();
  }

  }
|  operand '*' operand  { 

    if (command_options.not_only_parse)
  {
    CHECK_INVARIANT(($1 && $3), "An arith_operand cannot have null entries");

    $$ = new Mult_Ast($1, $3, yylineno);
  
    $$->set_data_type($1->get_data_type());
    $$->check_ast();
  }

  }
|  operand '/' operand    { 

   if (command_options.not_only_parse)
  {
    CHECK_INVARIANT(($1 && $3), "An arith_operand cannot have null entries");

    $$ = new Divide_Ast($1, $3, yylineno);
  
    $$->set_data_type($1->get_data_type());
    $$->check_ast();
  }
  }
| '-' operand %prec UMINUS  { 

   if (command_options.not_only_parse)
  {
    CHECK_INVARIANT($2, "An arith_operand cannot have null entries");

    $$ = new UMinus_Ast($2, NULL, yylineno);

    $$->set_data_type($2->get_data_type());
    $$->check_ast();
  }

  }
|  '(' operand ')' {  
    if (command_options.not_only_parse)
  {
    $$ = $2;
    $$->set_data_type($2->get_data_type());
  }

  }
| logical_expression '?' operand ':' operand
  {
  if (command_options.not_only_parse)
  {
    CHECK_INVARIANT(($1 != NULL) && ($3 != NULL) && ($5 != NULL), "lhs/rhs of arithmetic expression cannot be null");
    Ast * cond = $1;
    Ast * lhs = $3;
    Ast * rhs = $5;
    Ast * cond_ast = new Conditional_Expression_Ast(cond, lhs, rhs, yylineno);
    cond_ast->check_ast();
    $$ = cond_ast;
  }
  }
|  expression_term  { 

    if (command_options.not_only_parse)
  {
    CHECK_INVARIANT(($1 != NULL), "An expression cannot have null values");

    $$ = $1;
    $$->set_data_type($1->get_data_type());
  }

  }
;
operand:  arith_expression  { 

       if (command_options.not_only_parse)
  {
    CHECK_INVARIANT(($1 != NULL), "Arithmetic expression cannot be null");
    $$ = $1;
    $$->set_data_type($1->get_data_type());
  }

  }
;
expression_term:  variable  {

    if (command_options.not_only_parse)
  {
    CHECK_INVARIANT(($1 != NULL), "An expression term cannot be null");
    $$ = $1;
  }

  }
|   constant  { 

if (command_options.not_only_parse)
  {
    CHECK_INVARIANT(($1 != NULL), "An expression term cannot be null");
    $$ = $1;
  }

  }
;
rel_expression: 

operand LT operand { 


    if (command_options.not_only_parse)
  {
    CHECK_INVARIANT(($1 && $3), "A rel_operand cannot have null entries");

    $$ = new Relational_Expr_Ast($1, less_than, $3, yylineno);
  
    
    $$->check_ast();
  }

  }
|  operand LE operand {


if (command_options.not_only_parse)
  {
    CHECK_INVARIANT(($1 && $3), "A rel_operand cannot have null entries");

    $$ = new Relational_Expr_Ast($1, less_equalto, $3, yylineno);
  
    
    $$->check_ast();
  }

  }
|  operand GT operand {

 if (command_options.not_only_parse)
  {
    CHECK_INVARIANT(($1 && $3), "A rel_operand cannot have null entries");

    $$ = new Relational_Expr_Ast($1, greater_than, $3, yylineno);
  
    
    $$->check_ast();
  }

  }
|  operand GE operand {

if (command_options.not_only_parse)
  {
    CHECK_INVARIANT(($1 && $3), "A rel_operand cannot have null entries");

    $$ = new Relational_Expr_Ast($1, greater_equalto, $3, yylineno);
  
  
    $$->check_ast();
  }

  }
|  operand NE operand {

   if (command_options.not_only_parse)
  {
    CHECK_INVARIANT(($1 && $3), "A rel_operand cannot have null entries");

    $$ = new Relational_Expr_Ast($1, not_equalto, $3, yylineno);
  
    
    $$->check_ast();
  }

  }
|  operand EQ operand {

     if (command_options.not_only_parse)
  {
    CHECK_INVARIANT(($1 && $3), "A rel_operand cannot have null entries");

    $$ = new Relational_Expr_Ast($1, equalto, $3, yylineno);
  
    
    $$->check_ast();
  }

  }
;
logical_expression: logical_expression AND logical_expression { 

     if (command_options.not_only_parse)
  {
    CHECK_INVARIANT(($1 != NULL) && ($3 != NULL), "lhs/rhs of boolean expression cannot be null");
    Logical_Op lop = _logical_and;
    Logical_Expr_Ast * logical_expr_ast = new Logical_Expr_Ast($1, lop, $3, yylineno);
    $$ = logical_expr_ast;
      $$->check_ast();
  }

  }
|  logical_expression OR logical_expression {

 
     if (command_options.not_only_parse)
  {
    CHECK_INVARIANT(($1 != NULL) && ($3 != NULL), "lhs/rhs of boolean expression cannot be null");
    Logical_Op lop = _logical_or;
    Logical_Expr_Ast * logical_expr_ast = new Logical_Expr_Ast($1, lop, $3, yylineno);
    $$ = logical_expr_ast;
    $$->check_ast();
  }
}
|  NOT logical_expression   { 
     if (command_options.not_only_parse)
  {
    CHECK_INVARIANT(($2 != NULL) , "rhs of boolean expression cannot be null");
    Logical_Op lop = _logical_not;
    Logical_Expr_Ast * logical_expr_ast = new Logical_Expr_Ast(NULL, lop, $2, yylineno);
    $$ = logical_expr_ast;
    $$->check_ast();
  }
}
|  '(' logical_expression ')'  {
 
   if (command_options.not_only_parse)
  {
    $$ = $2;
  }

  }
| rel_expression                {

if (command_options.not_only_parse)
  {
    $$ = $1;
     //std::cout<<$$->get_data_type()<<std::endl;
  }

  }
;
variable:  NAME   { 

      if (command_options.not_only_parse)
  {
    Symbol_Table_Entry * var_table_entry;

    CHECK_INVARIANT(($1 != NULL), "Variable name cannot be null");

    if (current_procedure->variable_in_symbol_list_check(*$1))
       var_table_entry = &(current_procedure->get_symbol_table_entry(*$1));

    else if (current_procedure->variable_in_formal_list_check(*$1))
       var_table_entry = &(current_procedure->get_formal_param_entry(*$1));

    else if (program_object.variable_in_symbol_list_check(*$1))
      var_table_entry = &(program_object.get_symbol_table_entry(*$1));


    else
      CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, "Variable has not been declared", yylineno);

    $$ = new Name_Ast(*$1, *var_table_entry, yylineno);

  }

  }
;
constant:  INTEGER_NUMBER  {

if (command_options.not_only_parse)
  {
    $$ = new Number_Ast<int>($1, int_data_type, yylineno);
  }

  }
|  DOUBLE_NUMBER  {

if (command_options.not_only_parse)
  {
    $$ = new Number_Ast<double>($1, double_data_type, yylineno);
  }

  }
;
