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
#include "versioning.hh"
#include "class.hh"

#include <iostream>
Procedure * current_procedure = NULL;	/* This global variable is used through out the script */
Class * current_class = NULL;

int offset = 0;

Symbol_Table * create_var_decl(Symbol_Table * list1, Symbol_Table * list2);
void proce_decl_func(Data_Type type, std::string name, Symbol_Table * list);
void proce_define_create(Data_Type type, std::string name, Symbol_Table * list);

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

  Access_Specifier access_name; //added by mansi
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


%type <symbol_table> class_decl_list
%type <access_name> access_specifier_type
%type <symbol_table> block
%type <string_value> func_name
%type <symbol_table> class_declaration_stat


%type <symbol_table> global_decl_statement
%type <symbol_table> optional_local_var_decl_list
%type <symbol_table> var_decl
%type <symbol_table> var_decl_list
%type <symbol_table> variable_list
%type <ast_list> statement_list
%type <ast> statement
%type <ast> assignment_statement
%type <ast> operand
%type <ast> variable

%type <ast> variable1

%type <ast> constant
%type <ast> arith_expression
%type <ast> expression_term
%type <ast>  rel_expression
%type <ast> if_condition
/* %type <ast> logical_expression */
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

%glr-parser
%start program

%%
program 
	: global_decl_statement 
	  func_def_list 
		{	
		 	if (command_options.construct_ast())
			{
				bool main_present = program_object.get_main_procedure() != NULL;
				CHECK_INPUT_AND_ABORT(main_present, "The main function should be defined", NO_FILE_LINE)
				bool main_is_void =  program_object.get_main_procedure()->get_return_type() == void_data_type;
				CHECK_INPUT_AND_ABORT(main_is_void, "The return type of the main function should be void", NO_FILE_LINE)
				int count_of_procedures = program_object.count_of_procedures();
				if (count_of_procedures > 1)
					this_version.needs_level(L5);		
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
                           global_decl_statement because the function names are stored
                           in a map<string, Procedure *> called procedure_map in the
                           program object.
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
	
    				Symbol_Table *list = create_var_decl($1, $2);
		
       		   		Symbol_Table * global_table = list;
		
       	        		if(global_table !=NULL)
       			 		program_object.set_global_table(*global_table);  		
  			}
  		}
	|global_decl_statement class_declaration_stat
			{
				if(command_options.construct_ast()) // added by mansi
				{	
					$$ = $1;	
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

	| class_declaration_stat
		{
			if(command_options.construct_ast())  // added by mansi
			{	
				$$ = NULL;	
			}
		}
;


class_decl_list:
		    class_decl_list func_decl
			{
				if (command_options.construct_ast())
				{
         			$$=$1;
				}
			}
		    |class_decl_list var_decl
			{
				if (command_options.construct_ast())
  			{
    				$$ = create_var_decl($1, $2);	
  			}
			}
		    
		  
		    |func_decl
			{
				if (command_options.construct_ast())
				{
         				$$=NULL;	
				}
			}
	            |var_decl
			{	
				if(command_options.construct_ast())
					$$=$1;	
			}
		   
;
class_declaration_stat: 
			CLASS NAME '{' 
			{
				if (command_options.construct_ast()) //added by mansi
  				{
    					CHECK_INVARIANT(($2 != NULL), "Class name cannot be null");

    					string class_name = *$2;
			
	  		 		CHECK_INPUT_AND_ABORT((program_object.variable_in_symbol_list_check(class_name) == false), 
						"Class name "+class_name+" coincides with that of a global variable", yylineno);
		
		    				CHECK_INPUT_AND_ABORT(program_object.class_exists(class_name)==false, 
								"Current class "+class_name+" already defined", yylineno-1);
	
			    			current_class = new Class( class_name, yylineno);
			      			current_class->set_class_is_defined();
			      			program_object.set_class_to_map(class_name, current_class);	
  				}

			}
			block '}'';'
			{
				if(command_options.construct_ast())
				{
					//current_class-> assign_offset_to_class_var();
				}
			}	
;

block: 
	access_specifier_type ':' class_decl_list 
	{
		if(command_options.construct_ast())
		{
			Access_Specifier access = $1;

			if(access == public_access_specifier)
			{
				Symbol_Table * global_table = $3;
				Symbol_Table local_table = current_class->get_local_class_list();
					CHECK_INPUT((global_table->var_in_loc_glo_class_sym_list_check(local_table) == false), 
        				"Variable is declared twice", yylineno);	
				
				current_class->set_global_class_list(*global_table);
				//cout<<"No of variables in global class list:"<<global_table->get_number_of_symbols()<<"\n";
			}
			else if(access == private_access_specifier)
			{
				Symbol_Table * local_table = $3;
				Symbol_Table  global_table = current_class->get_global_class_list();
					CHECK_INPUT((local_table->var_in_loc_glo_class_sym_list_check(global_table) == false), 
        				"Variable is declared twice", yylineno);

				current_class->set_local_class_list(*local_table);
				//cout<<"No of variables in local class list:"<<local_table->get_number_of_symbols()<<"\n";
			}
		}
	}
	block {
	}
	
	| %empty
	{
		if(command_options.construct_ast())
			$$ =NULL;
	}
	
;



;
access_specifier_type: 
			PUBLIC
			{
				if(command_options.construct_ast())
				{
					//cout<<"Access Specifier is PUBLIC\n";
					$$ = public_access_specifier;	
				}
			}	
			|PRIVATE
			{
				if(command_options.construct_ast())
				{
					//cout<<"Access Specifier is PRIVATE\n";
					$$ = private_access_specifier;
				}
			}
;

func_decl :
	 type func_name '('')' ';' 
		{ 	if (command_options.construct_ast())
  			{
                                proce_decl_func($1,*$2, NULL);
  			}
		}
	| type func_name '(' formal_param_list ')' ';' 
		{ 	if (command_options.construct_ast())
  			{
				proce_decl_func($1,*$2,$4);
  			}
		}
	| func_name '(' ')' ';'   
	{
		if (command_options.construct_ast())
  			{
				proce_decl_func(no_data_type,*$1,NULL);
  			}
	}
	| func_name '(' formal_param_list ')' ';'
	{
	   	if (command_options.construct_ast())
  			{	
				proce_decl_func(no_data_type, *$1,$3);
  			}
	}
;

func_def_list 
	: func_def_list func_def 
		{
			this_version.needs_level(L5);
		}
	| func_def 
		{
		}
;

func_name: 
	NAME COCO NAME 
	{
		 if (command_options.construct_ast()){

			CHECK_INPUT_AND_ABORT(program_object.proc_exists(*$3) == true,
							"class's function cannot be defined without been declared", yylineno);
				*$$ = *$1 + "_" + *$3;;	
		}
	}
	| NAME
	{
		 if (command_options.construct_ast()){
		$$ = $1;
		}
	}

;

func_def: type func_name '(' formal_param_list ')' 
	{
	  if (command_options.construct_ast())
  		{
			proce_define_create($1,*$2, $4);
  		}	
	}
	func_body
	| type func_name '(' ')'
	{
	  if (command_options.construct_ast())
  		{
    			proce_define_create($1,*$2, NULL);	
  		}
	}
	func_body
	| func_name '(' formal_param_list ')' 
	{
		if (command_options.construct_ast())
  		{
			proce_define_create(no_data_type,*$1, $3);
  		}
	}
	func_body
	| func_name '('')'
	{
		if (command_options.construct_ast())
  		{
			proce_define_create(no_data_type,*$1, NULL);
  		}
	}
	func_body

;

func_body: '{' optional_local_var_decl_list
	   {
		if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null");

    				Symbol_Table * local_table = $2;

    				if (local_table == NULL)
      					local_table = new Symbol_Table();

    				current_procedure->set_local_list(*local_table);
  			}
	   }
	 statement_list '}'
	   {
		if(command_options.construct_ast())
			{
    				CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null"); 
  
        			list<Ast *> * stmt_list = NULL;

          			if($4!=NULL)
               				stmt_list = $4;
          			else
               				stmt_list = new list<Ast *>;

         			current_procedure->set_ast_list(*stmt_list);     
			


				if (current_procedure->get_return_type() != void_data_type && current_procedure->get_return_type() != no_data_type )
				{	
					CHECK_INPUT_AND_ABORT(current_procedure->contains_return_ast(),
							"Missing return statement in a non-void procedure", yylineno);	
				}
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
	| STRING   
		{ 	if (command_options.construct_ast())
			{ $$ = string_data_type; }
		}
	| NAME  {
		
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
	: RETURN arith_expression ';'
		{ 	
			if (command_options.construct_ast())
    			{
      				Return_Ast* return_stmt = new Return_Ast($2, current_procedure->get_proc_name(), yylineno);
				current_procedure->set_found_return_ast();
      				$$ = return_stmt;
  			}
		}
	/*
	| RETURN ';' 
		{ 	
			if (command_options.construct_ast())
    			{
      				Return_Ast* return_stmt = new Return_Ast(NULL, current_procedure->get_proc_name(), yylineno);
      				$$ = return_stmt;
  			}
		}
	*/
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

    				$$ = create_var_decl($1, $2);	
  			}
		}
;

var_decl
	: type variable_list ';'
         	{ 	if (command_options.construct_ast())
			{
				Data_Type recent_type_name = $1;
        			Symbol_Table * decl_list = $2;
			
				CHECK_INVARIANT (decl_list != NULL, "List of variables is NULL in type declarations");
	
				if(recent_type_name == class_data_type)
				{
					decl_list->set_class_name_of_all_obj(recent_type_name, current_class->get_class_name());		
				        current_class-> assign_offset_to_class_var(*decl_list);	
				}
				else
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
 	| NAME
		{ 
			if (command_options.construct_ast())
			{
				if(program_object.class_name_exists(*$1))
				{
					current_class = program_object.get_class_prototype(*$1);
				}
				$$ = class_data_type;
			} 
			//this has to return the name of the class as type...like converting the name of the class into type!!!
		} 
;
;

variable_list
	: NAME  
		{ 	if (command_options.construct_ast())
	  		{
	        		string name = *$1;

				CHECK_INPUT_AND_ABORT(!program_object.proc_name_exists(name),"Variable "+name+" coincides with a procedure name", yylineno);

	      			Symbol_Table_Entry * decl_entry = new Symbol_Table_Entry(name, yylineno);    
	      			Symbol_Table * decl_list = new Symbol_Table();
	    			decl_list->push_symbol(decl_entry);
	    			$$=decl_list;
	  		}
	  	}
	| variable_list ',' NAME  
		{ 	if (command_options.construct_ast())
  			{
        			string name = *$3;

				CHECK_INPUT_AND_ABORT(!program_object.proc_name_exists(name),"Variable "+name+" coincides with a procedure name", yylineno);

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
	| variable DOT variable1 ASSIGN arith_expression ';'
		{
			if(command_options.construct_ast())
			{
			    CHECK_INVARIANT(($1 && $3), "Object cannot be null and it cannot point to null member of class");
			   	
				Ast * addr_var = new Addr_Ast($1, yylineno);
			    	Ast * offset_ast = new  Number_Ast<int>(offset, int_data_type, yylineno);
				Ast * plus_ast = new Plus_Ast(addr_var, offset_ast, yylineno);
				Ast * lhs_ast = new Deref_Ast(plus_ast, yylineno); 
				lhs_ast->valid_operand();
				lhs_ast->set_data_type($3->get_data_type());
				cout<<"\nData type of Deref is:"<<lhs_ast->get_data_type();
				$$ = new Assignment_Ast(lhs_ast, $5, yylineno);
				
			}
		}

	| variable DOT func_call ';'
		{
			if(command_options.construct_ast())
			{

			}
		}
;

if_condition
	: '(' arith_expression ')'
		{ 	if (command_options.construct_ast())
			{
				CHECK_INPUT_AND_ABORT($2->get_data_type() == bool_data_type, "Wrong type of Condition in IF Statement", yylineno);
				$$ = $2;
			}
		}
;

if_statement
	: IF if_condition statement ELSE statement 
		{ 	if (command_options.construct_ast())
			{	
    				$$ = new Selection_Statement_Ast($2,$3,$5,yylineno);
			}
				
  		}
	| IF if_condition statement  
		{ 	if (command_options.construct_ast())
			{	
    				$$ = new Selection_Statement_Ast($2,$3,NULL,yylineno);
			}
  		}
;

do_while_statement
	: DO statement WHILE '(' arith_expression ')' ';' 
		{ 	if (command_options.construct_ast())
			{	
    				$$ = new Iteration_Statement_Ast($5,$2,yylineno,true);
			}
		}
;

while_statement
	: WHILE '(' arith_expression ')' statement 
		{ 	if (command_options.construct_ast())
			{	
    				$$ = new Iteration_Statement_Ast($3,$5,yylineno,false);
			}
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
  			}
  		}
	| operand '-' operand  
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 && $3), "An arith_operand cannot have null entries");
    				$$ = new Minus_Ast($1, $3, yylineno);
  			}
  		}
	| operand '*' operand  
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 && $3), "An arith_operand cannot have null entries");
    				$$ = new Mult_Ast($1, $3, yylineno);
  			}
  		}
	| operand '/' operand    
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 && $3), "An arith_operand cannot have null entries");
    				$$ = new Divide_Ast($1, $3, yylineno);
  			}
  		}
	| '-' operand %prec UMINUS  
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT($2, "An arith_operand cannot have null entries");
    				$$ = new UMinus_Ast($2, NULL, yylineno);
  			}
  		}
	| '(' operand ')' 
		{  	if (command_options.construct_ast())
  			{
    				$$ = $2;
  			}
  		}
	| operand '?' operand ':' operand
  		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 != NULL) && ($3 != NULL) && ($5 != NULL), "lhs/rhs of arithmetic expression cannot be null");
    				Ast * cond = $1;
    				Ast * lhs = $3;
    				Ast * rhs = $5;
    				Ast * cond_ast = new Conditional_Expression_Ast(cond, lhs, rhs, yylineno);
    				$$ = cond_ast;
  			}
  		}
	| expression_term  
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 != NULL), "An expression cannot have null values");
    				$$ = $1;
  			}
  		}
;

operand
	: arith_expression  
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 != NULL), "Arithmetic expression cannot be null");
    				$$ = $1;
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
	| variable DOT variable1
		{
			if(command_options.construct_ast())
			{
				CHECK_INVARIANT(($1 && $3), "An arith_operand cannot have null entries");
		
				Ast * addr_var = new Addr_Ast($1, yylineno);
				Ast * offset_ast = new  Number_Ast<int>(offset, int_data_type, yylineno);
				Ast * plus_ast = new Plus_Ast(addr_var, offset_ast, yylineno);
				$$ = new Deref_Ast(plus_ast, yylineno);
				$$->valid_operand();
				$$->set_data_type($3->get_data_type());
				cout<<"\nData type of Deref is:"<<$$->get_data_type();
		
			}
		}
;

rel_expression
	: operand LT operand 
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 && $3), "A rel_operand cannot have null entries");
    				$$ = new Relational_Expr_Ast($1, less_than, $3, yylineno);
  			}
  		}
	| operand LE operand 
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 && $3), "A rel_operand cannot have null entries");
    				$$ = new Relational_Expr_Ast($1, less_equalto, $3, yylineno);
  			}
  		}
	| operand GT operand 
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 && $3), "A rel_operand cannot have null entries");
    				$$ = new Relational_Expr_Ast($1, greater_than, $3, yylineno);
  			}
  		}
	| operand GE operand 
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 && $3), "A rel_operand cannot have null entries");
    				$$ = new Relational_Expr_Ast($1, greater_equalto, $3, yylineno);
  			}
  		}
	| operand NE operand 
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 && $3), "A rel_operand cannot have null entries");
    				$$ = new Relational_Expr_Ast($1, not_equalto, $3, yylineno);
  			}
  		}
	| operand EQ operand 
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 && $3), "A rel_operand cannot have null entries");
    				$$ = new Relational_Expr_Ast($1, equalto, $3, yylineno);
  			}
  		}
;

arith_expression
	: operand AND operand 
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 != NULL) && ($3 != NULL), "lhs/rhs of boolean expression cannot be null");
    				$$ = new Logical_Expr_Ast($1, and_op, $3, yylineno);
  			}
  		}
	| operand OR operand 
		{ 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(($1 != NULL) && ($3 != NULL), "lhs/rhs of boolean expression cannot be null");
    				$$ = new Logical_Expr_Ast($1, or_op, $3, yylineno);
  			}
		}
	| NOT operand   
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

variable1: NAME
		{
			if(command_options.construct_ast())
			{
				Symbol_Table_Entry * var_table_entry;
				
				if(current_class!=NULL && current_class->variable_in_global_class_list_check(*$1))
				{
					var_table_entry = &(current_class->get_global_class_symbol_table_entry(*$1));
			   
					offset = current_class->get_class_var_start_offset(*$1);

					cout<<"\n Value of Offset is:"<<offset;
				}
				else
      					CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, "Variable "+*$1+" has not been declared", yylineno);

    				$$ = new Name_Ast(*$1, *var_table_entry, yylineno);
			}
		}

;
variable
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
					if(var_table_entry->get_data_type() == class_data_type )
					{
						current_class = program_object.get_class_prototype(var_table_entry->get_class_name());
						//cout<<"current class name:"<<current_class->get_class_name()<<" at line no:"<<yylineno<<"\n";
					}
				}
    				else if (current_procedure->variable_in_formal_list_check(*$1))
       					var_table_entry = &(current_procedure->get_formal_param_entry(*$1));
    				else if (program_object.variable_in_symbol_list_check(*$1))
      					var_table_entry = &(program_object.get_symbol_table_entry(*$1));
    				else if(current_class->variable_in_global_class_list_check(*$1))
				{
					var_table_entry = &(current_class->get_global_class_symbol_table_entry(*$1));
			   
				}
				else
      					CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, "Variable "+*$1+" has not been declared", yylineno);

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
%%


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


void proce_decl_func(Data_Type type, std::string name, Symbol_Table * list) {
	
				CHECK_INVARIANT((name != " " ), "Procedure name cannot be null");

    				string proc_name = name;
				
				Data_Type recent_type_name = type;


  		 		CHECK_INPUT_AND_ABORT((program_object.variable_in_symbol_list_check(proc_name) == false), 
					"Procedure name "+proc_name+" coincides with that of a global variable", yylineno);
				
				 Procedure *pdecl = NULL;
				if(recent_type_name == no_data_type )
				{
    					 CHECK_INPUT_AND_ABORT((current_class->get_class_name() == name), 
"Constructor name "+name+ " should be same as that of its class "+current_class->get_class_name() + " else the function should have some return type", yylineno);
				}

				 pdecl = new Procedure(recent_type_name, proc_name, yylineno);

				if(list !=NULL)
				{
    				Symbol_Table * formal_param_table = list;
    				pdecl->set_formal_param_list(*formal_param_table);
				}

    				program_object.set_proc_to_map(proc_name,pdecl);
}

void proce_define_create(Data_Type type, std::string name, Symbol_Table * list) {

	CHECK_INVARIANT((name != " "), "Procedure name cannot be null");

    	string proc_name = name;
				
	Data_Type recent_type_name = type;
			
  	CHECK_INPUT_AND_ABORT((program_object.variable_in_symbol_list_check(proc_name) == false), 
					"Procedure name "+proc_name+" coincides with that of a global variable", yylineno);

    				if(program_object.proc_exists(proc_name))
				{
	    				current_procedure=program_object.get_procedure_prototype(proc_name);

	    				CHECK_INPUT_AND_ABORT(current_procedure->is_proc_defined()==false, 
							"Current procedure "+proc_name+" already defined", yylineno-1);
					
					if(list!=NULL)
					{	
	    				Symbol_Table * new_formal_param_table = list;
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
	
					current_procedure->set_formal_param_list(*new_formal_param_table);
					}
					
					if(type!=no_data_type)
						CHECK_INPUT_AND_ABORT(current_procedure->get_return_type()==recent_type_name,
							"Return type in declaration and definition do not match", yylineno-1);	      						
					
					current_procedure->set_proc_is_defined();	
		    		}
		    		else
				{
					
		    			current_procedure = new Procedure(recent_type_name, proc_name, yylineno);
					
		    			Symbol_Table * formal_param_table = list;
				
		      			current_procedure->set_proc_is_defined();
					
					if(list!=NULL)
		      				current_procedure->set_formal_param_list(*formal_param_table);

		      			program_object.set_proc_to_map(proc_name, current_procedure);
		    		}
}
