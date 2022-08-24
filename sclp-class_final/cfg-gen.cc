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
#include<list>
#include<map>
#include<set>
#include<queue>
#include<stack>
#include<algorithm>

using namespace std;

#include "common-headers.hh"

// For every procedure gen_cfg() function is called from gen_cfg() of Program class
void Program::gen_cfg()
{
	map<string, Procedure *>::iterator procedure;
	for(procedure = procedure_map.begin(); procedure != procedure_map.end(); procedure++)
	{
		(procedure->second)->gen_cfg();
	}
}

// For every procedure print_cfg() function is called from print_cfg() of Program class
void Program::print_cfg()
{
	ostream * file_buffer;	// For printing cfg using file_buffer
	command_options.create_cfg_buffer();

	file_buffer = &(command_options.get_cfg_buffer());

	map<string, Procedure *>::iterator procedure;
	for(procedure = procedure_map.begin(); procedure != procedure_map.end(); procedure++)
	{
		CHECK_INVARIANT(procedure->second!=NULL, "Procedure object pointer cannot be Null");
		procedure->second->print_cfg(*file_buffer);		
	}
}

void Program::draw_cfg()
{
	ostream * graph_file_buffer;	// For printing cfg using file_buffer 
	command_options.create_cfg_dot_buffer();
	graph_file_buffer = &(command_options.get_cfg_dot_buffer());

	map<string, Procedure *>::iterator procedure;
	for(procedure = procedure_map.begin(); procedure != procedure_map.end(); procedure++)
	{
		CHECK_INVARIANT(procedure->second!=NULL, "Procedure object pointer cannot be Null");
		procedure->second->print_cfg_dot_graph(*graph_file_buffer);		
	}
}


///////////////////////////////////////////Procedure Class//////////////////////////////////////

// IDENTIFYING LEADER STATEMENTS 
list<TAC_Stmt *> Procedure::identify_leader_stmts()
{
	list<TAC_Stmt *> leaderStatements={};	

	list<TAC_Stmt *>::iterator tac_stmt;

	tac_stmt = tac_stmt_list->begin();
	leaderStatements.push_back(*tac_stmt);
		
	for(tac_stmt++ ; tac_stmt != tac_stmt_list->end(); tac_stmt++)
	{
	        // Considering Label_TAC_Stmt as leader statement
		if((*tac_stmt)->is_leader_statement())
		{
			leaderStatements.push_back(*tac_stmt);				
		}
		// Considering Goto and if goto statements as leader statement
		else if((*tac_stmt)->is_next_leader_statement())			
		{
			if(*tac_stmt != tac_stmt_list->back())  // Checking if tac_stmt iterator has not reached the end of tac_stmt_list
			{
				tac_stmt++;
				leaderStatements.push_back(*tac_stmt);
			}
			else
				return leaderStatements;	
		}	
	}

	return leaderStatements;
}

// CREATING BASIC BLOCKS
void Procedure::gen_bb(list<TAC_Stmt *> leader_statements)
{

	//// Basic Blocks creation  /////
	list<Basic_Block *> basic_block_list;
	list<TAC_Stmt *>::iterator leader_stmt;
	TAC_Stmt * second_last_leader;
	map<int,Basic_Block*> lnum_to_bb;			// Map of label number to Basic Block ID -- which label is to be targeted to which bb
	list<int> label_stmts;
	int label = -1;

	Basic_Block* bb = new Basic_Block({},-1);		//Dummy Entry Block

	basic_block_list.push_back(bb);

	// Traversing from first leader statement to second last leader statement 
	for(leader_stmt = leader_statements.begin(),second_last_leader = *(--leader_statements.end()) ; *leader_stmt!=second_last_leader;leader_stmt++)
	{
		TAC_Stmt* next_leader =  *next(leader_stmt,1);
		
		label_stmts = get_label_list(leader_statements,leader_stmt);  //  To find all labels that are going to map to the next basic_block
		if(!label_stmts.empty())
			label = label_stmts.back();
		else 
			label =-1;

		bb = create_bb(*leader_stmt,next_leader,label); // Creating basic block from one leader statement to the next leader statement

		for(auto labels = label_stmts.begin();labels!=label_stmts.end();labels++)
		{		
			lnum_to_bb.insert(make_pair(*labels,bb));
		}
		
		basic_block_list.push_back(bb);
	}

	label_stmts = get_label_list(leader_statements,leader_stmt); // For last leader statement
	if(!label_stmts.empty())
			label = label_stmts.back();
		else 
			label =-1;
	
	bb = create_bb(*leader_stmt,*(next(leader_stmt,1)),label);

	for(auto labels = label_stmts.begin();labels!=label_stmts.end();labels++)
	{		
		lnum_to_bb.insert(make_pair(*labels,bb));
	}
	basic_block_list.push_back(bb);
	if(bb->has_single_label())
		bb->add_instruction(new NOP_TAC_Stmt());

	if(!(bb->get_tac_stmts()).empty())
		label =-1;
	bb = new Basic_Block({},label);		// Dummy Exit Block

	for(auto labels = label_stmts.begin();labels!=label_stmts.end();labels++)
	{		
		lnum_to_bb.insert(make_pair(*labels,bb));
	}
	basic_block_list.push_back(bb);
	cfg->set_bb_list(basic_block_list);

	remove_empty_bb(lnum_to_bb);

	connect_bb();

}

// REMOVE EMPTY BLOCKS
void Procedure::remove_empty_bb(map<int,Basic_Block* >& lnum_to_bb)
{

	list<Basic_Block*> bb_list = cfg->get_bb_list();
	list<Basic_Block*>::iterator b,next_bb,next_to_next,bb;
	map<int,Basic_Block* >::iterator l_to_bbid,lnumbbid; 
	set<Basic_Block* > del_bb;
	list<Basic_Block* > new_bb_list;
 
	for(bb = bb_list.begin();bb!=bb_list.end();bb++)
	{
		next_bb = next(bb,1);
                if(next_bb!=bb_list.end() && (*next_bb)->get_tac_stmts().size() == 0)
		{
			next_to_next = next(next_bb,1);
			Basic_Block* assign_bb;
			if(next_to_next!=bb_list.end())
			{
				assign_bb = (*next_to_next);
				for(l_to_bbid = lnum_to_bb.begin();l_to_bbid!=lnum_to_bb.end();l_to_bbid++)
				{
					if((*l_to_bbid).second == (*next_bb))
						lnum_to_bb[(*l_to_bbid).first] = assign_bb;
				}
				del_bb.insert((*next_bb));
			}	
		}
	}

	int bbid = 1;
	for(auto bb = bb_list.begin();bb!=bb_list.end();bb++)
	{
		if(del_bb.find((*bb)) == del_bb.end())
		{
			(*bb)->set_bb_id(bbid);
			cfg->add_mapping(bbid,(*bb));
			bbid++;			
			new_bb_list.push_back(*bb);

		}
	}
	cfg->set_bb_list(new_bb_list);  // Setting new basic block list without empty blocks

	for(bb = new_bb_list.begin();bb!=new_bb_list.end();bb++)
	{
		list<TAC_Stmt *> tac_stmts = (*bb)->get_tac_stmts();
		if(tac_stmts.size()!=0 && ((tac_stmts.back())->has_target()))
		{
			Label_TAC_Opd * result = dynamic_cast<Label_TAC_Opd *> (tac_stmts.back()->get_result());
			int label = result->get_label();
			if(lnum_to_bb.find(label)!=lnum_to_bb.end())
			{
				result->set_label_number(lnum_to_bb[label]);
				lnum_to_bb.erase(label);
			}
		}
	}
}	

// To find all labels that are going to map to the next basic_block 
list<int> Procedure::get_label_list(list<TAC_Stmt *> leader_stmts,list<TAC_Stmt *>::iterator leader_stmt)
{
		TAC_Stmt* next_leader =  *next(leader_stmt,1);
		int label_num;
		Label_TAC_Stmt * current_label;

		list<int> label_stmts;
		
                if((*leader_stmt)!=NULL && typeid(**leader_stmt) == typeid(Label_TAC_Stmt))
		{
			current_label = dynamic_cast<Label_TAC_Stmt *> (*leader_stmt);
			label_num = current_label->get_label();
			label_stmts.push_back(label_num);
			
			while((next_leader!=*leader_stmts.end()) && (next_leader)->is_leader_statement()) 
			{
				if(distance(find(tac_stmt_list->begin(),tac_stmt_list->end(),*leader_stmt),find(tac_stmt_list->begin(),tac_stmt_list->end(),next_leader)) == 1)
				{
					current_label = dynamic_cast<Label_TAC_Stmt *> (next_leader);
					label_num = current_label->get_label();
					label_stmts.push_back(label_num);
					leader_stmt++;
					next_leader =  *next(leader_stmt,1);
				}
				else break;	
			}
		}
		return label_stmts;
}

// CREATING A BASIC BLOCK BETWEEN TWO LEADER STATEMENTS
Basic_Block* Procedure::create_bb(TAC_Stmt* current_leader_stmt,TAC_Stmt* next_leader_stmt,int label)
{
	list<TAC_Stmt *>::iterator tac_stmt;
	list<TAC_Stmt *> bb_tac_list;
	Basic_Block *bb = NULL;
	for(tac_stmt = find(tac_stmt_list->begin(),tac_stmt_list->end(),current_leader_stmt);tac_stmt!=find(tac_stmt_list->begin(),tac_stmt_list->end(),next_leader_stmt);tac_stmt++)  // find() returns the iterator where required leader stmt is found in tac_stmt_list
	{
		bb_tac_list.push_back(*tac_stmt);
	}
	bb = new Basic_Block(bb_tac_list,label);
	return bb;
}

// GET REQUIRED BASIC BLOCK FROM MAP OF BASIC BLOCK ID to BASIC BLOCK
int Procedure:: get_bb(int label_num, map<int,int> label_map)
{
	int bb_id=label_num;
	while(label_map.count(bb_id) > 0)
	{
		bb_id = label_map[bb_id];
	}
	return bb_id;
}

// CONNECT BASIC BLOCKS GENERATED TO GET A CFG
void Procedure::connect_bb()
{
	list<Basic_Block* >::iterator bb,next_bb;
	list<Basic_Block* > bb_list =cfg->get_bb_list();

	TAC_Stmt* current_stmt =NULL;
	If_Goto_TAC_Stmt* if_goto_stmt = NULL;
	Goto_TAC_Stmt* goto_stmt = NULL;

	TAC_Opd* result =NULL;
	
	for(bb = bb_list.begin(); bb!=bb_list.end(); bb++)
	{
		current_stmt = (*bb)->get_tac_stmts().back();

		if(current_stmt!=NULL && (current_stmt)->needs_true_edge())
		{
			
			if_goto_stmt = dynamic_cast<If_Goto_TAC_Stmt*>(current_stmt);
			
			result = if_goto_stmt->get_result();	
			int bbNum = result->get_label();

			Basic_Block * succ = cfg->get_bb_from_id(bbNum);
			if(succ!=NULL)
			{(*bb)->add_successor(succ,"True"); // "True is the type of edge"

			succ->add_predecessor((*bb));}
			if((*bb)!=bb_list.back())
			{	
				next_bb = std::next(bb,1);
				(*bb)->add_successor(*next_bb,"False");  // "False" is the type of edge
				(*next_bb)->add_predecessor((*bb));
			}

		}
		else if(current_stmt!=NULL && (current_stmt)->needs_fall_through_edge())
		{			
			goto_stmt = dynamic_cast<Goto_TAC_Stmt*>(current_stmt);
			
			result = goto_stmt->get_result();
			int bbNum = result->get_label();	

			Basic_Block * succ = cfg->get_bb_from_id(bbNum);
			if(succ!=NULL){
			(*bb)->add_successor(succ,"Fall Through"); // Fall through is the type of edge
			succ->add_predecessor((*bb));}

		}
		else
		{
			if((*bb)!=bb_list.back())
			{	
				next_bb = std::next(bb,1);
				
				(*bb)->add_successor(*next_bb,"Fall Through");
				(*next_bb)->add_predecessor((*bb));
			}
		}
	}

	if(cfg->get_entry_block() == NULL)   ///// Setting Entry Block
	{
		cfg->set_entry_basic_block(bb_list.front());
	}

	if(cfg->get_exit_block() == NULL)   /////  Setting Exit Block
	{
		cfg->set_exit_basic_block(bb_list.back());
		cfg->set_no_of_basic_blocks(bb_list.back()->get_bb_id());   ////// Setting number of basic blocks in CFG object
	}
}

// gen_cfg() is called for every Procedure from gen_cfg of Program class
void Procedure::gen_cfg()
{
	list<TAC_Stmt *> leaders = identify_leader_stmts();
	gen_bb(leaders);  // Create CFG by connecting basic blocks
	validate_cfg();   // CFG Validation
	dfs_numbering(); 
	*tac_stmt_list = cfg->generate_tac();
	//cfg.print_tac();
}


// PRINTING CFG
void Procedure::print_cfg(ostream & file_buffer)
{
	file_buffer << "\n**PROCEDURE BEGIN**\n";
	list<Basic_Block* >::iterator bb;
	list<Basic_Block* > bb_list =cfg->get_bb_list();
	for(bb = bb_list.begin(); bb!=bb_list.end(); bb++)
	{
		(*bb)->print_bb(file_buffer);
	}
	file_buffer << "**PROCEDURE END**\n\n";
}

// For GRAPH in .dot file
void Procedure::print_cfg_dot_graph(ostream & graph_file_buffer)
{
	list<Basic_Block *> bb_list = cfg->get_bb_list(); 

	graph_file_buffer << "digraph{\n";
	for(auto bb : bb_list)
	{
		graph_file_buffer << "bb"<<(*bb).get_bb_id() <<";";
	}
	list<pair<int,string>> bb_succ_list ;
	for(auto bb : bb_list)
	{
		bb_succ_list = (*bb).get_successors_list();
		for(auto bb_succ : bb_succ_list)
			graph_file_buffer << "bb"<<(*bb).get_bb_id() <<"->bb"<<(bb_succ).first<<";";
		graph_file_buffer << "\n";
	}
	graph_file_buffer << "}\n";
}


void Procedure::validate_cfg()
{
	CHECK_INVARIANT(validate_entry_exit(),"Invalid CFG is generated : violates single entry ,single exit rule ");
	CHECK_INVARIANT(cfg->get_entry_block(),"No Entry block found");
	CHECK_INVARIANT(cfg->get_exit_block(),"No Exit block found");
	CHECK_INVARIANT(reachable_from_entry_block(),"Unreachable basic blocks ");
	CHECK_INVARIANT(exit_block_reachable(),"Exit block not reachable");
	CHECK_INVARIANT(validate_pred_succ(),"Predecessor and Successors incorrectly set");
}

// VALIDATION OF UNIQUE ENTRY AND EXIT BLOCK
bool Procedure::validate_entry_exit()
{	
	int pre =  0;
	int succ = 0;

	list<Basic_Block * > bbList = cfg->get_bb_list();

	list<Basic_Block * >::iterator bb;

	if(!cfg->get_entry_block()->get_predecessors_list().empty())
		return false;
	if(!cfg->get_exit_block()->get_successors_list().empty())
		return false;

	for(bb = bbList.begin(); bb!= bbList.end(); bb++)
	{
		if((*bb)->get_predecessors_list().empty())
			pre ++;
		if((*bb)->get_successors_list().empty())
			succ ++;
	}

	if(pre == 1 && succ == 1)
		return true;
	return false;
}

// VALIDATES IF EVERY BLOCK REACHES THE EXIT BLOCK 
bool Procedure::exit_block_reachable()
{       
	// Breadth first search is used 

	Basic_Block* exitBB = cfg->get_exit_block();

	Basic_Block* tempBB = exitBB;

	set<int> visited ;
	queue <int> q_bb_id;

	q_bb_id.push(tempBB->get_bb_id());
        visited.insert(tempBB->get_bb_id());
      
	list<int>::iterator succ;
	int next = 0;
	
	do
	{       
		next = q_bb_id.front();		 
		tempBB = cfg->get_bb_from_id(next);
		list<int> succList = tempBB->get_predecessors_list();
		q_bb_id.pop();
		for(succ = succList.begin(); succ!=succList.end(); succ++)
		{
			if(!visited.count(*succ))
			{
				q_bb_id.push(*succ);			
				visited.insert(*succ);
			}			
		}		
	}while(!q_bb_id.empty());       
	
	if(visited.size() == cfg->get_bb_list().size())
		return true; 
	return false;       
}

// VALIDATES IF EACH BLOCK IS REACHABLE FROM ENTRY BLOCK
bool Procedure::reachable_from_entry_block()
{       
	Basic_Block* entryBB = cfg->get_entry_block();

	Basic_Block* currBB = entryBB;

	set<int> visited ;
	queue <int> q_bb_id;
	q_bb_id.push(currBB->get_bb_id());
	
        visited.insert(entryBB->get_bb_id());
      
	list<pair<int,string>>::iterator successor;
	int curr_BB_Id = 0;

	while(!q_bb_id.empty())
	{       
		int curr_BB_Id = q_bb_id.front();
		currBB = cfg->get_bb_from_id(curr_BB_Id);

		q_bb_id.pop();
		
		list<pair<int,string>> succList = currBB->get_successors_list(); 
		for(successor = succList.begin(); successor!=succList.end(); successor++)
		{
			if(!visited.count((*successor).first))
			{
				q_bb_id.push((*successor).first);
				visited.insert((*successor).first);
			}	
		}
	}      

	if(visited.size() == cfg->get_bb_list().size())
		return true; 
	return false;       
}

// CHECK IF APPROPRIATE PREDECESSORS AND SUCCESSORA ARE SET 
bool Procedure::validate_pred_succ()
{ 
	list<Basic_Block * > bbList = cfg->get_bb_list();
	list<pair<int,string>> succList;

	list<Basic_Block * >::iterator bb;
	list<pair<int,string>>::iterator successor;

	for(bb = bbList.begin(); bb!=bbList.end(); bb++)
	{
      	succList = (*bb)->get_successors_list(); // Successor list - list of basic block ID'S of successors
		int predecessor = (*bb)->get_bb_id();

		for(successor = succList.begin(); successor != succList.end(); successor++)
		{
			list<int> pred = (cfg->get_bb_from_id((*successor).first))->get_predecessors_list();
			if(!(std::find(pred.begin(),pred.end(),predecessor)!=pred.end()))
                             return false;
		}	
	}
	return true;
}

void Procedure::dfs_numbering()
{   
	int time = 1;
	int succ_pushed = 0;
	Basic_Block* entryBB = cfg->get_entry_block();
	Basic_Block* currBB = entryBB;

	set<int> visited ;
	stack <int> stack_bbid;
	stack_bbid.push(currBB->get_bb_id());
	currBB -> set_dfs_pre_number(time++);
	cfg->add_dfs_mapping(currBB->get_bb_id(),time);

	visited.insert(entryBB->get_bb_id());
	list<pair<int,string>>::iterator succ;
	while(!stack_bbid.empty())
	{       
		int curr_BB_Id = stack_bbid.top();
		succ_pushed = 0;
		currBB = cfg->get_bb_from_id(curr_BB_Id);
		
		list<pair<int,string>> succList = currBB->get_successors_list(); 
		for(succ = succList.begin(); succ!=succList.end(); succ++)
		{
			if(!visited.count((*succ).first))
			{	
				succ_pushed++;
				stack_bbid.push((*succ).first);
				currBB = cfg->get_bb_from_id(stack_bbid.top());
				currBB->set_dfs_pre_number(time++);
				cfg->add_dfs_mapping(curr_BB_Id,time);
				visited.insert((*succ).first);
			}	
		}		
	 	if(succ_pushed == 0){
			stack_bbid.pop();
			currBB->set_dfs_post_number(time++);
		}
	}      
}





