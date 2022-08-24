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
#include<iostream>

using namespace std;

#include"common-classes.hh"
#include"user-options.hh"
#include"error-display.hh"
#include"user-options.hh"
#include"symbol-table.hh"
#include"tac.hh"
#include"ast.hh"
#include"rtl.hh"
#include"procedure.hh"
#include"program.hh"
#include"class.hh"

Symbol_Table::Symbol_Table()
{
	start_offset_of_first_symbol = 0;
	size_in_bytes = 0;
}

Symbol_Table::~Symbol_Table()
{}

void Symbol_Table::set_table_scope(Table_Scope list_scope)
{
	scope = list_scope;

	list<Symbol_Table_Entry *>::iterator i;
	for(i = variable_table.begin(); i != variable_table.end(); i++)
		(*i)->set_symbol_scope(list_scope);
}


void Symbol_Table :: operator=(const Symbol_Table & obj) //added by mansi
{
	variable_table = obj.variable_table;
	scope = obj.scope;	
}



void Symbol_Table::set_type_of_all_syms(Data_Type dt)
{
	list<Symbol_Table_Entry *>::iterator i;

	for(i = variable_table.begin(); i != variable_table.end(); i++)
		(*i)->set_data_type(dt);
}

//******************************************
void Symbol_Table:: set_class_name_of_all_obj(Data_Type dt, string className) //mansi
{
	list<Symbol_Table_Entry *>::iterator i;

	for(i = variable_table.begin(); i != variable_table.end(); i++)
	{
		(*i)->set_data_type(dt);
		(*i)->set_class_name(className);
	}

}
//*****************************************

Table_Scope Symbol_Table::get_table_scope()
{
	return scope;
}
Offset_With_Ref_To Symbol_Table_Entry::get_ref_offset()
{
	return ref_off;
}

bool Symbol_Table::is_empty()
{
	return variable_table.empty();
}

void Symbol_Table::push_symbol(Symbol_Table_Entry * variable)
{
	variable_table.push_back(variable);
}

void Symbol_Table::global_list_in_proc_check()
{
	list<Symbol_Table_Entry *>::iterator i;
	for (i = variable_table.begin(); i != variable_table.end(); i++)
	{
		string name = (*i)->get_variable_name();
		CHECK_INPUT((program_object.proc_name_exists(name) == false),
			"Global variable should not match procedure name", NO_FILE_LINE);
	}
}

void Symbol_Table::global_list_in_proc_map_check()
{
	list<Symbol_Table_Entry *>::iterator i;
	for (i = variable_table.begin(); i != variable_table.end(); i++)
	{
		string name = (*i)->get_variable_name();
		program_object.proc_name_exists(name);
	}
}


bool Symbol_Table::variable_in_symbol_list_check(string variable)
{
	list<Symbol_Table_Entry *>::iterator i;
	for (i = variable_table.begin(); i != variable_table.end(); i++)
	{
		if ((*i)->get_variable_name() == variable)
			return true;
	}

	return false;
}


 
bool Symbol_Table :: var_in_loc_glo_class_sym_list_check(Symbol_Table new_list)  //mansi
{
       list<Symbol_Table_Entry *>::iterator i ;
       for (i = variable_table.begin(); i != variable_table.end(); i++)
       {
	      if(new_list.variable_in_symbol_list_check((*i)->get_variable_name()) == true)
			return true;
       }
	
	return false;
}
//////////////////////////////////////////////////////////////////////////////////////

Symbol_Table_Entry & Symbol_Table::get_symbol_table_entry(string variable_name)
{
	list<Symbol_Table_Entry *>::iterator i;
	for (i = variable_table.begin(); i != variable_table.end(); i++)
	{
		if ((*i)->get_variable_name() == variable_name)
			return **i;
	}
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "The variable symbol entry doesn't exist");
}


void Symbol_Table_Entry :: set_class_size(int class_size) 
{
  size_of_class = -class_size;
 // cout<<"Size of object of class is:"<<size_of_class<<"\n";
}

int Symbol_Table_Entry :: get_size_of_class_obj()
{
	return size_of_class;
}


void Symbol_Table ::  set_size_of_class_obj(int size)
{
	list<Symbol_Table_Entry *>:: iterator i;
	for(i = variable_table.begin(); i != variable_table.end(); i++)
	{
		(*i)->set_class_size(size);
	}
	
}

/////////////////////////////////////////////////////////////

Symbol_Table_Entry::Symbol_Table_Entry()
{
	variable_data_type = void_data_type;

	start_offset = end_offset = 0;
	register_description = NULL;
      ref_off = fp_ref;
}

Symbol_Table_Entry::Symbol_Table_Entry(string & name, Data_Type new_data_type, int line, Offset_With_Ref_To ro)
{
	variable_name = name;
	variable_data_type = new_data_type;

	start_offset = end_offset = 0;
	register_description = NULL;
	ref_off = ro;

	lineno = line;
}

Symbol_Table_Entry::Symbol_Table_Entry(string & name, int line, Offset_With_Ref_To ro)
{
	variable_name = name;
	variable_data_type = void_data_type;

	start_offset = end_offset = 0;
	register_description = NULL;
	ref_off = ro;

	lineno = line;
}


Symbol_Table_Entry::~Symbol_Table_Entry()
{}

bool Symbol_Table_Entry::operator==(Symbol_Table_Entry & entry)
{
	if (variable_name != entry.get_variable_name())
		return false;
	else if (variable_data_type != entry.get_data_type())
		return false;
	else if (scope != entry.get_symbol_scope())
		return false;

	return true;
}

void Symbol_Table_Entry::set_symbol_scope(Table_Scope sp)
{
	scope = sp;
}

Table_Scope Symbol_Table_Entry::get_symbol_scope()
{
	return scope;
}

int Symbol_Table_Entry::get_lineno()
{
	return lineno;
}

Data_Type Symbol_Table_Entry::get_data_type()
{
	return variable_data_type;
}

void Symbol_Table_Entry::set_data_type(Data_Type dt)
{
	variable_data_type = dt;
}

void Symbol_Table_Entry::set_class_name(string className)//mansi
{
   class_name = className;
}

string Symbol_Table_Entry::get_class_name()//mansi
{
   return class_name;
}


Access_Specifier Symbol_Table_Entry:: get_access_type() //added by mansi
{
	return access;
}

void Symbol_Table_Entry :: set_access_type(Access_Specifier as) //added by mansi
{
	access = as;
}



string Symbol_Table_Entry::get_variable_name()
{
	return variable_name;
}
//added
void  Symbol_Table::append_list(Symbol_Table sym_t,int lineno)
{
 
   list<Symbol_Table_Entry *>::iterator i;
	
	for (i = sym_t.variable_table.begin(); i != sym_t.variable_table.end(); i++)
	{
		string variable_name = (*i)->get_variable_name();

          list<Symbol_Table_Entry *>::iterator t;

	   for (t = variable_table.begin(); t != variable_table.end(); t++)
	  {
 			CHECK_INPUT(((*t)->get_variable_name() == variable_name)==false , "Variable is declared twice", lineno);	
			   
	  }
	   
	    push_symbol(*i);

    }


}

/*

void Symbol_Table::operator==(Symbol_Table & formal_list)
{
	list<Symbol_Table_Entry *> & f_list = formal_list.get_table();

	list<Symbol_Table_Entry *>::iterator i;
	list<Symbol_Table_Entry *>::iterator j;

	for (i = variable_table.begin(), j = f_list.begin(); i != variable_table.end() && j != f_list.end(); i++, j++)
	{
		Data_Type type1 = (*i)->get_data_type();
		Data_Type type2 = (*j)->get_data_type();

		CHECK_INPUT((type1 == type2), "Data type of one of the parameters of the procedure and its prototype doesn't match", NO_FILE_LINE);

		string name1 = (*i)->get_variable_name();
		string name2 = (*j)->get_variable_name();

		CHECK_INPUT((name1 == name2), "Variable name of one of the parameters of the procedre and its prototypes doesn't match", NO_FILE_LINE);
	}

	CHECK_INPUT((i == variable_table.end() && j == f_list.end()),
			"Procedure and its prototype parameter f_list length doens't match", NO_FILE_LINE);
}

*/

list<Symbol_Table_Entry *> & Symbol_Table::get_table()
{
	return variable_table;
}

bool Symbol_Table::variable_in_formal_list_check(string variable)
{
	list<Symbol_Table_Entry *>::iterator i;
	for (i = variable_table.begin(); i != variable_table.end(); i++)
	{
		if ((*i)->get_variable_name() == variable)
			return true;
	}

	return false;
}

string Symbol_Table::get_variable_in_formal_list(int position)
{
	int count = 1;

	list<Symbol_Table_Entry *>::iterator i;
	for (i = variable_table.begin(); i != variable_table.end(); i++, count++)
	{
		if (count == position)
			return (*i)->get_variable_name();
	}

	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Actual and formal parameter should match");
}

Symbol_Table_Entry &  Symbol_Table::get_symbol_table_entry_by_index(int position)
{
	int count = 1;

	list<Symbol_Table_Entry *>::iterator i;
	for (i = variable_table.begin(); i != variable_table.end(); i++, count++)
	{
		if (count == position)
			return *(*i);
	}

	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Actual and formal parameter should match");
}
