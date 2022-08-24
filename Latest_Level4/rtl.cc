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

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>

using namespace std;

#include "common-classes.hh"
#include "error-display.hh"
#include "tac.hh"
#include "rtl.hh"
#include "reg-alloc.hh"
#include "symbol-table.hh"
#include "ast.hh"
#include "program.hh"

/****************************** Class RTL_Opd *****************************/

Register_Descriptor * RTL_Opd::get_reg()
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method get_reg not implemented");
}

/****************************** Class Mem_Opd *****************************/

Mem_Opd::Mem_Opd(Symbol_Table_Entry & se) 
{
	symbol_entry = &se;
}

Mem_Opd & Mem_Opd::operator=(const Mem_Opd & rhs)
{
	this->symbol_entry = rhs.symbol_entry;
	return *this;
}

void Mem_Opd::print_rtl_opd(ostream & file_buffer) 
{
	file_buffer << symbol_entry->get_variable_name();
}

void Mem_Opd::print_asm_opd(ostream & file_buffer) 
{
	Table_Scope symbol_scope = symbol_entry->get_symbol_scope();

	CHECK_INVARIANT(((symbol_scope == local) || (symbol_scope == global) || (symbol_scope == formal)), 
			"Wrong scope value");

	if ((symbol_scope == local) || (symbol_scope == formal))
	{
		if(symbol_entry->get_ref_offset() == fp_ref)
		{
			int offset = symbol_entry->get_start_offset();
			file_buffer << offset << "($fp)";
		}
		else if (symbol_entry->get_ref_offset() == sp_ref)
		{
			int offset = symbol_entry->get_start_offset();
			file_buffer << offset << "($sp)";
		}
	}
	else
		file_buffer << symbol_entry->get_variable_name();
}

/****************************** Class Register_Opd *****************************/

Register_Opd::Register_Opd(Register_Descriptor * reg) 
{
	register_description = reg;
}

Register_Descriptor * Register_Opd::get_reg()    
{ 
	return register_description;
}

Register_Opd& Register_Opd::operator=(const Register_Opd& rhs)
{
	this->register_description = rhs.register_description;
	return *this;
}

void Register_Opd::print_rtl_opd(ostream & file_buffer) 
{
	file_buffer<<register_description->get_name();
}

void Register_Opd::print_asm_opd(ostream & file_buffer) 
{
	file_buffer<<"$"<<register_description->get_name();
}

/****************************** Class Int_Const_Opd *****************************/

Int_Const_Opd::Int_Const_Opd(int n) 
{
	num = n;
}

Int_Const_Opd & Int_Const_Opd::operator=(const Int_Const_Opd & rhs)
{
	this->num = rhs.num;
	return *this;
}

void Int_Const_Opd::print_rtl_opd(ostream & file_buffer) 
{
	file_buffer<<num;
}

void Int_Const_Opd::print_asm_opd(ostream & file_buffer) 
{
	file_buffer<<num;
}

/****************************** Class Str_Const_Opd *****************************/

Str_Const_Opd::Str_Const_Opd(int n) 
{
	str_key = n;
}

Str_Const_Opd & Str_Const_Opd::operator=(const Str_Const_Opd & rhs)
{
	this->str_key = rhs.str_key;
	return *this;
}

void Str_Const_Opd::print_rtl_opd(ostream & file_buffer) 
{
	file_buffer << "_str_" << str_key;
}

void Str_Const_Opd::print_asm_opd(ostream & file_buffer) 
{
	file_buffer << "_str_" << str_key;
}



/****************************** Class Double_Const_Opd *****************************/

Double_Const_Opd::Double_Const_Opd(double n) 
{
	num = n;
}

Double_Const_Opd & Double_Const_Opd::operator=(const Double_Const_Opd & rhs)
{
	this->num = rhs.num;
	return *this;
}

void Double_Const_Opd::print_rtl_opd(ostream & file_buffer) 
{
	file_buffer<<num;
}

void Double_Const_Opd::print_asm_opd(ostream & file_buffer) 
{
	file_buffer<<num;
}

/****************************** Class RTL_Stmt *****************************/

RTL_Stmt::RTL_Stmt()
{
	comment = "";
}

RTL_Stmt::~RTL_Stmt() {}

void RTL_Stmt::set_comment(string s)
{
	comment = s;
}

string RTL_Stmt::get_comment()
{
	return comment;
}

void RTL_Stmt::print_rtl_comment(ostream & file_buffer)
{
	if (get_comment() != "")
		file_buffer << "\t;; " << get_comment();
}

RTL_Insn_Descriptor & RTL_Stmt::get_op()
{ 
	return rtl_desc; 
}

RTL_Opd * RTL_Stmt::get_opd1()
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method get_Opd1 not implemented");
}

RTL_Opd * RTL_Stmt::get_opd2()
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method get_Opd2 not implemented");
}

RTL_Opd * RTL_Stmt::get_result()
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method get_Result not implemented");
}

void RTL_Stmt::set_opd1(RTL_Opd * rtl_opd)
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method set_Opd1 not implemented");
}

void RTL_Stmt::set_opd2(RTL_Opd * rtl_opd)
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method set_Opd2 not implemented");
}

void RTL_Stmt::set_result(RTL_Opd * rtl_opd)
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual methos set_Result not implemented");
}

/*************************** Class Move_RTL_Stmt *****************************/

Move_RTL_Stmt::Move_RTL_Stmt(RTL_Op op, RTL_Opd * o1, RTL_Opd * res)
{
	CHECK_INVARIANT((machine_desc_object.spim_instruction_table[op] != NULL),
			"Instruction description in spim table cannot be null");

	rtl_desc = *(machine_desc_object.spim_instruction_table[op]);
	opd1 = o1;   
	result = res; 
}

RTL_Opd * Move_RTL_Stmt::get_opd1()          { return opd1; }
RTL_Opd * Move_RTL_Stmt::get_result()        { return result; }

void Move_RTL_Stmt::set_opd1(RTL_Opd * io)   { opd1 = io; }
void Move_RTL_Stmt::set_result(RTL_Opd * io) { result = io; }

Move_RTL_Stmt& Move_RTL_Stmt::operator=(const Move_RTL_Stmt& rhs)
{
	rtl_desc = rhs.rtl_desc;
	opd1 = rhs.opd1;
	result = rhs.result; 

	return *this;
}

void Move_RTL_Stmt::print_rtl_stmt(ostream & file_buffer)
{
	CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a move RTL Stmt");
	CHECK_INVARIANT (result, "Result cannot be NULL for a move RTL Stmt");

	string operation_name = rtl_desc.get_name();

	RTL_Format rtl_format = rtl_desc.get_rtl_format();

	switch (rtl_format)
	{
	case rtl_r_op_o1: 
			file_buffer << "\t" << operation_name << ":    \t";
			result->print_rtl_opd(file_buffer);
			file_buffer << " <- ";
			opd1->print_rtl_opd(file_buffer);
			file_buffer << "    ";
			print_rtl_comment(file_buffer);
			file_buffer << "\n";

			break; 

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, 
				"Intermediate code format not supported");
		break;
	}
}

void Move_RTL_Stmt::print_assembly(ostream & file_buffer)
{
	CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a move RTL Stmt");
	CHECK_INVARIANT (result, "Result cannot be NULL for a move RTL Stmt");
	string op_name = rtl_desc.get_mnemonic();

	ASM_Format assem_format = rtl_desc.get_assembly_format();
	switch (assem_format)
	{
	case asm_op_r_o1: 
			file_buffer << "\t" << op_name << " ";
			result->print_asm_opd(file_buffer);
			file_buffer << ", ";
			opd1->print_asm_opd(file_buffer);
			file_buffer << "\n";

			break; 

	case asm_op_o1_r: 
			file_buffer << "\t" << op_name << " ";
			opd1->print_asm_opd(file_buffer);
			file_buffer << ", ";
			result->print_asm_opd(file_buffer);
			file_buffer << "\n";

			break; 

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Intermediate code format not supported");
		break;
	}
}

/******************************* Class Compute_RTL_Stmt ****************************/

Compute_RTL_Stmt::Compute_RTL_Stmt(RTL_Op op, RTL_Opd * o1, RTL_Opd * o2, RTL_Opd * res)
{
	CHECK_INVARIANT((machine_desc_object.spim_instruction_table[op] != NULL),
			"Instruction description in spim table cannot be null");

	rtl_desc = *(machine_desc_object.spim_instruction_table[op]);
	opd1 = o1;   
	opd2 = o2;   
	result = res; 
}

RTL_Opd * Compute_RTL_Stmt::get_opd1()          { return opd1; }
RTL_Opd * Compute_RTL_Stmt::get_opd2()          { return opd2; }
RTL_Opd * Compute_RTL_Stmt::get_result()        { return result; }

void Compute_RTL_Stmt::set_opd1(RTL_Opd * io)   { opd1 = io; }
void Compute_RTL_Stmt::set_opd2(RTL_Opd * io)   { opd2 = io; }
void Compute_RTL_Stmt::set_result(RTL_Opd * io) { result = io; }

Compute_RTL_Stmt& Compute_RTL_Stmt::operator=(const Compute_RTL_Stmt& rhs)
{
	rtl_desc = rhs.rtl_desc;
	opd1 = rhs.opd1;
	opd2 = rhs.opd2;
	result = rhs.result; 

	return *this;
}

void Compute_RTL_Stmt::print_rtl_stmt(ostream & file_buffer)
{
	CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a compute RTL Stmt");
	// CHECK_INVARIANT (result, "Result cannot be NULL for a compute RTL Stmt");

	string operation_name = rtl_desc.get_name();

	RTL_Format rtl_format = rtl_desc.get_rtl_format();

	switch (rtl_format)
	{
	case rtl_r_o1_op_o2: 
			CHECK_INVARIANT (opd2, "Opd2 cannot be NULL for a compute RTL Stmt");
			file_buffer << "\t" << operation_name << ":    \t";
			result->print_rtl_opd(file_buffer);
			file_buffer << " <- ";
			opd1->print_rtl_opd(file_buffer);
			file_buffer << " , ";
			opd2->print_rtl_opd(file_buffer);
			print_rtl_comment(file_buffer);
			file_buffer << "\n";
			break; 

	case rtl_r_op_o1:
			file_buffer << "\t" << operation_name << ":    \t";
			result->print_rtl_opd(file_buffer);
			file_buffer << " <- ";
			opd1->print_rtl_opd(file_buffer);
			print_rtl_comment(file_buffer);
			file_buffer << "\n";
			break;
	case rtl_o1_op_o2:
	         CHECK_INVARIANT (opd2, "Opd2 cannot be NULL for a compute RTL Stmt");
			file_buffer << "\t" << operation_name << ":    \t c1";
			file_buffer << " <- ";
			opd1->print_rtl_opd(file_buffer);
			file_buffer << " , ";
			opd2->print_rtl_opd(file_buffer);
			print_rtl_comment(file_buffer);
			file_buffer << "\n";
			break; 

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, 
				"Intermediate code format not supported");
		break;
	}
}

void Compute_RTL_Stmt::print_assembly(ostream & file_buffer)
{
	CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a compute RTL Stmt");
	string op_name = rtl_desc.get_mnemonic();

	ASM_Format assem_format = rtl_desc.get_assembly_format();
	switch (assem_format)
	{
	case asm_op_r_o1_o2: 
			CHECK_INVARIANT (opd2, "Opd2 cannot be NULL for a compute RTL Stmt");
			file_buffer << "\t" << op_name << " ";
			result->print_asm_opd(file_buffer);
			file_buffer << ", ";
			opd1->print_asm_opd(file_buffer);
			file_buffer << ", ";
			opd2->print_asm_opd(file_buffer);
			file_buffer << "\n";

			break; 

	case asm_op_o1_o2_r:
			CHECK_INVARIANT (opd2, "Opd2 cannot be NULL for a compute RTL Stmt"); 
			file_buffer << "\t" << op_name << " ";
			opd1->print_asm_opd(file_buffer);
			file_buffer << ", ";
			opd2->print_asm_opd(file_buffer);
			file_buffer << ", ";
			result->print_asm_opd(file_buffer);
			file_buffer << "\n";

			break; 

	case asm_op_r_o1: 
			file_buffer << "\t" << op_name << " ";
			result->print_asm_opd(file_buffer);
			file_buffer << ", ";
			opd1->print_asm_opd(file_buffer);
			file_buffer << "\n";

			break; 

	case asm_op_o1_r: 
			file_buffer << "\t" << op_name << " ";
			opd1->print_asm_opd(file_buffer);
			file_buffer << ", ";
			result->print_asm_opd(file_buffer);
			file_buffer << "\n";

			break; 

    case asm_op_o1_o2: 
			file_buffer << "\t" << op_name << " ";
			opd1->print_asm_opd(file_buffer);
			file_buffer << ", ";
			opd2->print_asm_opd(file_buffer);
			file_buffer << "\n";

			break; 

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Intermediate code format not supported");
		break;
	}
}

/*************************** Class Control_Flow_RTL_Stmt *****************************/

Control_Flow_RTL_Stmt::Control_Flow_RTL_Stmt(RTL_Op op, RTL_Opd * o1, RTL_Opd * o2, string label, int size)
{
	rtl_desc = *(machine_desc_object.spim_instruction_table[op]);
	opd1 = o1;
	opd2 = o2;
	offset = label;
	actual_param_size = size;
}

RTL_Opd * Control_Flow_RTL_Stmt::get_opd1()		{ return opd1; }
RTL_Opd * Control_Flow_RTL_Stmt::get_opd2()		{ return opd2; }
string Control_Flow_RTL_Stmt::get_Offset()		{ return offset; }
    
void Control_Flow_RTL_Stmt::set_opd1(RTL_Opd * io_P)	{ opd1 = io_P; }
void Control_Flow_RTL_Stmt::set_opd2(RTL_Opd * io_P)	{ opd2 = io_P; }
void Control_Flow_RTL_Stmt::set_Offset(string label)	{ offset = label; }
    
Control_Flow_RTL_Stmt& Control_Flow_RTL_Stmt::operator=(const Control_Flow_RTL_Stmt& rhs)
{   
	rtl_desc = rhs.rtl_desc;
	opd1 = rhs.opd1;
	opd2 = rhs.opd2;
	offset = rhs.offset;

	return *this;
}

void Control_Flow_RTL_Stmt::print_rtl_stmt(ostream & file_buffer)
{
	string ops = rtl_desc.get_name();

	RTL_Format icf = rtl_desc.get_rtl_format();

	switch (icf)
	{
	case rtl_op_o1_o2_st:
			if (opd2 != NULL)
			{
				file_buffer << "\t" << ops << ":    \t";
				opd1->print_rtl_opd(file_buffer);
				file_buffer << " , ";
				opd2->print_rtl_opd(file_buffer);
				file_buffer << " : goto ";
				file_buffer << offset ;
				print_rtl_comment(file_buffer);
				file_buffer << "\n";
			}

			else
			{
				file_buffer << "\tgoto ";
				file_buffer << offset;
				print_rtl_comment(file_buffer);
				file_buffer << "\n";
			}

			break;		
	case rtl_op_st:
			if (rtl_desc.get_op() == rtl_call)
			{
				file_buffer << "\tcall ";
				file_buffer << offset;
				print_rtl_comment(file_buffer);
				file_buffer << "\n";
			}

			else
			{
				if (offset.substr(0, 8) == "epilogue")
				{
					file_buffer << "\treturn";
					print_rtl_comment(file_buffer);
					file_buffer << "\n";
				}	
				else
				{
					file_buffer << "\tgoto ";
					file_buffer << offset ;
					print_rtl_comment(file_buffer);
					file_buffer << "\n";
				}
			}

			break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Intermediate code format not supported")
		break;
	}
}

void Control_Flow_RTL_Stmt::print_assembly(ostream & file_buffer)
{
	string ops = rtl_desc.get_mnemonic();

	ASM_Format acf = rtl_desc.get_assembly_format();

	switch (acf)
	{
	case asm_op_o1_o2_st:
			if (opd2 != NULL)
			{
				file_buffer << "\t" << ops << " ";
				opd1->print_asm_opd(file_buffer);
				file_buffer << ", ";
				opd2->print_asm_opd(file_buffer);
				file_buffer << ", ";
				file_buffer << offset << " ";
				file_buffer << "\n";
			}

			else
			{
				CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a move RTL Stmt"); 
				file_buffer << "\t" << ops << " ";
				opd1->print_asm_opd(file_buffer);
				file_buffer << ", $zero, ";
				file_buffer << offset;
				file_buffer << " \n";
			}

			break;

	case asm_op_st:
			if (rtl_desc.get_op() == rtl_call)
			{
				if (actual_param_size > 0)
					file_buffer << "\tsub $sp, $sp, " << actual_param_size << "\n";
				file_buffer << "\t" << ops << " ";
				file_buffer << offset << "\n";

				if (actual_param_size > 0)
					file_buffer << "\tadd $sp, $sp, " << actual_param_size << "\n";
			}
			else
			{
	        		file_buffer << "\t" << ops << " ";
				file_buffer << offset << "\n";
			}

			break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Assembly code format not supported");
		break;
	}
}
/*************************** Class Label_RTL_Stmt *****************************/

Label_RTL_Stmt::Label_RTL_Stmt(RTL_Op op, string label)
{
	CHECK_INVARIANT((machine_desc_object.spim_instruction_table[op] != NULL),
			"Instruction description in spim table cannot be null");

	rtl_desc = *(machine_desc_object.spim_instruction_table[op]);
   
	this->label = label; 
}

string Label_RTL_Stmt::get_label()        { return label; }

void Label_RTL_Stmt::set_label(string label) { this->label = label; }

Label_RTL_Stmt& Label_RTL_Stmt::operator=(const Label_RTL_Stmt& rhs)
{
	rtl_desc = rhs.rtl_desc;
	label = rhs.label; 
	return *this;
}

void Label_RTL_Stmt::print_rtl_stmt(ostream & file_buffer)
{
	CHECK_INVARIANT (!label.empty(), "Label cannot be empty for a label RTL Stmt");

	string operation_name = rtl_desc.get_name();
	if(operation_name == "jump")	// jump operation must be replaced by goto
		operation_name = "goto";
	RTL_Format rtl_format = rtl_desc.get_rtl_format();

	switch (rtl_format)
	{
	case rtl_op_st:	// jump and label
			if(operation_name.empty())
				file_buffer << "\n" << label << ":    \t" << "\n";
			else
				file_buffer << "\t" << operation_name << " " << label << "\n";
			break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, 
				"Intermediate code format not supported");
		break;
	}
}

void Label_RTL_Stmt::print_assembly(ostream & file_buffer)
{
	CHECK_INVARIANT (!label.empty(), "Label cannot be empty for a move RTL Stmt");
	string op_name = rtl_desc.get_mnemonic();

	ASM_Format assem_format = rtl_desc.get_assembly_format();
	switch (assem_format)
	{
	case asm_op_st:	// label
			if(op_name.empty())
				file_buffer << "\n" << label << ":    \t" << "\n";
			else	// jump
				file_buffer << "\t" << op_name << " " << label << "\n";
			break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Assembly code format not supported");
		break;
	}
}

/******************************* Class RTL_For_TAC ****************************/

RTL_For_TAC::RTL_For_TAC()
{
	
	rtl_stmt_list = *(new list<RTL_Stmt *>());
}

RTL_For_TAC::RTL_For_TAC(list<RTL_Stmt *> & rtl_l, Register_Descriptor * reg)
{
	
	rtl_stmt_list = rtl_l;
	result_register = reg;
}

void RTL_For_TAC::append_rtl_stmt(RTL_Stmt & rtl_stmt)
{
	
	rtl_stmt_list.push_back(&rtl_stmt);
}

list<RTL_Stmt *> & RTL_For_TAC::get_rtl_list()  
{ 
	return rtl_stmt_list;
}
void RTL_For_TAC::set_rtl_list(list<RTL_Stmt *> * rtl_list_new)
{
	rtl_stmt_list = *rtl_list_new;
}

Register_Descriptor * RTL_For_TAC::get_reg()
{
	
	return result_register;
}

void RTL_For_TAC::set_reg(Register_Descriptor * reg)
{
	
	result_register = reg;
}

RTL_For_TAC& RTL_For_TAC::operator=(const RTL_For_TAC& rhs)
{
	this->rtl_stmt_list = rhs.rtl_stmt_list;
	this->result_register = rhs.result_register;
	return *this;
}

void RTL_For_TAC::print_rtl_stmt_list(ostream & file_buffer)
{
	list<RTL_Stmt *> rtl_stmt_list;
	list<RTL_Stmt *>::iterator i;
	for (i = rtl_stmt_list.begin(); i != rtl_stmt_list.end(); i++)
	{
		//cout << "Inside the loop in the RTL Statement List printing function" << endl; // TEST
		RTL_Stmt * rtl_stmt = *i;
		CHECK_INVARIANT(*i, "RTL statement pointer cannot be Null");
		rtl_stmt->print_rtl_stmt(file_buffer);
	}
}


/************************ class RTL_Insn_Descriptor ********************************/

RTL_Op RTL_Insn_Descriptor::get_op()                   	{ return inst_op; }
string RTL_Insn_Descriptor::get_name()                       { return name; }
string RTL_Insn_Descriptor::get_mnemonic()                   { return mnemonic; }
string RTL_Insn_Descriptor::get_rtl_symbol()                  { return rtl_symbol; }
RTL_Format RTL_Insn_Descriptor::get_rtl_format()            { return rtl_format; }
ASM_Format RTL_Insn_Descriptor::get_assembly_format()   { return assem_format; }

RTL_Insn_Descriptor::RTL_Insn_Descriptor (RTL_Op op, string temp_name, string temp_mnemonic, 
						string rtl_sym, RTL_Format rtl_form, ASM_Format as_form)
{
	inst_op = op;
	name = temp_name; 
	mnemonic = temp_mnemonic;
	rtl_symbol = rtl_sym;
	rtl_format = rtl_form;
	assem_format = as_form;
}

RTL_Insn_Descriptor::RTL_Insn_Descriptor()
{
	inst_op = rtl_nop;
	name = "";
	mnemonic = "";
	rtl_symbol = "";
	rtl_format = rtl_nsy;
	assem_format = asm_nsy;
}

Read_RTL_Stmt::Read_RTL_Stmt() 
{
	rtl_desc = *(machine_desc_object.spim_instruction_table[rtl_read]);
}

void Read_RTL_Stmt::print_rtl_stmt(ostream & file_buffer) 
{
	string op_name = rtl_desc.get_mnemonic();
	file_buffer << "\t" << "read";
	file_buffer << "\t\t\t";
	print_rtl_comment(file_buffer);
	file_buffer << "\n";
}

void Read_RTL_Stmt::print_assembly(ostream & file_buffer)
{
	string op_name = rtl_desc.get_mnemonic();
	file_buffer << "\t" << op_name  << "\n";
 }

Write_RTL_Stmt::Write_RTL_Stmt() 
{
	rtl_desc = *(machine_desc_object.spim_instruction_table[rtl_print]);
}

Write_RTL_Stmt::~Write_RTL_Stmt(){}

void Write_RTL_Stmt::print_rtl_stmt(ostream & file_buffer) 
{
	file_buffer << "\t" << "write";
	file_buffer << "\t\t\t";
	print_rtl_comment(file_buffer);
	file_buffer << "\n";
}

void Write_RTL_Stmt::print_assembly(ostream & file_buffer)
{
	string op_name = rtl_desc.get_mnemonic();
  	file_buffer << "\t" << op_name  << "\n";
 }
