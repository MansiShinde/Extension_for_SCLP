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
#include "rtl.hh"
#include "tac.hh"
#include "reg-alloc.hh"
#include "symbol-table.hh"
#include "ast.hh"
#include "program.hh"

Machine_Description machine_desc_object;

//////////////////////////// Register Descriptor ///////////////////////////////

Register_Descriptor::Register_Descriptor(Spim_Register reg, string s, Register_Val_Type vt, Register_Use_Category uc)
{
	reg_id = reg;
	reg_name = s;
	value_type = vt;
	reg_use = uc; 
	used_for_expr_result = false;
	reg_occupied = false;
	used_for_fn_return = false;
}

Register_Use_Category Register_Descriptor::get_use_category()	{ return reg_use; }
Spim_Register Register_Descriptor::get_register()		{ return reg_id; }
string Register_Descriptor::get_name()				{ return reg_name; }

bool Register_Descriptor::is_register_occupied()		{ return reg_occupied; }
void Register_Descriptor::set_register_occupied()		{ reg_occupied = true; }
void Register_Descriptor::reset_register_occupied()		{ reg_occupied = false; }

bool Register_Descriptor::is_used_for_expr_result()		{ return used_for_expr_result; }
void Register_Descriptor::reset_use_for_expr_result()		{ used_for_expr_result = false; 
								  reg_occupied = false; 
								  used_for_fn_return = false; }
void Register_Descriptor::set_use_for_expr_result()		{ used_for_expr_result = true; }

void Register_Descriptor::set_used_for_fn_return()		{ used_for_fn_return = true; }
void Register_Descriptor::reset_used_for_fn_return()		{ used_for_fn_return = false; }
bool Register_Descriptor::is_used_for_fn_return()		{ return used_for_fn_return; }

Register_Val_Type Register_Descriptor::get_register_type()      { return value_type; }


template <Register_Use_Category dt>
bool Register_Descriptor::is_free()     
{
	if ((reg_use == dt) && 
	(is_used_for_expr_result() == false) && 
	(is_register_occupied() == false)) 
		return true;
	else 
		return false;
}

/******************************* Machine Description *****************************************/

void Machine_Description::initialize_register_table()
{
	spim_register_table[zero] = new Register_Descriptor(zero, "zero", int_num, fixed_reg);
	spim_register_table[v0] = new Register_Descriptor(v0, "v0", int_num, int_reg);
	spim_register_table[v1] = new Register_Descriptor(v1, "v1", int_num, fn_result);
	spim_register_table[a0] = new Register_Descriptor(a0, "a0", int_num, argument);
	spim_register_table[a1] = new Register_Descriptor(a1, "a1", int_num, argument);
	spim_register_table[a2] = new Register_Descriptor(a2, "a2", int_num, argument);
	spim_register_table[a3] = new Register_Descriptor(a3, "a3", int_num, argument);
	spim_register_table[t0] = new Register_Descriptor(t0, "t0", int_num, int_reg);
	spim_register_table[t1] = new Register_Descriptor(t1, "t1", int_num, int_reg);
	spim_register_table[t2] = new Register_Descriptor(t2, "t2", int_num, int_reg);
	spim_register_table[t3] = new Register_Descriptor(t3, "t3", int_num, int_reg);
	spim_register_table[t4] = new Register_Descriptor(t4, "t4", int_num, int_reg);
	spim_register_table[t5] = new Register_Descriptor(t5, "t5", int_num, int_reg);
	spim_register_table[t6] = new Register_Descriptor(t6, "t6", int_num, int_reg);
	spim_register_table[t7] = new Register_Descriptor(t7, "t7", int_num, int_reg);
	spim_register_table[t8] = new Register_Descriptor(t8, "t8", int_num, int_reg);
	spim_register_table[t9] = new Register_Descriptor(t9, "t9", int_num, int_reg);
	spim_register_table[s0] = new Register_Descriptor(s0, "s0", int_num, int_reg);
	spim_register_table[s1] = new Register_Descriptor(s1, "s1", int_num, int_reg);
	spim_register_table[s2] = new Register_Descriptor(s2, "s2", int_num, int_reg);
	spim_register_table[s3] = new Register_Descriptor(s3, "s3", int_num, int_reg);
	spim_register_table[s4] = new Register_Descriptor(s4, "s4", int_num, int_reg);
	spim_register_table[s5] = new Register_Descriptor(s5, "s5", int_num, int_reg);
	spim_register_table[s6] = new Register_Descriptor(s6, "s6", int_num, int_reg);
	spim_register_table[s7] = new Register_Descriptor(s7, "s7", int_num, int_reg);
	spim_register_table[f0] = new Register_Descriptor(f0, "f0", float_num, fn_result);
	spim_register_table[f2] = new Register_Descriptor(f2, "f2", float_num, float_reg);
	spim_register_table[f4] = new Register_Descriptor(f4, "f4", float_num, float_reg);
	spim_register_table[f6] = new Register_Descriptor(f6, "f6", float_num, float_reg);
	spim_register_table[f8] = new Register_Descriptor(f8, "f8", float_num, float_reg);
	spim_register_table[f10] = new Register_Descriptor(f10, "f10", float_num, float_reg);
	spim_register_table[f12] = new Register_Descriptor(f12, "f12", float_num, float_reg);
	spim_register_table[f14] = new Register_Descriptor(f14, "f14", float_num, float_reg);
	spim_register_table[f16] = new Register_Descriptor(f16, "f16", float_num, float_reg);
	spim_register_table[f18] = new Register_Descriptor(f18, "f18", float_num, float_reg);
	spim_register_table[f20] = new Register_Descriptor(f20, "f20", float_num, float_reg);
	spim_register_table[f22] = new Register_Descriptor(f22, "f22", float_num, float_reg);
	spim_register_table[f24] = new Register_Descriptor(f24, "f24", float_num, float_reg);
	spim_register_table[f26] = new Register_Descriptor(f26, "f26", float_num, float_reg);
	spim_register_table[f28] = new Register_Descriptor(f28, "f28", float_num, float_reg);
	spim_register_table[f30] = new Register_Descriptor(f30, "f30", float_num, float_reg);
	spim_register_table[gp] = new Register_Descriptor(gp, "gp", int_num, pointer);
	spim_register_table[sp] = new Register_Descriptor(sp, "sp", int_num, pointer);
	spim_register_table[fp] = new Register_Descriptor(fp, "fp", int_num, pointer);
	spim_register_table[ra] = new Register_Descriptor(ra, "ra", int_num, ret_address);

	no_reg = new Register_Descriptor (none, "none", void_reg_type, fixed_reg);
}

Register_Descriptor * Machine_Description::get_no_reg()
{
	return no_reg;
}

void Machine_Description::initialize_instruction_table()
{
	// load 
	spim_instruction_table[rtl_imm_load] = new RTL_Insn_Descriptor(rtl_imm_load, "iLoad", "li", "", rtl_r_op_o1, asm_op_r_o1);
	spim_instruction_table[rtl_imm_load_d] = new RTL_Insn_Descriptor(rtl_imm_load_d, "iLoad.d", "li.d", "", rtl_r_op_o1, asm_op_r_o1);
	spim_instruction_table[rtl_load] = new RTL_Insn_Descriptor(rtl_load, "load", "lw", "", rtl_r_op_o1, asm_op_r_o1);
	spim_instruction_table[rtl_load_d] = new RTL_Insn_Descriptor(rtl_load_d, "load.d", "l.d", "", rtl_r_op_o1, asm_op_r_o1);
	spim_instruction_table[rtl_load_addr] = new RTL_Insn_Descriptor(rtl_load_addr, "load_addr", "la", "", rtl_r_op_o1, asm_op_r_o1);
	// store
	spim_instruction_table[rtl_store] = new RTL_Insn_Descriptor(rtl_store, "store", "sw", "", rtl_r_op_o1, asm_op_o1_r);
	spim_instruction_table[rtl_store_d] = new RTL_Insn_Descriptor(rtl_store_d, "store.d", "s.d", "", rtl_r_op_o1, asm_op_o1_r);
	// logical
	spim_instruction_table[rtl_and] = new RTL_Insn_Descriptor(rtl_and, "and", "and", "", rtl_r_o1_op_o2, asm_op_r_o1_o2);
	spim_instruction_table[rtl_or] = new RTL_Insn_Descriptor(rtl_or, "or", "or", "", rtl_r_o1_op_o2, asm_op_r_o1_o2);
	spim_instruction_table[rtl_not] = new RTL_Insn_Descriptor(rtl_not, "not", "sltu", "", rtl_r_op_o1, asm_op_r_o1);
	// move
	spim_instruction_table[rtl_move] = new RTL_Insn_Descriptor(rtl_move, "move", "move", "", rtl_r_op_o1, asm_op_r_o1);
	spim_instruction_table[rtl_move_d] = new RTL_Insn_Descriptor(rtl_move_d, "move.d", "mov.d", "", rtl_r_op_o1, asm_op_r_o1);
	// control flow
	spim_instruction_table[rtl_beq] = new RTL_Insn_Descriptor(rtl_beq, "beq", "beq", "", rtl_op_o1_o2_st, asm_op_o1_o2_st);
	spim_instruction_table[rtl_bne] = new RTL_Insn_Descriptor(rtl_bne, "bne", "bne", "", rtl_op_o1_o2_st, asm_op_o1_o2_st);
	spim_instruction_table[rtl_bgtz] = new RTL_Insn_Descriptor(rtl_bgtz, "bgtz", "bgtz", "", rtl_op_o1_o2_st, asm_op_o1_o2_st);
	spim_instruction_table[rtl_bgez] = new RTL_Insn_Descriptor(rtl_bgez, "bgez", "bgez", "", rtl_op_o1_o2_st, asm_op_o1_o2_st);
	spim_instruction_table[rtl_bltz] = new RTL_Insn_Descriptor(rtl_bltz, "bltz", "bltz", "", rtl_op_o1_o2_st, asm_op_o1_o2_st);
	spim_instruction_table[rtl_blez] = new RTL_Insn_Descriptor(rtl_blez, "blez", "blez", "", rtl_op_o1_o2_st, asm_op_o1_o2_st);
	spim_instruction_table[rtl_bc1t] = new RTL_Insn_Descriptor(rtl_bc1t, "bc1t", "bc1t", "", rtl_op_st, asm_op_st);
	spim_instruction_table[rtl_bc1f] = new RTL_Insn_Descriptor(rtl_bc1f, "bc1f", "bc1f", "", rtl_op_st, asm_op_st);
	spim_instruction_table[rtl_jump] = new RTL_Insn_Descriptor(rtl_jump, "jump", "j", "", rtl_op_st, asm_op_st);
	spim_instruction_table[rtl_call] = new RTL_Insn_Descriptor(rtl_call, "call", "jal", "", rtl_op_st, asm_op_st);
	spim_instruction_table[rtl_return] = new RTL_Insn_Descriptor(rtl_return, "return", "", "", rtl_op, asm_op);
	spim_instruction_table[rtl_label] = new RTL_Insn_Descriptor(rtl_label, "", "", "", rtl_op_st, asm_op_st);
	// compute
	spim_instruction_table[rtl_add] = new RTL_Insn_Descriptor(rtl_add, "add", "add", "", rtl_r_o1_op_o2, asm_op_r_o1_o2);
	spim_instruction_table[rtl_sub] = new RTL_Insn_Descriptor(rtl_sub, "sub", "sub", "", rtl_r_o1_op_o2, asm_op_r_o1_o2);
	spim_instruction_table[rtl_mult] = new RTL_Insn_Descriptor(rtl_mult, "mul", "mul", "", rtl_r_o1_op_o2, asm_op_r_o1_o2);
	spim_instruction_table[rtl_div] = new RTL_Insn_Descriptor(rtl_div, "div", "div", "", rtl_r_o1_op_o2, asm_op_r_o1_o2);
	spim_instruction_table[rtl_imm_add] = new RTL_Insn_Descriptor(rtl_imm_add, "addi", "addi", "", rtl_r_o1_op_o2, asm_op_r_o1_o2);
	spim_instruction_table[rtl_uminus] = new RTL_Insn_Descriptor(rtl_uminus, "uminus", "neg", "", rtl_r_op_o1, asm_op_r_o1);
	spim_instruction_table[rtl_add_d] = new RTL_Insn_Descriptor(rtl_add_d, "add.d", "add.d", "", rtl_r_o1_op_o2, asm_op_r_o1_o2);
	spim_instruction_table[rtl_sub_d] = new RTL_Insn_Descriptor(rtl_sub_d, "sub.d", "sub.d", "", rtl_r_o1_op_o2, asm_op_r_o1_o2);
	spim_instruction_table[rtl_mult_d] = new RTL_Insn_Descriptor(rtl_mult_d, "mul.d", "mul.d", "", rtl_r_o1_op_o2, asm_op_r_o1_o2);
	spim_instruction_table[rtl_div_d] = new RTL_Insn_Descriptor(rtl_div_d, "div.d", "div.d", "", rtl_r_o1_op_o2, asm_op_r_o1_o2);
	spim_instruction_table[rtl_uminus_d] = new RTL_Insn_Descriptor(rtl_uminus_d, "uminus.d", "neg.d", "", rtl_r_op_o1, asm_op_r_o1);
	// comparison 
	spim_instruction_table[rtl_slt] = new RTL_Insn_Descriptor(rtl_slt, "slt", "slt", "", rtl_r_o1_op_o2, asm_op_r_o1_o2);
	spim_instruction_table[rtl_sle] = new RTL_Insn_Descriptor(rtl_sle, "sle", "sle", "", rtl_r_o1_op_o2, asm_op_r_o1_o2);
	spim_instruction_table[rtl_sgt] = new RTL_Insn_Descriptor(rtl_sgt, "sgt", "sgt", "", rtl_r_o1_op_o2, asm_op_r_o1_o2);
	spim_instruction_table[rtl_sge] = new RTL_Insn_Descriptor(rtl_sge, "sge", "sge", "", rtl_r_o1_op_o2, asm_op_r_o1_o2);
	spim_instruction_table[rtl_seq] = new RTL_Insn_Descriptor(rtl_seq, "seq", "seq", "", rtl_r_o1_op_o2, asm_op_r_o1_o2);
	spim_instruction_table[rtl_sne] = new RTL_Insn_Descriptor(rtl_sne, "sne", "sne", "", rtl_r_o1_op_o2, asm_op_r_o1_o2);
	spim_instruction_table[rtl_seq_d] = new RTL_Insn_Descriptor(rtl_seq_d, "seq.d", "c.eq.d", "", rtl_o1_op_o2, asm_op_o1_o2);
        spim_instruction_table[rtl_slt_d] = new RTL_Insn_Descriptor(rtl_slt_d, "slt.d", "c.lt.d", "", rtl_o1_op_o2, asm_op_o1_o2);
	spim_instruction_table[rtl_sle_d] = new RTL_Insn_Descriptor(rtl_sle_d, "sle.d", "c.le.d", "", rtl_o1_op_o2, asm_op_o1_o2);
	spim_instruction_table[rtl_sgt_d] = new RTL_Insn_Descriptor(rtl_sgt_d, "sgt.d", "c.le.d", "", rtl_o1_op_o2, asm_op_o1_o2);
	spim_instruction_table[rtl_sge_d] = new RTL_Insn_Descriptor(rtl_sge_d, "sge.d", "c.lt.d", "", rtl_o1_op_o2, asm_op_o1_o2);
	spim_instruction_table[rtl_sne_d] = new RTL_Insn_Descriptor(rtl_sne_d, "sne.d", "c.eq.d", "", rtl_o1_op_o2, asm_op_o1_o2);
	// input output
	spim_instruction_table[rtl_print] = new RTL_Insn_Descriptor(rtl_print, "write", "syscall", "", rtl_op, asm_op);
	spim_instruction_table[rtl_read] = new RTL_Insn_Descriptor(rtl_read, "read", "syscall", "", rtl_op, asm_op); 
	// nop
	spim_instruction_table[rtl_nop] = new RTL_Insn_Descriptor(rtl_nop, "nop", "nop", "", rtl_op, asm_op); 
}

void Machine_Description::validate_init_local_register_mapping_before_fn_call()
{
	map<Spim_Register, Register_Descriptor *>::iterator i;
	for (i = spim_register_table.begin(); i != spim_register_table.end(); i++)
	{
		Register_Descriptor * reg_desc = i->second;

		if (reg_desc->is_used_for_fn_return() == false)
		{
			if (reg_desc->get_use_category() == int_reg)
			{
				CHECK_INVARIANT((reg_desc->is_free<int_reg>()), "GP data registers should be free at the start of a basic block or after a function call");
			}

			else if (reg_desc->get_use_category() == float_reg)
			{
				CHECK_INVARIANT((reg_desc->is_free<float_reg>()), "Float data registers should be free at the start of a basic block or after a function call");
			}
		}
	}
}

void Machine_Description::validate_init_local_register_mapping()
{
	map<Spim_Register, Register_Descriptor *>::iterator i;
	for (i = spim_register_table.begin(); i != spim_register_table.end(); i++)
	{
		Register_Descriptor * reg_desc = i->second;

		if (reg_desc->get_use_category() == int_reg)
		{
			CHECK_INVARIANT((reg_desc->is_free<int_reg>()), "GP data registers should be free at the start of a basic block or after a function call");
		}

		else if (reg_desc->get_use_category() == float_reg)
		{
			CHECK_INVARIANT((reg_desc->is_free<float_reg>()), "Float data registers should be free at the start of a basic block or after a function call");
		}
	}
}


template <Register_Use_Category dt>
Register_Descriptor * Machine_Description::get_new_register()
{
	Register_Descriptor * reg_desc;
	int count = 0;

	map<Spim_Register, Register_Descriptor *>::iterator i;
	for (i = spim_register_table.begin(); i != spim_register_table.end(); i++)
	{
		reg_desc = i->second;

		CHECK_INVARIANT((reg_desc != NULL), "Null register descriptor in the register table");

		if (reg_desc->is_free<dt>()) 
		{       
			reg_desc->set_register_occupied();
			return reg_desc;
		}
	}

	machine_desc_object.clear_reg_not_used_for_expr_result();

	count = machine_desc_object.count_free_register<dt>();
	if (count > 0)
		return get_new_register<dt>();
	else
	{
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, 
			"Error in get_new_reg or register requirements of input program cannot be met");
	}
}

template <Register_Use_Category dt>
int Machine_Description::count_free_register()
{
	Register_Descriptor * rdp = NULL;
	int count = 0;

	map<Spim_Register, Register_Descriptor *>::iterator i;
	for (i = spim_register_table.begin(); i != spim_register_table.end(); i++)
	{
		rdp = i->second;
		if (rdp->is_free<dt>())
			count++;
	}

	return count;
}

void Machine_Description::clear_reg_not_used_for_expr_result()
{
	map<Spim_Register, Register_Descriptor *>::iterator i;

	for (i = spim_register_table.begin(); i != spim_register_table.end(); i++)
	{
		Register_Descriptor * rdp = i->second;

		if(!rdp->is_used_for_expr_result()) 
		{
			rdp->reset_register_occupied();  /* reset reg occupied i.e register is not anymore occupied */
			break;
		}
	}
}

template bool Register_Descriptor::is_free<int_reg>();
template bool Register_Descriptor::is_free<float_reg>();

template Register_Descriptor * Machine_Description::get_new_register<int_reg>();
template Register_Descriptor * Machine_Description::get_new_register<float_reg>();

template int Machine_Description::count_free_register<int_reg>();
template int Machine_Description::count_free_register<float_reg>();
