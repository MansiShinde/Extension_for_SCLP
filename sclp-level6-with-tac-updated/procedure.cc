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
#include<iostream>

using namespace std;

#include "common-headers.hh"


Procedure::Procedure(Basic_Data_Type proc_return_type, string proc_name, int line)
{
	return_type = proc_return_type;
	name = proc_name;

	lineno = line;
	proc_is_defined = false;
	proc_is_declared = false;
	tac_stmt_list = new list <TAC_Stmt*>;
	rtl_stmt_list = new list <RTL_Stmt*>;
	return_ast_exists = false;
	cfg = new CFG();
}

Procedure::~Procedure()
{
	list<Ast *>::iterator i;
	for (i = ast_list.begin(); i != ast_list.end(); i++)
		delete (*i);
	list<TAC_Stmt *>::iterator j;
	for (j = tac_stmt_list->begin(); j != tac_stmt_list->end(); j++)
		delete (*j);
	list<RTL_Stmt *>::iterator k;
	for (k = rtl_stmt_list->begin(); k != rtl_stmt_list->end(); k++)
		delete (*k);
}

bool Procedure::is_proc_called()
{
	return proc_is_called;
}

void Procedure::set_proc_is_called()
{
	proc_is_called = true;
}


bool Procedure::is_proc_empty()
{
	bool empty_proc = true;
	list<Ast *>::iterator i;
	for (i = ast_list.begin(); i != ast_list.end(); i++)
	{
		empty_proc = false;
	}
	return empty_proc;
}

bool Procedure::is_proc_defined()
{
	return proc_is_defined;
}

void Procedure::set_proc_is_defined()
{
	CHECK_INPUT_AND_ABORT(proc_is_defined==false,"Procedure "+name+" is already defined once",NO_FILE_LINE);
	proc_is_defined = true;
}

bool Procedure::is_proc_declared()
{
	return proc_is_declared;
}

void Procedure::set_proc_is_declared()
{
	CHECK_INPUT_AND_ABORT(proc_is_declared==false,"Procedure "+name+" is already declared once",NO_FILE_LINE);
	proc_is_declared = true;
}

Register_Descriptor * Procedure::get_return_register()
{
	if(get_return_type() != void_data_type)
		return return_register;
	else
	    CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "No return register for a void procedure");
}

void Procedure::set_return_register()
{

	Basic_Data_Type dt = get_return_type();
	if (dt != void_data_type)
	{
		return_register = machine_desc_object.get_return_register_for_procedure(dt);
	}
	else
		return_register = NULL;
}

TAC_Opd * Procedure::get_return_label()
{
	return return_label;
}

void Procedure::set_return_label(TAC_Opd * ret_label)
{
	return_label = ret_label;
}

TAC_Opd * Procedure::get_return_temp()
{
	return return_temp;
}

void Procedure::set_return_temp(TAC_Opd * ret_temp)
{
	return_temp = ret_temp;
}

/*
void Procedure::operator==(Procedure & proc_proto)
{
	CHECK_INPUT((return_type == proc_proto.get_return_type()), 
		"Last return statement type, of procedure, and its prototype should match", lineno);

	CHECK_INPUT((name == proc_proto.get_proc_name()), 
		"Procedure name of the procedure and its prototype should match", lineno);

	formal_param_table == proc_proto.get_formal_param_list();
}
*/

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

void Procedure::set_ast_list(list<Ast *> & al)
{
	ast_list = al;
}

Symbol_Table Procedure::get_local_list()
{

	return local_symbol_table;
}

Basic_Data_Type Procedure::get_return_type()
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

void Procedure::set_found_return_ast()
{
	return_ast_exists = true;
}

bool Procedure::contains_return_ast()
{
	return return_ast_exists;
}

Symbol_Table_Entry * Procedure::append_new_local_name(string name, Basic_Data_Type dt, int line)
{
      	Symbol_Table_Entry *se = new Symbol_Table_Entry(name, dt, line);
      	se->set_symbol_scope(local);
	local_symbol_table.push_symbol(se, line);
	//local_symbol_table.print(cout);	// TEST
	return se;
}


///////////////////////////////////////////////////////////////////////////

void Procedure:: set_proc_is_mangled()
{
	proc_mangled = true;
}


bool Procedure::is_proc_mangled()
{
	return proc_mangled;
}

void Procedure::assign_class_type_to_function(Class_Type_Expr *ci)
{
	class_prototype = ci;
}

Class_Type_Expr * Procedure::get_class_type_info()
{
	return class_prototype;
}

