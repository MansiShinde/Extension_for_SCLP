
/*********************************************************************************************

                                sclp : A C Language Processor
                                --------------------------------

           About:

           Originally implemented in 2012 by Tanu Kanvar and Uday Khedker 
           (http://www.cse.iitb.ac.in/~uday) for the courses cs302+cs316
           Language Processors (theory and lab) at IIT Bombay.

           Initial release date Jan 15, 2013.

           Updated by N. Venkatesh in Jan 2019.

           Updated by Uday Khedker with the help of Nisha Biju and Saari
           Rajan in Jan 2020.

           Copyrights reserved  by Uday Khedker. This  implemenation has
           been made available purely  for academic purposes without any
           warranty of any kind.


***********************************************************************************************/

#ifndef BASIC_BLOCK_HH
#define BASIC_BLOCK_HH

#include<string>
#include<map>
#include<list>

#define BB_SPACE "      "

using namespace std;

class Basic_Block;

class Basic_Block
{
	int id_number;
	list<Ast *> bb_ast_list;

	list<Icode_Stmt *> bb_icode_list;

	int lineno;

public:
	Basic_Block(int basic_block_number, int line);
	~Basic_Block();

	int get_bb_number();
	void set_ast_list(list<Ast *> & ast_list);

	void print_bb(ostream & file_buffer);

	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);

	// compile
	void gen_icode();
	void print_assembly(ostream & file_buffer);
	void print_icode(ostream & file_buffer);
};

#endif
