
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

#include<string>
#include<fstream>

using namespace std;

#include"common-classes.hh"
#include"local-environment.hh"
#include"user-options.hh"
#include"error-display.hh"
#include"symbol-table.hh"
#include"ast.hh"
#include"basic-block.hh"
#include"procedure.hh"
#include"program.hh"

Local_Environment interpreter_global_table;

Eval_Result & Program::evaluate()
{
	Procedure * main = get_main_procedure(command_options.get_output_buffer());
	CHECK_INPUT_AND_ABORT((main != NULL), "No main function found in the program", NO_FILE_LINE);

	global_symbol_table.create(interpreter_global_table);

	command_options.create_output_buffer();
	ostream & file_buffer = command_options.get_output_buffer();
	file_buffer << "Evaluating Program\n";
	file_buffer << GLOB_SPACE << "Global Variables (before evaluating):\n";

	interpreter_global_table.print(file_buffer);

    Local_Environment temp_table;
	Eval_Result & result = main->evaluate(temp_table,file_buffer);

	file_buffer << GLOB_SPACE << "Global Variables (after evaluating):\n";
	interpreter_global_table.print(file_buffer);

	return result;
}
