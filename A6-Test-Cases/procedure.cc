
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
#include<iostream>

using namespace std;

#include"common-classes.hh"
#include"error-display.hh"
#include"user-options.hh"
#include"local-environment.hh"
#include"symbol-table.hh"
#include"ast.hh"
#include"basic-block.hh"
#include"procedure.hh"
#include"program.hh"

Procedure::Procedure(Data_Type proc_return_type, string proc_name, int line)
{
	return_type = proc_return_type;
	name = proc_name;

	lineno = line;
	 proc_is_defined = false;
}

Procedure::~Procedure()
{
	list<Ast *>::iterator i;
	for (i = statement_list.begin(); i != statement_list.end(); i++)
		delete (*i);
	//delete basic_block;
}

bool Procedure::is_proc_called()
{
	return proc_is_called;
}

void Procedure::set_proc_is_called()
{
	proc_is_called = true;
}

bool Procedure::is_proc_defined()
{
	return proc_is_defined;
}

void Procedure::set_proc_is_defined()
{
	proc_is_defined = true;
}

void Procedure::operator==(Procedure & proc_proto)
{
	CHECK_INPUT((return_type == proc_proto.get_return_type()), 
		"Last return statement type, of procedure, and its prototype should match", lineno);

	CHECK_INPUT((name == proc_proto.get_proc_name()), 
		"Procedure name of the procedure and its prototype should match", lineno);

	formal_param_table == proc_proto.get_formal_param_list();
}

void Procedure::operator=(Procedure & proc_proto)
{
	name = proc_proto.get_proc_name();
	return_type = proc_proto.get_return_type();
	formal_param_table = proc_proto.get_formal_param_list();
}

Symbol_Table & Procedure::get_formal_param_list()
{
	return formal_param_table;
}


string Procedure::get_proc_name()
{
	return name;
}

string Procedure::get_variable_in_formal_list(int position)
{
	return formal_param_table.get_variable_in_formal_list(position);
}



void Procedure::set_local_list(Symbol_Table & new_list)
{
	local_symbol_table = new_list;
	local_symbol_table.set_table_scope(local);
}

void Procedure::set_ast_list(list<Ast *> & ast_list)
{
	statement_list = ast_list;
}

Symbol_Table Procedure::get_local_list()
{

	return local_symbol_table;
}

Data_Type Procedure::get_return_type()
{
	return return_type;
}

bool Procedure::variable_in_symbol_list_check(string variable)
{
	return local_symbol_table.variable_in_symbol_list_check(variable);
}

Symbol_Table_Entry & Procedure::get_symbol_table_entry(string variable_name)
{
	return local_symbol_table.get_symbol_table_entry(variable_name);
}

void Procedure::print(ostream & file_buffer)
{
	file_buffer << PROC_SPACE << "Procedure: " << name << ", Return Type: ";

	switch(return_type)
	{
	case void_data_type: file_buffer << "VOID\n"; break;
	case int_data_type: file_buffer << "INT\n"; break;
	case double_data_type: file_buffer << "FLOAT\n"; break;
	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}

	list<Symbol_Table_Entry *>::iterator si;

	for (si = formal_param_table.get_table().begin(); si != formal_param_table.get_table().end(); si++)
	{
		file_buffer<<"\t"<<(*si)->get_variable_name() << " " <<" Type:"; 
		switch((*si)->get_data_type())
	{
	case void_data_type: file_buffer << "VOID\n"; break;
	case int_data_type: file_buffer << "INT\n"; break;
	case double_data_type: file_buffer << "FLOAT\n"; break;
	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}
	}


	
	list<Ast *>::iterator i;
	for(i = statement_list.begin(); i != statement_list.end(); i++)
		(*i)->print(file_buffer);
}

void Procedure::set_formal_param_list(Symbol_Table & new_list)
{
	formal_param_table = new_list;
	formal_param_table.set_table_scope(formal);
}

bool Procedure::variable_in_formal_list_check(string variable)
{
	return formal_param_table.variable_in_symbol_list_check(variable);
}

Symbol_Table_Entry & Procedure::get_formal_param_entry(string variable_name)
{
	return formal_param_table.get_symbol_table_entry(variable_name);
}

