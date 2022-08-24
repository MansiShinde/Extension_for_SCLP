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
#include<stack>


#define LOC_VAR_SPACE "      "

using namespace std;

class Procedure;
class Class_Type_Expr;

class Procedure
{
	Basic_Data_Type return_type;

	Register_Descriptor * return_register;

	string name;

	bool proc_mangled; //for class function

	Class_Type_Expr * class_prototype; //for class functions. class function can access the whole of class

	list<Ast *> ast_list;

	list <TAC_Stmt*> *tac_stmt_list;

	list <RTL_Stmt*> *rtl_stmt_list;

	Symbol_Table local_symbol_table;

	Symbol_Table formal_param_table;
	Symbol_Table temporary_variable_table;//Rnd Work

// ------- changed on 25-02-2020 ------
	CFG * cfg;
	LV_DFV lv_dfv_obj;//Rnd Work
	AV_DFV av_dfv_obj;

	int lineno;

	bool proc_is_called;
	bool proc_is_defined;
	bool proc_is_declared;
	bool return_ast_exists;

	TAC_Opd * return_label;
	TAC_Opd * return_temp;

public:
	Procedure(Basic_Data_Type proc_return_type, string proc_name, int line);
	~Procedure();

	//void operator==(Procedure & proc_proto);
	void operator=(Procedure & proc_proto);

	void assign_class_type_to_function(Class_Type_Expr * ci); //used for class
	Class_Type_Expr * get_class_type_info(); //used for class 

	void set_proc_is_mangled(); //for function declaration and definition
	bool is_proc_mangled(); //for class function

	bool is_proc_called();
	void set_proc_is_called();
	bool is_proc_defined();
	void set_proc_is_defined();
	bool is_proc_declared();
	void set_proc_is_declared();

	string get_proc_name();
	void set_local_list(Symbol_Table & new_list);
	Symbol_Table get_local_list();
	Symbol_Table_Entry * append_new_local_name(string name, Basic_Data_Type dt, int line);

	void set_ast_list(list<Ast *> & ast_list);
	
	void set_formal_param_list(Symbol_Table & new_list);
	Symbol_Table & get_formal_param_list();
	Basic_Data_Type get_return_type();
	void set_return_type(Basic_Data_Type ret_type);
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


//-------------------------Changed on 25-02-2020-------------------------
	void gen_cfg();
	void perform_lva();//Rnd Work
	void perform_ava();
	void populate_symbol_table_temporary();
	void post_order_traversal(list<Basic_Block *>);
	void reverse_post_order_traversal(list<Basic_Block *>);



	//void print_icode(ostream & file_buffer);
	void print_tac(ostream & file_buffer);
	void print_rtl(ostream & file_buffer);
	void print_assembly(ostream & file_buffer);


//-------------------------Changed on 25-02-2020-------------------------
	void print_cfg(ostream & file_buffer);
	void print_lv(ostream &file_buffer);//Rnd Work
	void print_av(ostream &file_buffer);
	void print_cfg_dot_graph(ostream & graph_file_buffer);


	Register_Descriptor * get_return_register();
	void set_return_register();

	TAC_Opd * get_return_label();
	void set_return_label(TAC_Opd * return_label);

	TAC_Opd * get_return_temp();
	void set_return_temp(TAC_Opd * return_temp);
private:



//-------------------------Changed on 25-02-2020-------------------------
	list<TAC_Stmt *> identify_leader_stmts();
	void gen_bb(list<TAC_Stmt *> leader_statements);
	void connect_bb();


	Basic_Block* create_bb(TAC_Stmt* current_leader_stmt,TAC_Stmt* next_leader_stmt,int label );
	list<int> get_label_list(list<TAC_Stmt *> leader_stmts,list<TAC_Stmt *> :: iterator leader_stmt);

	void validate_cfg();
	bool validate_entry_exit();

	bool reachable_from_entry_block();
	bool exit_block_reachable();
	bool validate_pred_succ();
	map<int,int> get_label_mapping();
	int get_bb(int label_num, map<int,int> label_map);
	void empty_list(list<TAC_Stmt *>& bb_tac_list);
	void dfs_numbering();
	void remove_empty_bb(map<int,Basic_Block*>& lnum_to_bbid);

	void print_prologue(ostream & file_buffer);
	void print_epilogue(ostream & file_buffer);
};

#endif
