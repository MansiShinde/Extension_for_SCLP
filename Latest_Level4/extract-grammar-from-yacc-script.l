%{
#include <stdio.h>

	/* ASSUMPTION:
           All actions in the yacc script must begin on a new line for this script 
           to produce new lines correctly in the output. 
        */

int preamble = 1;
int depth=0;
int old_depth=0;

%}
%%
^[%][%] {
		preamble=0;
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
		
