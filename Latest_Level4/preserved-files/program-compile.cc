
/*********************************************************************************************

                                sclp : A C Language Processor
                                --------------------------------

           About:

           Originally implemented in 2012 by Tanu Kanvar and Uday Khedker 
           (http://www.cse.iitb.ac.in/~uday) for the courses cs302+cs316
           Language Processors (theory and lab) at IIT Bombay.

           Initial release date Jan 15, 2013.

           Updated by N. Venkatesh in Jan 2019.

           Updated by Uday Khedker with the help of Nisha Biju and Saari
           Rajan in Jan 2020.

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
#include"icode.hh"
#include"reg-alloc.hh"
#include"symbol-table.hh"
#include"ast.hh"
#include"basic-block.hh"
#include"procedure.hh"
#include"program.hh"

void Program::print_sym()
{
	ostream * file_buffer;

	command_options.create_symtab_buffer();
	file_buffer = &(command_options.get_symtab_buffer());

	*file_buffer << "Program:\n";

	*file_buffer << "\nGlobal Declarations:\n";
	global_symbol_table.print(*file_buffer);

	map<string, Procedure *>::iterator i;
	for(i = procedure_map.begin(); i != procedure_map.end(); i++)
		i->second->print_sym(*file_buffer);
}

void Program::gen_icode()
{
	// set up machine details
	machine_desc_object.initialize_register_table();
	machine_desc_object.initialize_instruction_table();

	// assign offsets to global variables
	global_symbol_table.set_start_offset_of_first_symbol(0);
	global_symbol_table.assign_offsets();

	map<string, Procedure *>::iterator i;
	for(i = procedure_map.begin(); i != procedure_map.end(); i++)
	{
		i->second->gen_icode();
		if(command_options.is_show_ic_selected())
		{
			ostream * file_buffer;
			command_options.create_ic_buffer();
			file_buffer = &(command_options.get_ic_buffer());

			i->second->print_icode(*file_buffer);
			*file_buffer << "\n";
		}
	}
}

void Program::print_assembly()
{
	command_options.create_output_buffer();
	ostream & file_buffer = command_options.get_output_buffer();

	if (!global_symbol_table.is_empty())
		file_buffer << "\n\t.data\n";

	global_symbol_table.print_assembly(file_buffer);

	// print each procedure
	// print each procedure
	map<string, Procedure *>::iterator i;
	for (i = procedure_map.begin(); i != procedure_map.end(); i++)
		i->second->print_assembly(file_buffer);
}
