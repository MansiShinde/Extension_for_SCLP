
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

Program program_object;

Program::Program()
{}

Program::~Program()
{}

void Program::delete_all()
{
	map<string, Procedure *>::iterator i;
	for (i = procedure_map.begin(); i != procedure_map.end(); i++)
		delete i->second;
}

void Program::set_global_table(Symbol_Table & new_global_table)
{
	global_symbol_table = new_global_table;
	global_symbol_table.set_table_scope(global);
}


bool Program::variable_proc_name_check(string symbol)
{
	if (procedure_map.find(symbol) == procedure_map.end()) // Not found
		return false;

	return true;
}

bool Program::variable_in_symbol_list_check(string variable)
{
	return global_symbol_table.variable_in_symbol_list_check(variable);
}

Symbol_Table_Entry & Program::get_symbol_table_entry(string variable_name)
{
	return global_symbol_table.get_symbol_table_entry(variable_name);
}

void Program::global_list_in_proc_check()
{
	global_symbol_table.global_list_in_proc_check();
}

void Program::print()
{
	ostream * file_buffer;

	command_options.create_ast_buffer();
	file_buffer = &(command_options.get_ast_buffer());
	*file_buffer << "Program:\n";

	map<string, Procedure *>::iterator i;
	for(i = procedure_map.begin(); i != procedure_map.end(); i++)
		i->second->print(*file_buffer);
}

void Program::set_procedure_map(map<string, Procedure *> & proc_list)
{
	procedure_map = proc_list;
}

void Program::called_proc_are_defined_check()
{
	map<string, Procedure *>::iterator i;
	for(i = procedure_map.begin(); i != procedure_map.end(); i++)
	{
		if (i->second->is_proc_called())
			CHECK_INPUT(i->second->is_proc_defined(), "Called procedure is not defined", NO_FILE_LINE);
	}
}

bool Program::variable_in_proc_map_check(string variable)
{
	if (procedure_map.find(variable) == procedure_map.end()) // Not found
		return false;

	return true;
}


void Program::global_list_in_proc_map_check()
{
	global_symbol_table.global_list_in_proc_map_check();
}

Procedure * Program::get_main_procedure(ostream & file_buffer)
{
	map<string, Procedure *>::iterator i;
	for(i = procedure_map.begin(); i != procedure_map.end(); i++)
	{
		if (i->second != NULL && i->second->get_proc_name() == "main")
				return i->second;
	}
	
	return NULL;
}

Procedure * Program::get_procedure_prototype(string proc_name)
{
	// when find functions == end(), means the string has not been found
	if (procedure_map.find(proc_name) == procedure_map.end())
	{
		CHECK_INPUT(CONTROL_SHOULD_NOT_REACH,"Procedure corresponding to the name is not found", NO_FILE_LINE);

		return NULL;
	}

	return procedure_map[proc_name];
}

void Program::set_proc_to_map(string proc_name, Procedure * proc)
{
	if (procedure_map.find(proc_name) != procedure_map.end())
	{
		Procedure * temp = procedure_map[proc_name];
		CHECK_INPUT((temp->is_proc_defined() == false), 
			"Procedure has already been defined before", NO_FILE_LINE);
	}

	procedure_map[proc_name] = NULL;
	procedure_map[proc_name] = proc;
}

bool Program::is_procedure_exists(string proc_name)
{

if (procedure_map.find(proc_name) == procedure_map.end())
	{

		return false;
	}

	return true;

}
