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

#ifndef CFG_HH

#define CFG_HH

using namespace std;

class CFG
{
	Basic_Block* entry_block;	
	Basic_Block* exit_block;
	list<Basic_Block* > bb_list;
	list<TAC_Stmt *> cfg_tac_list;
	map<int,Basic_Block* > id_to_block;
	map<int,Basic_Block*> bb_to_lnum;
	map<int,int> dfs_number_to_bbid;
	int no_of_basic_blocks;
	set<string > variables_list; 

public :
	
	CFG();
	~CFG();
	void set_bb_list(list<Basic_Block*> basic_blocks);

	void add_mapping(int id, Basic_Block* bb);

	Basic_Block* get_entry_block();
	Basic_Block* get_exit_block();
	void set_entry_basic_block(Basic_Block* entry);
	void set_exit_basic_block(Basic_Block* exit);
	Basic_Block* get_bb_from_id(int bbNum);
	
	void set_no_of_basic_blocks(int noOfBB);
	void add_dfs_mapping(int bb_id,int dfs_number);
	int get_bbid_from_dfs_num(int dfs_number);
	
	bool empty();
	void set_variable_list(set<string> var_list);
	set<string> get_variables_list();
	
	list<Basic_Block* > get_bb_list();
	
	list<TAC_Stmt *> generate_tac();

	string is_edge_present(int ,int);
	int predNumber(int succ,Basic_Block* pred);

};
#endif
