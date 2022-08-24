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

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <list>
#include <stdio.h>

using namespace std;

#include "common-headers.hh"

extern int yyparse();
extern int yylex();
extern  FILE *yyin;
extern int yylineno;
extern  FILE *yyout;

User_Options command_options;

#include <dirent.h>

// See the documentation in versioning.hh
// A1 = (L2, phase_parse)
// A2 = (L3, phase_ast)
// A3 = (L3, phase_tac)
// A4 = (L4, phase_tac)
// A5 = (L4, phase_rtl)
// A6 = (L5, phase_asm)
// In order to create a reference implementation for level li, choose parameters 
// (li, phase_asm).  For example, for creating full compiler for level L2, use 
// the options "-e L=L2 -e P=phase_asm" in the Makefile. This will instantation the 
// definition of this_version to the following:
// Implementation_Version this_version(L2, phase_asm);
// This can be achieved by using the script build.sh with arguments -l<level< and -p<phase>
// in any order where phase is one of the scan, parse, ast, tac, rtl, and asm

Implementation_Version this_version(LEVEL, PHASE);

int main(int argc, char * argv[]) 
{
	bool parse_result = false;
	bool scan_result = false;
	

	command_options.process_user_command_options(argc, argv);

	if (command_options.construct_parse())
		parse_result = yyparse();	// AST is constructed along with parsing.
	else
		while (yylex())
			;                // yylex is invoked repeatedly until it return 0.

	CHECK_INPUT_AND_ABORT((!parse_result), "Cannot parse the input program", yylineno);
	
	if (command_options.construct_ast())
	{
		if ((error_status() == false) && (command_options.is_show_ast_selected()))
		    program_object.print_ast();
        }

	if ((command_options.is_show_symtab_selected()) && (error_status() == false))
		program_object.print_sym();
	

	if (command_options.construct_tac() && (error_status() == false))
		program_object.gen_tac();

	
		
	if ((command_options.is_show_tac_selected()) && (error_status() == false))
		program_object.print_tac();
	

	if((command_options.construct_cfg()) && (error_status() == false))
		program_object.gen_cfg();
	
	
	if((command_options.is_show_cfg_selected()) && (error_status() == false))
		program_object.print_cfg();

	if((command_options.is_show_draw_cfg_dot_selected()) && (error_status() == false))
		program_object.draw_cfg();
	

	

#if 0
	if ((command_options.perform_lv_analysis()) && (error_status() == false)) //RnD Work
		program_object.perform_lva();
	
	if ((command_options.is_lv_selected()) && (error_status() == false)) //RnD Work
		program_object.print_lv();

	if(command_options.perform_av_analysis() && error_status()==false) //Rnd Work
		program_object.perform_ava();

	if ((command_options.is_av_selected()) && (error_status() == false)) //RnD Work
		program_object.print_av();
#endif

	if (command_options.construct_rtl() && (error_status() == false))
	     	program_object.gen_rtl();
		
	

	if ((command_options.is_show_rtl_selected()) && (error_status() == false))
	     	program_object.print_rtl();


	if (command_options.construct_asm() && (error_status() == false)) 
		program_object.print_assembly();
		/* construct_asm implies show_asm */

	program_object.delete_all();

	return 0;
}
