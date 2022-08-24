
/*********************************************************************************************

                                sclp : A C Language Processor
                                --------------------------------

           About:

           Implemented   by  Tanu  Kanvar (tanu@cse.iitb.ac.in) and Uday
           Khedker    (http://www.cse.iitb.ac.in/~uday)  for the courses
           cs302+cs316: Language  Processors  (theory and  lab)  at  IIT
           Bombay.

           Initial release  date  Jan  15, 2013.  

           Copyrights reserved  by Uday Khedker. This  implemenation has
           been made available purely  for academic purposes without any
           warranty of any kind.


***********************************************************************************************/

#ifndef SYMBOL_TABLE_HH
#define SYMBOL_TABLE_HH

#include<string>
#include<list>

using namespace std;

class Symbol_Table;
class Symbol_Table_Entry;

typedef enum
{
	void_data_type,
	int_data_type,
	double_data_type,
} Data_Type;

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
	_logical_not,
	_logical_or,
	_logical_and
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

	void operator==(Symbol_Table & list);


	Table_Scope get_table_scope();
	void set_table_scope(Table_Scope list_scope);

	void print(ostream & file_buffer);

	void push_symbol(Symbol_Table_Entry * variable);

	bool variable_in_symbol_list_check(string variable);
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
	int get_size_of_value_type(Data_Type dt);

public:
	void set_start_offset_of_first_symbol(int n);
	int get_start_offset_of_first_symbol();

	void assign_offsets();
	int get_size();

	void print_assembly(ostream & file_buffer);
};

class Symbol_Table_Entry
{
	string variable_name;
	Data_Type variable_data_type;
	Table_Scope scope;

	int lineno;

	// compile
	int start_offset;
	int end_offset;
	Register_Descriptor * register_description;
    Offset_With_Ref_To ref_off;

public:
	Symbol_Table_Entry();
	Symbol_Table_Entry(string & name, Data_Type new_data_type, int line, Offset_With_Ref_To ro = fp_ref);
	~Symbol_Table_Entry();

	Offset_With_Ref_To get_ref_offset();

	int get_lineno();

	bool operator==(Symbol_Table_Entry & entry);

	void set_symbol_scope(Table_Scope sp);
	Table_Scope get_symbol_scope();

	Data_Type get_data_type();
	void set_data_type(Data_Type dt);
	string get_variable_name();

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

#endif
