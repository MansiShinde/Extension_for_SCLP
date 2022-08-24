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

#ifndef BASIC_BLOCK_HH
#define BASIC_BLOCK_HH

#include<string>
#include<map>
#include<list>
#include<utility>

#define BB_SPACE "      "

using namespace std;

class Basic_Block;

class Basic_Block
{
	static int block_cnt;

	int id_number;	
	int dfs_prenumber;
	int dfs_postnumber;
	int lnum;
	list<TAC_Stmt *> bb_tac_list;
	list<int > predecessors;
	list<pair<int,string >> successors;

	list<int> children;  // The list of basic blocks that it is dominating

	int lineno;

public:
	Basic_Block();
	Basic_Block(list<TAC_Stmt*> bb_list,int lnum);
	~Basic_Block();

	int get_bb_id();
	void set_bb_id(int id);

	void add_predecessor(Basic_Block * predecessor);
	void add_successor(Basic_Block * successor,string type_of_edge);

	void set_dfs_pre_number(int pre_no);
	void set_dfs_post_number(int post_no);
	list<int> get_predecessors_list();
	list<pair<int,string>> get_successors_list();

	void add_instruction(TAC_Stmt* stmt);
	void print_bb(ostream & file_buffer);
	list<TAC_Stmt *> get_tac_stmts();
	void add_child(Basic_Block* child);
	list<int> get_children_list();
	bool has_label();
	int get_label_number();
	bool has_single_label();

};

#endif
