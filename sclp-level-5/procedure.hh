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

#ifndef PROCEDURE_HH
#define PROCEDURE_HH

#include<string>
#include<map>
#include<list>

#define LOC_VAR_SPACE "      "

using namespace std;

class Procedure;

class Procedure
{
	Data_Type return_type;

	Register_Descriptor * return_register;

	string name;

	list<Ast *> ast_list;

	list <TAC_Stmt*> *tac_stmt_list;

	list <RTL_Stmt*> *rtl_stmt_list;

	Symbol_Table local_symbol_table;

	Symbol_Table formal_param_table;

	int lineno;

	bool proc_is_called;
	bool proc_is_defined;
	bool return_ast_exists;

	TAC_Opd * return_label;
	TAC_Opd * return_temp;

public:
	Procedure(Data_Type proc_return_type, string proc_name, int line);
	~Procedure();

	//void operator==(Procedure & proc_proto);
	void operator=(Procedure & proc_proto);

	bool is_proc_called();
	void set_proc_is_called();
	bool is_proc_defined();
	void set_proc_is_defined();

	string get_proc_name();
	void set_local_list(Symbol_Table & new_list);
	Symbol_Table get_local_list();

	void set_ast_list(list<Ast *> & ast_list);
	
	void set_formal_param_list(Symbol_Table & new_list);
	Symbol_Table & get_formal_param_list();
	Data_Type get_return_type();
	void set_return_type(Data_Type ret_type);
	Symbol_Table_Entry & get_symbol_table_entry(string variable_name);
	Symbol_Table_Entry & get_formal_param_entry(string variable_name);

	void print_ast(ostream & file_buffer);
	void print_sym(ostream & file_buffer);

	bool variable_in_symbol_list_check(string variable);
	bool variable_in_formal_list_check(string variable);
	bool contains_return_ast();
	void set_found_return_ast();

	// compile
	string get_variable_in_formal_list(int position);

	//void gen_icode();
	void gen_tac();
	void gen_rtl();
	//void print_icode(ostream & file_buffer);
	void print_tac(ostream & file_buffer);
	void print_rtl(ostream & file_buffer);
	void print_assembly(ostream & file_buffer);

	Register_Descriptor * get_return_register();
	void set_return_register();

	TAC_Opd * get_return_label();
	void set_return_label(TAC_Opd * return_label);

	TAC_Opd * get_return_temp();
	void set_return_temp(TAC_Opd * return_temp);
private:
	void print_prologue(ostream & file_buffer);
	void print_epilogue(ostream & file_buffer);
};

#endif
