/* BEGIN_BANNER */
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

#include "common-classes.hh"
#include "error-display.hh"
#include "user-options.hh"
#include "symbol-table.hh"
#include "tac.hh"
#include "ast.hh"
#include "rtl.hh"
#include "procedure.hh"
#include "program.hh"

#include <iostream>
Procedure * current_procedure = NULL;	/* This global variable is used through out the script */
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
	Procedure * procedure;
  Data_Type type_name;
};

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


%type <symbol_table> global_decl_statement
%type <symbol_table> optional_local_var_decl_list
%type <symbol_table> var_decl
%type <symbol_table> var_decl_list
%type <symbol_table> variables_list
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
%type <ast> if_statement
%type <ast> do_while_statement
%type <ast> while_statement
%type <ast> compound_statement
%type <ast> print_statement 
%type <ast> read_statement 
%type <ast> call_statement
%type <ast> return_statement
%type <ast> func_call
%type <type_name> param_type
%type <type_name> type
%type <symbol_table> formal_param
%type <symbol_table> formal_param_list
%type <ast_list> actual_arg_list
%type <ast_list> non_empty_arg_list
%type <ast> actual_arg
%type <symbol_table> dummy_formal_param
%type <symbol_table> dummy_formal_param_list


%start program

%%
program 
	: global_decl_statement 
	  func_def_list 
		{ 	if (command_options.construct_ast())
			{
				bool main_absent = !((program_object.get_main_procedure(cout) == NULL || 
					(program_object.get_main_procedure(cout))->get_return_type() != void_data_type));
				CHECK_INVARIANT(main_absent, "Main should be defined and it's type should be void")
			}
		}
	| func_def_list 
		{ /* This action is only to suppress $$=$1 assignment which is
                                type incorrect in this case. 
					- Uday Tue Dec 31 17:54:52 IST 2019
                             */
                }
;

global_decl_statement
	: global_decl_statement 
	  func_decl 
		{ 	if (command_options.construct_ast())
         			$$=$1;
			/* This action does not include the func_decl in the value of
                           global_decl_statement because the function names go in a
                           separate list in the program. 
                           The Symbol_Table stores variables names only and not function
                           names because it is used to record the declarations inside a
                           procedure too.
				- Uday Mon Dec 30 14:39:00 IST 2019
                        */
		}  
	| global_decl_statement 
	  var_decl  
	  	{ 	if (command_options.construct_ast())
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
					/* See the comment in the action of the first RHS above for the meaning of NULL */
       		   		}
		
       		   		Symbol_Table * global_table = $$;
		
       	        		if(global_table !=NULL)
       			 		program_object.set_global_table(*global_table);
    
  			}
  		}  
	| var_decl  
		{   	if (command_options.construct_ast())
  			{
         			$$=$1;
                   		Symbol_Table * global_table = $$;

               			if(global_table !=NULL)
        				program_object.set_global_table(*global_table);
   			}
 		}
	| func_decl 
		{   	if (command_options.construct_ast())
         			$$=NULL;
			/* See the comment in the action of the first RHS above for the meaning of NULL */
 		}
;

func_decl 
	: type NAME '(' dummy_formal_param_list ')' ';'  
		{  	if (command_options.construct_ast())
  			{
  	 			CHECK_INVARIANT(($2 != NULL), "Procedure name cannot be null");
  	 			string proc_name = *$2;

				Data_Type recent_type_name = $1;
	
  		 		CHECK_INPUT ((program_object.variable_in_symbol_list_check(proc_name) == false), 
				"Procedure name cannot be same as global variable", yylineno);
	
   				Procedure *pdecl = new Procedure(recent_type_name, proc_name, yylineno);
    				Symbol_Table * formal_param_table = $4;
	
    				pdecl->set_formal_param_list(*formal_param_table);
    				program_object.set_proc_to_map(proc_name,pdecl);
    				dummy_no=0;
  			}
		}
	| type NAME '('')' ';' 
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($2 != NULL), "Procedure name cannot be null");

    				string proc_name = *$2;

				Data_Type recent_type_name = $1;

    				Procedure *pdecl = new Procedure(recent_type_name, proc_name, yylineno);

    				program_object.set_proc_to_map(proc_name, pdecl);
  			}
		}
	| type NAME '(' formal_param_list ')' ';' 
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($2 != NULL), "Procedure name cannot be null");
    				string proc_name = *$2;

				Data_Type recent_type_name = $1;

    				Procedure *pdecl = new Procedure(recent_type_name, proc_name, yylineno);

    				Symbol_Table * formal_param_table = $4;
				
    				pdecl->set_formal_param_list(*formal_param_table);
    				program_object.set_proc_to_map(proc_name,pdecl);
    				
  			}
		}
;

dummy_formal_param_list
	: dummy_formal_param_list ',' dummy_formal_param 
		{ 	if (command_options.construct_ast())
 			{
	    			Symbol_Table * decl_list_1 = $1;
	    			Symbol_Table * decl_list_2 = $3;
	    			decl_list_1->append_list(*decl_list_2,yylineno);
	    			$$=decl_list_1;
			}
		}
	| dummy_formal_param 
		{ $$=$1; }
;

dummy_formal_param 
	: param_type 
		{ 	if (command_options.construct_ast())
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

func_def_list 
	: func_def_list func_def 
	| func_def 
;

func_def  
	: type NAME '(' formal_param_list')'  
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($2 != NULL), "Procedure name cannot be null");
    				string proc_name = *$2;

				Data_Type recent_type_name = $1;

    				if(program_object.proc_exists(proc_name))
				{
	    				current_procedure=program_object.get_procedure_prototype(proc_name);
	
	    				CHECK_INPUT_AND_ABORT(current_procedure->is_proc_defined()==false, 
							"Current procedure "+proc_name+" already defined", yylineno-1);
	
	    				Symbol_Table * new_formal_param_table = $4;
	    				Symbol_Table * old_formal_param_table = &current_procedure->get_formal_param_list();
	
		    			CHECK_INPUT_AND_ABORT(old_formal_param_table->get_number_of_symbols()==
						new_formal_param_table->get_number_of_symbols(), 
						"definition and declaration should have same number of params", yylineno-1);
	
	    				for(int i=1;i<=new_formal_param_table->get_number_of_symbols();i++)
					{
		    				CHECK_INPUT_AND_ABORT(new_formal_param_table->get_symbol_table_entry_by_index(i).get_data_type()==
		    					old_formal_param_table->get_symbol_table_entry_by_index(i).get_data_type(),
		 					"Types of parameters in declaration and definition do not match", yylineno-1);
	    				}
	
	    				CHECK_INPUT_AND_ABORT(current_procedure->get_return_type()==recent_type_name,
							"Return type in declaration and definition do not match", yylineno-1);
			
	      				current_procedure->set_proc_is_defined();
			
	      				current_procedure->set_formal_param_list(*new_formal_param_table);
		    		}
		    		else
				{
		    			current_procedure = new Procedure(recent_type_name, proc_name, yylineno);
		    			Symbol_Table * formal_param_table = $4;
				
		      			current_procedure->set_proc_is_defined();
		      			current_procedure->set_formal_param_list(*formal_param_table);
		      			program_object.set_proc_to_map(proc_name, current_procedure);
		    		}
		
  			}

  		}
	  '{' optional_local_var_decl_list    
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null");

    				Symbol_Table * local_table = $8;

    				if (local_table == NULL)
      					local_table = new Symbol_Table();

    				current_procedure->set_local_list(*local_table);
  			}
  		}
	  statement_list '}'  
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null"); 
  
        			list<Ast *> * stmt_list = NULL;

          			if($10!=NULL)
               				stmt_list = $10;
          			else
               				stmt_list = new list<Ast *>;

         			current_procedure->set_ast_list(*stmt_list);     
        
         			current_procedure = NULL;
  			}
  		}
	| type NAME '(' ')'  
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($2 != NULL), "Procedure name cannot be null");

    				string proc_name = *$2;

				Data_Type recent_type_name = $1;

     				if(program_object.proc_exists(proc_name))
				{
    					current_procedure=program_object.get_procedure_prototype(proc_name);

    					CHECK_INPUT_AND_ABORT(current_procedure->is_proc_defined()==false, 
						"Current procedure already defined",yylineno-1);
    					CHECK_INPUT_AND_ABORT(current_procedure->get_return_type()==recent_type_name,
						"return type in declaration and definition not matching",yylineno-1)

      					current_procedure->set_proc_is_defined();
	    			}
	    			else
				{
	    				current_procedure = new Procedure(recent_type_name, proc_name, yylineno);
	      				current_procedure->set_proc_is_defined();
	      				program_object.set_proc_to_map(proc_name, current_procedure);
	    			}
  			}
		}
	  '{' optional_local_var_decl_list    
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null");

    				Symbol_Table * local_table = $7;

    				if (local_table == NULL)
      					local_table = new Symbol_Table();

    				current_procedure->set_local_list(*local_table);
  			}

  		}
	  statement_list '}'  
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null"); 
  
        			list<Ast *> * stmt_list = NULL;
	
          			if($9!=NULL)
               				stmt_list = $9;
          			else
               				stmt_list = new list<Ast *>;
	
         			current_procedure->set_ast_list(*stmt_list);     
	
       				current_procedure = NULL;
  			}
  		}
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
      				Symbol_Table_Entry * decl_entry = new Symbol_Table_Entry(name, $1, yylineno);
      				Symbol_Table * decl_list = new Symbol_Table();
      				decl_list->push_symbol(decl_entry);
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
		{ 	if (command_options.construct_ast())
    			{
  				Call_Ast* func_call = new Call_Ast(*$1,yylineno);
        			if($3!=NULL)
   					func_call->set_actual_param_list(*$3);

      				func_call->type_check_actual_formal_param(((program_object.get_procedure_prototype(*$1))->get_formal_param_list()));
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
	: arith_expression 
		{ 	if (command_options.construct_ast())
  				$$= $1;
  		}
;

return_statement
	: RETURN ';' 
		{ 	if (command_options.construct_ast())
    			{
      				Return_Ast* return_stmt = new Return_Ast(NULL, current_procedure->get_proc_name(), yylineno);
      				$$ = return_stmt;
  			}
		}

	| RETURN arith_expression ';'
		{ 	if (command_options.construct_ast())
    			{
      				Return_Ast* return_stmt = new Return_Ast($2, current_procedure->get_proc_name(), yylineno);
      				$$ = return_stmt;
  			}
		}
;

optional_local_var_decl_list
	: %empty 
		{ 	if (command_options.construct_ast())
        			$$ = NULL;
  		}
	| var_decl_list  
		{ 	if (command_options.construct_ast())
  			{
         			if($1!=NULL)
           				$$ = $1;
  			}  
  		}
;

var_decl_list
	: var_decl 
		{ 	if (command_options.construct_ast())
         			$$ = $1;
  		}
	| var_decl_list var_decl 
		{ 	if (command_options.construct_ast())
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

  			}
		}
;

var_decl
	: type variables_list ';'
         	{ 	if (command_options.construct_ast())
			{
				Data_Type recent_type_name = $1;
        			Symbol_Table * decl_list = $2;
	
				CHECK_INVARIANT (decl_list != NULL, "List of variables is NULL in type declarations");
	
			 	decl_list->set_type_of_all_syms(recent_type_name);
	
  				$$ = decl_list;
   		 	}	
		}
;

type
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
;

variables_list
	: NAME  
		{ 	if (command_options.construct_ast())
	  		{
	        		string name = *$1;
	      			Symbol_Table_Entry * decl_entry = new Symbol_Table_Entry(name, yylineno);    
	      			Symbol_Table * decl_list = new Symbol_Table();
	    			decl_list->push_symbol(decl_entry);
	    			$$=decl_list;
	  		}
	  	}
	| variables_list ',' NAME  
		{ 	if (command_options.construct_ast())
  			{
        			string name = *$3;
        			Symbol_Table * decl_list = $1;
        			CHECK_INPUT((decl_list->variable_in_symbol_list_check(name) == false), 
        				"Variable is declared twice", yylineno);

        			Symbol_Table_Entry * decl_entry = new Symbol_Table_Entry(name, yylineno);    
        			decl_list->push_symbol(decl_entry);
         			$$=decl_list;
  			}
  		}
;

assignment_statement
	: variable ASSIGN arith_expression ';' 
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 && $3), "Lhs/Rhs of assignment expression cannot be null");
    				$$ = new Assignment_Ast($1, $3, yylineno);
    				$$->type_check_ast();
  			}
		}
	| variable ASSIGN func_call ';' 
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 && $3), "Lhs/Rhs of assignment expression cannot be null");
    				$$ = new Assignment_Ast($1, $3, yylineno);
    				$$->type_check_ast();
  			}
		}
	| variable ASSIGN logical_expression ';'
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 && $3), "Lhs/Rhs of assignment expression cannot be null");
    				$$ = new Assignment_Ast($1, $3, yylineno);
    				$$->type_check_ast();
  			}
		}
;

if_statement
	: IF '(' logical_expression ')' statement ELSE statement 
		{ 	if (command_options.construct_ast())
			{	
    				$$ = new Selection_Statement_Ast($3,$5,$7,yylineno);
				command_options.set_no_rtl_support();	// CHECK: Temporary provision
			}
				
  		}
	| IF '(' logical_expression ')' statement  
		{ 	if (command_options.construct_ast())
			{	
    				$$ = new Selection_Statement_Ast($3,$5,NULL,yylineno);
				command_options.set_no_rtl_support();	// CHECK: Temporary provision
			}
  		}
;

do_while_statement
	: DO statement WHILE '(' logical_expression ')' ';' 
		{ 	if (command_options.construct_ast())
			{	
    				$$ = new Iteration_Statement_Ast($5,$2,yylineno,true);
				command_options.set_no_rtl_support();	// CHECK: Temporary provision
			}
		}
;

while_statement
	: WHILE '(' logical_expression ')' statement 
		{ 	if (command_options.construct_ast())
			{	
    				$$ = new Iteration_Statement_Ast($3,$5,yylineno,false);
				command_options.set_no_rtl_support();	// CHECK: Temporary provision
			}
  		}
;

compound_statement
	: '{' statement_list '}' 
		{ 	if (command_options.construct_ast())
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

print_statement
	: WRITE variable ';' 
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT($2 , "A variable cannot be null in a print statement");
    				$$ = new Write_Ast($2,yylineno);

  			}
		}
	| WRITE STRING_CONSTANT ';'
		{ 	if (command_options.construct_ast())
  			{
				string s = *$2;
				int str_num = program_object.add_string_to_string_table(s);
    				String_Constant_Ast * str_ast = new String_Constant_Ast(str_num, string_data_type, yylineno);
    				$$ = new Write_Ast(str_ast,yylineno);

  			}
		}
;

read_statement
	: READ variable ';' 
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT($2 , "A variable cannot be null in a read statement");
    				$$ = new Read_Ast($2,yylineno);
  			}
		}

;

arith_expression
	: operand '+' operand  
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 && $3), "An arith_operand cannot have null entries");
    				$$ = new Plus_Ast($1, $3, yylineno);
    				$$->set_data_type($1->get_data_type());
    				$$->type_check_ast();
  			}
  		}
	| operand '-' operand  
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 && $3), "An arith_operand cannot have null entries");
    				$$ = new Minus_Ast($1, $3, yylineno);
    				$$->set_data_type($1->get_data_type());
    				$$->type_check_ast();
  			}
  		}
	| operand '*' operand  
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 && $3), "An arith_operand cannot have null entries");
    				$$ = new Mult_Ast($1, $3, yylineno);
    				$$->set_data_type($1->get_data_type());
    				$$->type_check_ast();
  			}
  		}
	| operand '/' operand    
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 && $3), "An arith_operand cannot have null entries");
    				$$ = new Divide_Ast($1, $3, yylineno);
    				$$->set_data_type($1->get_data_type());
    				$$->type_check_ast();
  			}
  		}
	| '-' operand %prec UMINUS  
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT($2, "An arith_operand cannot have null entries");
    				$$ = new UMinus_Ast($2, NULL, yylineno);
    				$$->set_data_type($2->get_data_type());
    				$$->type_check_ast();
  			}
  		}
	| '(' operand ')' 
		{  	if (command_options.construct_ast())
  			{
    				$$ = $2;
    				$$->set_data_type($2->get_data_type());
  			}
  		}
	| logical_expression '?' operand ':' operand
  		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 != NULL) && ($3 != NULL) && ($5 != NULL), "lhs/rhs of arithmetic expression cannot be null");
    				Ast * cond = $1;
    				Ast * lhs = $3;
    				Ast * rhs = $5;
    				Ast * cond_ast = new Conditional_Expression_Ast(cond, lhs, rhs, yylineno);
    				cond_ast->type_check_ast();
    				$$ = cond_ast;
				command_options.set_no_rtl_support();	// CHECK: Temporary provision
  			}
  		}
	| expression_term  
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 != NULL), "An expression cannot have null values");
    				$$ = $1;
    				$$->set_data_type($1->get_data_type());
  			}
  		}
;

operand
	: arith_expression  
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 != NULL), "Arithmetic expression cannot be null");
    				$$ = $1;
    				$$->set_data_type($1->get_data_type());
  			}
  		}
;

expression_term
	: variable  
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 != NULL), "An expression term cannot be null");
    				$$ = $1;
  			}
  		}
	| constant  
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 != NULL), "An expression term cannot be null");
    				$$ = $1;
  			}
  		}
;

rel_expression
	: operand LT operand 
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 && $3), "A rel_operand cannot have null entries");
    				$$ = new Relational_Expr_Ast($1, less_than, $3, yylineno);
    				$$->set_data_type(bool_data_type);
    				$$->type_check_ast();
  			}
  		}
	| operand LE operand 
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 && $3), "A rel_operand cannot have null entries");
    				$$ = new Relational_Expr_Ast($1, less_equalto, $3, yylineno);
    				$$->set_data_type(bool_data_type);
    				$$->type_check_ast();
  			}
  		}
	| operand GT operand 
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 && $3), "A rel_operand cannot have null entries");
    				$$ = new Relational_Expr_Ast($1, greater_than, $3, yylineno);
    				$$->set_data_type(bool_data_type);
    				$$->type_check_ast();
  			}
  		}
	| operand GE operand 
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 && $3), "A rel_operand cannot have null entries");
    				$$ = new Relational_Expr_Ast($1, greater_equalto, $3, yylineno);
    				$$->set_data_type(bool_data_type);
    				$$->type_check_ast();
  			}
  		}
	| operand NE operand 
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 && $3), "A rel_operand cannot have null entries");
    				$$ = new Relational_Expr_Ast($1, not_equalto, $3, yylineno);
    				$$->set_data_type(bool_data_type);
    				$$->type_check_ast();
  			}
  		}
	| operand EQ operand 
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 && $3), "A rel_operand cannot have null entries");
    				$$ = new Relational_Expr_Ast($1, equalto, $3, yylineno);
    				$$->set_data_type(bool_data_type);
    				$$->type_check_ast();
  			}
  		}
;

logical_expression
	: logical_expression AND logical_expression 
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 != NULL) && ($3 != NULL), "lhs/rhs of boolean expression cannot be null");
    				Logical_Op lop = _logical_and;
    				Logical_Expr_Ast * logical_expr_ast = new Logical_Expr_Ast($1, lop, $3, yylineno);
    				logical_expr_ast->set_data_type(bool_data_type);
      				logical_expr_ast->type_check_ast();
    				$$ = logical_expr_ast;
  			}
  		}
	| logical_expression OR logical_expression 
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 != NULL) && ($3 != NULL), "lhs/rhs of boolean expression cannot be null");
    				Logical_Op lop = _logical_or;
    				Logical_Expr_Ast * logical_expr_ast = new Logical_Expr_Ast($1, lop, $3, yylineno);
    				logical_expr_ast->set_data_type(bool_data_type);
    				logical_expr_ast->type_check_ast();
    				$$ = logical_expr_ast;
  			}
		}
	| NOT logical_expression   
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($2 != NULL) , "rhs of boolean expression cannot be null");
    				Logical_Op lop = _logical_not;
    				Logical_Expr_Ast * logical_expr_ast = new Logical_Expr_Ast($2, lop, NULL, yylineno);
    				logical_expr_ast->set_data_type(bool_data_type);
    				logical_expr_ast->type_check_ast();
    				$$ = logical_expr_ast;
  			}
		}
	| '(' logical_expression ')'  
		{ 	if (command_options.construct_ast())
    				$$ = $2;
  		}
	| rel_expression                
		{ 	if (command_options.construct_ast())
    				$$ = $1;
  		}
	|  AT variable
		{ 	if (command_options.construct_ast())
    				$$ = $2;
  		}
;

variable
	: NAME   
		{ 	if (command_options.construct_ast())
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

constant
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
  		}
;
