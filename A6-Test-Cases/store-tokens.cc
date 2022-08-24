/*********************************************************************************************

                                sclp : A C Language Processor
                                --------------------------------

           About:

           Implemented   by  Tanu  Kanvar (tanu@cse.iitb.ac.in) and Uday
           Khedker    (http://www.cse.iitb.ac.in/~uday)  for the courses
           cs302+cs316: Language  Processors  (theory and  lab)  at  IIT
           Bombay.

           Initial release  date  Jan  15, 2013.  

           Copyrights reserved  by Uday Khedker. This  implemenation has
           been made available purely  for academic purposes without any
           warranty of any kind.


***********************************************************************************************/

#include "user-options.hh"
#include <string>
void store_token_name(std::string token_name, char *lexeme,int lineno)
{
           if(command_options.is_show_tokens_selected()) {
	ostream & file_buffer = command_options.get_tokens_buffer();
	file_buffer << "\tToken Name: " << token_name;
         file_buffer << " \tToken: " << lexeme;
         file_buffer <<" \t Lineno: "<< lineno <<"\n";
        }
}
