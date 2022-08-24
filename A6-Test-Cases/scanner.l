
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

%{

#include "common-classes.hh"
#include "local-environment.hh"
#include "user-options.hh"
#include "error-display.hh"
#include "symbol-table.hh"
#include "ast.hh"
#include "basic-block.hh"
#include "procedure.hh"
#include "program.hh"
#include "store-tokens.hh"
#include "parser.tab.h"

#include<iostream>

%}
%option noyywrap
digit [0-9]
%%
[ \t]*   {}
else     {    store_token_name("ELSE",yytext,yylineno); return ELSE;}
if       {    store_token_name("IF",yytext,yylineno); return IF; }
do       {    store_token_name("DO",yytext,yylineno); return DO;}
while    {    store_token_name("WHILE",yytext,yylineno); return WHILE;}
int		{
			store_token_name("INTEGER",yytext,yylineno);
			return INTEGER; 
		}
float	{
			store_token_name("FLOAT",yytext,yylineno);
			return FLOAT;
		}

void	{
			store_token_name("VOID",yytext,yylineno);
			return VOID;
		}
print   {
            store_token_name("PRINT",yytext,yylineno);
                  return PRINT;
        }
main    {
	          store_token_name("NAME",yytext,yylineno);
                   yylval.string_value = new std::string(yytext);
                 return NAME;
        }
return   {
             store_token_name("RETURN",yytext,yylineno);
                  return RETURN;
        }

{digit}+[.]{digit}*|{digit}*[.]{digit}+ 	{
				store_token_name("FNUM",yytext,yylineno);
				yylval.double_value = strtod(yytext,NULL);
				return DOUBLE_NUMBER;
			}
[[:digit:]]+ 	{ 
		                 store_token_name("NUM",yytext,yylineno);
				           yylval.integer_value = atoi(yytext);
				            return INTEGER_NUMBER; 
			          }

[[:alpha:]_][[:alpha:][:digit:]_]* {
					store_token_name("NAME",yytext,yylineno);
					       int i;
					    for(i=0;yytext[i]!='\0';i++);
                            yytext[i]='_';
                            yytext[i+1]='\0';
                   yylval.string_value = new std::string(yytext);
                 return NAME; 
				}
"="	{
		store_token_name("ASSIGN_OP",yytext,yylineno);
		return ASSIGN;
	}
">"   {  store_token_name("GREATER_THAN",yytext,yylineno); return GT;}
"<"   {  store_token_name("LESS_THAN",yytext,yylineno); return LT;}
">="  {  store_token_name("GREATER_THAN_EQUAL",yytext,yylineno); return GE;}
"<="  {  store_token_name("LESS_THAN_EQUAL",yytext,yylineno); return LE;}
"!="  {  store_token_name("NOT_EQUAL",yytext,yylineno); return NE;}
"=="  {   store_token_name("EQUAL",yytext,yylineno); return EQ;}
"+"|"-"|"*"|"/"	{
			store_token_name("ARITHOP",yytext,yylineno);
			return *yytext;
		}
"&&"   { store_token_name("AND",yytext,yylineno);  return AND; }
"||"   { store_token_name("OR",yytext,yylineno); return OR;}
"!"     { store_token_name("NOT",yytext,yylineno); return NOT; }
[{}();,?:]	{
			store_token_name("META CHAR",yytext,yylineno);
                            return *yytext;
		}

\n    		  { }
\/\/.*        { }

.		{ 
			string error_message;
			error_message =  "Illegal character `" + *yytext;
			error_message += "' on line " + yylineno;
			
			CHECK_INPUT(CONTROL_SHOULD_NOT_REACH, error_message, yylineno);
		}
%%
