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

#ifndef ERROR_DISPLAY_HH
#define ERROR_DISPLAY_HH

#include <sstream>
#include <string>

#define NO_FILE_LINE -1

#define CONTROL_SHOULD_NOT_REACH false

using namespace std;

bool error_status();
void report_violation_of_condition(bool condition, string s, int lineno);
void report_violation_and_abort(bool condition, string s, int lineno);
void check_invariant_underlying_function(bool condition, string error_message);

#define CHECK_INVARIANT(x,y) \
     {  stringstream ___new___string___; \
        ___new___string___ << y << " (Invariant at line " << __LINE__ <<  ", file " << __FILE__ << ").\n"; \
        check_invariant_underlying_function(x, ___new___string___.str()); \
     }

#define CHECK_INPUT(condition, error_message, lineno)							\
	report_violation_of_condition(condition, error_message, lineno);

#define CHECK_INPUT_AND_ABORT(condition, error_message, lineno)							\
	report_violation_and_abort(condition, error_message, lineno);

#endif
