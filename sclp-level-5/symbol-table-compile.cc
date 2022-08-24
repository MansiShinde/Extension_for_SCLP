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
#include"rtl.hh"
#include"tac.hh"
#include"reg-alloc.hh"
#include"symbol-table.hh"
#include"ast.hh"
#include"procedure.hh"
#include"program.hh"

void Symbol_Table::set_start_offset_of_first_symbol(int n)
{
	start_offset_of_first_symbol = n;
}

int Symbol_Table::get_start_offset_of_first_symbol()
{
	return start_offset_of_first_symbol;
}

int Symbol_Table::get_size_in_bytes()
{
	return size_in_bytes;
}

int Symbol_Table::get_number_of_symbols()
{
	return variable_table.size();
}

void Symbol_Table::assign_offsets()
{
	size_in_bytes = start_offset_of_first_symbol;
	int offset;

	list<Symbol_Table_Entry *>::iterator i;
	for (i = variable_table.begin(); i != variable_table.end(); i++)
	{
		int size;
		if (((*i)->get_symbol_scope() == local) || ((*i)->get_symbol_scope() == global))
		{
			if((*i)->get_data_type() == class_data_type)
			{
				size = (*i)->get_size_of_class_obj();
			}
			else
			size = -(get_size_of_value_type((*i)->get_data_type()));

			(*i)->set_start_offset(size_in_bytes);
			size_in_bytes -= size;
			(*i)->set_end_offset(size_in_bytes);
		}

		else if ((*i)->get_symbol_scope() == formal)
		{
			size = get_size_of_value_type((*i)->get_data_type());

			(*i)->set_end_offset(size_in_bytes);
			size_in_bytes -= size;
			(*i)->set_start_offset(size_in_bytes);
		}

		else
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option for scope of symbol");


	}

}

int Symbol_Table::get_size_of_value_type(Data_Type dt)
{
	switch(dt)
	{
		case int_data_type: return -4; break;
		case double_data_type: return -8; break;
		case string_data_type: return -4; break;
		case bool_data_type: return -4; break;
		//case class_data_type: return -get_class_size(); break;
	case void_data_type: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Attempt to seek size of type void");
	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Data type not supported");
	}
}

void Symbol_Table::print_assembly(ostream & file_buffer)
{
	list<Symbol_Table_Entry *>::iterator i;
	for (i = variable_table.begin(); i != variable_table.end(); i++)
	{
		if (scope == global) {

			  if(((*i)->get_data_type()==int_data_type) 
                               || ((*i)->get_data_type()==bool_data_type)
                               || ((*i)->get_data_type()==string_data_type))
			file_buffer << (*i)->get_variable_name() << ":\t.word 0\n";
		    if( (*i)->get_data_type()==double_data_type)
			file_buffer << (*i)->get_variable_name() << ":\t.double 0.0\n";
		}



	}
}

void Symbol_Table::print(ostream & file_buffer)
{
	list<Symbol_Table_Entry *>::iterator i;

	for(i = variable_table.begin(); i != variable_table.end(); i++)
	{
		string name = (*i)->get_variable_name();
		Data_Type dt = (*i)->get_data_type();
		int start_off = (*i)->get_start_offset();
		int end_off = (*i)->get_end_offset();

		file_buffer << "   Name: " << name;

		switch(dt)
		{
		case int_data_type: file_buffer << " Type: INT"; break;
		case double_data_type: file_buffer << " Type : Double"; break;
		case string_data_type: file_buffer << " Type : String"; break;
		case bool_data_type: file_buffer << " Type : Bool"; break;
		case class_data_type: file_buffer <<" Type:"<<(*i)->get_class_name();
							break;
		defualt: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Wrong data type while printing symbol table");
		} 

		file_buffer << " Entity Type: VAR";

		if (start_off == end_off)
			file_buffer << " (No offset assigned yet)\n";
		else
			file_buffer << " Start Offset: " << -start_off << " End Offset: " << -end_off << "\n";
	}
}

int Symbol_Table:: get_start_offset_of_var(string varname) //mansi
{
	list<Symbol_Table_Entry *>::iterator i;
	int offset;

	for(i = variable_table.begin(); i != variable_table.end(); i++)
	{
		string name = (*i)->get_variable_name();
		//cout<<"variable name is:"<<name<<"\n";
		//cout<<"Value of offset:"<<(*i)->get_start_offset()<<"\n";
		
		if(varname == name)
			{
				offset = (*i)->get_start_offset();
				//cout<<"Value if offset:"<<offset<<"\n";
				break;
			}
		
	}
	return -offset;
}

/////////////////////////////////////////////////////////////

int Symbol_Table_Entry::get_start_offset()	{ return start_offset; }
int Symbol_Table_Entry::get_end_offset()	{ return end_offset; }
void Symbol_Table_Entry::set_start_offset(int n) { start_offset = n; }
void Symbol_Table_Entry::set_end_offset(int n)	{ end_offset = n; }

void Symbol_Table_Entry::set_register(Register_Descriptor * reg)
{
	register_description = reg;
}

Register_Descriptor * Symbol_Table_Entry::get_register()
{
	return register_description;
}

void Symbol_Table_Entry::free_register(Register_Descriptor * destination_reg_descr)
{
	CHECK_INVARIANT((destination_reg_descr != NULL), 
			"The register descriptor of the destination should not be NULL while freeing a register");

	/* Remove the destination from its register descriptor */
	destination_reg_descr->reset_use_for_expr_result();
	/* Remove the register descriptor from the destination */
	set_register(NULL);
}

void Symbol_Table_Entry::update_register(Register_Descriptor * result_reg_descr)
{
	CHECK_INVARIANT((result_reg_descr != NULL), 
			"The register descriptor of the result should not be NULL while updating register information");

	Register_Descriptor * destination_reg_descr = get_register();

	if(destination_reg_descr != NULL)
		free_register(destination_reg_descr);

	/* set the register of the destination as the new register */
	set_register(result_reg_descr);

	/* add the destination to the list of sym of the register */
	result_reg_descr->set_use_for_expr_result();
}
