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

#ifndef SYMBOL_TABLE_HH
#define SYMBOL_TABLE_HH

#include<string>
#include<list>

using namespace std;

class Symbol_Table;
class Symbol_Table_Entry;
class Decl_Type_Info;
class Class_Type_Expr;

typedef enum
{
	void_data_type,
	int_data_type,
	double_data_type,
	bool_data_type,
	string_data_type,

	class_data_type,

	
} Basic_Data_Type;


typedef enum //change by mansi
{
        private_access_specifier,
	public_access_specifier
}Access_Specifier;


typedef enum
{
	global,
	local,
	formal
} Table_Scope;

typedef enum
{
	less_equalto,
	less_than,
	greater_than,
	greater_equalto,
	equalto,
	not_equalto
} Relational_Op;


typedef enum
{
	not_op,
	or_op,
	and_op
} Logical_Op;



typedef enum
{
	fp_ref,
	sp_ref
} Offset_With_Ref_To;

class Symbol_Table
{
	list<Symbol_Table_Entry *> variable_table;
	Table_Scope scope;

	// compile
	int size_in_bytes;		// size of list
	int start_offset_of_first_symbol;

public:
	Symbol_Table();
	~Symbol_Table();

	bool is_empty();

	//void operator==(Symbol_Table & list);


	Table_Scope get_table_scope();
	void set_table_scope(Table_Scope list_scope);

	void print(ostream & file_buffer);

	void push_symbol(Symbol_Table_Entry * variable, int lineno);

	bool variable_in_symbol_list_check(string variable);

	bool var_in_loc_glo_class_sym_list_check(Symbol_Table  list); // check variable in local and global class symbol table

	Symbol_Table_Entry & get_symbol_table_entry(string variable_name);
	void global_list_in_proc_check();

	void append_list(Symbol_Table sym_t,int lineno);

	void create(Local_Environment & local_global_variables_table);
        list<Symbol_Table_Entry *> & get_table();

	bool variable_in_formal_list_check(string variable);
	string get_variable_in_formal_list(int position);
	Symbol_Table_Entry & get_symbol_table_entry_by_index(int position);

	void global_list_in_proc_map_check();

	// compile
private:
	int get_size_of_value_type(Basic_Data_Type dt);

public:
	void set_start_offset_of_first_symbol(int n);
	int get_start_offset_of_first_symbol();

	void assign_offsets();
	int get_size_in_bytes();
	int get_number_of_symbols();
	void set_type_of_all_syms(Basic_Data_Type dt);    /* We don't expect the types of all syms to be same.
                                                       This function is used to set type of all variables 
                                                       in a single declaration statement. See the RHS of 
						       the non-terminal var_decl in parser.y.
                                                    */    

	void print_assembly(ostream & file_buffer);
};

class Symbol_Table_Entry
{
	string variable_name;
	Basic_Data_Type variable_data_type;
	Table_Scope scope;
	Decl_Type_Info *decl_type_info;

	Class_Type_Expr * class_prototype; //for class
	int class_object_size;

	int lineno;

	// compile
	int start_offset;
	int end_offset;
	Register_Descriptor * register_description;
     	Offset_With_Ref_To ref_off;

public:
	Symbol_Table_Entry();
	Symbol_Table_Entry(string & name, Basic_Data_Type new_data_type, int line, Offset_With_Ref_To ro = fp_ref);

	/* 
		Local variables are accesed with respect to fp whereas formal parameters are accessed 
        	with respect to fp. There is only one call to the constructor (in ast-compile.cc) 
        	that uses sp_ref. The default value is fp_ref.
	*/

	Symbol_Table_Entry(string & name, int line, Offset_With_Ref_To ro = fp_ref);

	/*
		Constructor without the type name needed during parsing because the type name
        	precedes the list of variables.
	*/

	~Symbol_Table_Entry();

	Offset_With_Ref_To get_ref_offset();

	int get_lineno();

	bool operator==(Symbol_Table_Entry & entry);

	void set_symbol_scope(Table_Scope sp);
	Table_Scope get_symbol_scope();

	Basic_Data_Type get_basic_data_type();
	void set_data_type(Basic_Data_Type dt);
	string get_variable_name();
	void set_decl_type_info(Decl_Type_Info * ti);
	Decl_Type_Info * get_decl_type_info();
	bool is_a_scalar();

	void assign_class_type_to_object(Class_Type_Expr * ci); //used for class
	Class_Type_Expr * get_class_type_info(); //used for class 
	void set_class_obj_size(); //calculate size of class object
	int get_class_obj_size();

	//compile
	void set_start_offset(int num);
	int get_start_offset();
	void set_end_offset(int num);
	int get_end_offset();

	Register_Descriptor * get_register();
	void set_register(Register_Descriptor * reg);
	void free_register(Register_Descriptor * destination_reg_descr);
	void update_register(Register_Descriptor * result_reg_descr);
};

string printable_data_type(Basic_Data_Type dt);

int number_of_bytes_for_value_type(Basic_Data_Type dt);
#endif
