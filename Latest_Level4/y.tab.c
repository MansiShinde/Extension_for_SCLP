/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 39 "parser.y" /* yacc.c:339  */

extern "C" void yyerror(char *s);
extern int yylineno;
extern int yylex(void);

#include "common-classes.hh"
#include "error-display.hh"
#include "user-options.hh"
#include "symbol-table.hh"
#include "tac.hh"
#include "ast.hh"
#include "rtl.hh"
#include "procedure.hh"
#include "program.hh"

#include <iostream>
Procedure * current_procedure = NULL;	/* This global variable is used through out the script */
int dummy_no=0;

#line 86 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
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
#line 60 "parser.y" /* yacc.c:355  */

	int integer_value;
	double double_value;
	std::string * string_value;
	list<Ast *> * ast_list;
	Ast * ast;
	Symbol_Table * symbol_table;
	Symbol_Table_Entry * symbol_entry;
	Procedure * procedure;
  Data_Type type_name;

#line 204 "y.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 221 "y.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  17
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   269

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  45
/* YYNRULES -- Number of rules.  */
#define YYNRULES  103
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  200

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   288

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      40,    41,    36,    34,    45,    35,     2,    37,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    25,    44,
       2,     2,     2,    24,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    42,     2,    43,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    26,
      27,    28,    29,    30,    31,    32,    33,    38,    39
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   142,   142,   151,   160,   173,   205,   215,   220,   225,
     230,   234,   235,   241,   260,   274,   294,   303,   308,   322,
     323,   328,   377,   327,   407,   435,   406,   468,   477,   484,
     497,   501,   505,   512,   530,   537,   541,   545,   549,   553,
     557,   561,   565,   569,   576,   583,   597,   601,   608,   616,
     627,   634,   642,   652,   656,   666,   670,   698,   714,   718,
     722,   726,   730,   734,   740,   750,   766,   774,   782,   793,
     801,   811,   821,   831,   848,   856,   869,   880,   889,   898,
     907,   916,   925,   932,   945,   956,   967,   974,   984,   993,
    1002,  1011,  1020,  1029,  1041,  1052,  1063,  1074,  1078,  1082,
    1089,  1111,  1115,  1119
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "CLASS", "PRIVATE", "PUBLIC",
  "INTEGER_NUMBER", "DOUBLE_NUMBER", "NAME", "STRING_CONSTANT", "INTEGER",
  "FLOAT", "STRING", "BOOL", "ASSIGN", "VOID", "IF", "WHILE", "DO", "ELSE",
  "WRITE", "READ", "RETURN", "AT", "'?'", "':'", "OR", "AND", "NE", "EQ",
  "LT", "LE", "GT", "GE", "'+'", "'-'", "'*'", "'/'", "NOT", "UMINUS",
  "'('", "')'", "'{'", "'}'", "';'", "','", "$accept", "program",
  "global_decl_statement", "class_declaration_stat", "block",
  "access_specifier_type", "func_decl", "dummy_formal_param_list",
  "dummy_formal_param", "func_def_list", "func_def", "$@1", "$@2", "$@3",
  "$@4", "formal_param_list", "formal_param", "param_type",
  "statement_list", "statement", "call_statement", "func_call",
  "actual_arg_list", "non_empty_arg_list", "actual_arg",
  "return_statement", "optional_local_var_decl_list", "var_decl_list",
  "var_decl", "type", "variables_list", "assignment_statement",
  "if_statement", "do_while_statement", "while_statement",
  "compound_statement", "print_statement", "read_statement",
  "arith_expression", "operand", "expression_term", "rel_expression",
  "logical_expression", "variable", "constant", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,    63,    58,   279,   280,   281,   282,
     283,   284,   285,   286,    43,    45,    42,    47,   287,   288,
      40,    41,   123,   125,    59,    44
};
# endif

#define YYPACT_NINF -102

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-102)))

#define YYTABLE_NINF -51

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     195,    -3,  -102,  -102,  -102,  -102,  -102,  -102,    26,   243,
    -102,  -102,   243,  -102,  -102,    23,     0,  -102,  -102,   243,
    -102,  -102,    53,    27,    -4,    10,    41,   147,  -102,    90,
    -102,  -102,    61,    82,   155,  -102,  -102,  -102,    64,    -7,
    -102,    31,  -102,   103,  -102,    68,   195,  -102,    34,   103,
    -102,    71,    81,   201,    83,   201,  -102,  -102,   243,   107,
    -102,   243,  -102,  -102,  -102,  -102,    84,  -102,    92,  -102,
     243,  -102,   131,   243,   159,  -102,  -102,  -102,  -102,    64,
      58,    74,  -102,    83,   114,   119,   121,   120,    38,   135,
      62,  -102,  -102,  -102,  -102,   127,  -102,  -102,  -102,  -102,
    -102,  -102,  -102,  -102,   167,   102,   140,   140,   140,   156,
    -102,   139,   141,   142,  -102,  -102,  -102,   135,   140,   140,
     140,  -102,   149,   213,  -102,  -102,   233,  -102,  -102,   113,
    -102,   144,  -102,   123,   152,  -102,    60,  -102,   150,   163,
     173,  -102,  -102,  -102,  -102,  -102,  -102,   189,   168,  -102,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,  -102,   171,   183,     1,  -102,   140,   120,
     120,   140,  -102,  -102,   213,   213,   213,   213,   213,   213,
      50,    50,  -102,  -102,   203,   202,  -102,  -102,  -102,  -102,
    -102,   242,  -102,   175,   140,   120,   208,   213,  -102,  -102
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,    63,    58,    59,    61,    62,    60,     0,     0,
       8,     7,     3,    20,     6,     0,     0,     1,     4,     2,
       5,    19,     0,    64,     0,     0,     0,     0,    57,     0,
      12,    11,     0,     0,     0,    30,    31,    32,    24,     0,
      17,     0,    28,    18,    65,     0,     0,    24,     0,     0,
      14,     0,     0,     0,    21,     0,    29,     9,    10,     0,
      21,    53,    13,    16,    18,    15,     0,    27,    64,    25,
      54,    55,     0,    53,     0,    34,    56,    64,    22,     0,
       0,     0,    34,     0,   100,     0,     0,     0,     0,     0,
       0,    34,    26,    33,    42,     0,    43,    35,    36,    37,
      38,    39,    40,    41,     0,     0,    47,     0,     0,     0,
     100,     0,     0,     0,   101,   102,   103,     0,     0,     0,
       0,    51,    85,     0,    84,    98,     0,    86,    87,     0,
      44,     0,    23,     0,    46,    49,    85,    85,     0,     0,
       0,    75,    74,    76,    99,    81,    96,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    73,     0,    85,     0,    45,     0,     0,
       0,     0,    82,    97,    92,    93,    88,    89,    90,    91,
      77,    78,    79,    80,     0,    95,    94,    67,    66,    68,
      48,    70,    72,     0,     0,     0,     0,    83,    69,    71
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -102,  -102,   216,  -102,  -102,  -102,     2,  -102,   210,   255,
      11,  -102,  -102,  -102,  -102,   -30,   211,   -18,   -79,   -86,
    -102,   134,  -102,  -102,    99,  -102,   196,  -102,     4,    20,
    -102,  -102,  -102,  -102,  -102,  -102,  -102,  -102,   -88,  -101,
    -102,  -102,   -98,   -81,  -102
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     8,     9,    10,    32,    33,    11,    39,    40,    12,
      13,    66,    82,    51,    75,    41,    42,    43,    81,    93,
      94,    95,   133,   134,   135,    96,    69,    70,    14,    72,
      24,    97,    98,    99,   100,   101,   102,   103,   137,   123,
     124,   125,   126,   127,   128
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     104,   109,   122,   105,    48,    16,   104,   112,   113,   138,
     139,    18,   129,    20,    30,    31,    49,   145,   136,   147,
      15,   146,   148,    21,   104,   160,    17,   161,   162,    15,
      21,    23,    22,   166,    52,    64,   144,    49,    53,    22,
      28,    29,    25,   165,    80,   189,   110,   111,   104,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
      18,    26,    20,   185,   186,    71,    59,    27,   114,   115,
     110,   116,    54,   193,    76,    60,    55,    71,    59,    55,
     136,    34,    84,   191,   192,   117,   158,   159,   104,   104,
      85,    86,    87,   197,    88,    89,    90,   118,    44,    83,
     119,   -50,   120,    55,    45,   -50,   121,    46,    50,   198,
      84,    56,    57,    61,   104,    68,    91,    92,    85,    86,
      87,    84,    88,    89,    90,    62,    73,    65,    84,    85,
      86,    87,    74,    88,    89,    90,    85,    86,    87,    77,
      88,    89,    90,   110,    91,   132,   114,   115,   110,   116,
     114,   115,    84,   116,   106,    91,   163,    35,    36,   107,
      37,   108,    91,   117,   167,    35,    36,   117,    37,    35,
      36,   130,    37,   140,   160,   118,   161,   162,   119,   118,
     120,   131,   119,   141,   120,   142,   143,   160,    38,   161,
     162,   169,   160,   149,   161,   162,    47,   168,     1,   160,
      79,   161,   162,     2,   170,     3,     4,     5,     6,   173,
       7,    35,    36,   171,    37,   187,   196,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   188,   194,   162,
     172,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,     2,   199,     3,     4,     5,     6,   160,     7,   161,
     162,   195,    58,    63,    19,   164,    67,   190,     0,    78
};

static const yytype_int16 yycheck[] =
{
      81,    87,    90,    82,    34,     8,    87,    88,    89,   107,
     108,     9,    91,     9,     4,     5,    34,   118,   106,   120,
       0,   119,   120,    12,   105,    24,     0,    26,    27,     9,
      19,     8,    12,   131,    41,    53,   117,    55,    45,    19,
      44,    45,    42,   131,    74,    44,     8,     9,   129,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
      58,     8,    58,   161,   162,    61,    46,    40,     6,     7,
       8,     9,    41,   171,    70,    41,    45,    73,    58,    45,
     168,    40,     8,   169,   170,    23,    36,    37,   169,   170,
      16,    17,    18,   194,    20,    21,    22,    35,     8,    41,
      38,    41,    40,    45,    43,    45,    44,    25,    44,   195,
       8,     8,    44,    42,   195,     8,    42,    43,    16,    17,
      18,     8,    20,    21,    22,    44,    42,    44,     8,    16,
      17,    18,    40,    20,    21,    22,    16,    17,    18,     8,
      20,    21,    22,     8,    42,    43,     6,     7,     8,     9,
       6,     7,     8,     9,    40,    42,    43,    10,    11,    40,
      13,    40,    42,    23,    41,    10,    11,    23,    13,    10,
      11,    44,    13,    17,    24,    35,    26,    27,    38,    35,
      40,    14,    38,    44,    40,    44,    44,    24,    41,    26,
      27,    41,    24,    44,    26,    27,    41,    45,     3,    24,
      41,    26,    27,     8,    41,    10,    11,    12,    13,    41,
      15,    10,    11,    40,    13,    44,    41,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    44,    25,    27,
      41,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,     8,    44,    10,    11,    12,    13,    24,    15,    26,
      27,    19,    46,    53,     9,   131,    55,   168,    -1,    73
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     8,    10,    11,    12,    13,    15,    47,    48,
      49,    52,    55,    56,    74,    75,     8,     0,    52,    55,
      74,    56,    75,     8,    76,    42,     8,    40,    44,    45,
       4,     5,    50,    51,    40,    10,    11,    13,    41,    53,
      54,    61,    62,    63,     8,    43,    25,    41,    61,    63,
      44,    59,    41,    45,    41,    45,     8,    44,    48,    75,
      41,    42,    44,    54,    63,    44,    57,    62,     8,    72,
      73,    74,    75,    42,    40,    60,    74,     8,    72,    41,
      61,    64,    58,    41,     8,    16,    17,    18,    20,    21,
      22,    42,    43,    65,    66,    67,    71,    77,    78,    79,
      80,    81,    82,    83,    89,    64,    40,    40,    40,    65,
       8,     9,    89,    89,     6,     7,     9,    23,    35,    38,
      40,    44,    84,    85,    86,    87,    88,    89,    90,    64,
      44,    14,    43,    68,    69,    70,    84,    84,    88,    88,
      17,    44,    44,    44,    89,    85,    88,    85,    88,    44,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      24,    26,    27,    43,    67,    84,    88,    41,    45,    41,
      41,    40,    41,    41,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    88,    88,    44,    44,    44,
      70,    65,    65,    88,    25,    19,    41,    85,    65,    44
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    46,    47,    47,    48,    48,    48,    48,    48,    49,
      50,    51,    51,    52,    52,    52,    53,    53,    54,    55,
      55,    57,    58,    56,    59,    60,    56,    61,    61,    62,
      63,    63,    63,    64,    64,    65,    65,    65,    65,    65,
      65,    65,    65,    65,    66,    67,    68,    68,    69,    69,
      70,    71,    71,    72,    72,    73,    73,    74,    75,    75,
      75,    75,    75,    75,    76,    76,    77,    77,    77,    78,
      78,    79,    80,    81,    82,    82,    83,    84,    84,    84,
      84,    84,    84,    84,    84,    85,    86,    86,    87,    87,
      87,    87,    87,    87,    88,    88,    88,    88,    88,    88,
      89,    90,    90,    90
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     1,     2,     2,     1,     1,     1,     6,
       3,     1,     1,     6,     5,     6,     3,     1,     1,     2,
       1,     0,     0,    11,     0,     0,    10,     3,     1,     2,
       1,     1,     1,     2,     0,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     4,     1,     0,     3,     1,
       1,     2,     3,     0,     1,     1,     2,     3,     1,     1,
       1,     1,     1,     1,     1,     3,     4,     4,     4,     7,
       5,     7,     5,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     3,     5,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     3,     1,     2,
       1,     1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 144 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
			{
				bool main_absent = !((program_object.get_main_procedure(cout) == NULL || 
					(program_object.get_main_procedure(cout))->get_return_type() != void_data_type));
				CHECK_INVARIANT(main_absent, "Main should be defined and it's type should be void")
			}
		}
#line 1462 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 152 "parser.y" /* yacc.c:1646  */
    { /* This action is only to suppress $$=$1 assignment which is
                                type incorrect in this case. 
					- Uday Tue Dec 31 17:54:52 IST 2019
                             */
                }
#line 1472 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 162 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
         			(yyval.symbol_table)=(yyvsp[-1].symbol_table);
			/* This action does not include the func_decl in the value of
                           global_decl_statement because the function names go in a
                           separate list in the program. 
                           The Symbol_Table stores variables names only and not function
                           names because it is used to record the declarations inside a
                           procedure too.
				- Uday Mon Dec 30 14:39:00 IST 2019
                        */
		}
#line 1488 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 175 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				// if declaration is local then no need to check in global list
    				// if declaration is global then this list is global list
	
    				Symbol_Table * decl_list_1 = (yyvsp[-1].symbol_table);
    				Symbol_Table * decl_list_2 = (yyvsp[0].symbol_table);
	
       		   		if(decl_list_1!=NULL && decl_list_2!=NULL) 
				{    
       		 			decl_list_1->append_list(*decl_list_2,yylineno);
       		      			(yyval.symbol_table)=decl_list_1;
       		 		}
       		   		else 
				{
       		           		if(decl_list_1!=NULL)
       		                  		(yyval.symbol_table) = decl_list_1;
       		           		else if(decl_list_2!=NULL)
       		                   		(yyval.symbol_table)= decl_list_2;
       		            		else  (yyval.symbol_table)=NULL;
					/* See the comment in the action of the first RHS above for the meaning of NULL */
       		   		}
		
       		   		Symbol_Table * global_table = (yyval.symbol_table);
		
       	        		if(global_table !=NULL)
       			 		program_object.set_global_table(*global_table);
    
  			}
  		}
#line 1523 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 206 "parser.y" /* yacc.c:1646  */
    {   	if (command_options.construct_ast())
  			{
         			(yyval.symbol_table)=(yyvsp[0].symbol_table);
                   		Symbol_Table * global_table = (yyval.symbol_table);

               			if(global_table !=NULL)
        				program_object.set_global_table(*global_table);
   			}
 		}
#line 1537 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 216 "parser.y" /* yacc.c:1646  */
    {   	if (command_options.construct_ast())
         			(yyval.symbol_table)=NULL;
			/* See the comment in the action of the first RHS above for the meaning of NULL */
 		}
#line 1546 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 221 "parser.y" /* yacc.c:1646  */
    {	printf("\nclass_declaration_stat");
		}
#line 1553 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 226 "parser.y" /* yacc.c:1646  */
    { printf("\nCLASS");
		}
#line 1560 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 242 "parser.y" /* yacc.c:1646  */
    {  	if (command_options.construct_ast())
  			{
  	 			CHECK_INVARIANT(((yyvsp[-4].string_value) != NULL), "Procedure name cannot be null");
  	 			string proc_name = *(yyvsp[-4].string_value);

				Data_Type recent_type_name = (yyvsp[-5].type_name);
	
  		 		CHECK_INPUT ((program_object.variable_in_symbol_list_check(proc_name) == false), 
				"Procedure name cannot be same as global variable", yylineno);
	
   				Procedure *pdecl = new Procedure(recent_type_name, proc_name, yylineno);
    				Symbol_Table * formal_param_table = (yyvsp[-2].symbol_table);
	
    				pdecl->set_formal_param_list(*formal_param_table);
    				program_object.set_proc_to_map(proc_name,pdecl);
    				dummy_no=0;
  			}
		}
#line 1583 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 261 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-3].string_value) != NULL), "Procedure name cannot be null");

    				string proc_name = *(yyvsp[-3].string_value);

				Data_Type recent_type_name = (yyvsp[-4].type_name);

    				Procedure *pdecl = new Procedure(recent_type_name, proc_name, yylineno);

    				program_object.set_proc_to_map(proc_name, pdecl);
  			}
		}
#line 1601 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 275 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-4].string_value) != NULL), "Procedure name cannot be null");
    				string proc_name = *(yyvsp[-4].string_value);

				Data_Type recent_type_name = (yyvsp[-5].type_name);

    				Procedure *pdecl = new Procedure(recent_type_name, proc_name, yylineno);

    				Symbol_Table * formal_param_table = (yyvsp[-2].symbol_table);
				
    				pdecl->set_formal_param_list(*formal_param_table);
    				program_object.set_proc_to_map(proc_name,pdecl);
    				
  			}
		}
#line 1622 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 295 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
 			{
	    			Symbol_Table * decl_list_1 = (yyvsp[-2].symbol_table);
	    			Symbol_Table * decl_list_2 = (yyvsp[0].symbol_table);
	    			decl_list_1->append_list(*decl_list_2,yylineno);
	    			(yyval.symbol_table)=decl_list_1;
			}
		}
#line 1635 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 304 "parser.y" /* yacc.c:1646  */
    { (yyval.symbol_table)=(yyvsp[0].symbol_table); }
#line 1641 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 309 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
      				dummy_no++;
      				string name = "dummy"+to_string(dummy_no);
      				Symbol_Table_Entry * decl_entry = new Symbol_Table_Entry(name, (yyvsp[0].type_name), yylineno);
      				Symbol_Table * decl_list = new Symbol_Table();
      				decl_list->push_symbol(decl_entry);
      				(yyval.symbol_table) = decl_list;
  			}
		}
#line 1656 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 328 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-3].string_value) != NULL), "Procedure name cannot be null");
    				string proc_name = *(yyvsp[-3].string_value);

				Data_Type recent_type_name = (yyvsp[-4].type_name);

    				if(program_object.proc_exists(proc_name))
				{
	    				current_procedure=program_object.get_procedure_prototype(proc_name);
	
	    				CHECK_INPUT_AND_ABORT(current_procedure->is_proc_defined()==false, 
							"Current procedure "+proc_name+" already defined", yylineno-1);
	
	    				Symbol_Table * new_formal_param_table = (yyvsp[-1].symbol_table);
	    				Symbol_Table * old_formal_param_table = &current_procedure->get_formal_param_list();
	
		    			CHECK_INPUT_AND_ABORT(old_formal_param_table->get_number_of_symbols()==
						new_formal_param_table->get_number_of_symbols(), 
						"definition and declaration should have same number of params", yylineno-1);
	
	    				for(int i=1;i<=new_formal_param_table->get_number_of_symbols();i++)
					{
		    				CHECK_INPUT_AND_ABORT(new_formal_param_table->get_symbol_table_entry_by_index(i).get_data_type()==
		    					old_formal_param_table->get_symbol_table_entry_by_index(i).get_data_type(),
		 					"Types of parameters in declaration and definition do not match", yylineno-1);
	    				}
	
	    				CHECK_INPUT_AND_ABORT(current_procedure->get_return_type()==recent_type_name,
							"Return type in declaration and definition do not match", yylineno-1);
			
	      				current_procedure->set_proc_is_defined();
			
	      				current_procedure->set_formal_param_list(*new_formal_param_table);
		    		}
		    		else
				{
		    			current_procedure = new Procedure(recent_type_name, proc_name, yylineno);
		    			Symbol_Table * formal_param_table = (yyvsp[-1].symbol_table);
				
		      			current_procedure->set_proc_is_defined();
		      			current_procedure->set_formal_param_list(*formal_param_table);
		      			program_object.set_proc_to_map(proc_name, current_procedure);
		    		}
		
  			}

  		}
#line 1709 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 377 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null");

    				Symbol_Table * local_table = (yyvsp[0].symbol_table);

    				if (local_table == NULL)
      					local_table = new Symbol_Table();

    				current_procedure->set_local_list(*local_table);
  			}
  		}
#line 1726 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 390 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null"); 
  
        			list<Ast *> * stmt_list = NULL;

          			if((yyvsp[-1].ast_list)!=NULL)
               				stmt_list = (yyvsp[-1].ast_list);
          			else
               				stmt_list = new list<Ast *>;

         			current_procedure->set_ast_list(*stmt_list);     
        
         			current_procedure = NULL;
  			}
  		}
#line 1747 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 407 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-2].string_value) != NULL), "Procedure name cannot be null");

    				string proc_name = *(yyvsp[-2].string_value);

				Data_Type recent_type_name = (yyvsp[-3].type_name);

     				if(program_object.proc_exists(proc_name))
				{
    					current_procedure=program_object.get_procedure_prototype(proc_name);

    					CHECK_INPUT_AND_ABORT(current_procedure->is_proc_defined()==false, 
						"Current procedure already defined",yylineno-1);
    					CHECK_INPUT_AND_ABORT(current_procedure->get_return_type()==recent_type_name,
						"return type in declaration and definition not matching",yylineno-1)

      					current_procedure->set_proc_is_defined();
	    			}
	    			else
				{
	    				current_procedure = new Procedure(recent_type_name, proc_name, yylineno);
	      				current_procedure->set_proc_is_defined();
	      				program_object.set_proc_to_map(proc_name, current_procedure);
	    			}
  			}
		}
#line 1779 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 435 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null");

    				Symbol_Table * local_table = (yyvsp[0].symbol_table);

    				if (local_table == NULL)
      					local_table = new Symbol_Table();

    				current_procedure->set_local_list(*local_table);
  			}

  		}
#line 1797 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 449 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null"); 
  
        			list<Ast *> * stmt_list = NULL;
	
          			if((yyvsp[-1].ast_list)!=NULL)
               				stmt_list = (yyvsp[-1].ast_list);
          			else
               				stmt_list = new list<Ast *>;
	
         			current_procedure->set_ast_list(*stmt_list);     
	
       				current_procedure = NULL;
  			}
  		}
#line 1818 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 469 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
			{ 
    				Symbol_Table * decl_list_1 = (yyvsp[-2].symbol_table);
    				Symbol_Table * decl_list_2 = (yyvsp[0].symbol_table);
    				decl_list_1->append_list(*decl_list_2,yylineno);
    				(yyval.symbol_table)=decl_list_1;
			}
		}
#line 1831 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 478 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
			{ (yyval.symbol_table) = (yyvsp[0].symbol_table);}
		}
#line 1839 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 485 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
			{
      				string name = *(yyvsp[0].string_value);
      				Symbol_Table_Entry * decl_entry = new Symbol_Table_Entry(name, (yyvsp[-1].type_name), yylineno);
      				Symbol_Table * decl_list = new Symbol_Table();
      				decl_list->push_symbol(decl_entry);
      				(yyval.symbol_table) = decl_list;
			}
		}
#line 1853 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 498 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
			{ (yyval.type_name) = int_data_type; }
		}
#line 1861 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 502 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
			{ (yyval.type_name) = double_data_type; }
		}
#line 1869 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 506 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
			{ (yyval.type_name) = bool_data_type; }
		}
#line 1877 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 513 "parser.y" /* yacc.c:1646  */
    { 	if(command_options.construct_ast())
  			{
    				list<Ast *> * temp_ast_list = (yyvsp[-1].ast_list);
    				Ast * stmt = (yyvsp[0].ast);
    				list<Ast *> * temp_ast_list_new;
				
    				if (temp_ast_list == NULL)
      					temp_ast_list_new = new list<Ast *>;
    				else
      					temp_ast_list_new = temp_ast_list;
   
				if(stmt!=NULL)
    					temp_ast_list_new->push_back(stmt);

    				(yyval.ast_list) = temp_ast_list_new;
  			}
  		}
#line 1899 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 531 "parser.y" /* yacc.c:1646  */
    {  	if (command_options.construct_ast())
            			(yyval.ast_list) = NULL;
  		}
#line 1907 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 538 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast()) 
             			(yyval.ast) = (yyvsp[0].ast);
           	}
#line 1915 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 542 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast()) 
             			(yyval.ast) = (yyvsp[0].ast);
           	}
#line 1923 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 546 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast()) 
             			(yyval.ast) = (yyvsp[0].ast);
           	}
#line 1931 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 550 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast()) 
             			(yyval.ast) = (yyvsp[0].ast);
           	}
#line 1939 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 554 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast()) 
             			(yyval.ast) = (yyvsp[0].ast);
           	}
#line 1947 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 558 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast()) 
             			(yyval.ast) = (yyvsp[0].ast);
           	}
#line 1955 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 562 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast()) 
             			(yyval.ast) = (yyvsp[0].ast);
           	}
#line 1963 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 566 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast()) 
             			(yyval.ast) = (yyvsp[0].ast);
               	}
#line 1971 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 570 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast()) 
             			(yyval.ast) = (yyvsp[0].ast);
                }
#line 1979 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 577 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  				(yyval.ast)=(yyvsp[-1].ast);
		}
#line 1987 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 584 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
    			{
  				Call_Ast* func_call = new Call_Ast(*(yyvsp[-3].string_value),yylineno);
        			if((yyvsp[-1].ast_list)!=NULL)
   					func_call->set_actual_param_list(*(yyvsp[-1].ast_list));

      				func_call->type_check_actual_formal_param(((program_object.get_procedure_prototype(*(yyvsp[-3].string_value)))->get_formal_param_list()));
   				(yyval.ast)=func_call;
			}
		}
#line 2002 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 598 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  				(yyval.ast_list)=(yyvsp[0].ast_list);
		}
#line 2010 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 602 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  				(yyval.ast_list)=NULL;
		}
#line 2018 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 609 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
    			{
        			list<Ast *> * arg_list = (yyvsp[-2].ast_list);
        			arg_list->push_back((yyvsp[0].ast));
        			(yyval.ast_list)= arg_list;
  			}
		}
#line 2030 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 617 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
    			{
        			list<Ast *> * arg_list = new list<Ast *>;
        			arg_list->push_back((yyvsp[0].ast));
        			(yyval.ast_list)= arg_list;
  			}
		}
#line 2042 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 628 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  				(yyval.ast)= (yyvsp[0].ast);
  		}
#line 2050 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 635 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
    			{
      				Return_Ast* return_stmt = new Return_Ast(NULL, current_procedure->get_proc_name(), yylineno);
      				(yyval.ast) = return_stmt;
  			}
		}
#line 2061 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 643 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
    			{
      				Return_Ast* return_stmt = new Return_Ast((yyvsp[-1].ast), current_procedure->get_proc_name(), yylineno);
      				(yyval.ast) = return_stmt;
  			}
		}
#line 2072 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 653 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
        			(yyval.symbol_table) = NULL;
  		}
#line 2080 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 657 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
         			if((yyvsp[0].symbol_table)!=NULL)
           				(yyval.symbol_table) = (yyvsp[0].symbol_table);
  			}  
  		}
#line 2091 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 667 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
         			(yyval.symbol_table) = (yyvsp[0].symbol_table);
  		}
#line 2099 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 671 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				// if declaration is local then no need to check in global list
    				// if declaration is global then this list is global list

    				Symbol_Table * decl_list_1 = (yyvsp[-1].symbol_table);
    				Symbol_Table * decl_list_2 = (yyvsp[0].symbol_table);

          			if(decl_list_1!=NULL && decl_list_2!=NULL) 
				{    
        				decl_list_1->append_list(*decl_list_2,yylineno);
             				(yyval.symbol_table)=decl_list_1;
        			}
          			else 
				{
                  			if(decl_list_1!=NULL)
                         			(yyval.symbol_table) = decl_list_1;
                  			else if(decl_list_2!=NULL)
                          			(yyval.symbol_table)= decl_list_2;
                   			else  (yyval.symbol_table)=NULL;
          			}

  			}
		}
#line 2128 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 699 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
			{
				Data_Type recent_type_name = (yyvsp[-2].type_name);
        			Symbol_Table * decl_list = (yyvsp[-1].symbol_table);
	
				CHECK_INVARIANT (decl_list != NULL, "List of variables is NULL in type declarations");
	
			 	decl_list->set_type_of_all_syms(recent_type_name);
	
  				(yyval.symbol_table) = decl_list;
   		 	}	
		}
#line 2145 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 715 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
				(yyval.type_name) = int_data_type; 
		}
#line 2153 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 719 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
				(yyval.type_name) = double_data_type;
		}
#line 2161 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 723 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
				(yyval.type_name) = void_data_type; 
		}
#line 2169 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 727 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
				(yyval.type_name) = string_data_type; 
		}
#line 2177 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 731 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
				(yyval.type_name) = bool_data_type; 
		}
#line 2185 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 735 "parser.y" /* yacc.c:1646  */
    { 
		}
#line 2192 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 741 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
	  		{
	        		string name = *(yyvsp[0].string_value);
	      			Symbol_Table_Entry * decl_entry = new Symbol_Table_Entry(name, yylineno);    
	      			Symbol_Table * decl_list = new Symbol_Table();
	    			decl_list->push_symbol(decl_entry);
	    			(yyval.symbol_table)=decl_list;
	  		}
	  	}
#line 2206 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 751 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
        			string name = *(yyvsp[0].string_value);
        			Symbol_Table * decl_list = (yyvsp[-2].symbol_table);
        			CHECK_INPUT((decl_list->variable_in_symbol_list_check(name) == false), 
        				"Variable is declared twice", yylineno);

        			Symbol_Table_Entry * decl_entry = new Symbol_Table_Entry(name, yylineno);    
        			decl_list->push_symbol(decl_entry);
         			(yyval.symbol_table)=decl_list;
  			}
  		}
#line 2223 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 767 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-3].ast) && (yyvsp[-1].ast)), "Lhs/Rhs of assignment expression cannot be null");
    				(yyval.ast) = new Assignment_Ast((yyvsp[-3].ast), (yyvsp[-1].ast), yylineno);
    				(yyval.ast)->type_check_ast();
  			}
		}
#line 2235 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 775 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-3].ast) && (yyvsp[-1].ast)), "Lhs/Rhs of assignment expression cannot be null");
    				(yyval.ast) = new Assignment_Ast((yyvsp[-3].ast), (yyvsp[-1].ast), yylineno);
    				(yyval.ast)->type_check_ast();
  			}
		}
#line 2247 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 783 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-3].ast) && (yyvsp[-1].ast)), "Lhs/Rhs of assignment expression cannot be null");
    				(yyval.ast) = new Assignment_Ast((yyvsp[-3].ast), (yyvsp[-1].ast), yylineno);
    				(yyval.ast)->type_check_ast();
  			}
		}
#line 2259 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 794 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
			{	
    				(yyval.ast) = new Selection_Statement_Ast((yyvsp[-4].ast),(yyvsp[-2].ast),(yyvsp[0].ast),yylineno);
				command_options.set_no_rtl_support();	// CHECK: Temporary provision
			}
				
  		}
#line 2271 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 802 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
			{	
    				(yyval.ast) = new Selection_Statement_Ast((yyvsp[-2].ast),(yyvsp[0].ast),NULL,yylineno);
				command_options.set_no_rtl_support();	// CHECK: Temporary provision
			}
  		}
#line 2282 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 812 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
			{	
    				(yyval.ast) = new Iteration_Statement_Ast((yyvsp[-2].ast),(yyvsp[-5].ast),yylineno,true);
				command_options.set_no_rtl_support();	// CHECK: Temporary provision
			}
		}
#line 2293 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 822 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
			{	
    				(yyval.ast) = new Iteration_Statement_Ast((yyvsp[-2].ast),(yyvsp[0].ast),yylineno,false);
				command_options.set_no_rtl_support();	// CHECK: Temporary provision
			}
  		}
#line 2304 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 832 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
       				CHECK_INVARIANT(((yyvsp[-1].ast_list) != NULL), " Block of statements cannot be empty");
      				Sequence_Ast *sast = new Sequence_Ast(yylineno);

              			list<Ast *>::iterator t;

      				for (t = (yyvsp[-1].ast_list)->begin(); t != (yyvsp[-1].ast_list)->end(); ++t)
             				sast->ast_push_back(*t);

             			(yyval.ast)=sast;
  			}
  		}
#line 2322 "y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 849 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT((yyvsp[-1].ast) , "A variable cannot be null in a print statement");
    				(yyval.ast) = new Write_Ast((yyvsp[-1].ast),yylineno);

  			}
		}
#line 2334 "y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 857 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
				string s = *(yyvsp[-1].string_value);
				int str_num = program_object.add_string_to_string_table(s);
    				String_Constant_Ast * str_ast = new String_Constant_Ast(str_num, string_data_type, yylineno);
    				(yyval.ast) = new Write_Ast(str_ast,yylineno);

  			}
		}
#line 2348 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 870 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT((yyvsp[-1].ast) , "A variable cannot be null in a read statement");
    				(yyval.ast) = new Read_Ast((yyvsp[-1].ast),yylineno);
  			}
		}
#line 2359 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 881 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-2].ast) && (yyvsp[0].ast)), "An arith_operand cannot have null entries");
    				(yyval.ast) = new Plus_Ast((yyvsp[-2].ast), (yyvsp[0].ast), yylineno);
    				(yyval.ast)->set_data_type((yyvsp[-2].ast)->get_data_type());
    				(yyval.ast)->type_check_ast();
  			}
  		}
#line 2372 "y.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 890 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-2].ast) && (yyvsp[0].ast)), "An arith_operand cannot have null entries");
    				(yyval.ast) = new Minus_Ast((yyvsp[-2].ast), (yyvsp[0].ast), yylineno);
    				(yyval.ast)->set_data_type((yyvsp[-2].ast)->get_data_type());
    				(yyval.ast)->type_check_ast();
  			}
  		}
#line 2385 "y.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 899 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-2].ast) && (yyvsp[0].ast)), "An arith_operand cannot have null entries");
    				(yyval.ast) = new Mult_Ast((yyvsp[-2].ast), (yyvsp[0].ast), yylineno);
    				(yyval.ast)->set_data_type((yyvsp[-2].ast)->get_data_type());
    				(yyval.ast)->type_check_ast();
  			}
  		}
#line 2398 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 908 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-2].ast) && (yyvsp[0].ast)), "An arith_operand cannot have null entries");
    				(yyval.ast) = new Divide_Ast((yyvsp[-2].ast), (yyvsp[0].ast), yylineno);
    				(yyval.ast)->set_data_type((yyvsp[-2].ast)->get_data_type());
    				(yyval.ast)->type_check_ast();
  			}
  		}
#line 2411 "y.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 917 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT((yyvsp[0].ast), "An arith_operand cannot have null entries");
    				(yyval.ast) = new UMinus_Ast((yyvsp[0].ast), NULL, yylineno);
    				(yyval.ast)->set_data_type((yyvsp[0].ast)->get_data_type());
    				(yyval.ast)->type_check_ast();
  			}
  		}
#line 2424 "y.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 926 "parser.y" /* yacc.c:1646  */
    {  	if (command_options.construct_ast())
  			{
    				(yyval.ast) = (yyvsp[-1].ast);
    				(yyval.ast)->set_data_type((yyvsp[-1].ast)->get_data_type());
  			}
  		}
#line 2435 "y.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 933 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-4].ast) != NULL) && ((yyvsp[-2].ast) != NULL) && ((yyvsp[0].ast) != NULL), "lhs/rhs of arithmetic expression cannot be null");
    				Ast * cond = (yyvsp[-4].ast);
    				Ast * lhs = (yyvsp[-2].ast);
    				Ast * rhs = (yyvsp[0].ast);
    				Ast * cond_ast = new Conditional_Expression_Ast(cond, lhs, rhs, yylineno);
    				cond_ast->type_check_ast();
    				(yyval.ast) = cond_ast;
				command_options.set_no_rtl_support();	// CHECK: Temporary provision
  			}
  		}
#line 2452 "y.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 946 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[0].ast) != NULL), "An expression cannot have null values");
    				(yyval.ast) = (yyvsp[0].ast);
    				(yyval.ast)->set_data_type((yyvsp[0].ast)->get_data_type());
  			}
  		}
#line 2464 "y.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 957 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[0].ast) != NULL), "Arithmetic expression cannot be null");
    				(yyval.ast) = (yyvsp[0].ast);
    				(yyval.ast)->set_data_type((yyvsp[0].ast)->get_data_type());
  			}
  		}
#line 2476 "y.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 968 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[0].ast) != NULL), "An expression term cannot be null");
    				(yyval.ast) = (yyvsp[0].ast);
  			}
  		}
#line 2487 "y.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 975 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[0].ast) != NULL), "An expression term cannot be null");
    				(yyval.ast) = (yyvsp[0].ast);
  			}
  		}
#line 2498 "y.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 985 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-2].ast) && (yyvsp[0].ast)), "A rel_operand cannot have null entries");
    				(yyval.ast) = new Relational_Expr_Ast((yyvsp[-2].ast), less_than, (yyvsp[0].ast), yylineno);
    				(yyval.ast)->set_data_type(bool_data_type);
    				(yyval.ast)->type_check_ast();
  			}
  		}
#line 2511 "y.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 994 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-2].ast) && (yyvsp[0].ast)), "A rel_operand cannot have null entries");
    				(yyval.ast) = new Relational_Expr_Ast((yyvsp[-2].ast), less_equalto, (yyvsp[0].ast), yylineno);
    				(yyval.ast)->set_data_type(bool_data_type);
    				(yyval.ast)->type_check_ast();
  			}
  		}
#line 2524 "y.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 1003 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-2].ast) && (yyvsp[0].ast)), "A rel_operand cannot have null entries");
    				(yyval.ast) = new Relational_Expr_Ast((yyvsp[-2].ast), greater_than, (yyvsp[0].ast), yylineno);
    				(yyval.ast)->set_data_type(bool_data_type);
    				(yyval.ast)->type_check_ast();
  			}
  		}
#line 2537 "y.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 1012 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-2].ast) && (yyvsp[0].ast)), "A rel_operand cannot have null entries");
    				(yyval.ast) = new Relational_Expr_Ast((yyvsp[-2].ast), greater_equalto, (yyvsp[0].ast), yylineno);
    				(yyval.ast)->set_data_type(bool_data_type);
    				(yyval.ast)->type_check_ast();
  			}
  		}
#line 2550 "y.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 1021 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-2].ast) && (yyvsp[0].ast)), "A rel_operand cannot have null entries");
    				(yyval.ast) = new Relational_Expr_Ast((yyvsp[-2].ast), not_equalto, (yyvsp[0].ast), yylineno);
    				(yyval.ast)->set_data_type(bool_data_type);
    				(yyval.ast)->type_check_ast();
  			}
  		}
#line 2563 "y.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 1030 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-2].ast) && (yyvsp[0].ast)), "A rel_operand cannot have null entries");
    				(yyval.ast) = new Relational_Expr_Ast((yyvsp[-2].ast), equalto, (yyvsp[0].ast), yylineno);
    				(yyval.ast)->set_data_type(bool_data_type);
    				(yyval.ast)->type_check_ast();
  			}
  		}
#line 2576 "y.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 1042 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-2].ast) != NULL) && ((yyvsp[0].ast) != NULL), "lhs/rhs of boolean expression cannot be null");
    				Logical_Op lop = _logical_and;
    				Logical_Expr_Ast * logical_expr_ast = new Logical_Expr_Ast((yyvsp[-2].ast), lop, (yyvsp[0].ast), yylineno);
    				logical_expr_ast->set_data_type(bool_data_type);
      				logical_expr_ast->type_check_ast();
    				(yyval.ast) = logical_expr_ast;
  			}
  		}
#line 2591 "y.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 1053 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-2].ast) != NULL) && ((yyvsp[0].ast) != NULL), "lhs/rhs of boolean expression cannot be null");
    				Logical_Op lop = _logical_or;
    				Logical_Expr_Ast * logical_expr_ast = new Logical_Expr_Ast((yyvsp[-2].ast), lop, (yyvsp[0].ast), yylineno);
    				logical_expr_ast->set_data_type(bool_data_type);
    				logical_expr_ast->type_check_ast();
    				(yyval.ast) = logical_expr_ast;
  			}
		}
#line 2606 "y.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 1064 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[0].ast) != NULL) , "rhs of boolean expression cannot be null");
    				Logical_Op lop = _logical_not;
    				Logical_Expr_Ast * logical_expr_ast = new Logical_Expr_Ast((yyvsp[0].ast), lop, NULL, yylineno);
    				logical_expr_ast->set_data_type(bool_data_type);
    				logical_expr_ast->type_check_ast();
    				(yyval.ast) = logical_expr_ast;
  			}
		}
#line 2621 "y.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 1075 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
    				(yyval.ast) = (yyvsp[-1].ast);
  		}
#line 2629 "y.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 1079 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
    				(yyval.ast) = (yyvsp[0].ast);
  		}
#line 2637 "y.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 1083 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
    				(yyval.ast) = (yyvsp[0].ast);
  		}
#line 2645 "y.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 1090 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				Symbol_Table_Entry * var_table_entry;

    				CHECK_INVARIANT(((yyvsp[0].string_value) != NULL), "Variable name cannot be null");

    				if (current_procedure->variable_in_symbol_list_check(*(yyvsp[0].string_value)))
       					var_table_entry = &(current_procedure->get_symbol_table_entry(*(yyvsp[0].string_value)));
    				else if (current_procedure->variable_in_formal_list_check(*(yyvsp[0].string_value)))
       					var_table_entry = &(current_procedure->get_formal_param_entry(*(yyvsp[0].string_value)));
    				else if (program_object.variable_in_symbol_list_check(*(yyvsp[0].string_value)))
      					var_table_entry = &(program_object.get_symbol_table_entry(*(yyvsp[0].string_value)));
    				else
      					CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, "Variable has not been declared", yylineno);

    				(yyval.ast) = new Name_Ast(*(yyvsp[0].string_value), *var_table_entry, yylineno);
  			}
  		}
#line 2668 "y.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 1112 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
    				(yyval.ast) = new Number_Ast<int>((yyvsp[0].integer_value), int_data_type, yylineno);
  		}
#line 2676 "y.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 1116 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
    				(yyval.ast) = new Number_Ast<double>((yyvsp[0].double_value), double_data_type, yylineno);
  		}
#line 2684 "y.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 1120 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
			{
				string s = *(yyvsp[0].string_value);
				int str_num = program_object.add_string_to_string_table(s);
    				(yyval.ast) = new String_Constant_Ast(str_num, string_data_type, yylineno);
			}
  		}
#line 2696 "y.tab.c" /* yacc.c:1646  */
    break;


#line 2700 "y.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
