%{
# include "y.tab.h"

%}

digit [0-9]
letter [a-zA-Z_]
operator [-+*/]
%%
int				{ 
					printf ("token is INT"); 
					return INT; 
				}
void				{ 
					printf ("token is VOID"); 
					return VOID; 
				}
{letter}({letter}|{digit})*    	{ 
					printf ("token is %s\t",yytext); 
					return NAME; 
				}
[ ]|\t				{ 
					printf (" "); 
				}
\n				{ 
					printf ("line number = %d\n", yylineno);
					yylineno++;
				}
. 				{ 
					printf ("token is %c", yytext[0]); 
					return yytext[0]; 
				}

