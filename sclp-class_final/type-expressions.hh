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

#include "common-classes.hh"

class Type_Expr 
{
public:
	Type_Expr();
	~Type_Expr();
	virtual void print_type_expression(int sc, ostream & file_buffer, Basic_Data_Type bt) = 0;
	virtual int get_dim_count ();
	virtual int get_total_size();
	virtual bool is_array_type();
	virtual bool is_pointer_type();
	virtual bool is_dummy_leaf();
	virtual int get_cumulative_size_of_array_dim_from_end(int d);
	virtual int compute_cumulative_size_of_remaining_dims();
	virtual int get_cumulative_size_of_remaining_dims();
	virtual int compute_total_size();
	virtual int count_indirection_levels();
	virtual void assign_dimension_number(int d);
	bool match_type(Type_Expr * t2);
	virtual Type_Expr * get_child();
};

class Pointer_Type_Expr: public Type_Expr
{
	Type_Expr * opd;
public:
	Pointer_Type_Expr(Type_Expr * opd);
	~Pointer_Type_Expr();
	void print_type_expression(int sc, ostream & file_buffer, Basic_Data_Type bt);
	//Type_Expr * get_type_node_at_depth(int d);
	bool is_pointer_type();
	int count_indirection_levels();
	Type_Expr * get_child();
};

class Array_Type_Expr: public Type_Expr
{
	Type_Expr * opd;

	int dim_size;
	int cumulative_size_of_remaining_dims;
	int total_size;
	int dim_count;
	int dimension_number;
public:
	Array_Type_Expr(int size, Type_Expr * opd, int d);
	~Array_Type_Expr();
	void print_type_expression(int sc, ostream & file_buffer, Basic_Data_Type bt);
	int get_dim_count();
	int compute_cumulative_size_of_remaining_dims();
	int compute_total_size();
	int get_cumulative_size_of_remaining_dims();
	int get_total_size();
	//Type_Expr * get_type_node_at_depth(int d);
	bool is_array_type();
	int get_cumulative_size_of_array_dim_from_end(int d);
	void assign_dimension_number(int d);
	Type_Expr * get_child();
};

class Dummy_Type_Expr_Leaf: public Type_Expr
{
public:
	Dummy_Type_Expr_Leaf();
	~Dummy_Type_Expr_Leaf();
	void print_type_expression(int sc, ostream & file_buffer, Basic_Data_Type bt);
	bool is_dummy_leaf();
};

class Decl_Type_Info
{
	Basic_Data_Type bdt;
	Type_Expr * ddt;
	int size;
	string name;
	int lineno;
	int max_depth;

public:
	Decl_Type_Info(Basic_Data_Type bt, Type_Expr * dt, int s, int n);
	~Decl_Type_Info();
	Basic_Data_Type get_base_data_type();
	void set_base_data_type(Basic_Data_Type dt);
	Type_Expr * get_derived_type();
	void set_derived_type(Type_Expr * te);
	int get_size();
	void set_size(int s);
	string get_name();
	void set_name(string s);
	int get_line_number();
	void set_line_number(int n);
	int get_max_depth();

	void print_decl_type_info(int sc, ostream & file_buffer);
	bool is_derived_type();
}; 

class Access_Type_Info
{
	Decl_Type_Info * declared_type;
	Ast * base;
	int curr_depth;
	int max_depth;
	Ast * deref_ast;

public:
	Access_Type_Info (Ast * base, Decl_Type_Info * dti, Ast * d_ast, int c_depth);
	~Access_Type_Info();

	void set_curr_depth(int d);
	int get_curr_depth();
	int get_max_depth();
	Decl_Type_Info * get_declared_type();
	int get_cumulative_size_of_array_dim_from_end(int d);
	Ast * get_name_ast();
	Ast * get_deref_ast();
	void set_deref_ast(Ast * ast);
	void check_depth_consistency();
	bool match_type(Access_Type_Info * ati);
};


class Class_Type_Expr 
{
	string class_name;
	Symbol_Table global_class_symbol_table;
	Symbol_Table local_class_symbol_table;
	bool class_is_defined;
	int lineno;

public:

	Class_Type_Expr(string name, int line);
	~Class_Type_Expr();

	string get_class_name();

	bool is_class_defined();
	void set_class_is_defined();

	void set_local_class_list(Symbol_Table & new_list);
	void set_global_class_list(Symbol_Table & new_list);

	Symbol_Table get_local_class_list();
	Symbol_Table get_global_class_list();
	
	Symbol_Table_Entry & get_local_class_symbol_table_entry(string variable_name);
	Symbol_Table_Entry & get_global_class_symbol_table_entry(string variable_name);

	bool variable_in_global_class_symbol_list_check(string variable);	
	bool variable_in_local_class_symbol_list_check(string variable);
	
	//Symbol table 
	void assign_offset_class_var();
	int get_global_class_symbol_table_size();
	int get_local_class_symbol_table_size();
	void print_sym(ostream & file_buffer);
};

