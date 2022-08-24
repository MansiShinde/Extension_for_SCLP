%{
# include "y.tab.h"
#include <stdio.h>
#include <string.h>
%}
%option noyywrap
digit [0-9]
%%

class    { return CLASS; }
public    {return PUBLIC; }
private    {return PRIVATE; }

do       {    return DO;}
while    {   return WHILE;}
if       {    return IF; }
else     {     return ELSE;}
int	 {	     
	      return INTEGER; 
	 }
float	 {
	      
	      return FLOAT;
	 }
bool	 {
	      return BOOL;
	 }
string   {
	      return STRING;
	 }
void	 {
	      return VOID;
	 }
print    {
               return WRITE;
         }

read     {
               return READ;
         }
main     {
                yylval.string_value = strdup(yytext);
                return NAME;
         }
return   {
                return RETURN;
         }
"="	 {
		return ASSIGN;
	 }
">"      {       return GT;}
"<"      {      return LT;}
">="     {    return GE;}
"<="     {     return LE;}
"!="     {     return NE;}
"=="     {     return EQ;}
"+"|"-"|"*"|"/"	{
		return *yytext;
	 }
"&&"     {        return AND; }
"||"     {       return OR;}
"!"      {       return NOT; }
[{}();,?:.]	{
                return *yytext;
	 }
{digit}+[.]{digit}*|{digit}*[.]{digit}+ 	{
		yylval.double_value = strtod(yytext,NULL);
		return DOUBLE_NUMBER;
		}
[[:digit:]]+ 		{ 
	        yylval.integer_value = atoi(yytext);
	        return INTEGER_NUMBER; 
	        }
["][^"\n]*["]	{
                yylval.string_value =strdup(yytext);
		return STRING_CONSTANT;
		}

[[:alpha:]_][[:alpha:][:digit:]_]* {
                yylval.string_value = strdup(yytext);
                return NAME; 
		}
[@] 		{ 
	        return AT; 
	        }

[ \t]*   {}
\n    	 {}
\/\/.*   {}

%%



