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
#include <execinfo.h>
#include <unistd.h>


#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <string>

using namespace std;

#include "common-headers.hh"
#include "stacktrace.h"

string Backtrace(int skip = 1);

extern User_Options command_options;

void check_invariant_underlying_function(bool condition, string error_message)
{
	string file_name = command_options.get_input_file_name();
	if (!condition)
	{
		cerr << "\nsclp internal error on input file: " << file_name << error_message << "Please report this error along with the input file that caused it, to uday@cse.iitb.ac.in.\n";

		print_stacktrace();
		//cerr << Backtrace();
		
		command_options.remove_files();
		exit(1);
	}
}

bool global_error_status = false;

void report_violation_of_condition(bool condition, string error_message, int lineno)
{
	string file_name = command_options.get_input_file_name();
	if (!condition)
	{
		if (lineno != NO_FILE_LINE)
			cerr <<  "sclp error: File: " << file_name << ", Line: " << lineno << ", Next Token: " << last_token_seen << ", Lexeme: \"" << last_lexeme_seen << "\"\n            Description: " << error_message << "\n";
		else
			cerr <<  "sclp error: File: " << file_name <<  " " << error_message << "\n";
		global_error_status = true;
	}
} 

void report_violation_and_abort(bool condition, string error_message, int lineno)
{
	string file_name = command_options.get_input_file_name();
	if (!condition)
	{
		if (lineno != NO_FILE_LINE)
			cerr <<  "sclp error: File: " << file_name << ", Line: " << lineno << ", Next Token: " << last_token_seen << ", Lexeme: \"" << last_lexeme_seen << "\"\n            Description: " << error_message << "\n";
		else
			cerr <<  "sclp error: File: " << file_name << " " << error_message << "\n";
		exit(1);
	}
} 

bool error_status()
{
	return global_error_status;
}
