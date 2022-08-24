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
#include <fstream>
#include <ostream>
#include <istream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>

using namespace std;

#include"common-classes.hh"
#include "user-options.hh"
#include "error-display.hh"

// We define implementation version as version_number = (level_number, phase_id) where
// version_number represents the assignment number
// level_number represents the language level
// phase_id represents a compilation phase
// The scanner asserts a needs_level function depending upon the tokens
// The parser sets needs_level for a function call
// The version_number is used to set appropriate stop-after-* flag which
// is then used in the construct_<phase> function to determine the 
// phase after which the processing stops.
// This allows a reference implementation for an assignment from A1 to A6
// to be created out of the same source.

typedef enum {
	A1,
	A2,
	A3,
	A4,
	A5,
	A6
} version_number;

typedef enum {
	L1,
	L2,
	L3,
	L4,
	L5
} level_number;

typedef enum {
	phase_scan,
	phase_parse,
	phase_ast,
	phase_tac,
	phase_rtl,
	phase_asm,
	phase_cfg,
	phase_ssa
} phase_id;

class Implementation_Version
{
	bool a1_support = false;
	bool a2_support = false;
	bool a3_support = false;
	bool a4_support = false;
	bool a5_support = false;
	bool a6_support = false;

	version_number vnum;
	level_number language_level;
	phase_id last_phase;
	

public:
	Implementation_Version(level_number ln, phase_id pid);
	~Implementation_Version() {};

	//void needs_version(version_number vn);
	void needs_level(level_number ln);
	void needs_phase(phase_id pid);
	phase_id get_last_phase();
	level_number get_language_level();
};

extern Implementation_Version this_version;

// Object this_version is instantiated in main.cc just outside the main function.
