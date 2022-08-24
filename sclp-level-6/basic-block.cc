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
#include<iterator>

using namespace std;

#include "common-headers.hh"


int Basic_Block::block_cnt = 1;

Basic_Block::Basic_Block() { }

Basic_Block::Basic_Block(list<TAC_Stmt*> bb_list,int label_num)
{
	bb_tac_list = bb_list;
	id_number = block_cnt++;//Rnd Work
	lnum = label_num;
}

Basic_Block::~Basic_Block()
{
	list<TAC_Stmt *>::iterator bb;
	for (bb = bb_tac_list.begin(); bb != bb_tac_list.end(); bb++)
		delete (*bb);
}

void Basic_Block::add_predecessor(Basic_Block* predecessor)
{
	predecessors.push_back(predecessor->get_bb_id());
}

list<TAC_Stmt *> Basic_Block::get_tac_stmts()
{
	return bb_tac_list;
}

int Basic_Block::get_bb_id()
{
	return id_number;
}

void Basic_Block::add_successor(Basic_Block* successor,string type_of_edge)
{
	successors.push_back(make_pair(successor->get_bb_id(),type_of_edge));
}

void Basic_Block::print_bb(ostream& file_buffer)
{
	file_buffer << "\n<bb" << id_number << ">\n";

	file_buffer << "    Preds: ";
	if(predecessors.size() == 0)
		file_buffer << "NONE ";
	else
	{
		int count=0;
		for(auto pred = predecessors.begin();pred!=predecessors.end();pred++)
		{
			if (count++ > 0)
				file_buffer << ", ";
			file_buffer << "<bb" << (*pred) << ">";
		}
	}
	file_buffer << "\n";
	for(auto i = bb_tac_list.begin() ; i!=bb_tac_list.end();i++){
		(*i)->print_tac_stmt_in_cfg(file_buffer);
	}
	
	file_buffer <<"    Succs: ";
	if(successors.size() == 0)
		file_buffer << "NONE ";
	else
	{
		int count=0;
		for(auto succ = successors.begin();succ!=successors.end();succ++)
		{
			if (count++ > 0)
				file_buffer << ", ";
			file_buffer << "<bb" << (*succ).first << ">(" << (*succ).second <<")";
		}
	}
	file_buffer << "\n" ;
}


list<int> Basic_Block::get_predecessors_list()
{
	return predecessors;
}

list<pair<int,string>> Basic_Block::get_successors_list()
{
	return successors;
}

void Basic_Block ::set_dfs_pre_number(int pre_no)
{
	dfs_prenumber = pre_no;
}

void Basic_Block ::set_dfs_post_number(int post_no)
{
	dfs_postnumber = post_no;
}

bool Basic_Block::has_label()
{
	if(bb_tac_list.size() != 0 && (*bb_tac_list.begin())->is_label())
		return true;
	return false;
}

int Basic_Block::get_label_number()
{
	return lnum;
}

void Basic_Block::set_bb_id(int id)
{
	this->id_number = id;
}

bool Basic_Block::has_single_label()
{
	return bb_tac_list.size() == 1 && (*bb_tac_list.begin())->is_label();
}

void Basic_Block::add_instruction(TAC_Stmt* stmt)
{
	bb_tac_list.push_back(stmt);
}
