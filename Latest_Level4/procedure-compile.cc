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
#include"rtl.hh"
#include"tac.hh"
#include"reg-alloc.hh"
#include"symbol-table.hh"
#include"ast.hh"
#include"procedure.hh"
#include"program.hh"

void Procedure::print_sym(ostream & file_buffer)
{
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

void Procedure::print_ast(ostream & file_buffer)
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

	file_buffer << "   Begin: Abstract Syntax Tree ";
	list<Ast *>::iterator i;
	for(i = ast_list.begin(); i != ast_list.end(); i++)
		(*i)->print_ast(file_buffer);
	file_buffer << "\n   End: Abstract Syntax Tree \n";
}


void Procedure::gen_tac()
{
	//cout << "Generating tac for procedure " << get_proc_name() << endl; // TEST
	list<Ast *>::iterator i;
	for (i = ast_list.begin(); i != ast_list.end(); i++)
	{
		Ast * ast = *i;
		CHECK_INVARIANT(*i, "ast object pointer cannot be Null");
		TAC_For_Ast * ast_code = ast->gen_tac();

		list<TAC_Stmt *> * ast_tac = ast_code->get_tac_list();
		if (ast_tac != NULL)
		{
			if (tac_stmt_list == NULL)
				tac_stmt_list = ast_tac;
			else
				tac_stmt_list->splice(tac_stmt_list->end(), *ast_tac);
		}
	}
}

void Procedure::print_tac(ostream & file_buffer)
{
	file_buffer << "  Procedure: " << name << "\n";
	file_buffer << "  Begin: Three Address Code Statements\n";

	list<TAC_Stmt *>::iterator i;
	for (i = tac_stmt_list->begin(); i != tac_stmt_list->end(); i++)
		(*i)->print_tac_stmt(file_buffer);
	file_buffer << "  End: Three Address Code Statements\n";

}


void Procedure::gen_rtl()
{
	// assign offsets to formal symbol table
	formal_param_table.set_start_offset_of_first_symbol(8);
	formal_param_table.assign_offsets();

	// assign offsets to local symbol table
	local_symbol_table.set_start_offset_of_first_symbol(0);
	local_symbol_table.assign_offsets();
	machine_desc_object.validate_init_local_register_mapping();

	
	//cout << "Validated local register mappings " << endl; // TEST
	list<TAC_Stmt *>::iterator i;
	for (i = tac_stmt_list->begin(); i != tac_stmt_list->end(); i++)
	{
		TAC_Stmt * tac_stmt = *i;
		CHECK_INVARIANT(*i, "ast object pointer cannot be Null");
		RTL_For_TAC & rtl_code_with_result = tac_stmt->gen_rtl();

		list<RTL_Stmt *> &rtl_list = rtl_code_with_result.get_rtl_list();
		rtl_stmt_list->splice(rtl_stmt_list->end(), rtl_list);
	}
}

void Procedure::print_rtl(ostream & file_buffer)
{
	file_buffer << "  Procedure: " << name << "\n";
	file_buffer << "  Begin: RTL Statements\n";

	list<RTL_Stmt *>::iterator i;
	for (i = rtl_stmt_list->begin(); i != rtl_stmt_list->end(); i++)
	{
		// We may have NULL RTL stmt when results are temporaries
		// because we do not generate store for them. Instead,
		// their registers are forwarded to their ueses.
		if (*i)
		{
			(*i)->print_rtl_stmt(file_buffer);
		}
	}
	file_buffer << "  End: RTL Statements\n";

}


void Procedure::print_assembly(ostream & file_buffer)
{
	print_prologue(file_buffer);

	list<RTL_Stmt *>::iterator i;
	for (i = rtl_stmt_list->begin(); i != rtl_stmt_list->end(); i++)
	{
		// We may have NULL RTL stmt when results are temporaries
		// because we do not generate store for them. Instead,
		// their registers are forwarded to their ueses.
		if (*i)
		{
			(*i)->print_assembly(file_buffer);
		}
	}

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

	int size = local_symbol_table.get_size_in_bytes();
	size = -size;
	prologue << "\n\tsub $sp, $sp, " << (size + 8) << "\t\t# Make space for the locals\n";
	prologue << "# Prologue ends\n\n";

	file_buffer << prologue.str();
}

void Procedure::print_epilogue(ostream & file_buffer)
{
	stringstream epilogue;

	int size = local_symbol_table.get_size_in_bytes();
	size = -size;

	epilogue << "\n# Epilogue Begins\nepilogue_" << name << ":\n\tadd $sp, $sp, " << (size+8) << "\n";
	epilogue << "\tlw $fp, -4($sp)  \n\tlw $ra, 0($sp)   \n\t";

	epilogue << "jr        $31\t\t# Jump back to the called procedure\n# Epilogue Ends\n\n";

	file_buffer << epilogue.str();
}
