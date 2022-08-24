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

#include<iostream>
#include<fstream>
#include<typeinfo>

using namespace std;

#include"common-classes.hh"
#include"error-display.hh"
#include"user-options.hh"
#include"symbol-table.hh"
#include"tac.hh"
#include"ast.hh"
#include"rtl.hh"
#include"procedure.hh"
#include"program.hh"

void Ast::type_check_ast()
{
	stringstream msg;
	msg << "No type_check_ast() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void Assignment_Ast::type_check_ast()
{
	CHECK_INVARIANT((rhs != NULL), "Rhs of Assignment_Ast cannot be null");
	CHECK_INVARIANT((lhs != NULL), "Lhs of Assignment_Ast cannot be null");

	CHECK_INPUT((lhs->get_data_type() == rhs->get_data_type()),
		"Assignment statement data type not compatible", lineno);
}


void Selection_Statement_Ast::type_check_ast()
{
	CHECK_INPUT(CONTROL_SHOULD_NOT_REACH, "No type check for Selection Statement Ast", lineno);
}

void Iteration_Statement_Ast::type_check_ast()
{
	CHECK_INPUT(CONTROL_SHOULD_NOT_REACH, "No type check for Iteration Ast", lineno);
}

void Call_Ast::type_check_actual_formal_param(Symbol_Table & formal_list)
{
	list<Symbol_Table_Entry *> & formal_param_list = formal_list.get_table();

	list<Ast *>::iterator i;
	list<Symbol_Table_Entry *>::iterator j;

	for (i = actual_param_list.begin(), j = formal_param_list.begin(); i != actual_param_list.end() && j != formal_param_list.end(); i++, j++)
	{
		Data_Type type1 = (*i)->get_data_type();
		Data_Type type2 = (*j)->get_data_type();

		CHECK_INPUT((type1 == type2), "Actual and formal parameters data types are not matching", lineno);
	}

	CHECK_INPUT((j == formal_param_list.end() && i == actual_param_list.end()),
			"Actual and formal parameter counts do not match", lineno);
}

