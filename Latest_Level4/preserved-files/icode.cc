
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

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>

using namespace std;

#include "common-classes.hh"
#include "error-display.hh"
#include "icode.hh"
#include "reg-alloc.hh"
#include "symbol-table.hh"
#include "ast.hh"
#include "program.hh"

/****************************** Class Ics_Opd *****************************/

Register_Descriptor * Ics_Opd::get_reg()
{
	//TODO
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method get_reg not implemented");
}

/****************************** Class Mem_Addr_Opd *****************************/

Mem_Addr_Opd::Mem_Addr_Opd(Symbol_Table_Entry & se) 
{
	//TODO
	symbol_entry = &se;
}

Mem_Addr_Opd & Mem_Addr_Opd::operator=(const Mem_Addr_Opd & rhs)
{
	//TODO
	this->symbol_entry = rhs.symbol_entry;
	return *this;
}

void Mem_Addr_Opd::print_ics_opd(ostream & file_buffer) 
{
	//TODO
	file_buffer << symbol_entry->get_variable_name();
}

void Mem_Addr_Opd::print_asm_opd(ostream & file_buffer) 
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

/****************************** Class Register_Addr_Opd *****************************/

Register_Addr_Opd::Register_Addr_Opd(Register_Descriptor * reg) 
{
	//TODO
	register_description = reg;
}

Register_Descriptor * Register_Addr_Opd::get_reg()    
{ 
	//TODO 
	return register_description;
}

Register_Addr_Opd& Register_Addr_Opd::operator=(const Register_Addr_Opd& rhs)
{
	//TODO
	this->register_description = rhs.register_description;
	return *this;
}

void Register_Addr_Opd::print_ics_opd(ostream & file_buffer) 
{
	//TODO
	file_buffer<<register_description->get_name();
}

void Register_Addr_Opd::print_asm_opd(ostream & file_buffer) 
{
	//TODO
	file_buffer<<"$"<<register_description->get_name();
}

/****************************** Class Const_IC_Opd *****************************/

template <class DATA_TYPE>
Const_IC_Opd<DATA_TYPE>::Const_IC_Opd(DATA_TYPE n) 
{
	//TODO
	num = n;
}

template <class DATA_TYPE>
Const_IC_Opd<DATA_TYPE> & Const_IC_Opd<DATA_TYPE>::operator=(const Const_IC_Opd<DATA_TYPE> & rhs)
{
	//TODO
	this->num = rhs.num;
	return *this;
}

template <class DATA_TYPE>
void Const_IC_Opd<DATA_TYPE>::print_ics_opd(ostream & file_buffer) 
{
	//TODO
	file_buffer<<num;
}

template <class DATA_TYPE>
void Const_IC_Opd<DATA_TYPE>::print_asm_opd(ostream & file_buffer) 
{
	//TODO
	file_buffer<<num;
}

/****************************** Class Icode_Stmt *****************************/

Icode_Stmt::Icode_Stmt()
{
	comment = "";
}

Icode_Stmt::~Icode_Stmt() {}

void Icode_Stmt::set_comment(string s)
{
	comment = s;
}

string Icode_Stmt::get_comment()
{
	return comment;
}

void Icode_Stmt::print_icode_comment(ostream & file_buffer)
{
	if (get_comment() != "")
		file_buffer << "\t;; " << get_comment();
}

Instruction_Descriptor & Icode_Stmt::get_op()
{ 
	return op_desc; 
}

Ics_Opd * Icode_Stmt::get_opd1()
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method get_Opd1 not implemented");
}

Ics_Opd * Icode_Stmt::get_opd2()
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method get_Opd2 not implemented");
}

Ics_Opd * Icode_Stmt::get_result()
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method get_Result not implemented");
}

void Icode_Stmt::set_opd1(Ics_Opd * ics_opd)
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method set_Opd1 not implemented");
}

void Icode_Stmt::set_opd2(Ics_Opd * ics_opd)
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method set_Opd2 not implemented");
}

void Icode_Stmt::set_result(Ics_Opd * ics_opd)
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual methos set_Result not implemented");
}

/*************************** Class Move_IC_Stmt *****************************/

Move_IC_Stmt::Move_IC_Stmt(Tgt_Op op, Ics_Opd * o1, Ics_Opd * res)
{
	CHECK_INVARIANT((machine_desc_object.spim_instruction_table[op] != NULL),
			"Instruction description in spim table cannot be null");

	op_desc = *(machine_desc_object.spim_instruction_table[op]);
	opd1 = o1;   
	result = res; 
}

Ics_Opd * Move_IC_Stmt::get_opd1()          { return opd1; }
Ics_Opd * Move_IC_Stmt::get_result()        { return result; }

void Move_IC_Stmt::set_opd1(Ics_Opd * io)   { opd1 = io; }
void Move_IC_Stmt::set_result(Ics_Opd * io) { result = io; }

Move_IC_Stmt& Move_IC_Stmt::operator=(const Move_IC_Stmt& rhs)
{
	op_desc = rhs.op_desc;
	opd1 = rhs.opd1;
	result = rhs.result; 

	return *this;
}

void Move_IC_Stmt::print_icode(ostream & file_buffer)
{
	CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a move IC Stmt");
	CHECK_INVARIANT (result, "Result cannot be NULL for a move IC Stmt");

	string operation_name = op_desc.get_name();

	Icode_Format ic_format = op_desc.get_ic_format();

	switch (ic_format)
	{
	case i_r_op_o1: 
			file_buffer << "\t" << operation_name << ":    \t";
			result->print_ics_opd(file_buffer);
			file_buffer << " <- ";
			opd1->print_ics_opd(file_buffer);
			print_icode_comment(file_buffer);
			file_buffer << "\n";

			break; 

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, 
				"Intermediate code format not supported");
		break;
	}
}

void Move_IC_Stmt::print_assembly(ostream & file_buffer)
{
	CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a move IC Stmt");
	CHECK_INVARIANT (result, "Result cannot be NULL for a move IC Stmt");
	string op_name = op_desc.get_mnemonic();

	Assembly_Format assem_format = op_desc.get_assembly_format();
	switch (assem_format)
	{
	case a_op_r_o1: 
			file_buffer << "\t" << op_name << " ";
			result->print_asm_opd(file_buffer);
			file_buffer << ", ";
			opd1->print_asm_opd(file_buffer);
			file_buffer << "\n";

			break; 

	case a_op_o1_r: 
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

/******************************* Class Compute_IC_Stmt ****************************/

Compute_IC_Stmt::Compute_IC_Stmt(Tgt_Op op, Ics_Opd * o1, Ics_Opd * o2, Ics_Opd * res)
{
	CHECK_INVARIANT((machine_desc_object.spim_instruction_table[op] != NULL),
			"Instruction description in spim table cannot be null");

	op_desc = *(machine_desc_object.spim_instruction_table[op]);
	opd1 = o1;   
	opd2 = o2;   
	result = res; 
}

Ics_Opd * Compute_IC_Stmt::get_opd1()          { return opd1; }
Ics_Opd * Compute_IC_Stmt::get_opd2()          { return opd2; }
Ics_Opd * Compute_IC_Stmt::get_result()        { return result; }

void Compute_IC_Stmt::set_opd1(Ics_Opd * io)   { opd1 = io; }
void Compute_IC_Stmt::set_opd2(Ics_Opd * io)   { opd2 = io; }
void Compute_IC_Stmt::set_result(Ics_Opd * io) { result = io; }

Compute_IC_Stmt& Compute_IC_Stmt::operator=(const Compute_IC_Stmt& rhs)
{
	op_desc = rhs.op_desc;
	opd1 = rhs.opd1;
	opd2 = rhs.opd2;
	result = rhs.result; 

	return *this;
}

void Compute_IC_Stmt::print_icode(ostream & file_buffer)
{
	CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a compute IC Stmt");
	// CHECK_INVARIANT (result, "Result cannot be NULL for a compute IC Stmt");

	string operation_name = op_desc.get_name();

	Icode_Format ic_format = op_desc.get_ic_format();

	switch (ic_format)
	{
	case i_r_o1_op_o2: 
			CHECK_INVARIANT (opd2, "Opd2 cannot be NULL for a compute IC Stmt");
			file_buffer << "\t" << operation_name << ":    \t";
			result->print_ics_opd(file_buffer);
			file_buffer << " <- ";
			opd1->print_ics_opd(file_buffer);
			file_buffer << " , ";
			opd2->print_ics_opd(file_buffer);
			print_icode_comment(file_buffer);
			file_buffer << "\n";
			break; 

	case i_r_op_o1:
			file_buffer << "\t" << operation_name << ":    \t";
			result->print_ics_opd(file_buffer);
			file_buffer << " <- ";
			opd1->print_ics_opd(file_buffer);
			print_icode_comment(file_buffer);
			file_buffer << "\n";
			break;
	case i_o1_op_o2:
	         CHECK_INVARIANT (opd2, "Opd2 cannot be NULL for a compute IC Stmt");
			file_buffer << "\t" << operation_name << ":    \t c1";
			file_buffer << " <- ";
			opd1->print_ics_opd(file_buffer);
			file_buffer << " , ";
			opd2->print_ics_opd(file_buffer);
			print_icode_comment(file_buffer);
			file_buffer << "\n";
			break; 

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, 
				"Intermediate code format not supported");
		break;
	}
}

void Compute_IC_Stmt::print_assembly(ostream & file_buffer)
{
	CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a compute IC Stmt");
	//CHECK_INVARIANT (result, "Result cannot be NULL for a compute IC Stmt");
	string op_name = op_desc.get_mnemonic();

	Assembly_Format assem_format = op_desc.get_assembly_format();
	switch (assem_format)
	{
	case a_op_r_o1_o2: 
			CHECK_INVARIANT (opd2, "Opd2 cannot be NULL for a compute IC Stmt");
			file_buffer << "\t" << op_name << " ";
			result->print_asm_opd(file_buffer);
			file_buffer << ", ";
			opd1->print_asm_opd(file_buffer);
			file_buffer << ", ";
			opd2->print_asm_opd(file_buffer);
			file_buffer << "\n";

			break; 

	case a_op_o1_o2_r:
			CHECK_INVARIANT (opd2, "Opd2 cannot be NULL for a compute IC Stmt"); 
			file_buffer << "\t" << op_name << " ";
			opd1->print_asm_opd(file_buffer);
			file_buffer << ", ";
			opd2->print_asm_opd(file_buffer);
			file_buffer << ", ";
			result->print_asm_opd(file_buffer);
			file_buffer << "\n";

			break; 

	case a_op_r_o1: 
			file_buffer << "\t" << op_name << " ";
			result->print_asm_opd(file_buffer);
			file_buffer << ", ";
			opd1->print_asm_opd(file_buffer);
			file_buffer << "\n";

			break; 

	case a_op_o1_r: 
			file_buffer << "\t" << op_name << " ";
			opd1->print_asm_opd(file_buffer);
			file_buffer << ", ";
			result->print_asm_opd(file_buffer);
			file_buffer << "\n";

			break; 

    case a_op_o1_o2: 
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

/*************************** Class Control_Flow_IC_Stmt *****************************/

Control_Flow_IC_Stmt::Control_Flow_IC_Stmt(Tgt_Op op, Ics_Opd * o1, Ics_Opd * o2, string label, int size)
{
	op_desc = *(machine_desc_object.spim_instruction_table[op]);
	opd1 = o1;
	opd2 = o2;
	offset = label;
	actual_param_size = size;
}

Ics_Opd * Control_Flow_IC_Stmt::get_opd1()		{ return opd1; }
Ics_Opd * Control_Flow_IC_Stmt::get_opd2()		{ return opd2; }
string Control_Flow_IC_Stmt::get_Offset()		{ return offset; }
    
void Control_Flow_IC_Stmt::set_opd1(Ics_Opd * io_P)	{ opd1 = io_P; }
void Control_Flow_IC_Stmt::set_opd2(Ics_Opd * io_P)	{ opd2 = io_P; }
void Control_Flow_IC_Stmt::set_Offset(string label)	{ offset = label; }
    
Control_Flow_IC_Stmt& Control_Flow_IC_Stmt::operator=(const Control_Flow_IC_Stmt& rhs)
{   
	op_desc = rhs.op_desc;
	opd1 = rhs.opd1;
	opd2 = rhs.opd2;
	offset = rhs.offset;

	return *this;
}

void Control_Flow_IC_Stmt::print_icode(ostream & file_buffer)
{
	string ops = op_desc.get_name();

	Icode_Format icf = op_desc.get_ic_format();

	switch (icf)
	{
	case i_op_o1_o2_st:
			if (opd2 != NULL)
			{
				file_buffer << "\t" << ops << ":    \t";
				opd1->print_ics_opd(file_buffer);
				file_buffer << " , ";
				opd2->print_ics_opd(file_buffer);
				file_buffer << " : goto ";
				file_buffer << offset ;
				print_icode_comment(file_buffer);
				file_buffer << "\n";
			}

			else
			{
				file_buffer << "\tgoto ";
				file_buffer << offset;
				print_icode_comment(file_buffer);
				file_buffer << "\n";
			}

			break;		
	case i_op_st:
			if (op_desc.get_op() == jal)
			{
				file_buffer << "\tcall ";
				file_buffer << offset;
				print_icode_comment(file_buffer);
				file_buffer << "\n";
			}

			else
			{
				if (offset.substr(0, 8) == "epilogue")
				{
					file_buffer << "\treturn";
					print_icode_comment(file_buffer);
					file_buffer << "\n";
				}	
				else
				{
					file_buffer << "\tgoto ";
					file_buffer << offset ;
					print_icode_comment(file_buffer);
					file_buffer << "\n";
				}
			}

			break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Intermediate code format not supported")
		break;
	}
}

void Control_Flow_IC_Stmt::print_assembly(ostream & file_buffer)
{
	string ops = op_desc.get_mnemonic();

	Assembly_Format acf = op_desc.get_assembly_format();

	switch (acf)
	{
	case a_op_o1_o2_st:
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
				CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a move IC Stmt"); 
				file_buffer << "\t" << ops << " ";
				opd1->print_asm_opd(file_buffer);
				file_buffer << ", $zero, ";
				file_buffer << offset;
				file_buffer << " \n";
			}

			break;

	case a_op_st:
			if (op_desc.get_op() == jal)
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
/*************************** Class Label_IC_Stmt *****************************/

Label_IC_Stmt::Label_IC_Stmt(Tgt_Op op, string label)
{
	CHECK_INVARIANT((machine_desc_object.spim_instruction_table[op] != NULL),
			"Instruction description in spim table cannot be null");

	op_desc = *(machine_desc_object.spim_instruction_table[op]);
   
	this->label = label; 
}

string Label_IC_Stmt::get_label()        { return label; }

void Label_IC_Stmt::set_label(string label) { this->label = label; }

Label_IC_Stmt& Label_IC_Stmt::operator=(const Label_IC_Stmt& rhs)
{
	op_desc = rhs.op_desc;
	label = rhs.label; 
	return *this;
}

void Label_IC_Stmt::print_icode(ostream & file_buffer)
{
	CHECK_INVARIANT (!label.empty(), "Label cannot be empty for a label IC Stmt");

	string operation_name = op_desc.get_name();
	if(operation_name == "jump")	// jump operation must be replaced by goto
		operation_name = "goto";
	Icode_Format ic_format = op_desc.get_ic_format();

	switch (ic_format)
	{
	case i_op_st:	// jump and label
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

void Label_IC_Stmt::print_assembly(ostream & file_buffer)
{
	CHECK_INVARIANT (!label.empty(), "Label cannot be empty for a move IC Stmt");
	string op_name = op_desc.get_mnemonic();

	Assembly_Format assem_format = op_desc.get_assembly_format();
	switch (assem_format)
	{
	case a_op_st:	// label
			if(op_name.empty())
				file_buffer << "\n" << label << ":    \t" << "\n";
			else	// jump
				file_buffer << "\t" << op_name << " " << label << "\n";
			break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Assembly code format not supported");
		break;
	}
}

/******************************* Class Code_For_Ast ****************************/

Code_For_Ast::Code_For_Ast()
{
	
	ics_list = *(new list<Icode_Stmt *>());
}

Code_For_Ast::Code_For_Ast(list<Icode_Stmt *> & ic_l, Register_Descriptor * reg)
{
	
	ics_list = ic_l;
	result_register = reg;
}

void Code_For_Ast::append_ics(Icode_Stmt & ic_stmt)
{
	
	ics_list.push_back(&ic_stmt);
}

list<Icode_Stmt *> & Code_For_Ast::get_icode_list()  
{ 
	
	return ics_list;
}
void Code_For_Ast::set_icode_list(list<Icode_Stmt *> * ic_list_new)
{
	ics_list = *ic_list_new;
}

Register_Descriptor * Code_For_Ast::get_reg()
{
	
	return result_register;
}

void Code_For_Ast::set_reg(Register_Descriptor * reg)
{
	
	result_register = reg;
}

Code_For_Ast& Code_For_Ast::operator=(const Code_For_Ast& rhs)
{
	
	this->ics_list = rhs.ics_list;
	this->result_register = rhs.result_register;
	return *this;
}

/************************ class Instruction_Descriptor ********************************/

Tgt_Op Instruction_Descriptor::get_op()                   	{ return inst_op; }
string Instruction_Descriptor::get_name()                       { return name; }
string Instruction_Descriptor::get_mnemonic()                   { return mnemonic; }
string Instruction_Descriptor::get_ic_symbol()                  { return ic_symbol; }
Icode_Format Instruction_Descriptor::get_ic_format()            { return ic_format; }
Assembly_Format Instruction_Descriptor::get_assembly_format()   { return assem_format; }

Instruction_Descriptor::Instruction_Descriptor (Tgt_Op op, string temp_name, string temp_mnemonic, 
						string ic_sym, Icode_Format ic_form, Assembly_Format as_form)
{
	inst_op = op;
	name = temp_name; 
	mnemonic = temp_mnemonic;
	ic_symbol = ic_sym;
	ic_format = ic_form;
	assem_format = as_form;
}

Instruction_Descriptor::Instruction_Descriptor()
{
	inst_op = nop;
	name = "";
	mnemonic = "";
	ic_symbol = "";
	ic_format = i_nsy;
	assem_format = a_nsy;
}

template class Const_IC_Opd<int>;
template class Const_IC_Opd<double>;

Read_IC_Stmt::Read_IC_Stmt() 
{
	op_desc = *(machine_desc_object.spim_instruction_table[read]);
}

void Read_IC_Stmt::print_icode(ostream & file_buffer) 
{
	string op_name = op_desc.get_mnemonic();
	file_buffer << "\t" << "read" << "\t" << op_name;
	print_icode_comment(file_buffer);
	file_buffer << "\n";
}

void Read_IC_Stmt::print_assembly(ostream & file_buffer)
{
	string op_name = op_desc.get_mnemonic();
	file_buffer << "\t" << op_name  << "\n";
 }

Write_IC_Stmt::Write_IC_Stmt() 
{
	op_desc = *(machine_desc_object.spim_instruction_table[print]);
}

Write_IC_Stmt::~Write_IC_Stmt(){}

void Write_IC_Stmt::print_icode(ostream & file_buffer) 
{
	file_buffer << "\t" << "print";
	print_icode_comment(file_buffer);
	file_buffer << "\n";
}

void Write_IC_Stmt::print_assembly(ostream & file_buffer){


string op_name = op_desc.get_mnemonic();

  file_buffer << "\t" << op_name  << "\n";

 }
