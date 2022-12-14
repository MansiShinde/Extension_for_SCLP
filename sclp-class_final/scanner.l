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

%{

#include "common-headers.hh"
#include "parser.tab.h"

#include<iostream>


%}
%option noyywrap
digit [0-9]
%%

class    {store_token_name("CLASS",yytext,yylineno); return CLASS; }
public    {store_token_name("PUBLIC",yytext,yylineno); return PUBLIC; }
private    {store_token_name("PRIVATE",yytext,yylineno); return PRIVATE; }

"::"       {store_token_name("COCO",yytext,yylineno); return COCO;}

"."        {store_token_name("DOT",yytext,yylineno); return DOT;}



do       {    store_token_name("DO",yytext,yylineno); 
	      this_version.needs_level(L4);
              return DO;}
while    {    store_token_name("WHILE",yytext,yylineno); 
	      this_version.needs_level(L4);
	      return WHILE;}
if       {    store_token_name("IF",yytext,yylineno); 
	      this_version.needs_level(L4);
	      return IF; }
else     {    store_token_name("ELSE",yytext,yylineno); 
	      this_version.needs_level(L4);
	      return ELSE;}
int	 {
	      store_token_name("INTEGER",yytext,yylineno);
	      return INTEGER; 
	 }
float	 {
	      store_token_name("FLOAT",yytext,yylineno);
	      return FLOAT;
	 }
bool	 {
	      store_token_name("BOOL",yytext,yylineno);
	      return BOOL;
	 }
string   {
	      store_token_name("STRING",yytext,yylineno);
	      return STRING;
	 }
void	 {
	      store_token_name("VOID",yytext,yylineno);
	      return VOID;
	 }
print    {
               store_token_name("WRITE",yytext,yylineno);
               return WRITE;
         }

read     {
               store_token_name("READ",yytext,yylineno);
               return READ;
         }
main     {
	        store_token_name("NAME",yytext,yylineno);
                yylval.string_value = new std::string(yytext);
                return NAME;
         }
return   {
                store_token_name("RETURN",yytext,yylineno);
		this_version.needs_level(L5);
                return RETURN;
         }
"="	 {
	        store_token_name("ASSIGN_OP",yytext,yylineno);
		return ASSIGN;
	 }
">"      {      store_token_name("GREATER_THAN",yytext,yylineno); 
	        this_version.needs_level(L3);
	        return GT;}
"<"      {      store_token_name("LESS_THAN",yytext,yylineno); 
	        this_version.needs_level(L3);
                return LT;}
">="     {      store_token_name("GREATER_THAN_EQUAL",yytext,yylineno); 
	        this_version.needs_level(L3);
                return GE;}
"<="     {      store_token_name("LESS_THAN_EQUAL",yytext,yylineno); 
	        this_version.needs_level(L3);
                return LE;}
"!="     {      store_token_name("NOT_EQUAL",yytext,yylineno); 
	        this_version.needs_level(L3);
                return NE;}
"=="     {      store_token_name("EQUAL",yytext,yylineno); 
	        this_version.needs_level(L3);
                return EQ;}
"+"	{
	        store_token_name("PLUS",yytext,yylineno);
	        this_version.needs_level(L2);
                return *yytext;
	 }
"-"	{
	        store_token_name("MINUS",yytext,yylineno);
	        this_version.needs_level(L2);
                return *yytext;
	 }
"*"	{
	        store_token_name("MULT",yytext,yylineno);
	        this_version.needs_level(L2);
                return *yytext;
	 }
"/"	{
	        store_token_name("DIV",yytext,yylineno);
	        this_version.needs_level(L2);
                return *yytext;
	 }
"&&"     {      store_token_name("AND",yytext,yylineno);  
		this_version.needs_level(L3);
                return AND; }
"||"     {      store_token_name("OR",yytext,yylineno); 
		this_version.needs_level(L3);
                return OR;}
"!"      {      store_token_name("NOT",yytext,yylineno); 
		this_version.needs_level(L3);
                return NOT; }
"&"      {      store_token_name("ADDRESSOF",yytext,yylineno); 
		this_version.needs_level(L3);
                return ADDRESSOF; }
[{}()\]\[]	{
		store_token_name("DELIMITERS",yytext,yylineno);
                return *yytext;
	 }
[;,]	{
		store_token_name("PUNCTUATION",yytext,yylineno);
                return *yytext;
	 }
[?:]	{
		store_token_name("OP_FOR_TERNARY_EXPR",yytext,yylineno);
		this_version.needs_level(L3);
                return *yytext;
	 }
{digit}+[.]{digit}*|{digit}*[.]{digit}+ 	{
		store_token_name("FNUM",yytext,yylineno);
		yylval.double_value = strtod(yytext,NULL);
		return DOUBLE_NUMBER;
		}
[[:digit:]]+ 		{ 
	        store_token_name("NUM",yytext,yylineno);
	        yylval.integer_value = atoi(yytext);
	        return INTEGER_NUMBER; 
	        }
["][^"\n]*["]	{
		store_token_name("STR_CONST",yytext,yylineno);

		int i;
		for(i=0;yytext[i]!='\0';i++)
		{
			if (yytext[i+1] != '"')
				yytext[i]=yytext[i+1];
			else
                		yytext[i]='\0';
		}
                yylval.string_value = new std::string(yytext);
		return STRING_CONSTANT;
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
[ \t]*   {}
\n    	 {}
\/\/.*   {}
.	 { 
		string error_message;
		error_message =  "Illegal character \'" + string(yytext) + "\'";
		
		CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, error_message, yylineno);
	 }
%%
