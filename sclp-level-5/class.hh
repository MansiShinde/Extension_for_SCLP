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

#ifndef CLASS_HH
#define CLASS_HH

#include<string>
#include<map>
#include<list>
#include"procedure.hh"

#define LOC_VAR_SPACE "      "

using namespace std;

class Class;

class Class
{
	string name;

	Symbol_Table global_class_symbol_table;

	Symbol_Table local_class_symbol_table;

	map<string, Procedure *> procedure_map;

	int lineno;

	list<Ast *> ast_list;

	//bool return_ast_exists;

	bool class_is_defined;

public:
	Class(string class_name, int line);
	~Class();

	//void operator=(Class & class_proto); //added now
	
	bool is_class_defined();
	void set_class_is_defined();

	string get_class_name();
	void set_local_class_list(Symbol_Table & new_list);
	void set_global_class_list(Symbol_Table & new_list);
	Symbol_Table get_local_class_list();
	Symbol_Table get_global_class_list();
	
	void set_ast_list(list<Ast *> & ast_list);

	Symbol_Table_Entry & get_local_class_symbol_table_entry(string variable_name);
	Symbol_Table_Entry & get_global_class_symbol_table_entry(string variable_name);

	bool variable_in_global_class_list_check(string variable);	
	bool variable_in_local_symbol_list_check(string variable);

	int get_class_var_start_offset(string var_name);

	void assign_offset_to_class_var(Symbol_Table & list); //to compile

	void print_sym(ostream & file_buffer);  //print symbol table for class


private:
	void print_prologue(ostream & file_buffer);
	void print_epilogue(ostream & file_buffer);
};

#endif

