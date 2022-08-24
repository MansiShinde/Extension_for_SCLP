
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
#include"icode.hh"
#include"reg-alloc.hh"
#include"symbol-table.hh"
#include"ast.hh"
#include"basic-block.hh"
#include"procedure.hh"
#include"program.hh"

void Procedure::print_sym(ostream & file_buffer)
{
	//CHECK_INVARIANT((return_type == void_data_type), "Only void return type of funtion is allowed");

	file_buffer << PROC_SPACE << "Procedure: " << name << ", Return Type:";

	switch(return_type)
	{
	case void_data_type: file_buffer << "VOID\n"; break;
	case int_data_type: file_buffer << "INT\n"; break;
	case double_data_type: file_buffer << "FLOAT\n"; break;
	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}

	file_buffer << "   Local Declarartions\n";
	local_symbol_table.print(file_buffer);
}

void Procedure::compile()
{

	// assign offsets to formal symbol table
	formal_param_table.set_start_offset_of_first_symbol(8);
	formal_param_table.assign_offsets();

	// assign offsets to local symbol table
	local_symbol_table.set_start_offset_of_first_symbol(0);
	local_symbol_table.assign_offsets();




	// compile the program by visiting each basic block
	  //basic_block->compile();

     Code_For_Ast ast_code;

	machine_desc_object.validate_init_local_register_mapping();

	// compile the program by visiting each ast in the block
	list<Ast *>::iterator i;
	for (i = statement_list.begin(); i != statement_list.end(); i++)
	{
		Ast * ast = *i;

		// if (typeid(*ast) != typeid(Return_Ast))
		{
			/*
				This option has been removed. Uday.
			if (command_options.is_do_lra_selected() == true)
			{
				Lra_Outcome lra;
				ast_code = ast->compile_and_optimize_ast(lra);
			}

			else
			*/
				ast_code = ast->compile();

			list<Icode_Stmt *> & ast_icode_list = ast_code.get_icode_list();
			if (ast_icode_list.empty() == false)
			{
				if (bb_icode_list.empty())
					bb_icode_list = ast_icode_list;
				else
					bb_icode_list.splice(bb_icode_list.end(), ast_icode_list);
			}
		}
	}

	machine_desc_object.clear_local_register_mappings();

}

void Procedure::print_icode(ostream & file_buffer)
{
	file_buffer << "  Procedure: " << name << "\n";
	file_buffer << "  Intermediate Code Statements\n";

	    list<Icode_Stmt *>::iterator i;
	for (i = bb_icode_list.begin(); i != bb_icode_list.end(); i++)
		(*i)->print_icode(file_buffer);

	//basic_block->print_icode(file_buffer);
}

void Procedure::print_assembly(ostream & file_buffer)
{
	print_prologue(file_buffer);

	list<Icode_Stmt *>::iterator i;
	for (i = bb_icode_list.begin(); i != bb_icode_list.end(); i++)
		(*i)->print_assembly(file_buffer);

	//basic_block->print_assembly(file_buffer);

	print_epilogue(file_buffer);
}

void Procedure::print_prologue(ostream & file_buffer)
{
	stringstream prologue;

	prologue << "\n\
	.text \t\t\t# The .text assembler directive indicates\n\
	.globl " << name << "\t\t# The following is the code (as oppose to data)\n";

	prologue << name << ":\t\t\t\t# .globl makes main know to the \n\t\t\t\t# outside of the program.\n\
# Prologue begins \n\
	sw $ra, 0($sp)\t\t# Save the return address\n\
	sw $fp, -4($sp)\t\t# Save the frame pointer\n\
	sub $fp, $sp, 8\t\t# Update the frame pointer\n";

	int size = local_symbol_table.get_size();
	size = -size;
	prologue << "\n\tsub $sp, $sp, " << (size + 8) << "\t\t# Make space for the locals\n";
	prologue << "# Prologue ends\n\n";

	file_buffer << prologue.str();
}

void Procedure::print_epilogue(ostream & file_buffer)
{
	stringstream epilogue;

	int size = local_symbol_table.get_size();
	size = -size;

	epilogue << "\n# Epilogue Begins\nepilogue_" << name << ":\n\tadd $sp, $sp, " << (size+8) << "\n";
	epilogue << "\tlw $fp, -4($sp)  \n\tlw $ra, 0($sp)   \n\t";

	epilogue << "jr        $31\t\t# Jump back to the called procedure\n# Epilogue Ends\n\n";

	file_buffer << epilogue.str();
}
