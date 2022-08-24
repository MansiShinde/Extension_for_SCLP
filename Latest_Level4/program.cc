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

#include<string>
#include<fstream>

using namespace std;

#include"common-classes.hh"
#include"user-options.hh"
#include"error-display.hh"
#include"symbol-table.hh"
#include"tac.hh"
#include"ast.hh"
#include"rtl.hh"
#include"procedure.hh"
#include"program.hh"

Program program_object;

Program::Program()
{
	str_count = 0;
}

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

bool Program::proc_exists(string proc_name)
{

if (procedure_map.find(proc_name) == procedure_map.end())
	{

		return false;
	}

	return true;

}

int Program::add_string_to_string_table(string s)
{
	int str_key;

	if (reverse_map_of_string_table.find(s) != reverse_map_of_string_table.end())
		str_key = reverse_map_of_string_table[s];
	else
	{ 
		reverse_map_of_string_table[s] = str_count;
		string_table[str_count] = s;
		str_key = str_count++;
	}
	return str_key;
}

string Program::get_string(int str_key)
{
	string str_value;

	if (string_table.find(str_key) != string_table.end())
		str_value = string_table[str_key];
	else
		CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH,"Wrong key for string in the string table", NO_FILE_LINE);

	return str_value;
}

void Program::print_string_table(ostream & file_buffer)
{
	for (int i=0; i<str_count; i++)
	{
		file_buffer << "_str_" << i << ":\t.ascii \"" << get_string(i) << "\"" << endl;
	}
}
