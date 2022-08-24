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

#line 86 "parser.tab.c" /* yacc.c:339  */

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
   by #include "parser.tab.h".  */
#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
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



#line 206 "parser.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 223 "parser.tab.c" /* yacc.c:358  */

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
#define YYLAST   356

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  39
/* YYNRULES -- Number of rules.  */
#define YYNRULES  101
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  202

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
       0,   139,   139,   148,   157,   170,   202,   212,   217,   223,
     226,   230,   231,   236,   255,   269,   289,   298,   303,   317,
     318,   322,   325,   329,   332,   336,   339,   345,   354,   361,
     374,   378,   382,   389,   407,   413,   417,   421,   425,   429,
     433,   437,   441,   445,   452,   459,   462,   469,   477,   488,
     495,   503,   513,   517,   545,   561,   565,   569,   573,   577,
     581,   587,   597,   613,   621,   629,   640,   644,   651,   658,
     665,   669,   675,   683,   696,   707,   716,   725,   734,   743,
     752,   759,   771,   782,   793,   800,   810,   819,   828,   837,
     846,   855,   867,   878,   889,   900,   904,   908,   916,   940,
     944,   948
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
  "dummy_formal_param", "func_def_list", "func_def", "formal_param_list",
  "formal_param", "param_type", "statement_list", "statement",
  "call_statement", "func_call", "non_empty_arg_list", "actual_arg",
  "return_statement", "var_decl_list", "var_decl", "type",
  "variables_list", "assignment_statement", "if_statement",
  "do_while_statement", "while_statement", "compound_statement",
  "print_statement", "read_statement", "arith_expression", "operand",
  "expression_term", "rel_expression", "logical_expression", "variable",
  "constant", YY_NULLPTR
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

#define YYPACT_NINF -82

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-82)))

#define YYTABLE_NINF -99

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     323,    35,   -82,   -82,   -82,   -82,   -82,   -82,    48,   341,
     -82,   -82,   341,   -82,   -82,    43,    22,   -82,   -82,   341,
     -82,   -82,    82,   105,    14,   128,   110,    -2,   -82,   112,
     -82,   -82,    93,   115,    19,   -82,   -82,   -82,   133,   -35,
     -82,    -7,   -82,   147,   -82,   116,   323,   123,    20,   147,
     230,   -82,   126,   118,   153,   118,   -82,   -82,   341,   168,
     143,   -82,   136,   -82,   192,   -82,   -82,   -82,   245,   -82,
     -82,   165,     2,   167,   182,    96,   211,   220,    99,     4,
     -82,   166,   -82,   -82,   195,   -82,   -82,   -82,   -82,   -82,
     -82,   -82,   -82,   -82,   232,   -82,   -82,   151,    70,   183,
     226,   226,   204,   231,   -82,   203,   206,   210,   -82,   -82,
     -82,   220,   226,   226,   226,   -82,   215,   311,   -82,   -82,
     177,   -82,   -82,   -82,   194,   -82,   -82,   -82,   261,   -82,
     209,   218,    41,   -82,    50,   -82,    80,   -82,   100,   172,
     223,   -82,   -82,   -82,   -82,   -82,   -82,   274,   248,   -82,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   -82,   221,   227,    75,   -82,   233,   -82,
     226,    96,    96,   226,   -82,   -82,   311,   311,   311,   311,
     311,   311,   144,   144,   -82,   -82,   288,   253,   -82,   -82,
     -82,   -82,   -82,   262,   -82,   252,   226,    96,   238,   311,
     -82,   -82
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,    60,    55,    56,    58,    59,    57,     0,     0,
       8,     7,     3,    20,     6,     0,     0,     1,     4,     2,
       5,    19,     0,    61,     0,     0,     0,     0,    54,     0,
      12,    11,     0,     0,     0,    30,    31,    32,     0,     0,
      17,     0,    28,    18,    62,     0,     0,     0,     0,     0,
       0,    14,     0,     0,     0,     0,    29,     9,    10,     0,
       0,    23,     0,    52,     0,    13,    16,    18,     0,    15,
      27,    61,    60,     0,     0,     0,     0,     0,     0,     0,
      26,     0,    34,    42,     0,    43,    53,    35,    36,    37,
      38,    39,    40,    41,     0,    61,    21,     0,     0,     0,
       0,     0,    98,     0,    98,     0,     0,     0,    99,   100,
     101,     0,     0,     0,     0,    50,    83,     0,    82,    96,
       0,    84,    85,    71,     0,    24,    33,    44,     0,    25,
       0,     0,     0,    46,     0,    48,    83,    83,     0,     0,
       0,    73,    72,    74,    97,    79,    94,     0,     0,    51,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    70,     0,    83,     0,    22,     0,    45,
       0,     0,     0,     0,    80,    95,    90,    91,    86,    87,
      88,    89,    75,    76,    77,    78,     0,    93,    92,    64,
      63,    65,    47,    67,    69,     0,     0,     0,     0,    81,
      66,    68
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -82,   -82,   237,   -82,   -82,   -82,    -4,   -82,   234,   276,
      25,   -31,   235,    32,   -61,   -74,   -82,   158,   -82,   121,
     -82,   224,    -5,    84,   -82,   -82,   -82,   -82,   -82,   -82,
     -82,   -82,   -76,   -81,   -82,   -82,   -73,   -62,   -82
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     8,     9,    10,    32,    33,    11,    39,    40,    12,
      13,    41,    42,    43,    81,    82,    83,    84,   134,   135,
      85,    62,    14,    64,    24,    87,    88,    89,    90,    91,
      92,    93,   137,   117,   118,   119,   120,   121,   122
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      94,   103,   116,    48,    20,    18,    52,   126,    35,    36,
      53,    37,   102,    94,   106,   107,   -98,    94,   124,    94,
      73,    74,    75,   136,    76,    77,    78,   138,   139,    35,
      36,   145,    37,   147,    54,    94,   130,    21,    55,    38,
     146,   148,    99,    16,    21,    63,    79,   123,    17,   144,
     126,    23,   165,    20,    18,   166,   126,    86,    28,    29,
      47,    60,    94,    63,    25,    55,    49,   132,    94,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
      35,    36,   168,    37,    15,    67,    55,    49,   187,   188,
      26,   169,    86,    15,   136,   170,    22,   193,   194,   160,
     195,   161,   162,    22,   102,   108,   109,   104,   110,    94,
      94,   131,    73,    74,    75,   199,    76,    77,    78,   191,
      44,   -49,   111,   200,   160,   -49,   161,   162,    35,    36,
      59,    37,    30,    31,   112,    94,    45,   113,    79,   114,
      46,   171,    59,   115,    72,    27,     3,     4,     5,     6,
      34,     7,    73,    74,    75,    56,    76,    77,    78,    72,
      57,     3,     4,     5,     6,    50,     7,    73,    74,    75,
      65,    76,    77,    78,   102,    50,    71,    51,    79,    80,
     158,   159,    73,    74,    75,    68,    76,    77,    78,   108,
     109,   104,   110,    79,   129,    68,   160,    69,   161,   162,
      95,   160,   102,   161,   162,    98,   111,   100,    79,   125,
      73,    74,    75,   172,    76,    77,    78,   102,   112,   104,
     105,   113,   101,   114,   133,    73,    74,    75,   104,    76,
      77,    78,   108,   109,   104,   110,    79,   163,     2,   127,
       3,     4,     5,     6,    99,     7,   128,   141,   140,   111,
     142,    79,   167,     2,   143,     3,     4,     5,     6,   149,
       7,   112,    51,   173,   113,   189,   114,   108,   109,   102,
     110,   190,   160,    61,   161,   162,   160,    69,   161,   162,
     162,   197,   201,    58,   111,    19,   164,    66,    96,   175,
      70,   192,    97,   198,     0,     0,   112,     0,     0,   113,
       0,   114,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,     0,   196,     0,   174,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,     1,     0,     0,     0,
       0,     2,     0,     3,     4,     5,     6,     0,     7,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,     2,
       0,     3,     4,     5,     6,     0,     7
};

static const yytype_int16 yycheck[] =
{
      62,    75,    78,    34,     9,     9,    41,    81,    10,    11,
      45,    13,     8,    75,    76,    77,    14,    79,    79,    81,
      16,    17,    18,    99,    20,    21,    22,   100,   101,    10,
      11,   112,    13,   114,    41,    97,    97,    12,    45,    41,
     113,   114,    40,     8,    19,    50,    42,    43,     0,   111,
     124,     8,   128,    58,    58,   128,   130,    62,    44,    45,
      41,    41,   124,    68,    42,    45,    34,    98,   130,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
      10,    11,    41,    13,     0,    53,    45,    55,   161,   162,
       8,    41,    97,     9,   170,    45,    12,   171,   172,    24,
     173,    26,    27,    19,     8,     6,     7,     8,     9,   171,
     172,    41,    16,    17,    18,   196,    20,    21,    22,    44,
       8,    41,    23,   197,    24,    45,    26,    27,    10,    11,
      46,    13,     4,     5,    35,   197,    43,    38,    42,    40,
      25,    41,    58,    44,     8,    40,    10,    11,    12,    13,
      40,    15,    16,    17,    18,     8,    20,    21,    22,     8,
      44,    10,    11,    12,    13,    42,    15,    16,    17,    18,
      44,    20,    21,    22,     8,    42,     8,    44,    42,    43,
      36,    37,    16,    17,    18,    42,    20,    21,    22,     6,
       7,     8,     9,    42,    43,    42,    24,    44,    26,    27,
       8,    24,     8,    26,    27,    40,    23,    40,    42,    43,
      16,    17,    18,    41,    20,    21,    22,     8,    35,     8,
       9,    38,    40,    40,    41,    16,    17,    18,     8,    20,
      21,    22,     6,     7,     8,     9,    42,    43,     8,    44,
      10,    11,    12,    13,    40,    15,    14,    44,    17,    23,
      44,    42,    43,     8,    44,    10,    11,    12,    13,    44,
      15,    35,    44,    40,    38,    44,    40,     6,     7,     8,
       9,    44,    24,    43,    26,    27,    24,    44,    26,    27,
      27,    19,    44,    46,    23,     9,   128,    53,    43,    41,
      55,   170,    68,    41,    -1,    -1,    35,    -1,    -1,    38,
      -1,    40,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    -1,    25,    -1,    41,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,     3,    -1,    -1,    -1,
      -1,     8,    -1,    10,    11,    12,    13,    -1,    15,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,     8,
      -1,    10,    11,    12,    13,    -1,    15
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     8,    10,    11,    12,    13,    15,    47,    48,
      49,    52,    55,    56,    68,    69,     8,     0,    52,    55,
      68,    56,    69,     8,    70,    42,     8,    40,    44,    45,
       4,     5,    50,    51,    40,    10,    11,    13,    41,    53,
      54,    57,    58,    59,     8,    43,    25,    41,    57,    59,
      42,    44,    41,    45,    41,    45,     8,    44,    48,    69,
      41,    43,    67,    68,    69,    44,    54,    59,    42,    44,
      58,     8,     8,    16,    17,    18,    20,    21,    22,    42,
      43,    60,    61,    62,    63,    66,    68,    71,    72,    73,
      74,    75,    76,    77,    83,     8,    43,    67,    40,    40,
      40,    40,     8,    61,     8,     9,    83,    83,     6,     7,
       9,    23,    35,    38,    40,    44,    78,    79,    80,    81,
      82,    83,    84,    43,    60,    43,    61,    44,    14,    43,
      60,    41,    57,    41,    64,    65,    78,    78,    82,    82,
      17,    44,    44,    44,    83,    79,    82,    79,    82,    44,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      24,    26,    27,    43,    63,    78,    82,    43,    41,    41,
      45,    41,    41,    40,    41,    41,    79,    79,    79,    79,
      79,    79,    79,    79,    79,    79,    79,    82,    82,    44,
      44,    44,    65,    61,    61,    82,    25,    19,    41,    79,
      61,    44
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    46,    47,    47,    48,    48,    48,    48,    48,    49,
      50,    51,    51,    52,    52,    52,    53,    53,    54,    55,
      55,    56,    56,    56,    56,    56,    56,    57,    57,    58,
      59,    59,    59,    60,    60,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    62,    63,    63,    64,    64,    65,
      66,    66,    67,    67,    68,    69,    69,    69,    69,    69,
      69,    70,    70,    71,    71,    71,    72,    72,    73,    74,
      75,    75,    76,    76,    77,    78,    78,    78,    78,    78,
      78,    78,    78,    79,    80,    80,    81,    81,    81,    81,
      81,    81,    82,    82,    82,    82,    82,    82,    83,    84,
      84,    84
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     1,     2,     2,     1,     1,     1,     6,
       3,     1,     1,     6,     5,     6,     3,     1,     1,     2,
       1,     7,     9,     6,     8,     8,     7,     3,     1,     2,
       1,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     4,     3,     3,     1,     1,
       2,     3,     1,     2,     3,     1,     1,     1,     1,     1,
       1,     1,     3,     4,     4,     4,     7,     5,     7,     5,
       3,     2,     3,     3,     3,     3,     3,     3,     3,     2,
       3,     5,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     3,     1,     2,     1,     1,
       1,     1
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
#line 141 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
			{
				bool main_absent = !((program_object.get_main_procedure(cout) == NULL || 
					(program_object.get_main_procedure(cout))->get_return_type() != void_data_type));
				CHECK_INVARIANT(main_absent, "Main should be defined and it's type should be void")
			}
		}
#line 1482 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 149 "parser.y" /* yacc.c:1646  */
    { /* This action is only to suppress $$=$1 assignment which is
                                type incorrect in this case. 
					- Uday Tue Dec 31 17:54:52 IST 2019
                             */
                }
#line 1492 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 159 "parser.y" /* yacc.c:1646  */
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
#line 1508 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 172 "parser.y" /* yacc.c:1646  */
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
#line 1543 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 203 "parser.y" /* yacc.c:1646  */
    {   	if (command_options.construct_ast())
  			{
         			(yyval.symbol_table)=(yyvsp[0].symbol_table);
                   		Symbol_Table * global_table = (yyval.symbol_table);

               			if(global_table !=NULL)
        				program_object.set_global_table(*global_table);
   			}
 		}
#line 1557 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 213 "parser.y" /* yacc.c:1646  */
    {   	if (command_options.construct_ast())
         			(yyval.symbol_table)=NULL;
			/* See the comment in the action of the first RHS above for the meaning of NULL */
 		}
#line 1566 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 218 "parser.y" /* yacc.c:1646  */
    {	
		}
#line 1573 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 237 "parser.y" /* yacc.c:1646  */
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
#line 1596 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 256 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-3].string_value) != NULL), "Procedure name cannot be null");

    				string proc_name = *(yyvsp[-3].string_value);

				Data_Type recent_type_name = (yyvsp[-4].type_name);

    				Procedure *pdecl = new Procedure(recent_type_name, proc_name, yylineno);

    				program_object.set_proc_to_map(proc_name, pdecl);
  			}
		}
#line 1614 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 270 "parser.y" /* yacc.c:1646  */
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
#line 1635 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 290 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
 			{
	    			Symbol_Table * decl_list_1 = (yyvsp[-2].symbol_table);
	    			Symbol_Table * decl_list_2 = (yyvsp[0].symbol_table);
	    			decl_list_1->append_list(*decl_list_2,yylineno);
	    			(yyval.symbol_table)=decl_list_1;
			}
		}
#line 1648 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 299 "parser.y" /* yacc.c:1646  */
    { (yyval.symbol_table)=(yyvsp[0].symbol_table); }
#line 1654 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 304 "parser.y" /* yacc.c:1646  */
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
#line 1669 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 346 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
			{ 
    				Symbol_Table * decl_list_1 = (yyvsp[-2].symbol_table);
    				Symbol_Table * decl_list_2 = (yyvsp[0].symbol_table);
    				decl_list_1->append_list(*decl_list_2,yylineno);
    				(yyval.symbol_table)=decl_list_1;
			}
		}
#line 1682 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 355 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
			{ (yyval.symbol_table) = (yyvsp[0].symbol_table);}
		}
#line 1690 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 362 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
			{
      				string name = *(yyvsp[0].string_value);
      				Symbol_Table_Entry * decl_entry = new Symbol_Table_Entry(name, (yyvsp[-1].type_name), yylineno);
      				Symbol_Table * decl_list = new Symbol_Table();
      				decl_list->push_symbol(decl_entry);
      				(yyval.symbol_table) = decl_list;
			}
		}
#line 1704 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 375 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
			{ (yyval.type_name) = int_data_type; }
		}
#line 1712 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 379 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
			{ (yyval.type_name) = double_data_type; }
		}
#line 1720 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 383 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
			{ (yyval.type_name) = bool_data_type; }
		}
#line 1728 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 390 "parser.y" /* yacc.c:1646  */
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
#line 1750 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 408 "parser.y" /* yacc.c:1646  */
    {
	}
#line 1757 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 414 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast()) 
             			(yyval.ast) = (yyvsp[0].ast);
           	}
#line 1765 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 418 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast()) 
             			(yyval.ast) = (yyvsp[0].ast);
           	}
#line 1773 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 422 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast()) 
             			(yyval.ast) = (yyvsp[0].ast);
           	}
#line 1781 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 426 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast()) 
             			(yyval.ast) = (yyvsp[0].ast);
           	}
#line 1789 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 430 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast()) 
             			(yyval.ast) = (yyvsp[0].ast);
           	}
#line 1797 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 434 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast()) 
             			(yyval.ast) = (yyvsp[0].ast);
           	}
#line 1805 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 438 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast()) 
             			(yyval.ast) = (yyvsp[0].ast);
           	}
#line 1813 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 442 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast()) 
             			(yyval.ast) = (yyvsp[0].ast);
               	}
#line 1821 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 446 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast()) 
             			(yyval.ast) = (yyvsp[0].ast);
                }
#line 1829 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 453 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  				(yyval.ast)=(yyvsp[-1].ast);
		}
#line 1837 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 460 "parser.y" /* yacc.c:1646  */
    {
	}
#line 1844 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 463 "parser.y" /* yacc.c:1646  */
    {
	}
#line 1851 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 470 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
    			{
        			list<Ast *> * arg_list = (yyvsp[-2].ast_list);
        			arg_list->push_back((yyvsp[0].ast));
        			(yyval.ast_list)= arg_list;
  			}
		}
#line 1863 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 478 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
    			{
        			list<Ast *> * arg_list = new list<Ast *>;
        			arg_list->push_back((yyvsp[0].ast));
        			(yyval.ast_list)= arg_list;
  			}
		}
#line 1875 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 489 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  				(yyval.ast)= (yyvsp[0].ast);
  		}
#line 1883 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 496 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
    			{
      				Return_Ast* return_stmt = new Return_Ast(NULL, current_procedure->get_proc_name(), yylineno);
      				(yyval.ast) = return_stmt;
  			}
		}
#line 1894 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 504 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
    			{
      				Return_Ast* return_stmt = new Return_Ast((yyvsp[-1].ast), current_procedure->get_proc_name(), yylineno);
      				(yyval.ast) = return_stmt;
  			}
		}
#line 1905 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 514 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
         			(yyval.symbol_table) = (yyvsp[0].symbol_table);
  		}
#line 1913 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 518 "parser.y" /* yacc.c:1646  */
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
#line 1942 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 546 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
			{
				Data_Type recent_type_name = (yyvsp[-2].type_name);
        			Symbol_Table * decl_list = (yyvsp[-1].symbol_table);
	
				CHECK_INVARIANT (decl_list != NULL, "List of variables is NULL in type declarations");
	
			 	decl_list->set_type_of_all_syms(recent_type_name);
	
  				(yyval.symbol_table) = decl_list;
   		 	}	
		}
#line 1959 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 562 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
				(yyval.type_name) = int_data_type; 
		}
#line 1967 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 566 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
				(yyval.type_name) = double_data_type;
		}
#line 1975 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 570 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
				(yyval.type_name) = void_data_type; 
		}
#line 1983 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 574 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
				(yyval.type_name) = string_data_type; 
		}
#line 1991 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 578 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
				(yyval.type_name) = bool_data_type; 
		}
#line 1999 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 582 "parser.y" /* yacc.c:1646  */
    {
		}
#line 2006 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 588 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
	  		{
	        		string name = *(yyvsp[0].string_value);
	      			Symbol_Table_Entry * decl_entry = new Symbol_Table_Entry(name, yylineno);    
	      			Symbol_Table * decl_list = new Symbol_Table();
	    			decl_list->push_symbol(decl_entry);
	    			(yyval.symbol_table)=decl_list;
	  		}
	  	}
#line 2020 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 598 "parser.y" /* yacc.c:1646  */
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
#line 2037 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 614 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-3].ast) && (yyvsp[-1].ast)), "Lhs/Rhs of assignment expression cannot be null");
    				(yyval.ast) = new Assignment_Ast((yyvsp[-3].ast), (yyvsp[-1].ast), yylineno);
    				(yyval.ast)->type_check_ast();
  			}
		}
#line 2049 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 622 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-3].ast) && (yyvsp[-1].ast)), "Lhs/Rhs of assignment expression cannot be null");
    				(yyval.ast) = new Assignment_Ast((yyvsp[-3].ast), (yyvsp[-1].ast), yylineno);
    				(yyval.ast)->type_check_ast();
  			}
		}
#line 2061 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 630 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-3].ast) && (yyvsp[-1].ast)), "Lhs/Rhs of assignment expression cannot be null");
    				(yyval.ast) = new Assignment_Ast((yyvsp[-3].ast), (yyvsp[-1].ast), yylineno);
    				(yyval.ast)->type_check_ast();
  			}
		}
#line 2073 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 641 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
    				(yyval.ast) = new Selection_Statement_Ast((yyvsp[-4].ast),(yyvsp[-2].ast),(yyvsp[0].ast),yylineno);
  		}
#line 2081 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 645 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
    				(yyval.ast) = new Selection_Statement_Ast((yyvsp[-2].ast),(yyvsp[0].ast),NULL,yylineno);
  		}
#line 2089 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 652 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
    				(yyval.ast) = new Iteration_Statement_Ast((yyvsp[-2].ast),(yyvsp[-5].ast),yylineno,true);
		}
#line 2097 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 659 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
    				(yyval.ast) = new Iteration_Statement_Ast((yyvsp[-2].ast),(yyvsp[0].ast),yylineno,false);
  		}
#line 2105 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 666 "parser.y" /* yacc.c:1646  */
    { 	
  		}
#line 2112 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 670 "parser.y" /* yacc.c:1646  */
    {
	}
#line 2119 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 676 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT((yyvsp[-1].ast) , "A variable cannot be null in a print statement");
    				(yyval.ast) = new Write_Ast((yyvsp[-1].ast),yylineno);

  			}
		}
#line 2131 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 684 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
				string s = *(yyvsp[-1].string_value);
				int str_num = program_object.add_string_to_string_table(s);
    				String_Constant_Ast * str_ast = new String_Constant_Ast(str_num, string_data_type, yylineno);
    				(yyval.ast) = new Write_Ast(str_ast,yylineno);

  			}
		}
#line 2145 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 697 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT((yyvsp[-1].ast) , "A variable cannot be null in a read statement");
    				(yyval.ast) = new Read_Ast((yyvsp[-1].ast),yylineno);
  			}
		}
#line 2156 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 708 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-2].ast) && (yyvsp[0].ast)), "An arith_operand cannot have null entries");
    				(yyval.ast) = new Plus_Ast((yyvsp[-2].ast), (yyvsp[0].ast), yylineno);
    				(yyval.ast)->set_data_type((yyvsp[-2].ast)->get_data_type());
    				(yyval.ast)->type_check_ast();
  			}
  		}
#line 2169 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 717 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-2].ast) && (yyvsp[0].ast)), "An arith_operand cannot have null entries");
    				(yyval.ast) = new Minus_Ast((yyvsp[-2].ast), (yyvsp[0].ast), yylineno);
    				(yyval.ast)->set_data_type((yyvsp[-2].ast)->get_data_type());
    				(yyval.ast)->type_check_ast();
  			}
  		}
#line 2182 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 726 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-2].ast) && (yyvsp[0].ast)), "An arith_operand cannot have null entries");
    				(yyval.ast) = new Mult_Ast((yyvsp[-2].ast), (yyvsp[0].ast), yylineno);
    				(yyval.ast)->set_data_type((yyvsp[-2].ast)->get_data_type());
    				(yyval.ast)->type_check_ast();
  			}
  		}
#line 2195 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 735 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-2].ast) && (yyvsp[0].ast)), "An arith_operand cannot have null entries");
    				(yyval.ast) = new Divide_Ast((yyvsp[-2].ast), (yyvsp[0].ast), yylineno);
    				(yyval.ast)->set_data_type((yyvsp[-2].ast)->get_data_type());
    				(yyval.ast)->type_check_ast();
  			}
  		}
#line 2208 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 744 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT((yyvsp[0].ast), "An arith_operand cannot have null entries");
    				(yyval.ast) = new UMinus_Ast((yyvsp[0].ast), NULL, yylineno);
    				(yyval.ast)->set_data_type((yyvsp[0].ast)->get_data_type());
    				(yyval.ast)->type_check_ast();
  			}
  		}
#line 2221 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 753 "parser.y" /* yacc.c:1646  */
    {  	if (command_options.construct_ast())
  			{
    				(yyval.ast) = (yyvsp[-1].ast);
    				(yyval.ast)->set_data_type((yyvsp[-1].ast)->get_data_type());
  			}
  		}
#line 2232 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 760 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-4].ast) != NULL) && ((yyvsp[-2].ast) != NULL) && ((yyvsp[0].ast) != NULL), "lhs/rhs of arithmetic expression cannot be null");
    				Ast * cond = (yyvsp[-4].ast);
    				Ast * lhs = (yyvsp[-2].ast);
    				Ast * rhs = (yyvsp[0].ast);
    				Ast * cond_ast = new Conditional_Expression_Ast(cond, lhs, rhs, yylineno);
    				cond_ast->type_check_ast();
    				(yyval.ast) = cond_ast;
  			}
  		}
#line 2248 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 772 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[0].ast) != NULL), "An expression cannot have null values");
    				(yyval.ast) = (yyvsp[0].ast);
    				(yyval.ast)->set_data_type((yyvsp[0].ast)->get_data_type());
  			}
  		}
#line 2260 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 783 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[0].ast) != NULL), "Arithmetic expression cannot be null");
    				(yyval.ast) = (yyvsp[0].ast);
    				(yyval.ast)->set_data_type((yyvsp[0].ast)->get_data_type());
  			}
  		}
#line 2272 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 794 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[0].ast) != NULL), "An expression term cannot be null");
    				(yyval.ast) = (yyvsp[0].ast);
  			}
  		}
#line 2283 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 801 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[0].ast) != NULL), "An expression term cannot be null");
    				(yyval.ast) = (yyvsp[0].ast);
  			}
  		}
#line 2294 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 811 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-2].ast) && (yyvsp[0].ast)), "A rel_operand cannot have null entries");
    				(yyval.ast) = new Relational_Expr_Ast((yyvsp[-2].ast), less_than, (yyvsp[0].ast), yylineno);
    				(yyval.ast)->set_data_type(bool_data_type);
    				(yyval.ast)->type_check_ast();
  			}
  		}
#line 2307 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 820 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-2].ast) && (yyvsp[0].ast)), "A rel_operand cannot have null entries");
    				(yyval.ast) = new Relational_Expr_Ast((yyvsp[-2].ast), less_equalto, (yyvsp[0].ast), yylineno);
    				(yyval.ast)->set_data_type(bool_data_type);
    				(yyval.ast)->type_check_ast();
  			}
  		}
#line 2320 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 829 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-2].ast) && (yyvsp[0].ast)), "A rel_operand cannot have null entries");
    				(yyval.ast) = new Relational_Expr_Ast((yyvsp[-2].ast), greater_than, (yyvsp[0].ast), yylineno);
    				(yyval.ast)->set_data_type(bool_data_type);
    				(yyval.ast)->type_check_ast();
  			}
  		}
#line 2333 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 838 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-2].ast) && (yyvsp[0].ast)), "A rel_operand cannot have null entries");
    				(yyval.ast) = new Relational_Expr_Ast((yyvsp[-2].ast), greater_equalto, (yyvsp[0].ast), yylineno);
    				(yyval.ast)->set_data_type(bool_data_type);
    				(yyval.ast)->type_check_ast();
  			}
  		}
#line 2346 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 847 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-2].ast) && (yyvsp[0].ast)), "A rel_operand cannot have null entries");
    				(yyval.ast) = new Relational_Expr_Ast((yyvsp[-2].ast), not_equalto, (yyvsp[0].ast), yylineno);
    				(yyval.ast)->set_data_type(bool_data_type);
    				(yyval.ast)->type_check_ast();
  			}
  		}
#line 2359 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 856 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-2].ast) && (yyvsp[0].ast)), "A rel_operand cannot have null entries");
    				(yyval.ast) = new Relational_Expr_Ast((yyvsp[-2].ast), equalto, (yyvsp[0].ast), yylineno);
    				(yyval.ast)->set_data_type(bool_data_type);
    				(yyval.ast)->type_check_ast();
  			}
  		}
#line 2372 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 868 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-2].ast) != NULL) && ((yyvsp[0].ast) != NULL), "lhs/rhs of boolean expression cannot be null");
    				Logical_Op lop = _logical_and;
    				Logical_Expr_Ast * logical_expr_ast = new Logical_Expr_Ast((yyvsp[-2].ast), lop, (yyvsp[0].ast), yylineno);
    				(yyval.ast)->set_data_type(bool_data_type);
    				(yyval.ast) = logical_expr_ast;
      				(yyval.ast)->type_check_ast();
  			}
  		}
#line 2387 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 879 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[-2].ast) != NULL) && ((yyvsp[0].ast) != NULL), "lhs/rhs of boolean expression cannot be null");
    				Logical_Op lop = _logical_or;
    				Logical_Expr_Ast * logical_expr_ast = new Logical_Expr_Ast((yyvsp[-2].ast), lop, (yyvsp[0].ast), yylineno);
    				(yyval.ast)->set_data_type(bool_data_type);
    				(yyval.ast) = logical_expr_ast;
    				(yyval.ast)->type_check_ast();
  			}
		}
#line 2402 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 890 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
  			{
    				CHECK_INVARIANT(((yyvsp[0].ast) != NULL) , "rhs of boolean expression cannot be null");
    				Logical_Op lop = _logical_not;
    				Logical_Expr_Ast * logical_expr_ast = new Logical_Expr_Ast(NULL, lop, (yyvsp[0].ast), yylineno);
    				(yyval.ast)->set_data_type(bool_data_type);
    				(yyval.ast) = logical_expr_ast;
    				(yyval.ast)->type_check_ast();
  			}
		}
#line 2417 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 901 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
    				(yyval.ast) = (yyvsp[-1].ast);
  		}
#line 2425 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 905 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
    				(yyval.ast) = (yyvsp[0].ast);
  		}
#line 2433 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 909 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
    				(yyval.ast) = (yyvsp[0].ast);
  		}
#line 2441 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 917 "parser.y" /* yacc.c:1646  */
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
#line 2464 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 941 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
    				(yyval.ast) = new Number_Ast<int>((yyvsp[0].integer_value), int_data_type, yylineno);
  		}
#line 2472 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 945 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
    				(yyval.ast) = new Number_Ast<double>((yyvsp[0].double_value), double_data_type, yylineno);
  		}
#line 2480 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 949 "parser.y" /* yacc.c:1646  */
    { 	if (command_options.construct_ast())
			{
				string s = *(yyvsp[0].string_value);
				int str_num = program_object.add_string_to_string_table(s);
    				(yyval.ast) = new String_Constant_Ast(str_num, string_data_type, yylineno);
			}
  		}
#line 2492 "parser.tab.c" /* yacc.c:1646  */
    break;


#line 2496 "parser.tab.c" /* yacc.c:1646  */
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
