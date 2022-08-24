%{
#include <stdio.h>

	/* ASSUMPTION:
           All actions in the yacc script must begin on a new line for this script 
           to produce new lines correctly in the output. 
        */

%}

ws [ \t]
let [a-zA-Z_]
dig [0-9]
id {let}({let}|{dig})*
%%
{ws}*\n{ws}+/(":"|"|")     { 
				printf ("\n\t");
      			}
{ws}*\n{ws}+/[^:\|]     { 
				printf (" ");
      			}
.		{ 
			ECHO;
		}
