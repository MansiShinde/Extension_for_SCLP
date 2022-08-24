%{
#include <stdio.h>

	/* REQUIREMENTS:
           - All actions in the yacc script must begin on a new line for this script 
             to produce new lines correctly in the output. 
           - There should be no C style block comments of grammar rules
        */

int preamble = 1;
int depth=0;
int old_depth=0;

%}
%%
^[%][%] {
		if (preamble)
			preamble=0;
		else
			preamble=1;
	}
[']"{"[']|[']"}"[']	{ 
		if (!preamble)
			ECHO; 
		}
[\t ]*"{"    	{	
		old_depth = depth++;
	}
"}"   	{	
		old_depth = depth--; 
	}
.     { if ((depth == 0) && (!preamble))
		ECHO;
	else if (depth < 0)
	{	
		fprintf (stderr,"Error: Negative depth\n");
		exit(1);
	}
	else
		; /* ignore */
      }
\n     { 
	if ((depth == 0) && (old_depth == 1))
	{
		old_depth = 0;
	}
	else if ((depth == 0) && (old_depth == 0) && (!preamble))
	{
		ECHO;
	}
	else if (depth < 0)
	{	
		fprintf (stderr,"Error: Negative depth\n");
		exit(1);
	}
	else
		; /* ignore */
      }
		
