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

extern Program program_object;

class Program
{
	Symbol_Table global_symbol_table;
	map<string, Procedure *> procedure_map;
	map<int, string> string_table;
	map<string, int> reverse_map_of_string_table;
	int str_count;

public:
	Program();
	~Program();
	void delete_all();
	Procedure * get_procedure_prototype(string proc_name);
	void set_proc_to_map(string proc_name, Procedure * proc);

	void called_proc_are_defined_check();

	void set_procedure_map(map<string, Procedure *> & proc);

	void set_procedure(Procedure * proc, int line);
	void set_global_table(Symbol_Table & new_global_table);

	Symbol_Table_Entry & get_symbol_table_entry(string variable);

	int add_string_to_string_table(string s);
	string get_string(int str_key);

	void print_sym();
	void print_ast();


	Procedure * get_main_procedure(ostream & file_buffer);

	bool variable_proc_name_check(string symbol);
	bool variable_in_symbol_list_check(string variable);
	void global_list_in_proc_check();

    	bool variable_in_proc_map_check(string symbol);
	void global_list_in_proc_map_check();

    	bool proc_exists(string proc_name);

	// compile
	void gen_tac();
	void print_tac();
	void gen_rtl();
	void print_rtl();
	void print_assembly();
	void print_string_table(ostream & file_buffer);
};

#endif
