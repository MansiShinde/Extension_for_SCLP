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

#ifndef ICODE_HH
#define ICODE_HH

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "common-classes.hh"
#include"symbol-table.hh"
#include"procedure.hh"

/* 
	This file defines classes for the RTL (Register Transfer Language) code generated 
	by our compiler. It also defined classes for machine instructions.

	An RTL statement consists of an instruction, possibly two operands, and a result.
*/

typedef enum 
{			/* asm: assembly format; r: result; o1: opd1; o2: opd2; op: operator */
	asm_op,		/* Only the operator, no operand */
	asm_op_o1,	/* Only one operand, no result, eg. goto L */
	asm_op_r,		/* Only the result. Operand implicit? */ 
	asm_op_r_o1,	/* r <- o1 */
	asm_op_o1_r,	/* r <- o1 */
	asm_op_r_r_o1,	/* r <- r op o1 */
	asm_op_r_o1_o2,	/* r <- o1 op o2 */ 
	asm_op_o1_o2_r,	/* r <- o1 op o2 */
	asm_op_o1_o2_st,	/* for conditional branch*/
	asm_op_st,	/* label instr */
	asm_nsy,		/* not specified yet */
        asm_op_o1_o2
} ASM_Format;

typedef enum 
{			/* rtl: intermediate format; r: result; o1: opd1; o2: opd2; op: operator */
	rtl_op,		/* Only the operator, no operand */
	rtl_op_o1,	/* Only one operand, no result, eg. goto L */
	rtl_r_op,		/* Only the result. Operand implicit? */ 
	rtl_op_o1_r,    
	rtl_op_r_o1,    
	rtl_r_op_o1,	/* r <- o1 */
	rtl_r_r_op_o1,	/* r <- r op o1 */
	rtl_r_o1_op_o2,	/* r <- o1 op o2 */
	rtl_op_o1_o2_st,	/* for conditional branch */
	rtl_op_st,	/* label instr */
	rtl_nsy,		/* not specified yet */
	rtl_o1_op_o2
} RTL_Format;

typedef enum 
{ 
	rtl_push,
	// load 
	rtl_load_imm,
	rtl_load_imm_d, 
	rtl_load, 
	rtl_load_d, 
	rtl_load_addr,
	rtl_load_ind,
	rtl_load_ind_d,
	// store
	rtl_store, 
	rtl_store_d, 
	rtl_store_ind, 
	rtl_store_ind_d, 
	// logical
	rtl_and,
	rtl_or,
	rtl_not,
	// move
	rtl_move,
	rtl_move_d,
	// control flow
	rtl_bgtz,
	rtl_bgez,
	rtl_bltz,
	rtl_blez,
	rtl_bc1t,	// branch coprocessor1 on true
	rtl_bc1f,	// branch coprocessor1 on false
	rtl_beq,
	rtl_bne,
	rtl_goto,
	rtl_label,
	rtl_return,
	rtl_call,
	// compute
	rtl_add, 
	rtl_imm_add,
	rtl_sub,
	rtl_mult, 
	rtl_div, 
	rtl_uminus,
	rtl_add_d, 
	rtl_sub_d, 
	rtl_mult_d, 
	rtl_div_d, 
	rtl_uminus_d,
	// comparison 
	rtl_slt,
	rtl_sle,
	rtl_sgt,
	rtl_sge,
	rtl_sne,
	rtl_seq,
	rtl_sle_d,
	rtl_slt_d,
	rtl_seq_d,
	rtl_sgt_d,
	rtl_sge_d,
	rtl_sne_d,
	// input output
	rtl_print,
	rtl_read,
	// nop
	rtl_nop,
} RTL_Op;

///////////////////////// Instruction Descriptor ///////////////////////////////////

class RTL_Insn_Descriptor
{
	/* 
		This class stores objects representing machine instructions
		which in our case are assembly instructions supported by spim.
	*/

	RTL_Op inst_op;
	string name;		// symbol for printing in RTL code
	string mnemonic; 	// Symbol for printing in assembly
	RTL_Format rtl_format;   // format for printing in RTL code 
	ASM_Format assem_format;
	bool needs_special_handling; // Direct translation is not possible

public:
	RTL_Insn_Descriptor (RTL_Op op, string name, string mnn, RTL_Format rtlf, ASM_Format af, bool sp_handling);
	RTL_Insn_Descriptor();
	~RTL_Insn_Descriptor() {}

	RTL_Op get_op();
	string get_name();
	string get_mnemonic();
	RTL_Format get_rtl_format();
	ASM_Format get_assembly_format();
 
	void print_rtl_instruction_descriptor(ostream & file_buffer);
};

///////////////////////////// Icode statement operand ///////////////////////////////////

class RTL_Opd
{	
	/* 
		Abstract base class for RTL operands. From this class we 
		derive classes for three kinds of operands: memory addresses, 
		constants, and registers 
	*/

public:
	virtual Register_Descriptor * get_reg();
	virtual Basic_Data_Type get_basic_data_type();
	virtual Symbol_Table_Entry * get_symbol(); 

	/* Operands are printed differently in RTL and assembly code */

	virtual void print_rtl_opd(ostream & file_buffer) = 0;
	virtual void print_asm_opd(ostream & file_buffer) = 0;
};

class Mem_Opd: public RTL_Opd
{
	Symbol_Table_Entry * symbol_entry;

public:
	Mem_Opd(Symbol_Table_Entry & se);
	~Mem_Opd() {}

	void print_rtl_opd(ostream & file_buffer);
	void print_asm_opd(ostream & file_buffer);
	Symbol_Table_Entry * get_symbol();  

	Mem_Opd & operator= (const Mem_Opd & rhs);
};

class Register_Opd: public RTL_Opd
{
	Register_Descriptor * register_description;

public:
	Register_Opd(Register_Descriptor * rd);
	~Register_Opd() {}

	Register_Descriptor * get_reg();
	void print_rtl_opd(ostream & file_buffer);
	void print_asm_opd(ostream & file_buffer);

	Register_Opd & operator=(const Register_Opd & rhs);
};

class Int_Const_Opd: public RTL_Opd
{
	int num;

public:
	Int_Const_Opd (int num);
	~Int_Const_Opd() {}

	void print_rtl_opd(ostream & file_buffer);
	void print_asm_opd(ostream & file_buffer);
	Basic_Data_Type get_basic_data_type();

	Int_Const_Opd & operator=(const Int_Const_Opd & rhs);
};

class Double_Const_Opd: public RTL_Opd
{
	double num;

public:
	Double_Const_Opd (double num);
	~Double_Const_Opd() {}

	void print_rtl_opd(ostream & file_buffer);
	void print_asm_opd(ostream & file_buffer);
	Basic_Data_Type get_basic_data_type();

	Double_Const_Opd & operator=(const Double_Const_Opd & rhs);
};

class Str_Const_Opd: public RTL_Opd
{
	int str_key;

public:
	Str_Const_Opd (int key);
	~Str_Const_Opd() {}

	void print_rtl_opd(ostream & file_buffer);
	void print_asm_opd(ostream & file_buffer);

	Str_Const_Opd & operator=(const Str_Const_Opd & rhs);
};

///////////////////////////////// RTL statements //////////////////////////

class RTL_Stmt
{
	/* 
		Abstract base class for generated rtl statements. From this 
		class, we derive three classes: move, compute, control_Flow.
	*/

protected:
	RTL_Insn_Descriptor rtl_desc;
	string rtl_comment;
	string asm_comment;

public:
	RTL_Stmt();
	~RTL_Stmt(); 

	RTL_Insn_Descriptor & get_op();
	virtual RTL_Opd * get_opd1();
	virtual RTL_Opd * get_opd2();
	virtual RTL_Opd * get_result();
	virtual string get_rtl_comment();
	virtual string get_asm_comment();

	virtual void set_opd1(RTL_Opd * io);
	virtual void set_opd2(RTL_Opd * io);
	virtual void set_result(RTL_Opd * io);
	virtual void set_rtl_comment(string s);
	virtual void set_asm_comment(string s);
	virtual void set_comment(string s);	// Sets both rtl_comment as well as asm_comment

	virtual void print_rtl_stmt(ostream & file_buffer) = 0;
	virtual void print_assembly(ostream & file_buffer) = 0;
	virtual void print_rtl_comment(ostream & file_buffer);
	virtual void print_asm_comment(ostream & file_buffer);
};
class Write_RTL_Stmt: public RTL_Stmt
{

 public:
 	Write_RTL_Stmt();
 	~Write_RTL_Stmt();
 void print_rtl_stmt(ostream & file_buffer) ;
 void print_assembly(ostream & file_buffer);
};

class Read_RTL_Stmt: public RTL_Stmt
{
 public:
 	Read_RTL_Stmt();
 	~Read_RTL_Stmt();
 void print_rtl_stmt(ostream & file_buffer) ;
 void print_assembly(ostream & file_buffer);
};

class Move_RTL_Stmt: public RTL_Stmt
{ 
	RTL_Opd * opd1;   
	RTL_Opd * result; 

public:
	Move_RTL_Stmt(RTL_Op inst_op, RTL_Opd * opd1, RTL_Opd * result); 
	~Move_RTL_Stmt() {} 
	Move_RTL_Stmt & operator=(const Move_RTL_Stmt & rhs);

	RTL_Insn_Descriptor & get_inst_op_of_rtl_stmt();

	RTL_Opd * get_opd1();
	void set_opd1(RTL_Opd * io);

	RTL_Opd * get_result();
	void set_result(RTL_Opd * io);

	void print_rtl_stmt(ostream & file_buffer);
	void print_assembly(ostream & file_buffer);
};


// TODO -> write code for other RTL_Stmt derived classes
class Compute_RTL_Stmt: public RTL_Stmt
{ 
	RTL_Opd * opd1;   
	RTL_Opd * opd2;   
	RTL_Opd * result; 

public:
	Compute_RTL_Stmt(RTL_Op inst_op, RTL_Opd * opd1, RTL_Opd * opd2, RTL_Opd * result); 
	~Compute_RTL_Stmt() {} 
	Compute_RTL_Stmt & operator=(const Compute_RTL_Stmt & rhs);

	RTL_Insn_Descriptor & get_inst_op_of_rtl_stmt();

	RTL_Opd * get_opd1();
	void set_opd1(RTL_Opd * io);

	RTL_Opd * get_opd2();
	void set_opd2(RTL_Opd * io);

	RTL_Opd * get_result();
	void set_result(RTL_Opd * io);

	void print_rtl_stmt(ostream & file_buffer);
	void print_assembly(ostream & file_buffer);
	void print_logical_not_assembly(ostream & file_buffer);
};

class Control_Flow_RTL_Stmt: public RTL_Stmt
{
	RTL_Opd * opd1;
	RTL_Opd * opd2;
	string offset;
	int size_of_actual_params_of_call;

public:
	Control_Flow_RTL_Stmt(RTL_Op op, RTL_Opd * o1, RTL_Opd * o2, string label, int size_actual_params = 0);  
			
	~Control_Flow_RTL_Stmt() {}
	
	Control_Flow_RTL_Stmt& operator=(const Control_Flow_RTL_Stmt& rhs);

	RTL_Insn_Descriptor & get_inst_op_of_rtl_stmt();

	RTL_Opd * get_opd1();
	void set_opd1(RTL_Opd * io);

	RTL_Opd * get_opd2();
	void set_opd2(RTL_Opd * io);
 	
	string get_Offset();
	void set_Offset(string label);

	void print_rtl_stmt(ostream & file_buffer);
     	void print_assembly(ostream & file_buffer);
};


class Label_RTL_Stmt: public RTL_Stmt
{       
	string label; 

public:
	Label_RTL_Stmt(RTL_Op inst_op, string label); 
	~Label_RTL_Stmt() {} 
	Label_RTL_Stmt & operator=(const Label_RTL_Stmt & rhs);

	RTL_Insn_Descriptor & get_inst_op_of_rtl_stmt();

	string get_label();
	void set_label(string label);

	void print_rtl_stmt(ostream & file_buffer);
	void print_assembly(ostream & file_buffer);
};

class NOP_RTL_Stmt : public RTL_Stmt
{

public :
	NOP_RTL_Stmt() {}
	~NOP_RTL_Stmt() {}
	void print_rtl_stmt(ostream & file_buffer);
	void print_assembly(ostream & file_buffer);

};

//////////////////////// Intermediate code for Ast statements ////////////////////////

class RTL_For_TAC
{
	/* 
		This class describes the code generated for a given
		AST in a sequence ast. Note that a single AST would
		in general need multiple statements to implement it.
		We also need to remember the intermediate results of
		an AST because it could be a subtree of a larger AST.
	*/

	list<RTL_Stmt *> rtl_stmt_list;
	Register_Descriptor * result_register;

public:
	RTL_For_TAC();
	RTL_For_TAC(list<RTL_Stmt *> & ic_l, Register_Descriptor * reg);
	~RTL_For_TAC() {}

	void append_rtl_stmt(RTL_Stmt & rtl_stmt);
	list<RTL_Stmt *> & get_rtl_list();
	RTL_Stmt * get_first_rtl_stmt();
	void set_rtl_list(list<RTL_Stmt *> * ic_list_new);

	Register_Descriptor * get_reg();
	void set_reg(Register_Descriptor * reg);

	void print_rtl_stmt_list(ostream & file_buffer);

	RTL_For_TAC & operator=(const RTL_For_TAC & rhs);
};

#endif
