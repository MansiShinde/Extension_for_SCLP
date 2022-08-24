
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

#include<string>
#include<fstream>

using namespace std;

#include"common-classes.hh"
#include"local-environment.hh"
#include"error-display.hh"
#include"user-options.hh"
#include"symbol-table.hh"
#include"ast.hh"
#include"basic-block.hh"

Basic_Block::Basic_Block(int basic_block_number, int line)
{
	id_number = basic_block_number;

	lineno = line;
}

Basic_Block::~Basic_Block()
{
	list<Ast *>::iterator i;
	for (i = bb_ast_list.begin(); i != bb_ast_list.end(); i++)
		delete (*i);
}

void Basic_Block::set_ast_list(list<Ast *> & ast_list)
{
	bb_ast_list = ast_list;
}
int Basic_Block::get_bb_number()
{
	return id_number;
}

void Basic_Block::print_bb(ostream & file_buffer)
{
	file_buffer << BB_SPACE << "Basic_Block " << id_number << "\n";

	list<Ast *>::iterator i;
	for(i = bb_ast_list.begin(); i != bb_ast_list.end(); i++)
		(*i)->print(file_buffer);
}