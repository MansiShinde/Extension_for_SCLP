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

#include"common-classes.hh"
#include"error-display.hh"
#include"user-options.hh"
#include"symbol-table.hh"
#include"tac.hh"
#include"ast.hh"
#include"rtl.hh"
#include"procedure.hh"
#include"program.hh"
#include"reg-alloc.hh"
#include"class.hh"


Class::Class(string class_name, int line)
{
	
	name = class_name;
	lineno = line;

	class_is_defined = false;
	//tac_stmt_list = new list <TAC_Stmt*>;
	//rtl_stmt_list = new list <RTL_Stmt*>;
	//return_ast_exists = false;
}

Class::~Class()
{
	list<Ast *>::iterator i;
	//for (i = ast_list.begin(); i != ast_list.end(); i++)
	//	delete (*i);
	//list<TAC_Stmt *>::iterator j;
	//for (j = tac_stmt_list->begin(); j != tac_stmt_list->end(); j++)
	//	delete (*j);
	//list<RTL_Stmt *>::iterator k;
	//for (k = rtl_stmt_list->begin(); k != rtl_stmt_list->end(); k++)
	//	delete (*k);
}


bool Class::is_class_defined()
{
	return class_is_defined;
}


void Class::set_class_is_defined()
{
	class_is_defined = true;
}


string Class::get_class_name()
{
	return name;
}


void Class::set_local_class_list(Symbol_Table & new_list)
{
	local_class_symbol_table = new_list;
	local_class_symbol_table.set_table_scope(local);
}

void Class::set_global_class_list(Symbol_Table & new_list)
{
	global_class_symbol_table = new_list;
	global_class_symbol_table.set_table_scope(global);
}

/*

void Class::set_ast_list(list<Ast *> & al)
{
	ast_list = al;
}
*/

Symbol_Table Class::get_local_class_list()
{

	return local_class_symbol_table;
}

Symbol_Table Class::get_global_class_list()
{

	return global_class_symbol_table;
}


bool Class::variable_in_local_symbol_list_check(string variable)
{
	return local_class_symbol_table.variable_in_symbol_list_check(variable);
}


bool Class::variable_in_global_class_list_check(string variable)
{
	return global_class_symbol_table.variable_in_symbol_list_check(variable);
}


Symbol_Table_Entry & Class::get_local_class_symbol_table_entry(string variable_name)
{
	return local_class_symbol_table.get_symbol_table_entry(variable_name);
}


Symbol_Table_Entry & Class::get_global_class_symbol_table_entry(string variable_name)
{
	return global_class_symbol_table.get_symbol_table_entry(variable_name);
}

void Class:: assign_offset_to_class_var(Symbol_Table & list)
{
	global_class_symbol_table.set_start_offset_of_first_symbol(0);
	global_class_symbol_table.assign_offsets();

	local_class_symbol_table.set_start_offset_of_first_symbol(0);
	local_class_symbol_table.assign_offsets();

	list.set_size_of_class_obj(global_class_symbol_table.get_size_in_bytes() + local_class_symbol_table.get_size_in_bytes());
}

int Class:: get_class_var_start_offset(string var_name)
{
   return global_class_symbol_table.get_start_offset_of_var(var_name);
}


void Class:: print_sym(ostream & file_buffer)
{
	file_buffer << "\n**CLASS: " << name<<"\n" ;
	file_buffer << "\n Public Declarartions\n";
	global_class_symbol_table.print(file_buffer);

	file_buffer << "  \n Private Declarartions\n";

	local_class_symbol_table.print(file_buffer);

}
