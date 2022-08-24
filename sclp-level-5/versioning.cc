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
#include "versioning.hh"


Implementation_Version::Implementation_Version(level_number ln, phase_id pid)
{
	language_level=ln;
	last_phase = pid;
}


/*
void Implementation_Version::needs_version(version_number vn)
{
	string supported_version;

	switch (vnum)
	{
		case A1: supported_version = "The current implementation supports assignment A1."; break;
		case A2: supported_version = "The current implementation supports assignment A2."; break;
		case A3: supported_version = "The current implementation supports assignment A3."; break;
		case A4: supported_version = "The current implementation supports assignment A4."; break;
		case A5: supported_version = "The current implementation supports assignment A5."; break;
		case A6: supported_version = "The current implementation supports assignment A6."; break;
		default:
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH,"Wrong version number");
	}

	stringstream msg;
	
	switch (vn)
	{	
		case A1:	
			msg << "contains some feature of assignment A1. " << supported_version;
			CHECK_INPUT_AND_ABORT(a1_support == true, msg.str(), NO_FILE_LINE);
			break;
		case A2:	
			msg << "contains some feature of assignment A2. " << supported_version;
			CHECK_INPUT_AND_ABORT(a2_support == true, msg.str(), NO_FILE_LINE);
			break;
		case A3:	
			msg << "contains some feature of assignment A3. " << supported_version;
			CHECK_INPUT_AND_ABORT(a3_support == true, msg.str(), NO_FILE_LINE);
			break;
		case A4:	
			msg << "contains some feature of assignment A4. " << supported_version;
			CHECK_INPUT_AND_ABORT(a4_support == true, msg.str(), NO_FILE_LINE);
			break;
		case A5:	
			msg << "contains some feature of assignment A5. " << supported_version;
			CHECK_INPUT_AND_ABORT(a5_support == true, msg.str(), NO_FILE_LINE);
			break;
		case A6:	
			msg << "contains some feature of assignment A6. " << supported_version;
			CHECK_INPUT_AND_ABORT(a6_support == true, msg.str(), NO_FILE_LINE);
			break;
		default:
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH,"Wrong version number");
	}
	
}

*/

void Implementation_Version::needs_level(level_number ln)
{
	string supported_version;

	bool level_mismatch = ln > language_level? true: false;

	switch (language_level)
	{
		case L1: supported_version = "The current implementation supports level only L1."; break;
		case L2: supported_version = "The current implementation supports levels L1 and L2."; break;
		case L3: supported_version = "The current implementation supports levels L1, L2, and L3."; break;
		case L4: supported_version = "The current implementation supports levels L1, L2, L3, and L4."; break;
		case L5: supported_version = "The current implementation supports levels L1, L2, L3, L4, and L5."; break;
		case L6: supported_version = "The current implementation supports levels L1, L2, L3, L4, L5 and L6."; break;
		default:
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH,"Wrong version number");
	}

	stringstream msg;
	
	switch (ln)
	{	
		case L1:	
			msg << "contains some feature of language level L1. " << supported_version;
			CHECK_INPUT_AND_ABORT(level_mismatch==false, msg.str(), NO_FILE_LINE);
			break;
		case L2:	
			msg << "contains some feature of language level L2. " << supported_version;
			CHECK_INPUT_AND_ABORT(level_mismatch==false, msg.str(), NO_FILE_LINE);
			break;
		case L3:	
			msg << "contains some feature of language level L3. " << supported_version;
			CHECK_INPUT_AND_ABORT(level_mismatch==false, msg.str(), NO_FILE_LINE);
			break;
		case L4:	
			msg << "contains some feature of language level L4. " << supported_version;
			CHECK_INPUT_AND_ABORT(level_mismatch==false, msg.str(), NO_FILE_LINE);
			break;
		case L5:	
			msg << "contains some feature of language level L5. " << supported_version;
			CHECK_INPUT_AND_ABORT(level_mismatch==false, msg.str(), NO_FILE_LINE);
			break;
		case L6:	
			msg << "contains some feature of language level L6. " << supported_version;
			CHECK_INPUT_AND_ABORT(level_mismatch==false, msg.str(), NO_FILE_LINE);
			break;
		default:
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH,"Wrong version number");
	}
	
}

void Implementation_Version::needs_phase(phase_id pid)
{
	string supported_version;

	switch (last_phase)
	{
		case phase_scan: supported_version = "The current implementation supports only SCAN."; break;
		case phase_parse: supported_version = "The current implementation supports only SCAN and PARSE."; break;
		case phase_ast: supported_version = "The current implementation supports only SCAN, PARSE, and AST."; break;
		case phase_tac: supported_version = "The current implementation supports only SCAN, PARSE, AST, and TAC."; break;
		case phase_rtl: supported_version = "The current implementation supports only SCAN, PARSE, AST, TAC, and RTL."; break;
		case phase_asm: supported_version = "The current implementation supports all phases."; break;
		default:
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH,"Wrong version number");
	}

	stringstream msg;

	bool phase_mismatch = (pid > last_phase)? true: false;

	switch (pid)
	{	
		case phase_scan:	
			break;
		case phase_parse:	
			break;
		case phase_ast:	
			msg << "The -ast option not supported. " << supported_version;
			CHECK_INPUT_AND_ABORT(phase_mismatch==false, msg.str(), NO_FILE_LINE);
			break;
		case phase_tac:	
			msg << "The -tac option not supported. " << supported_version;
			CHECK_INPUT_AND_ABORT(phase_mismatch==false, msg.str(), NO_FILE_LINE);
			break;
		case phase_rtl:	
			msg << "The -rtl option not supported. " << supported_version;
			CHECK_INPUT_AND_ABORT(phase_mismatch==false, msg.str(), NO_FILE_LINE);
			break;
		case phase_asm:	
			break;
		default:
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH,"Wrong phase id");
	}
	
}

phase_id Implementation_Version::get_last_phase()
{
	return last_phase;
}
