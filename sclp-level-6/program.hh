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

#ifndef PROGRAM_HH
#define PROGRAM_HH

#include<string>

#define GLOB_SPACE "   "


using namespace std;

class Program;
class Class_Type_Expr;

extern Program program_object;

class Program
{
	Symbol_Table global_symbol_table;
	map<string, Procedure *> procedure_map;

	map<string, Class_Type_Expr *> class_map; //mansi

	map<int, string> string_table;
	map<string, int> reverse_map_of_string_table;
	int str_count;

public:
	Program();
	~Program();
	void delete_all();
	Procedure * get_procedure_entry(string proc_name);

	Class_Type_Expr * get_class_entry(string class_name); //mansi

	void set_class_to_map(string class_name, Class_Type_Expr * class_nm); //mansi

	void set_proc_to_map(string proc_name, Procedure * proc);

	void called_proc_are_defined_check();
	int count_of_procedures();

	void set_procedure_map(map<string, Procedure *> & proc);

	void set_class_map(map<string, Class_Type_Expr *> & class_nm); //  mansi

	void set_procedure(Procedure * proc, int line);

	void set_class(Class_Type_Expr * class_nm, int line); // mansi

	void set_global_table(Symbol_Table & new_global_table);

	Symbol_Table_Entry & get_symbol_table_entry(string variable);

	int add_string_to_string_table(string s);	// A separate table to record the strings 
	string get_string(int str_key);                 // occurring in the input program.

	void print_sym();
	void print_ast();


	Procedure * get_main_procedure();

	bool class_name_exists(string name); // mansi

	bool class_exists(string class_name); // mansi

	bool proc_name_exists(string name);
	bool is_proc_declared(string name);
	bool is_proc_defined(string name);
	bool variable_in_symbol_list_check(string variable);
	void global_list_in_proc_check();

	void global_list_in_proc_map_check();

    	bool proc_exists(string proc_name);

	// compile
	void gen_tac();
	void print_tac();

//-------------------------Changed on 25-02-2020-------------------------
	void gen_cfg();
	void print_cfg();
	void draw_cfg();
//------------------------- till here----------------------------
	void perform_lva();//RnD Work 
	void print_lv();
	void perform_ava();
	void print_av();
	void gen_rtl();
	void print_rtl();
	void print_assembly();
	void print_string_table(ostream & file_buffer);
};

#endif
