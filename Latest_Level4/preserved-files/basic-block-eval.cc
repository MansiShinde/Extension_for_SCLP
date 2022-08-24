
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

Eval_Result & Basic_Block::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	Eval_Result * result = NULL;

	file_buffer << "\n" << BB_SPACE << "Basic Block: " << id_number << "\n";

	list <Ast *>::iterator i;
	for (i = bb_ast_list.begin(); i != bb_ast_list.end(); i++)
	{
		CHECK_INVARIANT(((*i) != NULL), "Ast pointer seems to be NULL into the basic block");
		result = &((*i)->evaluate(eval_env, file_buffer)); 
	}

	return *result;
}
