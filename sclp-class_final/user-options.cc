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

#include "common-headers.hh"

extern  FILE *yyin;

User_Options::User_Options() 
{
	show_tokens_selected = false;
	show_ast_selected = false;
	show_symtab_selected = false;
	show_rtl_selected = false;
	show_tac_selected = false;

//-------------------------Changed on 25-02-2020-------------------------
	show_cfg_selected = false;
	show_ssa_selected = false;
	lv_selected = false;//Rnd Work
	av_selected = false;

	demo_mode_selected = false;

	stop_after_scan = false;
	stop_after_parse = false;
	stop_after_ast = false;
	stop_after_tac = false;
	stop_after_rtl = false;

//-------------------------Changed on 25-02-2020-------------------------
	stop_after_cfg = false;
	stop_after_ssa = false;

} 

User_Options::~User_Options()
{
	if (demo_mode_selected == false)
	{
		if (show_tokens_selected)
			tokens_file_buffer.close(); 

		if (show_ast_selected)
			ast_file_buffer.close(); 

		if (show_tac_selected)
			ast_file_buffer.close(); 

//-------------------------Changed on 25-02-2020-------------------------
		if(show_cfg_selected)
			cfg_file_buffer.close();
		if(show_draw_cfg_dot_selected){

			cfg_dot_file_buffer.close();
		}
		if(lv_selected)
			lv_file_buffer.close();
		if(av_selected)
			av_file_buffer.close();
		
		if(show_ssa_selected)
			ssa_file_buffer.close();
// ----------------------- till here ---------------------------------

		output_file_buffer.close(); 
	}
}

bool User_Options::is_show_tokens_selected() {	return show_tokens_selected; }
bool User_Options::is_show_ast_selected() { return construct_ast() && show_ast_selected; }
bool User_Options::is_show_tac_selected() { return construct_tac() && show_tac_selected; }
bool User_Options::is_lv_selected(){ return perform_lv_analysis() && lv_selected; } //RnD Work
bool User_Options::is_av_selected(){ return perform_av_analysis() && av_selected; }

bool User_Options::is_show_rtl_selected() { return construct_rtl() && show_rtl_selected; }
//bool User_Options::is_show_symtab_selected() { return construct_rtl() && show_symtab_selected; }
bool User_Options::is_show_symtab_selected() { return show_symtab_selected; }
bool User_Options::is_demo_mode_selected() { return demo_mode_selected; }

//-------------------------Changed on 25-02-2020-------------------------
bool User_Options::is_show_cfg_selected() { return construct_cfg() && show_cfg_selected; }

bool User_Options::is_show_draw_cfg_dot_selected() { return construct_draw_cfg_dot() && show_draw_cfg_dot_selected; }
bool User_Options::is_show_ssa_selected() { return construct_ssa() && show_ssa_selected; }
// ----------------------- till here ---------------------------------

bool User_Options::construct_parse() { return !stop_after_scan; }
bool User_Options::construct_ast() { return construct_parse() && (!stop_after_parse); }
bool User_Options::construct_tac() { return construct_ast() && (!stop_after_ast); }
bool User_Options::construct_cfg() { return construct_tac() && (!stop_after_tac); }
bool User_Options::perform_lv_analysis(){ return construct_rtl();} //RnD Work
bool User_Options::perform_av_analysis(){ return construct_rtl();}
bool User_Options::construct_rtl() { return construct_cfg() && (!stop_after_cfg); }
bool User_Options::construct_asm() { return construct_rtl() && (!stop_after_rtl); }


//-------------------------Changed on 25-02-2020-------------------------

bool User_Options::construct_draw_cfg_dot() {return construct_cfg() ;/*&& (!stop_after_cfg); */}

bool User_Options::construct_ssa() { return construct_cfg() && (!stop_after_cfg); }

// ----------------------- till here ---------------------------------


void User_Options::create_tokens_buffer()
{
	CHECK_INVARIANT(show_tokens_selected, "The show_tokens option should be set");

	if (demo_mode_selected)
		tokens_buffer = &cout;

	else
		tokens_buffer = &tokens_file_buffer; 
}

void User_Options::create_ast_buffer()
{
	CHECK_INVARIANT(show_ast_selected, "The show_ast option should be set");

	if (demo_mode_selected)
		ast_buffer = &cout;

	else
		ast_buffer = &ast_file_buffer; 
}

void User_Options::create_output_buffer()
{ 
	if (demo_mode_selected)
		output_buffer = &cout;
	else
		output_buffer = &output_file_buffer; 
}

void User_Options::create_symtab_buffer()
{
	CHECK_INVARIANT(show_symtab_selected, "The show_symtab options should be set");

	if (demo_mode_selected)
		symtab_buffer = &cout;
	else
		symtab_buffer = &symtab_file_buffer;
}

void User_Options::create_rtl_buffer()
{
	CHECK_INVARIANT(show_rtl_selected, "The show_rtl option should be set");

	if (demo_mode_selected)
		rtl_buffer = &cout;
	else
		rtl_buffer = &rtl_file_buffer;
}

void User_Options::create_tac_buffer()
{
	CHECK_INVARIANT(show_tac_selected, "The show_tac option should be set");

	if (demo_mode_selected)
		tac_buffer = &cout;
	else
		tac_buffer = &tac_file_buffer;
}

//-------------------------Changed on 25-02-2020-------------------------
void User_Options::create_cfg_buffer()
{
	CHECK_INVARIANT(show_cfg_selected, "The show_cfg option should be set");
	if(demo_mode_selected)
		cfg_buffer = &cout;
	else
		cfg_buffer = &cfg_file_buffer;

}
void User_Options::create_lv_buffer()//Rnd Work
{
	CHECK_INVARIANT(lv_selected, "The -lv option should be set");
	if(demo_mode_selected)
		lv_buffer = &cout;
	else
		lv_buffer = &lv_file_buffer;

}
void User_Options::create_av_buffer()//Rnd Work
{
	CHECK_INVARIANT(av_selected, "The -av option should be set");
	if(demo_mode_selected)
		av_buffer = &cout;
	else
		av_buffer = &av_file_buffer;

}
void User_Options::create_cfg_dot_buffer()
{
	CHECK_INVARIANT(show_draw_cfg_dot_selected, "The draw-cfg option should be set");
	if(demo_mode_selected)
		cfg_dot_buffer = &cout;
	else
		cfg_dot_buffer = &cfg_dot_file_buffer;

}

void User_Options::create_ssa_buffer()
{
	CHECK_INVARIANT(show_ssa_selected, "The show_ssa option should be set");
	
	if(demo_mode_selected)
		ssa_buffer = &cout;
	else
		ssa_buffer = &ssa_file_buffer;

}

// ---------------------------- till here -----------------------------


ostream & User_Options::get_tokens_buffer()
{
	if (tokens_buffer == NULL)
		tokens_buffer = &cout;

	*tokens_buffer << std::fixed;
	*tokens_buffer << std::setprecision(2);

	return *tokens_buffer;
}

ostream & User_Options::get_ast_buffer()
{
	if (ast_buffer == NULL)
		ast_buffer = &cout;

	*ast_buffer << std::fixed;
	*ast_buffer << std::setprecision(2);

	return *ast_buffer;
}

ostream & User_Options::get_output_buffer()
{
	if (output_buffer == NULL)
		output_buffer = &cout;

	*output_buffer << std::fixed;
	*output_buffer << std::setprecision(2);

	return *output_buffer;
}

ostream & User_Options::get_symtab_buffer()
{
	if (symtab_buffer == NULL)
		symtab_buffer = &cout;

	*symtab_buffer << std::fixed;
	*symtab_buffer << std::setprecision(2);

	return *symtab_buffer;
}

ostream & User_Options::get_rtl_buffer()
{
	if (rtl_buffer == NULL)
		rtl_buffer = &cout;

	*rtl_buffer << std::fixed;
	*rtl_buffer << std::setprecision(2);

	return *rtl_buffer;
}

ostream & User_Options::get_tac_buffer()
{
	if (tac_buffer == NULL)
		tac_buffer = &cout;

	*tac_buffer << std::fixed;
	*tac_buffer << std::setprecision(2);

	return *tac_buffer;
}

//-------------------------Changed on 25-02-2020-------------------------
ostream & User_Options::get_cfg_buffer()
{
	if(cfg_buffer == NULL)
		cfg_buffer = &cout;
	
	*cfg_buffer << std::fixed;
	*cfg_buffer << std::setprecision(2);

	return *cfg_buffer;
}
ostream & User_Options::get_cfg_dot_buffer()
{
	if(cfg_dot_buffer == NULL)
		cfg_dot_buffer = &cout;
	*cfg_dot_buffer << std::fixed;
	*cfg_dot_buffer << std::setprecision(2);

	return *cfg_dot_buffer;
}
ostream & User_Options::get_lv_buffer()//Rnd Work
{
	if(lv_buffer == NULL)
		lv_buffer = &cout;
	*lv_buffer << std::fixed;
	*lv_buffer << std::setprecision(2);

	return *lv_buffer;
}
ostream & User_Options::get_av_buffer()//Rnd Work
{
	if(av_buffer == NULL)
		av_buffer = &cout;
	*av_buffer << std::fixed;
	*av_buffer << std::setprecision(2);

	return *av_buffer;
}
ostream & User_Options::get_ssa_buffer()
{
	if(ssa_buffer == NULL)
		ssa_buffer = &cout;
	
	*ssa_buffer << std::fixed;
	*ssa_buffer << std::setprecision(2);

	return *ssa_buffer;
}
// ------------------------- till here --------------------------------

string User_Options::get_input_file_name()
{
	return source_file;
}

void User_Options::remove_files()
{
	remove(tokens_file_name.c_str());
	remove(ast_file_name.c_str());
	remove(output_file_name.c_str());
	
	remove(symtab_file_name.c_str());
	remove(rtl_file_name.c_str());
	remove(tac_file_name.c_str());

//-------------------------Changed on 7-1-2020-------------------------
	remove(cfg_file_name.c_str());
	remove(ssa_file_name.c_str());
	remove(cfg_dot_file_name.c_str());
	remove(lv_file_name.c_str());//Rnd Work
	remove(av_file_name.c_str());
// ------------------------ till here ---------------------------------
}

void User_Options::process_user_command_options(int argc, char * argv[])
{
	string input_file_name;

	bool user_input_file_entered = false;
	string user_input_file_name;
	char * user_input_file_c_string = NULL;

	const string usage = "\n     Usage: sclp [options] [file]\n\
                          Options:\n\
                          -help     Show this help\n\
                          -sa-scan  Stop after scanning\n\
                          -sa-parse Stop after parsing\n\
                          -sa-ast   Stop after constructing Abstract Syntac Tree (AST)\n\
                          -sa-tac   Stop after constructing Three Address Code (TAC)\n\
			  -sa-cfg   Stop after constructing Control Flow Graph(CFG) representation\n\
                          -sa-rtl   Stop after constructing Register Transfer Language (RTL) code\n\
                          -tokens   Show the tokens in file.toks (or out.toks)\n\
                          -ast      Show abstract syntax trees in file.ast (or out.ast)\n\
                          -tac      Show the Three Address Code in file.tac (or out.tac)\n\
			  -cfg      Show control flow graph in file.cfg (or out.cfg)\n\
                          -draw-cfg Show Graphical Representation of CFG in file.cfg.dot\n\
                          -lv       Show the result of live variables analysis in file.lv (or out.lv)\n\
                          -av       Show the result of available variables analysis in file.av (or out.av)\n\
                          -rtl	    Show the Register Transfer Language code in file.rtl (or out.rtl)\n\
                          -symtab   Show the symbol table after RTL consruction (when offsets are\n\
                                    allocated) in file.sym, (or out.sym)\n\
                          -d        Demo version. Use stdout for the outputs instead of files\n\n";


	for (int i = 1; i < argc; i++)
	{    
		char * option = strdup(argv[i]);
		if (!strncmp(option,"-",1))
		{
			if (!strcmp(option,"-d"))
				demo_mode_selected = true;

			else if (!strcmp(option, "-tokens"))
				show_tokens_selected = true;

			else if (!strcmp(option, "-ast")) 
			{
				this_version.needs_phase(phase_ast);
				show_ast_selected = true;
			}

			else if (!strcmp(option, "-symtab"))
				show_symtab_selected = true;

			else if (!strcmp(option, "-tac"))
			{
				this_version.needs_phase(phase_tac);
				show_tac_selected = true;
			}

//-------------------------Changed on 7-1-2020-------------------------
			else if (!strcmp(option, "-cfg"))
			{
				show_cfg_selected = true;
			}
                        else if (!strcmp(option, "-draw-cfg"))
			{
				show_draw_cfg_dot_selected = true;
			}
			else if (!strcmp(option ,"-lv")) //RnD Work
			{
				lv_selected = true;
			}
			else if(!strcmp(option, "-av"))
			{
				av_selected = true; 
			}
// ------------------------ till here ---------------------------------
			else if (!strcmp(option, "-rtl"))
			{
				this_version.needs_phase(phase_rtl);
				show_rtl_selected = true;
			}

			else if (!strcmp(option, "-sa-scan"))
				stop_after_scan = true;	

			else if (!strcmp(option, "-sa-parse"))
				stop_after_parse = true;	

			else if (!strcmp(option, "-sa-ast"))
				stop_after_ast = true;	

			else if (!strcmp(option, "-sa-tac"))
				stop_after_tac = true;	

//-------------------------Changed on 25-02-2020-------------------------
			else if (!strcmp(option, "-sa-cfg"))
				stop_after_cfg = true;

			else if (!strcmp(option, "-sa-rtl"))
				stop_after_rtl = true;	

			else if (!(strcmp(option,"--help")) || !(strcmp(option,"-help")))
			{        
				cerr << usage;
				exit(0);
			}
			else 
			{
				string error_message = "Unknown option `" + string(option) + "'" + usage;
				CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, error_message, NO_FILE_LINE);
			}
		}
		else if (user_input_file_entered)
		{
			string error_message = "Only one input file name can be provided " + usage;
			CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, error_message, NO_FILE_LINE);
		}
		else    
		{    
			user_input_file_entered = true;
			user_input_file_c_string = strdup(option);
		}
	}

	if (user_input_file_entered == true)
	{
		CHECK_INVARIANT (user_input_file_c_string, "Input file string cannot be NULL");

		user_input_file_name = string (user_input_file_c_string);
		source_file = user_input_file_name;
		input_file_name = user_input_file_name;
	}
	else
		user_input_file_name = "out";

	CHECK_INVARIANT(user_input_file_name.c_str(), "Input file name cannot be NULL");

	tokens_file_name = user_input_file_name + ".toks"; 
	ast_file_name = user_input_file_name + ".ast"; 
	symtab_file_name = user_input_file_name + ".sym";
	rtl_file_name = user_input_file_name + ".rtl";
	tac_file_name = user_input_file_name + ".tac";

//-------------------------Changed on 25-02-2020-------------------------
	cfg_file_name = user_input_file_name + ".cfg";
	cfg_dot_file_name = user_input_file_name+".cfg.dot";
	ssa_file_name = user_input_file_name + ".ssa";
	lv_file_name = user_input_file_name + ".lv";
	av_file_name = user_input_file_name + ".av";
// ----------------------- till here ---------------------------------

	output_file_name = user_input_file_name + ".spim";

	remove (tokens_file_name.c_str()); 
	remove (ast_file_name.c_str()); 
	remove (output_file_name.c_str()); 
	remove (symtab_file_name.c_str());
	remove (rtl_file_name.c_str());
	remove (tac_file_name.c_str());

//-------------------------Changed on 25-02-2020-------------------------
	remove (cfg_file_name.c_str());
	remove (ssa_file_name.c_str());
	remove (cfg_dot_file_name.c_str());
	remove (lv_file_name.c_str());//Rnd Work
	remove (av_file_name.c_str());
// ----------------------- till here ----------------------------------

	remove ("out.toks");
	remove ("out.ast");
	remove ("out.sym");
	remove ("out.rtl");
	remove ("out.tac");

	set_stop_after(this_version);

//-------------------------Changed on 25-02-2020-------------------------
	remove ("out.cfg");
	remove ("out.ssa");
	remove ("out.cfg.dot");
	remove ("out.lv");//Rnd Work
	remove ("out.av");
	
// ------------------------ till here ---------------------------------

	if (demo_mode_selected == false)
	{
		if (show_tokens_selected)
			tokens_file_buffer.open(tokens_file_name.c_str()); 

		if (show_ast_selected)
			ast_file_buffer.open(ast_file_name.c_str()); 

		if (show_symtab_selected)
			symtab_file_buffer.open(symtab_file_name.c_str());

		if (show_rtl_selected)
			rtl_file_buffer.open(rtl_file_name.c_str());

		if (show_tac_selected)
			tac_file_buffer.open(tac_file_name.c_str());

//-------------------------Changed on 25-02-2020-------------------------
		if (show_cfg_selected){
			cfg_file_buffer.open(cfg_file_name.c_str());
		}
		if (show_draw_cfg_dot_selected)
			cfg_dot_file_buffer.open(cfg_dot_file_name.c_str());
		if (show_ssa_selected)
			ssa_file_buffer.open(ssa_file_name.c_str());
		if(lv_selected)//Rnd Work
			lv_file_buffer.open(lv_file_name.c_str());
		if(av_selected)
			av_file_buffer.open(av_file_name.c_str());
// ------------------------ till here ---------------------------------

		if (construct_asm())
			output_file_buffer.open(output_file_name.c_str()); 

		if (show_tokens_selected && !tokens_file_buffer)
		        CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Unable to open output file for tokens");

		if (show_ast_selected && !ast_file_buffer)
		        CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Unable to open output file for AST");

		if (show_symtab_selected && !symtab_file_buffer)
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Unable to open output file for symtab");

		if (show_rtl_selected && !rtl_file_buffer)
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Unable to open output file for rtl");

		if (show_tac_selected && !tac_file_buffer)
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Unable to open output file for tac");

//-------------------------Changed on 25-02-2020-------------------------
		if (show_cfg_selected && !cfg_file_buffer)
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Unable to open output file for cfg");
		if (show_ssa_selected && !ssa_file_buffer)
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Unable to open output file for ssa");

// ----------------------- till here ----------------------------------
	}

 
        if(is_show_tokens_selected())
          	command_options.create_tokens_buffer();
        if(is_show_ast_selected())
          	command_options.create_ast_buffer();
        if(is_show_tac_selected())
          	command_options.create_tac_buffer();

//-------------------------Changed on 7-1-2020-------------------------
	if(is_show_cfg_selected()){
		command_options.create_cfg_buffer();
	}
	if(is_show_draw_cfg_dot_selected()){
		command_options.create_cfg_dot_buffer();
	}
	if(is_show_ssa_selected())
		command_options.create_ssa_buffer();
	if(is_lv_selected())
		command_options.create_lv_buffer();
	if(is_av_selected())
		command_options.create_av_buffer();
// ----------------------- till here ----------------------------------

        if(is_show_rtl_selected())
          	command_options.create_rtl_buffer();
        if(is_show_symtab_selected())
          	command_options.create_symtab_buffer();


      	yyin = fopen(input_file_name.c_str(),"r");

	CHECK_INPUT_AND_ABORT((yyin), "Cannot open the input file", NO_FILE_LINE);
}

void User_Options::set_stop_after(Implementation_Version v)
{
	phase_id pid = v.get_last_phase();
	switch (pid)
	{
		case phase_scan: stop_after_scan = true; break;
		case phase_parse: stop_after_parse = true; break;
		case phase_ast: stop_after_ast = true; break;
		case phase_tac: stop_after_tac = true; break;
		case phase_cfg: stop_after_cfg = true; break;
		case phase_ssa: stop_after_ssa = true; break;
		case phase_rtl: stop_after_rtl = true; break;
		case phase_asm: break;
		default:
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH,"Wrong phase id");
	}
}
