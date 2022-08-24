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

#ifndef AST_HH
#define AST_HH

#include<string>
#include<iostream>
#include<iomanip>
#include<typeinfo>
#include<list>

#define AST_SPACE "         "
#define AST_NODE_SPACE "            "
#define AST_SUB_NODE_SPACE "               "

#define AST_INDENT 4
#define INDENT_INCR 2

#define END_OF_PROG -3	
// Used by goto_ast and basic_block

using namespace std;


class Ast;
class Class_Type_Expr;

class Ast
{
protected:
	Basic_Data_Type node_data_type;

   static int labelCounter;
	int lineno;
	string get_new_label(){

		return "label"+to_string(labelCounter++);
	}

public:
	Ast();
	~Ast();

	virtual Basic_Data_Type get_basic_data_type();
	virtual void set_data_type(Basic_Data_Type dt);

	virtual bool is_value_zero();

	virtual void type_check_ast();
	virtual Symbol_Table_Entry & get_symbol_entry();

	virtual void print_ast(int space_count, ostream & file_buffer) = 0;

	virtual Ast * get_offset_ast();


	virtual TAC_For_Ast * gen_tac();
	virtual TAC_For_Ast * gen_tac_aggregate();
	virtual TAC_Op get_tac_operator_for_rel_op_in_ast();
	void print_spaces_on_new_line (int sc, ostream & file_buffer);
	int get_lineno();
	virtual int count_indirection_level();
	virtual bool is_a_scalar();
	virtual bool is_an_array();
	virtual bool is_a_pointer();
};

class Assignment_Ast: public Ast
{
	Ast * lhs;
	Ast * rhs;

public:
	Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs, int line);
	~Assignment_Ast();

	void type_check_ast();

	void print_ast(int space_count, ostream & file_buffer);

	TAC_For_Ast * gen_tac();
};

class Name_Ast: public Ast
{
	Symbol_Table_Entry * variable_symbol_entry;

public:
	Name_Ast(string & name, Symbol_Table_Entry & var_entry, int line);
	~Name_Ast();

	Basic_Data_Type get_basic_data_type();
	Symbol_Table_Entry & get_symbol_entry();
	void set_data_type(Basic_Data_Type dt);

	Ast * get_offset_ast();


	void print_ast(int space_count, ostream & file_buffer);

	TAC_For_Ast * gen_tac();
	TAC_For_Ast * gen_tac_aggregate();
	int count_indirection_level();
	bool is_a_scalar();
	bool is_an_array();
	bool is_a_pointer();
};

template <class T>
class Number_Ast: public Ast
{
	T constant;

public:
	Number_Ast(T number, Basic_Data_Type constant_data_type, int line);
	~Number_Ast();

	Basic_Data_Type get_basic_data_type();
	void set_data_type(Basic_Data_Type dt);
	bool is_value_zero();

	void print_ast(int space_count, ostream & file_buffer);

	TAC_For_Ast * gen_tac();
};

class String_Constant_Ast: public Ast
{
	int str_key;

public:
	String_Constant_Ast(int str_key, Basic_Data_Type constant_data_type, int line);
	~String_Constant_Ast();

	Basic_Data_Type get_basic_data_type();
	void set_data_type(Basic_Data_Type dt);
	bool is_null_string();

	void print_ast(int space_count, ostream & file_buffer);

	TAC_For_Ast * gen_tac();
};

class Arithmetic_Expr_Ast: public Ast
{
protected:
	Ast * lhs;
	Ast * rhs;

public:
	Arithmetic_Expr_Ast() {}
	~Arithmetic_Expr_Ast() {}

	Basic_Data_Type get_basic_data_type();
	void set_data_type(Basic_Data_Type dt);

	virtual void print_ast(int space_count, ostream & file_buffer) = 0;
	
	// compile
};

class Plus_Ast: public Arithmetic_Expr_Ast
{
public:
	Plus_Ast(Ast * l, Ast * r, int line);
	~Plus_Ast() {}

	void print_ast(int space_count, ostream & file_buffer);

	// compile
	TAC_For_Ast * gen_tac();
};

class Minus_Ast: public Arithmetic_Expr_Ast
{
public:
	Minus_Ast(Ast * l, Ast * r, int line);
	~Minus_Ast() {}

	void print_ast(int space_count, ostream & file_buffer);

	// compile
	TAC_For_Ast * gen_tac();
};

class Divide_Ast: public Arithmetic_Expr_Ast
{
public:
	Divide_Ast(Ast * l, Ast * r, int line);
	~Divide_Ast() {}

	void print_ast(int space_count, ostream & file_buffer);

	// compile
	TAC_For_Ast * gen_tac();
};

class Mult_Ast: public Arithmetic_Expr_Ast
{
public:
	Mult_Ast(Ast * l, Ast * r, int line);
	~Mult_Ast() {}

	void print_ast(int space_count, ostream & file_buffer);

	// compile
	TAC_For_Ast * gen_tac();
};

class UMinus_Ast: public Arithmetic_Expr_Ast
{
public:
	UMinus_Ast(Ast * l, Ast * r, int line);
	~UMinus_Ast() {}
	
	void print_ast(int space_count, ostream & file_buffer);

	// compile
	TAC_For_Ast * gen_tac();
};

class Conditional_Expression_Ast: public Arithmetic_Expr_Ast

{
protected:
	Ast* cond;
	Symbol_Table_Entry * result_temp_symbol;
public:
	Conditional_Expression_Ast(Ast* cond, Ast* l, Ast* r, Symbol_Table_Entry * se, int line);
	~Conditional_Expression_Ast();

	void print_ast(int space_count, ostream & file_buffer);
	TAC_For_Ast * gen_tac();
	Symbol_Table_Entry * get_result_sym_entry();

};


class Write_Ast: public Ast
{
     Ast *var;
public:
	Write_Ast(Ast *v, int line);
	~Write_Ast();

	void print_ast(int space_count, ostream & file_buffer);

	TAC_For_Ast * gen_tac();
};

class Read_Ast: public Ast
{
     Ast *var;
public:
	Read_Ast(Ast *v, int line);
	~Read_Ast();

	void print_ast(int space_count, ostream & file_buffer);

	TAC_For_Ast * gen_tac();
};


class Relational_Expr_Ast: public Ast
{
	Ast * lhs_condition;
	Ast * rhs_condition;
	Relational_Op rel_op;

public:
	Relational_Expr_Ast(Ast * lhs, Relational_Op rop, Ast * rhs, int line);
	~Relational_Expr_Ast();

	Basic_Data_Type get_basic_data_type();
	void set_data_type(Basic_Data_Type dt);


	TAC_For_Ast * gen_tac();
	TAC_Op get_tac_operator_for_rel_op_in_ast();

	void print_ast(int space_count, ostream & file_buffer);

private:
	string op_name();

};

class Logical_Expr_Ast: public Ast
{
	Ast * lhs_op;
	Ast * rhs_op;
	Logical_Op bool_op;

public:
	Logical_Expr_Ast(Ast * lhs, Logical_Op bop, Ast * rhs, int line);
	~Logical_Expr_Ast();

	Basic_Data_Type get_basic_data_type();
	void set_data_type(Basic_Data_Type dt);


	TAC_For_Ast * gen_tac();
	TAC_Op get_tac_operator_for_logical_op_in_ast();

	void print_ast(int space_count, ostream & file_buffer);

};

class Selection_Statement_Ast: public Ast {
protected:
	Ast* cond;
	Ast* then_part;
	Ast* else_part;
public:
	Selection_Statement_Ast(Ast * cond,Ast* then_part, Ast* else_part, int line);
	~Selection_Statement_Ast();

	Basic_Data_Type get_basic_data_type();
	void set_data_type(Basic_Data_Type dt);

	void type_check_ast();

	void print_ast(int space_count, ostream & file_buffer);
	TAC_For_Ast * gen_tac();

};

class Iteration_Statement_Ast: public Ast {
protected:
	Ast* cond;
	Ast* body;
	bool is_do_form;
public:
	Iteration_Statement_Ast(Ast * cond, Ast* body, int line, bool do_form);
	~Iteration_Statement_Ast();

	Basic_Data_Type get_basic_data_type();
	void set_data_type(Basic_Data_Type dt);

	void type_check_ast();

	void print_ast(int space_count, ostream & file_buffer);
	TAC_For_Ast * gen_tac();

};



class Sequence_Ast: public Ast{
	list<Ast *> seq_ast_statement_list;

public:
	Sequence_Ast(int line);
	~Sequence_Ast();
 	void ast_push_back(Ast * ast);
	void print_ast(int space_count, ostream & file_buffer);
	void print_assembly(ostream & file_buffer);
	TAC_For_Ast * gen_tac();
};

class Call_Ast: public Ast
{
	string procedure_name;
	list<Ast *> actual_param_list;

	Register_Descriptor * return_value_reg;

public:
	Call_Ast(string name, int line);
	~Call_Ast();

	Basic_Data_Type get_basic_data_type();
	void set_register(Register_Descriptor * reg);

	void type_check_actual_formal_param(Symbol_Table & formal_param_list);
	void set_actual_param_list(list<Ast *> & param_list);

	TAC_For_Ast * gen_tac();

	void print_ast(int space_count, ostream & file_buffer);
	
	// compile
};	

class Return_Ast: public Ast
{
	Ast * return_value;
	string proc_name;
public:
	Return_Ast(Ast * ret_val, string name, int line);
	~Return_Ast();

	Basic_Data_Type get_basic_data_type();
	TAC_For_Ast * gen_tac();

	void print_ast(int space_count, ostream & file_buffer);
};

class Pointer_Deref_Ast: public Ast
{
	Ast * opd;
public:
	Pointer_Deref_Ast(Ast * bname);
	~Pointer_Deref_Ast();

	Ast * get_opd_ast();

	Basic_Data_Type get_basic_data_type();
	int count_indirection_level();
	bool is_a_scalar();
	TAC_For_Ast * gen_tac();
	TAC_For_Ast * gen_tac_aggregate();
	Symbol_Table_Entry & get_symbol_entry();
	
	void print_ast(int space_count, ostream & file_buffer);
};

class Array_Access_Ast: public Ast
{
	Ast * base;
	Ast * offset;
public:
	Array_Access_Ast(Ast * bname, Ast * off);
	~Array_Access_Ast();

	Ast * get_base_ast();
	Ast * get_offset_ast();

	Basic_Data_Type get_basic_data_type();
	int count_indirection_level();
	TAC_For_Ast * gen_tac();
	TAC_For_Ast * gen_tac_aggregate();
	bool is_a_scalar();
	bool is_an_array();
	Symbol_Table_Entry & get_symbol_entry();
	
	void print_ast(int space_count, ostream & file_buffer);
};



class Address_Of_Ast: public Ast
{
	Ast * base;
public:
	Address_Of_Ast(Ast *b);
	~Address_Of_Ast();

	Ast * get_base_ast();

	Basic_Data_Type get_basic_data_type();
	int count_indirection_level();
	TAC_For_Ast * gen_tac();
	bool is_a_scalar();
	Symbol_Table_Entry & get_symbol_entry();
	
	void print_ast(int space_count, ostream & file_buffer);
};

class Field_Access_Ast: public Ast
{
	Ast * base;	// Here, base can be another Field_Access_Ast
	Ast * field;

	//Class_Type_Expr * cte;	

public:
	//Field_Access_Ast(Ast * bname, Ast * off, Class_Type_Expr & class_type);

	Field_Access_Ast(Ast *bname, Ast * off);
	
	~Field_Access_Ast();

	Ast * get_base_ast();
	Ast * get_field_ast();

	Basic_Data_Type get_basic_data_type();
	
	//string get_class_type();

	bool is_a_scalar();	
	TAC_For_Ast * gen_tac();	
	TAC_For_Ast * gen_tac_aggregate();	
	
	void print_ast(int space_count, ostream & file_buffer);
};


#endif
