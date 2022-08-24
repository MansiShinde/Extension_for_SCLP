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

#ifndef REG_ALLOC_HH
#define REG_ALLOC_HH

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>

/////////////////////////////// Register_Descriptor ////////////////////

typedef enum
{
	no_reg,	/* dummy to indicate no register */
	zero,	/* constant register */
	v0,	/* expression result register */
	v1,	/* function result register */
	a0,	/* argument register */
	a1, a2, a3,
	t0,      /* temporary caller-save registers */
	t1, t2, t3, t4, t5, t6, t7, t8, t9, 
	s0,	/* temporary callee-save registers */ 
	s1, s2, s3, s4, s5, s6, s7,
	mfc,	/* float register to int register */
	mtc,	/* int register to float register */
	f0, 	/* floating point registers */
	f2, f4, f6, f8,
	f10, f12, f14, f16, f18,
	f20, f22, f24, f26, f28, f30,
	gp,	/* global data pointer register */
	sp,	/* stack pointer register */
	fp,	/* frame pointer register */
	ra	/* return address register */
} Spim_Register;

typedef enum 
{ 
	int_num,
	float_num,
	void_reg_type
} Register_Val_Type;

typedef enum 
{ 
	fixed_reg, 
	int_reg, 
	fn_result, 
	argument,
	pointer, 
	ret_address,
	float_reg
} Register_Use_Category;

class Register_Descriptor
{
    Spim_Register reg_id;
    string reg_name;
    Register_Val_Type value_type;
    Register_Use_Category reg_use; 

    bool used_for_expr_result;
    bool reg_occupied;
    bool used_for_fn_return;

  public:
    Register_Descriptor (Spim_Register reg, string nam, Register_Val_Type vt, Register_Use_Category uc);
    Register_Descriptor() {}
    ~Register_Descriptor() {}

    void update_symbol_information(Symbol_Table_Entry & symbol_entry);

    bool find_symbol_entry_in_list(Symbol_Table_Entry & symbol_entry);
    void remove_symbol_entry_from_list(Symbol_Table_Entry & symbol_entry);

    Register_Use_Category get_use_category(); 
    Spim_Register get_register();

    string get_name();

    Register_Val_Type get_register_type();

    bool is_register_occupied();
    void set_register_occupied();
    void reset_register_occupied();

    bool is_used_for_fn_return();
    void set_used_for_fn_return();
    void reset_used_for_fn_return();

    bool is_used_for_expr_result();
    void set_use_for_expr_result();
    void reset_use_for_expr_result();

    template <Register_Use_Category dt>
    bool is_free();
};


///////////////////////////// Machine Description ////////////////////////////////

class Machine_Description
{
	Register_Descriptor * no_register;

public:
	map<RTL_Op, RTL_Insn_Descriptor *> spim_instruction_table;
	map<Spim_Register, Register_Descriptor *> spim_register_table;

	void initialize_instruction_table();
	void initialize_register_table();

	void validate_init_local_register_mapping_before_fn_call();
	void validate_init_local_register_mapping();
	void clear_local_register_mappings();

	void clear_reg_not_used_for_expr_result();

	template <Register_Use_Category dt>
	int count_free_register();

	template <Register_Use_Category dt>
	Register_Descriptor * get_new_register();
	Register_Descriptor * get_no_reg();
	Register_Descriptor * get_return_register_for_procedure(Data_Type dt);
};

extern Machine_Description machine_desc_object;

#endif
