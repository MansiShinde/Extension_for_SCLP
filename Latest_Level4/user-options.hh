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

#ifndef USER_OPTIONS_HH
#define USER_OPTIONS_HH

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <ostream>

using namespace std;

class User_Options;

class User_Options
{
	string source_file;
	string tokens_file_name;
	string ast_file_name;
	string output_file_name;
	string symtab_file_name;
	string rtl_file_name;
	string tac_file_name;

	bool show_tokens_selected;
	bool show_ast_selected;
	bool show_symtab_selected;
	bool show_rtl_selected;
	bool demo_mode_selected;
	bool show_tac_selected;

	bool stop_after_scan;
	bool stop_after_parse;
	bool stop_after_ast;
	bool stop_after_tac;
	bool stop_after_rtl;
	bool no_rtl_support_for_this_feature;	// CHECK: Temporary provision

	ostream * tokens_buffer;
	ostream * ast_buffer;
	ostream * output_buffer;
	ostream * symtab_buffer;
	ostream * rtl_buffer;
	ostream * tac_buffer;

	ofstream tokens_file_buffer;
	ofstream ast_file_buffer;
	ofstream output_file_buffer;
	ofstream symtab_file_buffer;
	ofstream rtl_file_buffer;
	ofstream tac_file_buffer;

	FILE * file_buffer;

public:
	User_Options(); 
	~User_Options();


	bool is_show_tokens_selected();
	bool is_show_ast_selected();
	bool is_demo_mode_selected();
	bool is_show_symtab_selected();
	bool is_show_rtl_selected();
	bool is_show_tac_selected();
	void stop_because_of_incomple_rtl_support();	// CHECK: Temporary provision
	void set_no_rtl_support();			// CHECK: Temporary provision

	bool construct_parse();
	bool construct_ast();
	bool construct_tac();
	bool construct_rtl();
	bool construct_asm();

	void process_user_command_options(int argc, char * argv[]);
	string get_file_name();

	void create_tokens_buffer();
	void create_ast_buffer();
	void create_output_buffer();
	void create_symtab_buffer();
	void create_rtl_buffer();
	void create_tac_buffer();

	ostream & get_tokens_buffer();
	ostream & get_ast_buffer();
	ostream & get_output_buffer();
	ostream & get_symtab_buffer();
	ostream & get_rtl_buffer();
	ostream & get_tac_buffer();

	void remove_files();
};

extern User_Options command_options;

#endif
