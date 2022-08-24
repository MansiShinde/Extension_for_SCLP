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

#ifndef TACODE_HH
#define TACODE_HH

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "rtl.hh"

/* 
	This file defines classes for three address code form. 
	A three address code (TAC) statement consists of an 
	operator, possibly two operands and a result.
*/


typedef enum 
{ 
/* Assignment */
	copy_3a,
/* Arithmetic operations */
	add_3a, 
	sub_3a,
	mult_3a, 
	div_3a, 
	uminus_3a,
/* Logical operations */	
	and_3a,
	or_3a,
	not_3a,
/* Comparison operations */
	lt_3a,
	leq_3a,
	gt_3a,
	geq_3a,
	eq_3a,
	neq_3a,
/* Intraprocedural control flow */
	goto_3a,
	if_goto_3a,
	label_3a,
/* Interprocedural control flow */
	ret_3a,
	call_3a,
/* Input/Output */
	read_3a,
	write_3a,
/* NOP  */
	nop_3a,
} TAC_Op;


class TAC_Stmt
{
protected:
	TAC_Op tac_operator;

public:
	TAC_Stmt();
	~TAC_Stmt();

	TAC_Op get_op();

	virtual TAC_Opd * get_opd1();
	virtual TAC_Opd * get_opd2();
	virtual TAC_Opd * get_result();
	virtual string get_comment();

	virtual void set_opd1(TAC_Opd * opd);
	virtual void set_opd2(TAC_Opd * opd);
	virtual void set_result(TAC_Opd * opd);
	virtual void set_comment(string s);
	virtual RTL_For_TAC & gen_rtl() {}     // CHECK: Flesh this out or make it null 

	virtual void print_tac_stmt(ostream & file_buffer) = 0;
};

///////////////////////////////// TAC Assignment/Copy Statement //////////////////////////////////////// 
/////////////////////                    Example:    x=y                              /////////////////

class Copy_TAC_Stmt: public TAC_Stmt
{ 
	TAC_Opd * opd1;   
	TAC_Opd * result; 

public:
	Copy_TAC_Stmt(TAC_Opd * lval, TAC_Opd * rval);   /* lval = rval */
	~Copy_TAC_Stmt() {} 

	TAC_Opd * get_opd1();
	TAC_Opd * get_result();

	void set_opd1(TAC_Opd * opd);
	void set_result(TAC_Opd * res);
	RTL_For_TAC & gen_rtl();

	void print_tac_stmt(ostream & file_buffer);
};

/////////////////////////////////  TAC Compute Statement   //////////////////////////////////// 
/// Arithmetic operations: add_3a, sub_3a, mult_3a, div_a, uminus_3a   ////////////////////////
///    Logical operations: and_3a, or_3a, not_3a                    //////////////////////////////
/// Comparison operations: lt_3a, leq_3a, gt_3a, geq_3a, eq_3a, neq_3a //////////////////////////////
/////////////////////        Examples: x = y op z; x = op z;                  /////////////////

class Compute_TAC_Stmt: public TAC_Stmt
{ 
	TAC_Opd * opd1;   
	TAC_Opd * opd2;   
	TAC_Opd * result; 
	Data_Type tac_stmt_type;
	// CHECK: In case of arithmetic operations, the type of the operands and the
	// type of result is same. For comparison operations, although the result is 
	// boolean, we need to know the type of the operands for selecting right rtl operator.
	// It is not clear what the data type should be for logical operations. 
	// Perhaps we can leave it as void_data_type.

public:
	Compute_TAC_Stmt(TAC_Op op, TAC_Opd * opd1, TAC_Opd * opd2, TAC_Opd * result, Data_Type dt); 
	~Compute_TAC_Stmt() {} 

	RTL_Op get_rtl_operator_for_tac_operator(); 

	TAC_Opd * get_opd1();
	TAC_Opd * get_opd2();
	TAC_Opd * get_result();

	void set_opd1(TAC_Opd * opd);
	void set_opd2(TAC_Opd * opd);
	void set_result(TAC_Opd * res);
	Data_Type get_data_type();

	RTL_For_TAC & gen_rtl();

	void print_tac_stmt(ostream & file_buffer);
private:
	string op_name(TAC_Op op);
	bool is_binary_op(TAC_Op op);
};

/////////////////////////////////   TAC Control Flow Statement    ////////////////////////////// 
/// Intraprocedural control flow: goto_3a, if_goto_3a, label_3a,  //////////////////////////////
/// Interprocedural control flow: ret_3a, call_3a                 //////////////////////////////
/////////////////////      Examples: goto L1:; if t1 then goto L1"; ////////////////////////////
/////////////////////      Examples: return x; L1:; w = f(x, y, z); ////////////////////////////

class If_Goto_TAC_Stmt: public TAC_Stmt
{
	TAC_Opd * opd1;		/* The condition result is true */
	TAC_Opd * result;	/* transfer the control here */

public:
	If_Goto_TAC_Stmt(TAC_Opd * opd1, TAC_Opd * l);
	~If_Goto_TAC_Stmt() {}
	
	TAC_Opd * get_opd1();
	TAC_Opd * get_result();

	void set_opd1(TAC_Opd * opd);
	void set_result(TAC_Opd * result);

	void print_tac_stmt(ostream & file_buffer);
};

class Goto_TAC_Stmt: public TAC_Stmt
{
	TAC_Opd * result;	/* Target label */

public:
	Goto_TAC_Stmt(TAC_Opd * result);
	~Goto_TAC_Stmt() {}
	
	TAC_Opd * get_result();
	void set_result(TAC_Opd * result);

	void print_tac_stmt(ostream & file_buffer);
};

class Call_TAC_Stmt: public TAC_Stmt
{
	TAC_Opd * opd1;		/* Callee function */
	TAC_Opd * result;	
	list <TAC_Opd *> actual_param_list;

public:
	Call_TAC_Stmt(TAC_Opd * opd1, TAC_Opd * result);
	~Call_TAC_Stmt() {}
	
	TAC_Opd * get_opd1();
	TAC_Opd * get_result();
	list <TAC_Opd *> get_actual_param_list();

	void set_opd1(TAC_Opd * opd);
	void set_result(TAC_Opd * opd);
	void set_actual_param_list(list <TAC_Opd *> apl);

	void print_tac_stmt(ostream & file_buffer);
};


class Label_TAC_Stmt: public TAC_Stmt
{       
        TAC_Opd * label_opd;

public:
	Label_TAC_Stmt(TAC_Opd * label_opd); 
	~Label_TAC_Stmt() {} 

	int get_label();

	string get_label_name();

	void print_tac_stmt(ostream & file_buffer);
};

/////////////////////////////////// TAC I/O Statement //////////////////////////////////////
//////////////////////////          read_3a, write_3a             //////////////////////////
/////////////////////////       Examples: read x; write x;        //////////////////////////

class IO_TAC_Stmt: public TAC_Stmt
{
	TAC_Opd * opd1;		

 public:
	IO_TAC_Stmt(TAC_Op op, TAC_Opd * opd1);
 	~IO_TAC_Stmt() {}

	TAC_Opd * get_opd1();
	void set_opd1(TAC_Opd * opd);
 	RTL_For_TAC & gen_rtl();
	void print_tac_stmt(ostream & file_buffer) ;

private:
	RTL_For_TAC & gen_rtl_for_read();
	RTL_For_TAC & gen_rtl_for_write();
	
};

/////////////////////////// TAC NOP (No Operation) Statement ////////////////////////////
/////////////////////////              Example: NOP          ////////////////////////////

class NOP_TAC_Stmt: public TAC_Stmt
{

 public:
	NOP_TAC_Stmt();
 	~NOP_TAC_Stmt();

	void print_tac_stmt(ostream & file_buffer) ;
};

/////////////////////////////////// TAC Statement Operands ///////////////////////////////////

class TAC_Opd
{	
	/* 
		Abstract base class for TAC operands. From this class we 
		derive classes for three kinds of operands: (source) variables, 
		constants, and temporaries. 
	*/

public:
	virtual void print_opd(ostream & file_buffer) = 0;
	virtual RTL_For_TAC & create_load_stmt(); 
	virtual RTL_For_TAC & create_store_stmt(Register_Descriptor *rd); 
	virtual Data_Type get_data_type(); 
	virtual void set_register_of_temporary(Register_Descriptor *rd);
	virtual Symbol_Table_Entry * get_symbol_entry();
	virtual int get_string_key();

	virtual int get_label();
	virtual string get_label_name();
	//virtual void print_tac_label(ostream & file_buffer);
};

class Variable_TAC_Opd: public TAC_Opd
{
	Symbol_Table_Entry * symbol_entry;

public:
	Variable_TAC_Opd(Symbol_Table_Entry * se);
	~Variable_TAC_Opd() {}

	void print_opd(ostream & file_buffer);
	RTL_For_TAC & create_load_stmt();
	RTL_For_TAC & create_store_stmt(Register_Descriptor *rd);
	Data_Type get_data_type();
	Symbol_Table_Entry * get_symbol_entry();

	Variable_TAC_Opd & operator= (const Variable_TAC_Opd & rhs);
};

class Temporary_TAC_Opd: public TAC_Opd
{
	int temp_number;
	Register_Descriptor * register_for_temporary;
	Data_Type data_type_of_temp;

public:
        static int tac_temp_count;
	Temporary_TAC_Opd(Data_Type dt);
	~Temporary_TAC_Opd() {}

	void print_opd(ostream & file_buffer);
	RTL_For_TAC & create_load_stmt();
	RTL_For_TAC & create_store_stmt(Register_Descriptor *rd);
	Register_Descriptor * get_register_of_temporary();
	void set_register_of_temporary(Register_Descriptor * reg);
	Data_Type get_data_type();
	void print_opd_with_register(ostream & file_buffer);			// CHECK: can be removed after debugging

	Temporary_TAC_Opd & operator=(const Temporary_TAC_Opd & rhs);
};


class Int_Const_TAC_Opd: public TAC_Opd
{
	int num;

public:
	Int_Const_TAC_Opd (int num);
	~Int_Const_TAC_Opd() {}

	void print_opd(ostream & file_buffer);
	RTL_For_TAC & create_load_stmt();
	Data_Type get_data_type();

	Int_Const_TAC_Opd & operator=(const Int_Const_TAC_Opd & rhs);
};

class Double_Const_TAC_Opd: public TAC_Opd
{
	double num;

public:
	Double_Const_TAC_Opd (double num);
	~Double_Const_TAC_Opd() {}

	void print_opd(ostream & file_buffer);
	RTL_For_TAC & create_load_stmt();
	Data_Type get_data_type();

	Double_Const_TAC_Opd & operator=(const Double_Const_TAC_Opd & rhs);
};

class Str_Const_TAC_Opd: public TAC_Opd
{
	int str_key;

public:
	Str_Const_TAC_Opd (int key);
	~Str_Const_TAC_Opd() {}

	void print_opd(ostream & file_buffer);
	RTL_For_TAC & create_load_stmt();
	Data_Type get_data_type();
	int get_string_key();

	Str_Const_TAC_Opd & operator=(const Str_Const_TAC_Opd & rhs);
};

class Label_TAC_Opd: public TAC_Opd
{       
        static int tac_label_count;
	int label_number;		/* Constant number from which the label is generated */

public:
	Label_TAC_Opd(); 
	~Label_TAC_Opd() {} 

	int get_label();

	string get_label_name();

	void print_opd(ostream & file_buffer);
};

//////////////////////// TAC statement list for Ast statements ////////////////////////

class TAC_For_Ast
{
	/* 
		This class describes the TAC statements generated for a given
		AST in a sequence asts. Note that a single AST would
		in general need multiple statements to implement it.
		We also need to remember the intermediate results of
		an AST because it could be a subtree of a larger AST.
	*/

	list<TAC_Stmt *> * TAC_list;
	TAC_Opd * result_temp;

public:
	TAC_For_Ast();
	TAC_For_Ast(list<TAC_Stmt *> * tac_l, TAC_Opd * res_temp);  /* The result of the computation of 
                                                   	               tac_1 is contained in res_temp 
                                             			    */ 
	~TAC_For_Ast() {}

	void append_tac_stmts(TAC_Stmt * tacs);
	list<TAC_Stmt *> * get_tac_list();
	void set_TAC_list(list<TAC_Stmt *> * tac_list_new);

	void print_tac_stmt_list(ostream & file_buffer);

	TAC_Opd * get_result();
	void set_result(TAC_Opd * result);

	TAC_For_Ast & operator= (const TAC_For_Ast & rhs);
};

#endif
