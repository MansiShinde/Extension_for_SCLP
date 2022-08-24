/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    CLASS = 258,
    PRIVATE = 259,
    PUBLIC = 260,
    INTEGER_NUMBER = 261,
    DOUBLE_NUMBER = 262,
    NAME = 263,
    STRING_CONSTANT = 264,
    INTEGER = 265,
    FLOAT = 266,
    STRING = 267,
    BOOL = 268,
    ASSIGN = 269,
    VOID = 270,
    IF = 271,
    WHILE = 272,
    DO = 273,
    ELSE = 274,
    WRITE = 275,
    READ = 276,
    RETURN = 277,
    AT = 278,
    OR = 279,
    AND = 280,
    NE = 281,
    EQ = 282,
    LT = 283,
    LE = 284,
    GT = 285,
    GE = 286,
    NOT = 287,
    UMINUS = 288
  };
#endif
/* Tokens.  */
#define CLASS 258
#define PRIVATE 259
#define PUBLIC 260
#define INTEGER_NUMBER 261
#define DOUBLE_NUMBER 262
#define NAME 263
#define STRING_CONSTANT 264
#define INTEGER 265
#define FLOAT 266
#define STRING 267
#define BOOL 268
#define ASSIGN 269
#define VOID 270
#define IF 271
#define WHILE 272
#define DO 273
#define ELSE 274
#define WRITE 275
#define READ 276
#define RETURN 277
#define AT 278
#define OR 279
#define AND 280
#define NE 281
#define EQ 282
#define LT 283
#define LE 284
#define GT 285
#define GE 286
#define NOT 287
#define UMINUS 288

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 7 "class_parser.y" /* yacc.c:1909  */

	int integer_value;
	double double_value;
	char *string_value;


#line 127 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
