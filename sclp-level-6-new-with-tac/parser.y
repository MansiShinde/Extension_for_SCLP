
/*******************************************************************************************************

                                sclp : A C Language Processor
                                --------------------------------

       About:

       Originally implemented by Tanu Kanvar and Uday Khedker (http://www.cse.iitb.ac.in/~uday)
       for the courses cs302+cs316 Language Processors (theory and lab) at IIT Bombay.

       Initial release date Jan 15, 2013.

       Updated by N. Venkatesh in Jan 2019. The main updates included
       - Changing the input language from a basic-block-based GCC GIMPLE dump to a subset of C.
       - Replacing the flexc++ and bisonc++ scripts by flex and bison.
       - Supporting print operation for variables.
       
       Updated by Uday Khedker with the help of Nisha Biju and Saari Rajan in Jan 2020. The main
       updates include
       - Introduction of three address code (TAC) phase between abstract syntax tree (AST)
         and Register Transfer Language (RTL) which was originally called icode.
       - Introduction of string data type with constant strings.
       - Support for read operation for integer and floating point variables, and print
         operation for all variables (including string variables) and string constants. 
       - Significant changes in command line switches for printing each representation (tokens,
         AST, TAC, RTL, and assembly) as well as stopping after each phase (scanning, parsing,
	 AST construction (i.e. semantic analysis), TAC generation and RTL generation).
       - Substantial clean up of the code including removal of AST to icode translation,
         and interpretation (which was buggy and had not been tested thorougly after the front
	 end and the input langauge changed in 2019). 

       Copyrights reserved  by Uday Khedker. This  implemenation has been made available purely  
       for academic purposes without any warranty of any kind.

*******************************************************************************************************/
/* END_BANNER */

%{
extern "C" void yyerror(char *s);
extern int yylineno;
extern int yylex(void);

#include "common-headers.hh"

#include <iostream>
Procedure * current_procedure = NULL;	/* This global variable is used through out the script */
Class_Type_Expr * current_class = NULL;

void check_to_add_global_func_in_class(Procedure * cur_proc); //mansi
Symbol_Table_Entry & class_field_present_in_class_proc(string * var); //mansi
Symbol_Table * create_var_decl(Symbol_Table * list1, Symbol_Table * list2); //mansi

Procedure * create_function_header(string * name, Basic_Data_Type dt);
Procedure * update_function_entry_status_and_add_formal_param_list(Procedure * cur_proc, Symbol_Table * fpl);
void add_local_var_decls (Procedure * cur_proc, Symbol_Table * local_decls);
Procedure * add_function_body(Procedure * cur_proc, list<Ast *> * body);
Ast * create_conditional_expression_ast(Procedure * cur_proc, Ast * cond, Ast * lhs, Ast * rhs, int line);
Symbol_Table * process_var_decl_item_list(Basic_Data_Type recent_type_name, list <Decl_Type_Info *> * decl_type_info_list);
Access_Type_Info * process_variable_name(Ast * name_ast);
Access_Type_Info * process_array_variable_name(Ast * name_ast);
Access_Type_Info * process_array_aggregate(Access_Type_Info * ati, Ast * index_expr);
Access_Type_Info * process_pointer_aggregate(Access_Type_Info * ati);

typedef enum
{
	func_declaration,
	func_definition,
} Parsing_Context;

Parsing_Context function_parsing_context = func_declaration;

using namespace std;

%}

%union 
{
	int integer_value;
	double double_value;
	string * string_value;
	list<Ast *> * ast_list;
	Ast * ast;
	Symbol_Table * symbol_table;
	Symbol_Table_Entry * symbol_entry;
	Procedure * procedure;
        Basic_Data_Type type_name;
	Decl_Type_Info *decl_type_info;
	list <Decl_Type_Info *> * decl_type_info_list;
	Access_Type_Info * access_type_info;

	Access_Specifier access_name;	
};


%token CLASS PRIVATE PUBLIC COCO DOT 

%token <integer_value> INTEGER_NUMBER
%token <double_value> DOUBLE_NUMBER
%token <string_value> NAME
%token <string_value> STRING_CONSTANT
%token INTEGER FLOAT STRING BOOL
%token ASSIGN VOID
%token IF WHILE DO ELSE
%token WRITE READ
%token RETURN
%token ADDRESSOF
/* %token AT */

%right '?' ':'
%left OR
%left AND
%right NOT
%left NE EQ
%left LT LE GT GE
%left '+' '-'
%left '*' '/'
%right UMINUS
%left IF
%left ELSE

%type <symbol_table> class_member_list
%type <access_name> access_specifier_type
%type <symbol_table> class_members
%type <string_value> func_name
%type <symbol_table> class_decl
%type <ast> field_name

%type <symbol_table> global_decl_statement_list
%type <symbol_table> optional_local_var_decl_stmt_list
%type <symbol_table> var_decl_stmt
%type <symbol_table> var_decl_stmt_list
%type <symbol_table> formal_param
%type <symbol_table> formal_param_list
%type <type_name> param_type
%type <type_name> named_type
%type <decl_type_info> var_decl_item
%type <decl_type_info_list> var_decl_item_list
%type <decl_type_info> array_decl
%type <decl_type_info> pointer_decl
%type <ast_list> actual_arg_list
%type <ast_list> non_empty_arg_list
%type <ast> actual_arg
%type <access_type_info> array_access
%type <access_type_info> pointer_access
%type <access_type_info> array_dimensions
%type <ast> variable_name
%type <ast> expression_operand
%type <ast> variable_as_operand
%type <ast> constant_as_operand
%type <ast> expression
%type <ast> expression_term
%type <ast>  rel_expression
%type <ast_list> statement_list
%type <ast> statement
%type <ast> assignment_statement
%type <ast> if_condition
%type <ast> if_statement
%type <ast> do_while_statement
%type <ast> while_statement
%type <ast> compound_statement
%type <ast> print_statement 
%type <ast> read_statement 
%type <ast> call_statement
%type <ast> return_statement
%type <ast> func_call

%glr-parser 	

%start program

%%
program 
	: global_decl_statement_list 
	  func_def_list 
		{
		 	if (command_options.construct_ast())
			{
				bool main_present = program_object.get_main_procedure() != NULL;
				CHECK_INPUT_AND_ABORT(main_present, "The main function should be defined", NO_FILE_LINE)
				bool main_is_void =  program_object.get_main_procedure()->get_return_type() == void_data_type;
				if (this_version.get_language_level() < L5)
					CHECK_INPUT_AND_ABORT(main_is_void, "The return type of the main function should be void", NO_FILE_LINE)
				int count_of_procedures = program_object.count_of_procedures();
				if (count_of_procedures > 1)
					this_version.needs_level(L5);
			}
		}
	| func_def_list 
		{ 
			if (command_options.construct_ast())
			{
				/*				
				bool main_present = program_object.get_main_procedure() != NULL;
				CHECK_INPUT_AND_ABORT(main_present, "The main function should be defined", NO_FILE_LINE)
				bool main_is_void =  program_object.get_main_procedure()->get_return_type() == void_data_type;
				if (this_version.get_language_level() < L5)
					CHECK_INPUT_AND_ABORT(main_is_void, "The return type of the main function should be void", NO_FILE_LINE)
				int count_of_procedures = program_object.count_of_procedures();
				if (count_of_procedures > 1)
					this_version.needs_level(L5);
				*/
			}
		}
;

global_decl_statement_list
	: global_decl_statement_list 
	  func_decl 
		{ 	if (command_options.construct_ast())
         			$$=$1;
			 // var_decls are accumulated but func_decls are not accumulated because more checks are needed in func_decl
			/* This action does not include the func_decl in the value of
                           global_decl_statement_list because the function names are stored
                           in a map<string, Procedure *> called procedure_map in the
                           program object.
			   var_decls are accumulated but func_decls are not accumulated.
                           The Symbol_Table stores variables names only and not function
                           names because it is used to record the declarations inside a
                           procedure too.
				- Uday Mon Dec 30 14:39:00 IST 2019
                        */
		}  
	| global_decl_statement_list 
	  var_decl_stmt  
	  	{ 	if (command_options.construct_ast())
  			{
			 	// var_decls are accumulated but func_decls are not accumulated because more checks are needed in func_decl

    				// if declaration is local then no need to check in global list
    				// if declaration is global then this list is global list
	
    				Symbol_Table *list = create_var_decl($1, $2);
		
       		   		Symbol_Table * global_table = list;
		
       	        		if(global_table !=NULL)
       			 		program_object.set_global_table(*global_table);  
    
  			}
  		} 
	| global_decl_statement_list 
	  class_decl
	{
		if(command_options.construct_ast()) // mansi
				{	
					$$ = $1;	
				}
	}
	
	| var_decl_stmt  
		{   	if (command_options.construct_ast())
  			{
			 // var_decls are accumulated but func_decls are not accumulated because more checks are needed in func_decl
			 // Hence $$ is NOT NULL
         			$$=$1;
                   		Symbol_Table * global_table = $$;

               			if(global_table !=NULL)
        				program_object.set_global_table(*global_table);
   			}
 		}
	| func_decl 
		{   	if (command_options.construct_ast())
         			$$=NULL;
			 // var_decls are accumulated but func_decls are not accumulated because more checks are needed in func_decl
			 // Hence $$ is NULL
 		}

	| class_decl
 	{
		if(command_options.construct_ast())  //mansi
			{	
				$$ = NULL;	
			}
	}	
;

class_decl
	: CLASS NAME '{' 
	{
		if(command_options.construct_ast())
		{
			CHECK_INVARIANT(($2 != NULL), "Class name cannot be null");

			string class_name = *$2;
		
			CHECK_INPUT_AND_ABORT((program_object.variable_in_symbol_list_check(class_name) == false), 
						"Class name "+class_name+" coincides with that of a global variable", yylineno);

			CHECK_INPUT_AND_ABORT(program_object.class_exists(class_name)==false, 
								"Current class "+class_name+" already defined", yylineno-1);

			current_class = new Class_Type_Expr(class_name, yylineno);
			current_class->set_class_is_defined();
			program_object.set_class_to_map(class_name, current_class);	
		}
	}

	class_members '}' ';'
	{
		if(command_options.construct_ast())
		{
			current_class->assign_offset_class_var(); //setting offset values of class variables;
			current_class = NULL;  //After class is defined with all its members, current_class is made NULL
		}
	}
;

class_members
	: access_specifier_type ':' class_member_list
	  {
		if(command_options.construct_ast())
		{
			Access_Specifier access = $1;

			if(access == public_access_specifier && $3!=NULL)
			{
				Symbol_Table * global_table = $3;
				
				Symbol_Table local_table = current_class->get_local_class_list();
					
				CHECK_INPUT((global_table->var_in_loc_glo_class_sym_list_check(local_table) == false),"Variable is declared twice", yylineno);	
				
				current_class->set_global_class_list(*global_table);
			}
			else if(access == private_access_specifier && $3!=NULL)
			{
				Symbol_Table * local_table = $3;
				
				Symbol_Table  global_table = current_class->get_global_class_list();
				
				CHECK_INPUT((local_table->var_in_loc_glo_class_sym_list_check(global_table) == false),
"Variable is declared twice", yylineno); //this is to check variable present in global symbol table before adding in local symbol table,if its present abort.
				
				current_class->set_local_class_list(*local_table);
			}
		}
	  }

	 class_members	{
	  }

	| %empty
	  {
		if(command_options.construct_ast())
			$$ =NULL;
   	  }	
;

class_member_list
	: class_member_list var_decl_stmt
	  {
		if (command_options.construct_ast())
  			{
    				$$ = create_var_decl($1, $2);	
  			}			
          }
	| class_member_list func_decl
          {
		if (command_options.construct_ast())
         		$$=$1;
          }
	| var_decl_stmt
	  {
		if(command_options.construct_ast())
			$$=$1;	
          }
	| func_decl
	  {
		if (command_options.construct_ast())
			$$=NULL;			
          }
;

access_specifier_type
	: PUBLIC
	{
		if(command_options.construct_ast())
		{
			$$ = public_access_specifier;	
		}
	}
	| PRIVATE
	{
		if(command_options.construct_ast())
		{
			$$ = private_access_specifier;
		}
	}
;

func_decl 
	: func_header '(' formal_param_list ')' ';'  
		{  	if (command_options.construct_ast())
  			{
				function_parsing_context = func_declaration;
				current_procedure = update_function_entry_status_and_add_formal_param_list(current_procedure, $formal_param_list);

				// We are done with the declaration of the current procedure.

				current_procedure = NULL;

  			}
		}
	| func_header '('')' ';' 
			{  	if (command_options.construct_ast())
  				{
					function_parsing_context = func_declaration;
					current_procedure = update_function_entry_status_and_add_formal_param_list(current_procedure, NULL);

					// We are done with the declaration of the current procedure.

					current_procedure = NULL;
				} 
			}
;

func_def_list 
	: func_def_list func_def 
		{
			this_version.needs_level(L5);
		}
	| func_def 
;

func_name
	: NAME COCO NAME
		{
			 if (command_options.construct_ast())
			{			
				string class_proc_name = *$1 + "_" + *$3;
				CHECK_INPUT_AND_ABORT(program_object.proc_name_exists(class_proc_name) == true, "class function need to be declared first in class and then define", yylineno);
				
				current_procedure = program_object.get_procedure_entry(class_proc_name);
				current_class = current_procedure->get_class_type_info();
			
				CHECK_INPUT_AND_ABORT(current_class->get_class_name() == *$1, "Function's class and given class does not match", yylineno);
				current_class = NULL;
				
				*$$ = class_proc_name;									
			}
		}
	| NAME
		{
		 	if (command_options.construct_ast())
				$$ = $1;
		}
;
func_header
	: named_type func_name
		{ 	if (command_options.construct_ast())
				current_procedure = create_function_header($2, $1);
			// We will let the check remain outside of the function because we do not 
			// want to assign anything to current_procedure if we do not construct ast.

  		}
	| func_name
		{
			if (command_options.construct_ast())
			{
				if(current_class!=NULL)
					CHECK_INPUT_AND_ABORT(current_class->get_class_name() == *$1, "Class name and Constructor name does not match", yylineno);
				current_procedure = create_function_header($1, void_data_type);

			}
		}
;

func_body
	:  '{' optional_local_var_decl_stmt_list    
		{ 	if (command_options.construct_ast())
				add_local_var_decls (current_procedure, $optional_local_var_decl_stmt_list);
  		}
	  statement_list '}'  
		{ 	if (command_options.construct_ast())
				current_procedure = add_function_body(current_procedure, $statement_list);
  		}
; 

func_def  
	: func_header '(' formal_param_list')'  
		{ 	if (command_options.construct_ast())
			{	
				function_parsing_context = func_definition;
				current_procedure = update_function_entry_status_and_add_formal_param_list(current_procedure, $formal_param_list);
			}
  		}
	  func_body
	| func_header '(' ')'  
		{ 	if (command_options.construct_ast())
			{	
				function_parsing_context = func_definition;
				current_procedure = update_function_entry_status_and_add_formal_param_list(current_procedure, NULL);
			}
  		}
	  func_body
;

formal_param_list 
	: formal_param_list ',' formal_param 
		{ 	if (command_options.construct_ast())
			{ 
    				Symbol_Table * decl_list_1 = $1;
    				Symbol_Table * decl_list_2 = $3;
    				decl_list_1->append_list(*decl_list_2,yylineno);
    				$$=decl_list_1;
			}
		}
	| formal_param 
		{ 	if (command_options.construct_ast())
			{ $$ = $1;}
		}
;

formal_param
	: param_type NAME 
		{ 	if (command_options.construct_ast())
			{
      				string name = *$2;

				CHECK_INPUT_AND_ABORT(!program_object.proc_name_exists(name),"Variable "+name+" coincides with a procedure name", yylineno);

      				Symbol_Table_Entry * decl_entry = new Symbol_Table_Entry(name, $1, yylineno);
				Type_Expr * dummy_leaf = new Dummy_Type_Expr_Leaf();
				Decl_Type_Info * dti = new Decl_Type_Info($1, dummy_leaf, 0, yylineno);
				decl_entry->set_decl_type_info(dti);
      				Symbol_Table * decl_list = new Symbol_Table();
      				decl_list->push_symbol(decl_entry,yylineno);
      				$$ = decl_list;
			}
		}
;

param_type 
	: INTEGER 
		{ 	if (command_options.construct_ast())
			{ $$ = int_data_type; }
		}
	| FLOAT   
		{ 	if (command_options.construct_ast())
			{ $$ = double_data_type; }
		}
	| BOOL   
		{ 	if (command_options.construct_ast())
			{ $$ = bool_data_type; }
		}
	| STRING   
		{ 	if (command_options.construct_ast())
			{ $$ = string_data_type; }
		}
;

statement_list
	: statement_list statement 
		{ 	if(command_options.construct_ast())
  			{
    				list<Ast *> * temp_ast_list = $1;
    				Ast * stmt = $2;
    				list<Ast *> * temp_ast_list_new;
				
    				if (temp_ast_list == NULL)
      					temp_ast_list_new = new list<Ast *>;
    				else
      					temp_ast_list_new = temp_ast_list;
   
				if(stmt!=NULL)
    					temp_ast_list_new->push_back(stmt);

    				$$ = temp_ast_list_new;
  			}
  		}
	| %empty 
		{  	if (command_options.construct_ast())
            			$$ = NULL;
  		}
;

statement
	: assignment_statement  
		{ 	if (command_options.construct_ast()) 
             			$$ = $1;
           	}
	| if_statement  
		{ 	if (command_options.construct_ast()) 
             			$$ = $1;
           	} 
	| do_while_statement 
		{ 	if (command_options.construct_ast()) 
             			$$ = $1;
           	}
	| while_statement   
		{ 	if (command_options.construct_ast()) 
             			$$ = $1;
           	} 
	| compound_statement 
		{ 	if (command_options.construct_ast()) 
             			$$ = $1;
           	} 
	| print_statement 
		{ 	if (command_options.construct_ast()) 
             			$$ = $1;
           	} 
	| read_statement 
		{ 	if (command_options.construct_ast()) 
             			$$ = $1;
           	} 
	| call_statement 
		{ 	if (command_options.construct_ast()) 
             			$$ = $1;
               	} 
	| return_statement 
		{ 	if (command_options.construct_ast()) 
             			$$ = $1;
                }
; 

call_statement
	: func_call ';' 
		{ 	if (command_options.construct_ast())
  				$$=$1;
		}
;

func_call
	: NAME '(' actual_arg_list ')' 
		{ 	
			this_version.needs_level(L5);
			if (command_options.construct_ast())
    			{
  				Call_Ast* func_call = new Call_Ast(*$1,yylineno);
        			if($3!=NULL)
   					func_call->set_actual_param_list(*$3);

      				func_call->type_check_actual_formal_param(((program_object.get_procedure_entry(*$1))->get_formal_param_list()));
   				$$=func_call;
			}
		}
;

actual_arg_list
	: non_empty_arg_list 
		{ 	if (command_options.construct_ast())
  				$$=$1;
		}
	| %empty 
		{ 	if (command_options.construct_ast())
  				$$=NULL;
		}
;

non_empty_arg_list
	: non_empty_arg_list ',' actual_arg 
		{ 	if (command_options.construct_ast())
    			{
        			list<Ast *> * arg_list = $1;
        			arg_list->push_back($3);
        			$$= arg_list;
  			}
		}
	| actual_arg    
		{ 	if (command_options.construct_ast())
    			{
        			list<Ast *> * arg_list = new list<Ast *>;
        			arg_list->push_back($1);
        			$$= arg_list;
  			}
		}
;

actual_arg
	: expression 
		{ 	if (command_options.construct_ast())
  				$$= $1;
  		}
;

return_statement
	: RETURN expression ';'
		{ 	
			if (command_options.construct_ast())
    			{
      				Return_Ast* return_stmt = new Return_Ast($2, current_procedure->get_proc_name(), yylineno);
				current_procedure->set_found_return_ast();
      				$$ = return_stmt;
  			}
		}
;

optional_local_var_decl_stmt_list
	: %empty 
		{ 	if (command_options.construct_ast())
        			$$ = NULL;
  		}
	| var_decl_stmt_list  
		{ 	if (command_options.construct_ast())
  			{
         			if($1!=NULL)
           				$$ = $1;
  			}  
  		}
;

var_decl_stmt_list
	: var_decl_stmt 
		{ 	if (command_options.construct_ast())
			{
         			$$ = $1;
			}
  		}
	| var_decl_stmt_list var_decl_stmt 
		{ 	if (command_options.construct_ast())
  			{
    				// if declaration is local then no need to check in global list
    				// if declaration is global then this list is global list			

    				$$ = create_var_decl($1, $2);
  			}
		}
;

var_decl_stmt 
	: named_type 
	  var_decl_item_list ';'
         	{ 	if (command_options.construct_ast()){
	
				$$ = process_var_decl_item_list($1, $2);
			}
		}
;

var_decl_item_list 
	: var_decl_item_list ','
	  var_decl_item 
		{ 	if (command_options.construct_ast())
  			{
				list <Decl_Type_Info *> * dtil = $1;
				dtil->push_front($3);
				// $3->print_decl_type_info(4,cout); TEST
				$$ = dtil;
  			}
  		}
	| var_decl_item 
		{ 	if (command_options.construct_ast())
  			{
				list <Decl_Type_Info *> * dtil = new list <Decl_Type_Info *>;
				dtil->push_back($1);
				// $1->print_decl_type_info(4,cout); TEST
				$$ = dtil;
  			}
  		}
	;

var_decl_item 
	: NAME  
		{ 	if (command_options.construct_ast())
	  		{
	        		string name = *$1;

				CHECK_INPUT_AND_ABORT(!program_object.proc_name_exists(name),"Variable "+name+" coincides with a procedure name", yylineno);

				// This check should be performed only inside a function definition
				if (function_parsing_context == func_definition)
        				CHECK_INPUT_AND_ABORT((current_procedure->variable_in_formal_list_check(name) == false), 
        					"Variable "+name+" is already declared as a formal parameter", yylineno);

				Type_Expr * dummy_leaf = new Dummy_Type_Expr_Leaf();
				Decl_Type_Info * dti = new Decl_Type_Info(void_data_type, dummy_leaf, 0, yylineno);
				dti->set_name(name);
				$$ = dti;
	  		}
	  	}
	| NAME  
          array_decl
		{ 	if (command_options.construct_ast())
	  		{
				string name = *$1;
				CHECK_INPUT_AND_ABORT(!program_object.proc_name_exists(name),"Variable "+name+" coincides with a procedure name", yylineno);

				// This check should be performed only inside a function definition
				if (function_parsing_context == func_definition)
        				CHECK_INPUT_AND_ABORT((current_procedure->variable_in_formal_list_check(name) == false), 
        					"Variable "+name+" is already declared as a formal parameter", yylineno);

				$2->set_name(name);
				$2->set_line_number(yylineno);
				$$=$2;
			}
		}
	| pointer_decl
	  NAME
		{ 	if (command_options.construct_ast())
			{
				Decl_Type_Info * dti = $1;

	        		string name = *$2;

				CHECK_INPUT_AND_ABORT(!program_object.proc_name_exists(name),"Variable "+name+" coincides with a procedure name", yylineno);

				// This check should be performed only inside a function definition
				if (function_parsing_context == func_definition)
        				CHECK_INPUT_AND_ABORT((current_procedure->variable_in_formal_list_check(name) == false), 
        					"Variable "+name+" is already declared as a formal parameter", yylineno);

				dti->set_name(name);
				$$ = dti;
			}
		}
	;
pointer_decl
	: '*'
		{ 	if (command_options.construct_ast())
			{
				Type_Expr * leaf = new Dummy_Type_Expr_Leaf();
				Type_Expr * dt = new Pointer_Type_Expr(leaf);
				Decl_Type_Info * dti = new Decl_Type_Info(void_data_type, dt,0,yylineno);
				$$ = dti;
			}
		}
	| '*' 
	  pointer_decl
		{ 	if (command_options.construct_ast())
	  		{
				Type_Expr * dt = new Pointer_Type_Expr($2->get_derived_type());
				int line = $2->get_line_number();
				Decl_Type_Info * dti = new Decl_Type_Info(void_data_type, dt,0,line);
				$$ = dti;
			}
		}
	;

array_decl
	:   '[' INTEGER_NUMBER  ']'
		{ 	if (command_options.construct_ast())
	  		{
				int size = $2;
				Type_Expr * dummy_leaf = new Dummy_Type_Expr_Leaf();
				Type_Expr * dt = new Array_Type_Expr(size,dummy_leaf, 1); 
				Decl_Type_Info * dti = new Decl_Type_Info(void_data_type, dt, size, yylineno);
				$$ = dti;
			}
		}
	|   '[' INTEGER_NUMBER  ']'
		array_decl
		{ 	
			// These rules read the last dimension of the array first so the first dimension
                        // on the top in the Array_Type_Expr tree.
			if (command_options.construct_ast())
	  		{
				int size = $2;
				Type_Expr * child_dt = $4->get_derived_type(); 
				int dim_count = child_dt->get_dim_count() + 1; 
				Type_Expr * dt = new Array_Type_Expr(size, child_dt, dim_count); 
				Decl_Type_Info * dti = new Decl_Type_Info(void_data_type, dt, size, yylineno);
				$$ = dti;
			}
		}
        ;

named_type
	: INTEGER 
		{ 	if (command_options.construct_ast())
				$$ = int_data_type; 
		}
	| FLOAT   
		{ 	if (command_options.construct_ast())
				$$ = double_data_type;
		}
	| VOID    
		{ 	if (command_options.construct_ast())
				$$ = void_data_type; 
		}
	| STRING    
		{ 	if (command_options.construct_ast())
				$$ = string_data_type; 
		}
	| BOOL    
		{ 	if (command_options.construct_ast())
				$$ = bool_data_type; 
		}
	| NAME
		{
			if(command_options.construct_ast())
			{
				string class_name = *$1;
				CHECK_INPUT_AND_ABORT((program_object.class_name_exists(class_name) == true), 
        					"Class "+class_name+"is not declared", yylineno);
				
				current_class = program_object.get_class_entry(class_name); //current_class is activated for object declaration
				$$ = class_data_type;
			}  
		}
;

assignment_statement
	: variable_as_operand ASSIGN expression ';' 
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 && $3), "Lhs/Rhs of assignment expression cannot be null");
    				$$ = new Assignment_Ast($1, $3, yylineno);
    				$$->type_check_ast();
  			}
		}
	| variable_as_operand ASSIGN func_call ';' 
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 && $3), "Lhs/Rhs of assignment expression cannot be null");
    				$$ = new Assignment_Ast($1, $3, yylineno);
    				$$->type_check_ast();
  			}
		}
	| variable_as_operand ASSIGN ADDRESSOF variable_name ';' 
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 && $4), "Lhs/Rhs of assignment expression cannot be null");
				
				Ast * addr_ast = new Address_Of_Ast($4);
    				$$ = new Assignment_Ast($1, addr_ast, yylineno);
    				$$->type_check_ast();
  			}
		}
;

if_condition
	: '(' expression ')'
		{ 	if (command_options.construct_ast())
				$$ = $2;
		}
;

if_statement
	: IF if_condition statement ELSE statement 
		{ 	if (command_options.construct_ast())
    				$$ = new Selection_Statement_Ast($2,$3,$5,yylineno);
				
  		}
	| IF if_condition statement  
		{ 	if (command_options.construct_ast())
    				$$ = new Selection_Statement_Ast($2,$3,NULL,yylineno);
  		}
;

do_while_statement
	: DO statement WHILE '(' expression ')' ';' 
		{ 	if (command_options.construct_ast())
    				$$ = new Iteration_Statement_Ast($5,$2,yylineno,true);
		}
;

while_statement
	: WHILE '(' expression ')' statement 
		{ 	if (command_options.construct_ast())
    				$$ = new Iteration_Statement_Ast($3,$5,yylineno,false);
  		}
;

compound_statement
	: '{' statement_list '}' 
		{ 	if (command_options.construct_ast())
  			{
      				Sequence_Ast *sast = new Sequence_Ast(yylineno);

              			list<Ast *>::iterator t;

				if ($2 != NULL)		/* We allow empty statement list also */
      				{	for (t = $2->begin(); t != $2->end(); ++t)
             					sast->ast_push_back(*t);
				}
             			$$=sast;
  			}
  		} 
; 

print_statement
	: WRITE expression_term ';' 
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT($2 , "Operand cannot be null in a print statement");
    				$$ = new Write_Ast($2,yylineno);

  			}
		}
;

read_statement
	: READ variable_name ';' 
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT($2 , "A variable cannot be null in a read statement");
    				$$ = new Read_Ast($2,yylineno);
  			}
		}

;

expression
	: expression_operand '+' expression_operand  
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 && $3), "An arith_expression_operand cannot have null entries");
    				$$ = new Plus_Ast($1, $3, yylineno);
  			}
  		}
	| expression_operand '-' expression_operand  
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 && $3), "An arith_expression_operand cannot have null entries");
    				$$ = new Minus_Ast($1, $3, yylineno);
  			}
  		}
	| expression_operand '*' expression_operand  
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 && $3), "An arith_expression_operand cannot have null entries");
    				$$ = new Mult_Ast($1, $3, yylineno);
  			}
  		}
	| expression_operand '/' expression_operand    
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 && $3), "An arith_expression_operand cannot have null entries");
    				$$ = new Divide_Ast($1, $3, yylineno);
  			}
  		}
	| '-' expression_operand %prec UMINUS  
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT($2, "An arith_expression_operand cannot have null entries");
    				$$ = new UMinus_Ast($2, NULL, yylineno);
  			}
  		}
	| '(' expression_operand ')' 
		{  	if (command_options.construct_ast())
  			{
    				$$ = $2;
  			}
  		}
	| expression_operand '?' expression_operand ':' expression_operand
  		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 != NULL) && ($3 != NULL) && ($5 != NULL), "lhs/rhs of arithmetic expression cannot be null");
    				Ast * cond = $1;
    				Ast * lhs = $3;
    				Ast * rhs = $5;
    				//Ast * cond_ast = new Conditional_Expression_Ast(cond, lhs, rhs, yylineno);
    				//$$ = cond_ast;
				$$ = create_conditional_expression_ast(current_procedure, cond, lhs, rhs, yylineno);
  			}
  		}
	| expression_term  
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 != NULL), "An expression cannot have null values");
    				$$ = $1;
  			}
  		}
	| expression_operand AND expression_operand 
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 != NULL) && ($3 != NULL), "lhs/rhs of boolean expression cannot be null");
    				$$ = new Logical_Expr_Ast($1, and_op, $3, yylineno);
  			}
  		}
	| expression_operand OR expression_operand 
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 != NULL) && ($3 != NULL), "lhs/rhs of boolean expression cannot be null");
    				$$ = new Logical_Expr_Ast($1, or_op, $3, yylineno);
  			}
		}
	| NOT expression_operand   
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($2 != NULL) , "rhs of boolean expression cannot be null");
    				$$ = new Logical_Expr_Ast($2, not_op, NULL, yylineno);
  			}
		}
	| rel_expression                
		{ 	if (command_options.construct_ast())
    				$$ = $1;
  		}
;

expression_operand
	: expression  
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 != NULL), "Arithmetic expression cannot be null");
    				$$ = $1;
  			}
  		}
;

expression_term
	: variable_as_operand  
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 != NULL), "An expression term cannot be null");
    				$$ = $1;
  			}
  		}
	| constant_as_operand  
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 != NULL), "An expression term cannot be null");
    				$$ = $1;
  			}
  		}
;

rel_expression
	: expression_operand LT expression_operand 
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 && $3), "A rel_expression_operand cannot have null entries");
    				$$ = new Relational_Expr_Ast($1, less_than, $3, yylineno);
  			}
  		}
	| expression_operand LE expression_operand 
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 && $3), "A rel_expression_operand cannot have null entries");
    				$$ = new Relational_Expr_Ast($1, less_equalto, $3, yylineno);
  			}
  		}
	| expression_operand GT expression_operand 
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 && $3), "A rel_expression_operand cannot have null entries");
    				$$ = new Relational_Expr_Ast($1, greater_than, $3, yylineno);
  			}
  		}
	| expression_operand GE expression_operand 
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 && $3), "A rel_expression_operand cannot have null entries");
    				$$ = new Relational_Expr_Ast($1, greater_equalto, $3, yylineno);
  			}
  		}
	| expression_operand NE expression_operand 
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 && $3), "A rel_expression_operand cannot have null entries");
    				$$ = new Relational_Expr_Ast($1, not_equalto, $3, yylineno);
  			}
  		}
	| expression_operand EQ expression_operand 
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 && $3), "A rel_expression_operand cannot have null entries");
    				$$ = new Relational_Expr_Ast($1, equalto, $3, yylineno);
  			}
  		}
;

variable_as_operand
	: variable_name
		{
			if (command_options.construct_ast())
    				$$ = $1;
		}
	|  array_access
		{ 	
			if (command_options.construct_ast())
  			{
				Access_Type_Info * ati = $1;
				ati->check_depth_consistency();

				CHECK_INVARIANT (ati != NULL, "Access Type Info cannot be NULL");

				Ast * name_ast = ati->get_name_ast();
				Ast * deref_ast = ati->get_deref_ast();
				if (deref_ast != NULL)
					$$ = deref_ast;
				else	
					$$ = name_ast;
  			}
  		}
	| pointer_access
		{ 	
			if (command_options.construct_ast())
  			{
				Access_Type_Info * ati = $1;
				ati->check_depth_consistency();

				CHECK_INVARIANT (ati != NULL, "Access Type Info cannot be NULL");

				Ast * name_ast = ati->get_name_ast();
				Ast * deref_ast = ati->get_deref_ast();
				if (deref_ast != NULL)
					$$ = deref_ast;
				else	
					$$ = name_ast;
  			}
  		}
	| variable_name DOT field_name
		{
			if(command_options.construct_ast())
			{
				Symbol_Table_Entry & base = $1->get_symbol_entry();
				Symbol_Table_Entry & field = $3->get_symbol_entry();

				CHECK_INPUT_AND_ABORT(base.get_basic_data_type()==class_data_type, "Object is not of class datatype",yylineno);
				current_class = base.get_class_type_info();
				
				//CHECK_INPUT_AND_ABORT(current_class->variable_in_global_class_list_check(field.get_variable_name()), "Field "+field.get_variable_name()+" doesnt belong to class "+current_class->get_class_name(), yylineno);
					
				$$ = new Field_Access_Ast($1, $3);
			}
		}
	;

field_name
	: NAME 
	{
		if(command_options.construct_ast())
		{
			Symbol_Table_Entry * var_table_entry;
			
			CHECK_INVARIANT(($1 != NULL), "Variable name cannot be null");
		
			if(current_class->variable_in_global_class_symbol_list_check(*$1))
			{
				var_table_entry = &(current_class->get_global_class_symbol_table_entry(*$1));
			   	current_class = NULL;
			}
			else
      				CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, "Variable "+*$1+" has not been declared in class "+current_class->get_class_name(), yylineno);

			
			$$ = new Name_Ast(*$1, *var_table_entry, yylineno);
		}		
	}
;

variable_name
	: NAME   
		{ 	
			// This instance of a variable corresponds to the base case in expressions.
			// The occurrences of NAMEs in declarations do not use the non-terminal variable.
			// This means that a symbol table entry must exist for NAME here.
			if (command_options.construct_ast())
  			{
    				Symbol_Table_Entry * var_table_entry;

    				CHECK_INVARIANT(($1 != NULL), "Variable name cannot be null");

    				if (current_procedure->variable_in_symbol_list_check(*$1))
				{
       					var_table_entry = &(current_procedure->get_symbol_table_entry(*$1));
					if(var_table_entry->get_basic_data_type() == class_data_type )
					{
						//activating current_class for object definition and to access current_class's fields
						current_class = var_table_entry->get_class_type_info();
					}
				}
    				else if (current_procedure->variable_in_formal_list_check(*$1))
       					var_table_entry = &(current_procedure->get_formal_param_entry(*$1));
    				else if (program_object.variable_in_symbol_list_check(*$1))
      					var_table_entry = &(program_object.get_symbol_table_entry(*$1));
				else if (current_procedure->is_proc_mangled()) //mangled means function belongs to class
				{
					var_table_entry =  &(class_field_present_in_class_proc($NAME));
					current_class = NULL;
				}
    				else
      					CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, "Variable "+*$1+" has not been declared", yylineno);

    				$$ = new Name_Ast(*$1, *var_table_entry, yylineno);
  			}
  		}
	;

array_access
	: variable_name 
		{
			if (command_options.construct_ast())
			{
				Access_Type_Info * ati = process_array_variable_name($1);	
				$<access_type_info>$ = ati;
			}
		}
		array_dimensions
		{
			if (command_options.construct_ast())
				$$ = $3;
		}
	;

pointer_access
	: '*' variable_name
		{
			if (command_options.construct_ast())
			{
				Access_Type_Info * ati = process_variable_name($2);	
				$$ = process_pointer_aggregate(ati);
			}
		}
        | '*' pointer_access
		{
			if (command_options.construct_ast())
				$$ = process_pointer_aggregate($2);
		}
	;

array_dimensions
	: '[' expression ']'
		{
			if (command_options.construct_ast())
			{
				Access_Type_Info * ati = $<access_type_info>0;
				Ast * index_expr = $2;
				$$ = process_array_aggregate(ati, index_expr);
			}
		}
	| array_dimensions '[' expression ']'
		{ 	
			// These rules read the first dimension of the array first. Contrast with the
			// declarations in which the last dimension is read first so that the first
			// dimension comes on the top in the Array_Type_Expr tree.
			if (command_options.construct_ast())
				$$ = process_array_aggregate($1,$3);
  		}
;

constant_as_operand
	: INTEGER_NUMBER  
		{ 	if (command_options.construct_ast())
    				$$ = new Number_Ast<int>($1, int_data_type, yylineno);
  		}
	| DOUBLE_NUMBER 
		{ 	if (command_options.construct_ast())
    				$$ = new Number_Ast<double>($1, double_data_type, yylineno);
  		}
	| STRING_CONSTANT
		{ 	if (command_options.construct_ast())
			{
				string s = *$1;
				int str_num = program_object.add_string_to_string_table(s);
    				$$ = new String_Constant_Ast(str_num, string_data_type, yylineno);
			}
		// It is possible to move STRING_CONSTANT out of constant but then it will have to be 
		// replicated in Assignment Statement, actual params in function calls, return values, 
		// Read, and Write statements
  		}
;
%%


Symbol_Table_Entry & class_field_present_in_class_proc(string * var)
{
		current_class = current_procedure->get_class_type_info();
		string name = * var;
		CHECK_INPUT_AND_ABORT((current_class->variable_in_global_class_symbol_list_check(name) == true || current_class->variable_in_local_class_symbol_list_check(name) == true),"variable "+name+" is not present in its class ", yylineno); //class function can access class's public and private members, hence this check

		if(current_class->variable_in_local_class_symbol_list_check(name))
			return current_class->get_local_class_symbol_table_entry(name);

		return current_class->get_global_class_symbol_table_entry(name);
			
}


Symbol_Table * create_var_decl(Symbol_Table * list1, Symbol_Table * list2) {

				Symbol_Table * decl_list_1 = list1;
    				Symbol_Table * decl_list_2 = list2;
			
				Symbol_Table * list = NULL;
	
       		   		if(list1!=NULL && list2!=NULL) 
				{    
       		 			list1->append_list(*list2,yylineno);
       		      			list=list1;
       		 		}
       		   		else 
				{
       		           		if(list1!=NULL)
       		                  		list = list1;
       		           		else if(list2!=NULL)
       		                   		list= list2;
       		            		else  list=NULL;
					//See the comment in the action of the first RHS above for the meaning of NULL 
       		   		}
		return list;
}


Procedure * create_function_header(string * name_ptr, Basic_Data_Type dt)
{
    	CHECK_INVARIANT((name_ptr != NULL), "Procedure name cannot be null");
	string proc_name = * name_ptr;	
	Basic_Data_Type recent_type_name = dt;
	Procedure * cur_proc = NULL;	

	CHECK_INPUT_AND_ABORT((program_object.variable_in_symbol_list_check(proc_name) == false), 
		"Procedure name "+proc_name+" coincides with that of a global variable", yylineno);
	//cout<<"Procedure name:"<<proc_name<< " Exists? "<< program_object.proc_exists(proc_name) <<"\n";

	if(program_object.proc_exists(proc_name))
	{
		cur_proc = program_object.get_procedure_entry(proc_name);

		if (cur_proc->is_proc_declared() )
			CHECK_INPUT_AND_ABORT(cur_proc->get_return_type()==recent_type_name,
				"A declaration with a different return type exists for procedure "+proc_name, yylineno);
	}
	else
	{
		if(current_class!=NULL)
		{
			/*			
			if(current_class->get_class_name() == proc_name)
			{
				CHECK_INPUT_AND_ABORT(recent_type_name == void_data_type,
				"Class name and contructor name does not match", yylineno);
			}
			*/
			string class_proc_name = current_class->get_class_name() + "_" + proc_name ;
			
			CHECK_INPUT_AND_ABORT(program_object.proc_exists(class_proc_name) == false, "class function already declared once", yylineno);			

			cur_proc = new Procedure(recent_type_name, class_proc_name, yylineno);
			cur_proc->set_proc_is_mangled();
			cur_proc->assign_class_type_to_function(current_class);
			program_object.set_proc_to_map(class_proc_name, cur_proc);
		} 
		else
		{	
			cur_proc = new Procedure(recent_type_name, proc_name, yylineno);
			program_object.set_proc_to_map(proc_name, cur_proc);
		}
	}

	return cur_proc;
}

Procedure * update_function_entry_status_and_add_formal_param_list(Procedure * cur_proc, Symbol_Table * fpl)
{
    	CHECK_INVARIANT((cur_proc != NULL), "Current procedure cannot be null");


	Symbol_Table * old_formal_param_table = &cur_proc->get_formal_param_list();

	if ((function_parsing_context == func_definition) && cur_proc->is_proc_declared())
	{
		if (fpl != NULL)
		{	Symbol_Table * new_formal_param_table = fpl;	// formal_param_list
	
			CHECK_INPUT_AND_ABORT(old_formal_param_table->get_number_of_symbols()==
				new_formal_param_table->get_number_of_symbols(), 
				"definition and declaration should have same number of params", yylineno);
	
	
			for(int i=1;i<=new_formal_param_table->get_number_of_symbols();i++)
			{
				CHECK_INPUT_AND_ABORT(new_formal_param_table->get_symbol_table_entry_by_index(i).get_basic_data_type()==
					old_formal_param_table->get_symbol_table_entry_by_index(i).get_basic_data_type(),
					"Types of parameters in declaration and definition do not match", yylineno);
			}
		}
		else
		    	CHECK_INPUT_AND_ABORT(old_formal_param_table->get_number_of_symbols()==0,
				"definition and declaration should have same number of params", yylineno);
	}

	if (fpl != NULL)
		cur_proc->set_formal_param_list(*fpl);

	if (function_parsing_context == func_definition)
		cur_proc->set_proc_is_defined();
	else if (function_parsing_context == func_declaration)
	{
		if(current_class!=NULL && cur_proc->is_proc_declared())
			check_to_add_global_func_in_class(cur_proc);
		else
			cur_proc->set_proc_is_declared();
	}

	return cur_proc;
}

void check_to_add_global_func_in_class(Procedure * cur_proc)
{
	string class_proc_name = current_class->get_class_name() + "_" + cur_proc->get_proc_name();
	CHECK_INPUT_AND_ABORT(program_object.proc_exists(class_proc_name) == false, "function is already declared in class", yylineno);
	
	Procedure * cp = new Procedure(cur_proc->get_return_type(), class_proc_name, yylineno);
		
	cur_proc->set_proc_is_mangled();
	cur_proc->assign_class_type_to_function(current_class);
	program_object.set_proc_to_map(class_proc_name, cp);	

}


void add_local_var_decls (Procedure * cur_proc, Symbol_Table * local_decls)
{
    	CHECK_INVARIANT((cur_proc != NULL), "Current procedure cannot be null");
    	if (local_decls == NULL)
      		local_decls = new Symbol_Table();

    	cur_proc->set_local_list(*local_decls);
}

Procedure * add_function_body(Procedure * cur_proc, list<Ast *> * body)
{ 
	CHECK_INVARIANT((cur_proc != NULL), "Current procedure cannot be null"); 

	list<Ast *> * stmt_list = NULL;

	if(body!=NULL)
		stmt_list = body;
	else
		stmt_list = new list<Ast *>;

	cur_proc->set_ast_list(*stmt_list);     

	if ((this_version.get_language_level() >= L5) && 
		(cur_proc->get_return_type() != void_data_type))
	{	
		CHECK_INPUT_AND_ABORT(cur_proc->contains_return_ast(),
				"Missing return statement in a non-void procedure", yylineno);	
	}

	return NULL;	// We have processed the body so we want to nullify the pointer
}

static int stored_temp_no = 0;

Ast * create_conditional_expression_ast(Procedure * cur_proc, Ast * cond, Ast * lhs, Ast * rhs, int lineno)
{
	CHECK_INVARIANT((cur_proc != NULL), "Current procedure cannot be null"); 
	CHECK_INVARIANT((cond != NULL), " Condition AST cannot be null"); 
	CHECK_INVARIANT((lhs != NULL), " Left operand of condition AST cannot be null"); 
	CHECK_INVARIANT((rhs != NULL), " Right operand of condition AST cannot be null"); 
	CHECK_INPUT_AND_ABORT((cond->get_basic_data_type() == bool_data_type),
		"Wrong type of condition in conditional ast", lineno);
	CHECK_INPUT_AND_ABORT((lhs->get_basic_data_type() == rhs->get_basic_data_type()),
		"Different data types of the two operands of conditional ast", lineno);

	Basic_Data_Type result_type = lhs->get_basic_data_type();

	string * result_name = new string ("stemp" + to_string(stored_temp_no++));

	Symbol_Table_Entry * res_sym_entry = cur_proc->append_new_local_name(*result_name, result_type, lineno);

	CHECK_INVARIANT((res_sym_entry != NULL), " Result name entry for condition AST cannot be null"); 

    	Ast * cond_ast = new Conditional_Expression_Ast(cond, lhs, rhs, res_sym_entry, lineno);

	CHECK_INVARIANT((cond_ast != NULL), " Conditional Expression AST cannot be null"); 

	return cond_ast;
}


Symbol_Table * process_var_decl_item_list(Basic_Data_Type recent_type_name, list <Decl_Type_Info *> * decl_type_info_list)
{

	CHECK_INPUT_AND_ABORT(recent_type_name != void_data_type,"Variables cannot have a void type", yylineno);

	Symbol_Table * decl_list = new Symbol_Table();
	CHECK_INVARIANT (decl_list != NULL, "List of variables is NULL in type declarations");
	
	for (auto it = decl_type_info_list->begin(); it!=decl_type_info_list->end(); it++)	//var_decl_item_list
	{
		(*it)->set_base_data_type(recent_type_name);

		Decl_Type_Info * decl_type_info = *it;
		int line = decl_type_info->get_line_number();
		string name = decl_type_info->get_name();
		Symbol_Table_Entry * decl_entry = new Symbol_Table_Entry(name, yylineno);
		
		if(recent_type_name == class_data_type) {
			decl_entry->assign_class_type_to_object(current_class);
			decl_entry->set_class_obj_size(); //setting object's size 
		}
    
		decl_entry->set_data_type(recent_type_name);
		decl_entry->set_decl_type_info(decl_type_info);
		decl_list->push_symbol(decl_entry,yylineno);
		
	}
	return decl_list;
}

Access_Type_Info * process_variable_name(Ast * name_ast)
{
    				// Make Access_Type_Info out of Name_Ast *	
	Symbol_Table_Entry & se = name_ast->get_symbol_entry();
	Decl_Type_Info * dti = se.get_decl_type_info();
	Ast * deref_ast = NULL;
	int depth = 0;
	Access_Type_Info  * access_type_info = new Access_Type_Info (name_ast, dti, deref_ast, depth);
	return access_type_info;
}

Access_Type_Info * process_array_variable_name(Ast * name_ast)
{
    				// Make Access_Type_Info out of Name_Ast *	
	Symbol_Table_Entry & se = name_ast->get_symbol_entry();
	Decl_Type_Info * dti = se.get_decl_type_info();
	Ast * deref_ast = NULL;
	int depth = 0;
	Access_Type_Info  * access_type_info = new Access_Type_Info (name_ast, dti, deref_ast, depth);
	return access_type_info;
}


Access_Type_Info * process_array_aggregate(Access_Type_Info * ati, Ast * index_expr)
{
	Ast * deref_ast = ati->get_deref_ast();
	Ast * name_ast = ati->get_name_ast();
	int depth = ati->get_curr_depth() + 1;
	Decl_Type_Info * dt = ati->get_declared_type();
	string name = dt->get_name();

	CHECK_INPUT_AND_ABORT(depth <= ati->get_max_depth(), 
		"Number of array dimensions exceeded than the total number of dimensions for variable " + name,
		NO_FILE_LINE);

	CHECK_INPUT_AND_ABORT(name_ast->is_an_array(), "Variable " + name + " not delcared as an array but used as an array", NO_FILE_LINE);

	Ast * offset_of_this_dim = index_expr;
	int factor = ati->get_cumulative_size_of_array_dim_from_end(depth);
	if (factor > 1)
	{
		Ast * factor_ast = new Number_Ast<int>(factor, int_data_type, yylineno);
		offset_of_this_dim = new Mult_Ast(index_expr, factor_ast, yylineno);
	}
	int size_in_bytes = number_of_bytes_for_value_type(dt->get_base_data_type());
	Ast * size_ast = new Number_Ast<int>(size_in_bytes, int_data_type, yylineno);
	Ast * offset_of_this_dim_in_bytes = new Mult_Ast(offset_of_this_dim, size_ast, yylineno);

	Ast * previous_index_expr = NULL;
	Ast * new_index_expr = offset_of_this_dim_in_bytes;
	if (deref_ast != NULL)
	{
		previous_index_expr = deref_ast->get_offset_ast();
		new_index_expr = new Plus_Ast(previous_index_expr, offset_of_this_dim_in_bytes, yylineno);
	}

	Ast * new_deref_ast = new Array_Access_Ast (name_ast, new_index_expr);

	Access_Type_Info * new_ati = new Access_Type_Info(name_ast, dt, new_deref_ast, depth);

	return new_ati;
}

Access_Type_Info * process_pointer_aggregate(Access_Type_Info * ati)
{
	Ast * deref_ast = ati->get_deref_ast();
	Ast * name_ast = ati->get_name_ast();
	int depth = ati->get_curr_depth() + 1;
	Decl_Type_Info * dt = ati->get_declared_type();
	string name = dt->get_name();

	CHECK_INPUT_AND_ABORT(name_ast->is_a_pointer(), "Variable " + name + " not delcared as a pointer but used as a pointer", NO_FILE_LINE);

	Ast * new_deref_ast;
	if (deref_ast == NULL)
		new_deref_ast = new Pointer_Deref_Ast (name_ast);
	else 
		new_deref_ast = new Pointer_Deref_Ast (deref_ast);


	Access_Type_Info * new_ati = new Access_Type_Info(name_ast, dt, new_deref_ast, depth);

	return new_ati;
}

