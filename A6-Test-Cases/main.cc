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

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <list>
#include <stdio.h>

using namespace std;

#include "common-classes.hh"
#include "local-environment.hh"
#include "error-display.hh"
#include "user-options.hh"
#include "symbol-table.hh"
#include "ast.hh"
#include "basic-block.hh"
#include "procedure.hh"
#include "program.hh"

extern int yyparse();
extern  FILE *yyin;
extern int yylineno;
extern  FILE *yyout;

User_Options command_options;

#include <dirent.h>

int main(int argc, char * argv[]) 
{
	string input_file_name = command_options.process_user_command_options(argc, argv);
 
         if(command_options.is_show_tokens_selected())
          command_options.create_tokens_buffer();
      
      yyin = fopen(input_file_name.c_str(),"r");
       

	CHECK_INPUT((!yyparse()), "Cannot parse the input program", yylineno);
	
	if (command_options.not_only_parse)
	{
		if ((error_status() == false) && (command_options.is_show_ast_selected()))
			    program_object.print();

		if ((error_status() == false) && (command_options.is_show_symtab_selected()))
		{ 
			   program_object.print_sym();
		}

		if ((error_status() == false) && (command_options.is_do_eval_selected()))
			   program_object.evaluate();

		if ((error_status() == false) && (command_options.is_do_eval_selected() == false))
		{	
			  program_object.compile();

			if (command_options.is_show_symtab_selected())
				program_object.print_sym();
		}

		program_object.delete_all();
	}

	return 0;
}
