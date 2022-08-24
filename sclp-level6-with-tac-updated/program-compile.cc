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

#include "common-headers.hh"

void Program::print_sym()
{
	ostream * file_buffer;

	command_options.create_symtab_buffer();
	file_buffer = &(command_options.get_symtab_buffer());

	*file_buffer << "Program:\n";

	*file_buffer << "\nGlobal Declarations:\n";
	global_symbol_table.print(*file_buffer);

	map<string, Class_Type_Expr *>:: iterator j;
	for(j = class_map.begin(); j != class_map.end(); j++)
		j->second->print_sym(*file_buffer);

	map<string, Procedure *>::iterator i;
	for(i = procedure_map.begin(); i != procedure_map.end(); i++)
	{
		Procedure * proc = i->second;
		CHECK_INPUT_AND_ABORT(!proc->is_proc_empty(), "Procedure " + proc->get_proc_name() + " is empty", NO_FILE_LINE);
		proc->print_sym(*file_buffer);
	}
}

void Program::print_ast()
{
	ostream * file_buffer;

	command_options.create_ast_buffer();
	file_buffer = &(command_options.get_ast_buffer());
	*file_buffer << "Program:\n";

	map<string, Procedure *>::iterator i;
	for(i = procedure_map.begin(); i != procedure_map.end(); i++)
	{
		Procedure * proc = i->second;
		CHECK_INPUT_AND_ABORT(!proc->is_proc_empty(), "Procedure " + proc->get_proc_name() + " is empty", NO_FILE_LINE);
		proc->print_ast(*file_buffer);
	}
}


void Program::gen_tac()
{
	map<string, Procedure *>::iterator i;
	for(i = procedure_map.begin(); i != procedure_map.end(); i++)
	{
		CHECK_INVARIANT(i->second!=NULL, "Procedure object pointer cannot be Null");
		Procedure * proc = i->second;
		CHECK_INPUT_AND_ABORT(!proc->is_proc_empty(), "Procedure " + proc->get_proc_name() + " is empty", NO_FILE_LINE);
		proc->gen_tac();
	}
}

void Program::print_tac()
{
	ostream * file_buffer;
	command_options.create_tac_buffer();
	file_buffer = &(command_options.get_tac_buffer());

	map<string, Procedure *>::iterator i;
	for(i = procedure_map.begin(); i != procedure_map.end(); i++)
	{
		CHECK_INVARIANT(i->second!=NULL, "Procedure object pointer cannot be Null");
		Procedure * proc = i->second;
		CHECK_INPUT_AND_ABORT(!proc->is_proc_empty(), "Procedure " + proc->get_proc_name() + " is empty", NO_FILE_LINE);
		proc->print_tac(*file_buffer);
	}
}

void Program::gen_rtl()
{
	// set up machine details
	machine_desc_object.initialize_register_table();
	machine_desc_object.initialize_instruction_table();
	//machine_desc_object.print_instruction_table(); // For testing alone.

	// assign offsets to global variables
	global_symbol_table.set_start_offset_of_first_symbol(0);
	global_symbol_table.assign_offsets();


	map<string, Procedure *>::iterator i;
	for(i = procedure_map.begin(); i != procedure_map.end(); i++)
	{
		CHECK_INVARIANT(i->second!=NULL, "Procedure object pointer cannot be Null");
		Procedure * proc = i->second;
		CHECK_INPUT_AND_ABORT(!proc->is_proc_empty(), "Procedure " + proc->get_proc_name() + " is empty", NO_FILE_LINE);
		proc->gen_rtl();
	}
}

void Program::print_rtl()
{
	ostream * file_buffer;
	command_options.create_rtl_buffer();
	file_buffer = &(command_options.get_rtl_buffer());

	map<string, Procedure *>::iterator i;
	for(i = procedure_map.begin(); i != procedure_map.end(); i++)
	{
		CHECK_INVARIANT(i->second!=NULL, "Procedure object pointer cannot be Null");
		Procedure * proc = i->second;
		CHECK_INPUT_AND_ABORT(!proc->is_proc_empty(), "Procedure " + proc->get_proc_name() + " is empty", NO_FILE_LINE);
		proc->print_rtl(*file_buffer);
	}
}

void Program::print_assembly()
{
	command_options.create_output_buffer();
	ostream & file_buffer = command_options.get_output_buffer();

	if ((!global_symbol_table.is_empty())||(!string_table.empty()))
		file_buffer << "\n\t.data\n";

	global_symbol_table.print_assembly(file_buffer);
	print_string_table(file_buffer);

	map<string, Procedure *>::iterator i;
	for (i = procedure_map.begin(); i != procedure_map.end(); i++)
	{
		Procedure * proc = i->second;
		CHECK_INPUT_AND_ABORT(!proc->is_proc_empty(), "Procedure " + proc->get_proc_name() + " is empty", NO_FILE_LINE);
		proc->print_assembly(file_buffer);
	}
}
