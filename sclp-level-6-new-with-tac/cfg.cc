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

#include<string>
#include<fstream>
#include<iostream>
#include<map>

using namespace std;

#include "common-headers.hh"

CFG::CFG()
{
	entry_block = NULL;
	exit_block = NULL;
}

CFG::~CFG() { }

Basic_Block* CFG::get_entry_block()
{
	return entry_block;
}

void CFG::set_entry_basic_block(Basic_Block* entry)
{
	entry_block = entry;
}

Basic_Block* CFG::get_exit_block()
{
	return exit_block;
}

void CFG::set_exit_basic_block(Basic_Block* exit)
{
	exit_block = exit;
}

void CFG::add_mapping(int id, Basic_Block* bb)	
{
	id_to_block[id] = bb;
}

void CFG::set_bb_list(list<Basic_Block*> basic_blocks)
{
	bb_list = basic_blocks;
}

list<Basic_Block* > CFG::get_bb_list()
{
	return bb_list;
}

Basic_Block* CFG::get_bb_from_id(int bbNum)
{
	return id_to_block[bbNum];
}

void CFG::set_no_of_basic_blocks(int noOfBB)
{
	no_of_basic_blocks = noOfBB;
}

string CFG::is_edge_present(int from,int to)
{
	Basic_Block* bb = get_bb_from_id(from);
	list<pair<int,string>> succ_list =  bb->get_successors_list();
	for(auto succ : succ_list)
	{
		if(succ.first == to)
			return succ.second;
	}
	return "NO-EDGE";
}

list<TAC_Stmt*> CFG::generate_tac()
{
	list<Basic_Block*> bb_list = this->bb_list;
	for(auto bb = bb_list.begin();bb!=bb_list.end();bb++)
	{
		{
			list<TAC_Stmt*> tac_list = (*bb)->get_tac_stmts();
			for(auto tac = tac_list.begin() ; tac!= tac_list.end();tac++)
			{
				cfg_tac_list.push_back(*tac);
			}
		}
	}
	return cfg_tac_list;
}

bool CFG::empty()
{
	return bb_list.size() == 0;
}

void CFG::add_dfs_mapping(int bb_id,int dfs_number)
{
	dfs_number_to_bbid[dfs_number] = bb_id;
}

int CFG::get_bbid_from_dfs_num(int dfs_number)
{
	if(dfs_number_to_bbid.find(dfs_number)!= dfs_number_to_bbid.end())
		return dfs_number_to_bbid[dfs_number];
	return -1;
}

////////////////////////////////////////// Print functions for CFG //////////////////////////////////////

void TAC_Stmt::print_tac_stmt_in_cfg(ostream &file_buffer) {}

//////////////////////////////      Asgn_TAC_Stmt     ////////////////////////////////

void Asgn_TAC_Stmt::print_tac_stmt_in_cfg(ostream & file_buffer)
{
	print_tac_stmt(file_buffer);
}

//////////////////////////////      Compute_TAC_Stmt     ////////////////////////////////

void Compute_TAC_Stmt::print_tac_stmt_in_cfg(ostream & file_buffer)
{
	print_tac_stmt(file_buffer);
}

/////////////////////////////////   If_Goto_TAC_Stmt   //////////////////////////////////////

void If_Goto_TAC_Stmt::print_tac_stmt_in_cfg(ostream & file_buffer)
{
	file_buffer << "\t" << "if(" ;
	opd1->print_opd(file_buffer);
	file_buffer << ")" ;
	file_buffer << " goto ";
	result->print_label_in_cfg(file_buffer);
}

/////////////////////////////////   Goto_TAC_Stmt   //////////////////////////////////////

void Goto_TAC_Stmt::print_tac_stmt_in_cfg(ostream & file_buffer)
{
	file_buffer << "\t" << "goto " ;
	result->print_label_in_cfg(file_buffer);
}

/////////////////////////////////   Label_TAC_Stmt   //////////////////////////////////////

void Label_TAC_Stmt::print_tac_stmt_in_cfg(ostream & file_buffer)
{
}

/////////////////////////////////   Label_TAC_Opd   //////////////////////////////////////

void Label_TAC_Opd::print_label_in_cfg(ostream & file_buffer)
{
	file_buffer << "<bb" << get_label() << ">" << endl;
}

void TAC_Opd::print_label_in_cfg(ostream & file_buffer)
{
	stringstream msg;
	msg << "No print_label_in_cfg() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

///////////////////////////////                  I/O TAC Statements            /////////////////////////////////

void IO_TAC_Stmt::print_tac_stmt_in_cfg(ostream & file_buffer) 
{
	print_tac_stmt(file_buffer);
}

///////////////////////////////                  Return TAC Statements            /////////////////////////////////

void Return_TAC_Stmt::print_tac_stmt_in_cfg(ostream & file_buffer) 
{
	print_tac_stmt(file_buffer);
}

///////////////////////////////                  Call TAC Statements            /////////////////////////////////

void Call_TAC_Stmt::print_tac_stmt_in_cfg(ostream & file_buffer) 
{
	print_tac_stmt(file_buffer);
}

/////////////////////////////        TAC_Stmt Default Virtual Function Members       ///////////////////////////////

void NOP_TAC_Stmt::print_tac_stmt_in_cfg(ostream & file_buffer)
{
	file_buffer << "\tNOP\n" ;
}
