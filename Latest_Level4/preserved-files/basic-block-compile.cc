
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
#include"icode.hh"
#include"reg-alloc.hh"
#include"symbol-table.hh"
#include"ast.hh"
#include"basic-block.hh"

void Basic_Block::gen_icode()
{
	Code_For_Ast ast_code;

	machine_desc_object.validate_init_local_register_mapping();

	// compile the program by visiting each ast in the block
	list<Ast *>::iterator i;
	for (i = bb_ast_list.begin(); i != bb_ast_list.end(); i++)
	{
		Ast * ast = *i;

		if (typeid(*ast) != typeid(Return_Ast))
		{
			ast_code = ast->gen_icode();

			list<Icode_Stmt *> & ast_icode_list = ast_code.get_icode_list();
			if (ast_icode_list.empty() == false)
			{
				if (bb_icode_list.empty())
					bb_icode_list = ast_icode_list;
				else
					bb_icode_list.splice(bb_icode_list.end(), ast_icode_list);
			}
		}
	}
}

void Basic_Block::print_assembly(ostream & file_buffer)
{
	list<Icode_Stmt *>::iterator i;
	for (i = bb_icode_list.begin(); i != bb_icode_list.end(); i++)
		(*i)->print_assembly(file_buffer);
}

void Basic_Block::print_icode(ostream & file_buffer)
{
	list<Icode_Stmt *>::iterator i;
	for (i = bb_icode_list.begin(); i != bb_icode_list.end(); i++)
		(*i)->print_icode(file_buffer);
}
