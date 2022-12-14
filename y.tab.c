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
#line 1 "class_parser.y" /* yacc.c:339  */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#line 72 "y.tab.c" /* yacc.c:339  */

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
#line 7 "class_parser.y" /* yacc.c:355  */

	int integer_value;
	double double_value;
	char *string_value;


#line 185 "y.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 202 "y.tab.c" /* yacc.c:358  */

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
#define YYLAST   424

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  39
/* YYNRULES -- Number of rules.  */
#define YYNRULES  103
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  206

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
       0,    45,    45,    49,    55,    59,    63,    66,    69,    75,
      80,    84,    85,    90,    93,    97,   103,   106,   111,   117,
     120,   126,   131,   137,   143,   149,   154,   159,   165,   174,
     177,   183,   189,   192,   195,   201,   204,   210,   213,   216,
     219,   222,   225,   228,   231,   234,   240,   246,   249,   256,
     259,   265,   271,   275,   281,   284,   290,   296,   299,   302,
     305,   308,   311,   317,   320,   326,   329,   332,   338,   341,
     347,   353,   359,   363,   369,   372,   378,   385,   388,   391,
     394,   397,   400,   403,   406,   412,   418,   421,   427,   430,
     433,   436,   439,   442,   448,   451,   454,   457,   460,   463,
     470,   478,   481,   484
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

#define YYPACT_NINF -66

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-66)))

#define YYTABLE_NINF -101

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     391,    15,   -66,   -66,   -66,   -66,   -66,   -66,    26,   409,
     -66,   -66,   409,   -66,   -66,    29,    22,   -66,   -66,   409,
     -66,   -66,    61,   -28,    93,    91,    19,    11,   -66,    64,
     -66,   -66,    32,    76,    17,   -66,   -66,   -66,    38,   -34,
     -66,    -9,   -66,    99,   -66,    66,   391,    75,     3,    99,
     137,   -66,    83,   131,    78,   131,   -66,   -66,   409,   124,
     101,    -5,   106,   121,   170,   167,   158,    96,   218,   -66,
     233,   -66,   -66,   127,   -66,   152,   -66,   169,   -66,   -66,
     -66,   -66,   -66,   -66,   -66,   168,   -66,   -66,   -66,   188,
     -66,   -66,   145,   250,   323,   323,   149,   176,   -66,   153,
     163,   173,   -66,   -66,   -66,   158,   323,   323,   323,   -66,
     178,   379,   -66,   -66,   157,   -66,   -66,   -66,   261,   -66,
     -66,   -66,   -66,   276,   -66,   -66,   329,   -66,   291,   203,
     115,   -66,     4,   -66,    12,   -66,    89,   239,   162,   -66,
     -66,   -66,   -66,   -66,   -66,   342,   315,   -66,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   -66,   -66,   185,   189,    50,   -66,   -66,   306,   193,
      88,   -66,   323,   170,   170,   323,   -66,   -66,   379,   379,
     379,   379,   379,   379,   191,   191,   -66,   -66,   356,   205,
     -66,   -66,   -66,   -66,   -66,   198,   -66,   224,   -66,   327,
     323,   170,   200,   379,   -66,   -66
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,    62,    57,    58,    60,    61,    59,     0,     0,
       8,     7,     3,    20,     6,     0,     0,     1,     4,     2,
       5,    19,     0,    63,     0,     0,     0,     0,    56,     0,
      12,    11,     0,     0,     0,    32,    33,    34,     0,     0,
      17,     0,    30,    18,    64,     0,     0,     0,     0,     0,
       0,    14,     0,     0,     0,     0,    31,     9,    10,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,    23,
       0,    36,    44,     0,    45,     0,    54,     0,    37,    38,
      39,    40,    41,    42,    43,     0,    13,    16,    18,     0,
      15,    29,    63,     0,     0,     0,   100,     0,   100,     0,
       0,     0,   101,   102,   103,     0,     0,     0,     0,    52,
      85,     0,    84,    98,     0,    86,    87,    73,     0,    28,
      35,    46,    26,     0,    55,    63,     0,    21,     0,     0,
       0,    48,     0,    50,    85,    85,     0,     0,     0,    75,
      74,    76,    99,    81,    96,     0,     0,    53,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    72,    24,     0,    85,     0,    27,    25,     0,     0,
       0,    47,     0,     0,     0,     0,    82,    97,    92,    93,
      88,    89,    90,    91,    77,    78,    79,    80,     0,    95,
      94,    66,    65,    67,    22,     0,    49,    69,    71,     0,
       0,     0,     0,    83,    68,    70
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -66,   -66,   201,   -66,   -66,   -66,    -4,   -66,   195,   243,
      -6,   -30,   207,   -15,   -58,   -62,   -66,   138,   -66,    95,
     -66,   179,    -8,    51,   -66,   -66,   -66,   -66,   -66,   -66,
     -66,   -66,   -64,   -65,   -66,   -66,   -61,   -50,   -66
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     8,     9,    10,    32,    33,    11,    39,    40,    12,
      13,    41,    42,    43,    70,    71,    72,    73,   132,   133,
      74,    75,    14,    77,    24,    78,    79,    80,    81,    82,
      83,    84,   135,   111,   112,   113,   114,   115,   116
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      85,    20,    97,   110,    48,    18,    21,    52,   120,  -100,
     118,    53,    27,    21,    85,   100,   101,   123,    85,    49,
      85,    35,    36,    16,    37,    85,    17,    35,    36,   134,
      37,   128,    54,   136,   137,    93,    55,    23,    88,    85,
      49,   143,    76,   145,    60,   171,   144,   146,    55,   172,
      20,    15,    38,   -51,    18,   142,   120,   -51,    47,    34,
      15,   120,   164,    22,    25,   165,   120,   124,    85,    26,
      22,   168,    44,    85,   158,    45,   159,   160,    85,    85,
      50,    76,    51,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   193,    30,    31,    59,   189,   190,
     170,    46,   102,   103,    98,   104,   120,    56,   134,    59,
      57,   197,   198,   158,   199,   159,   160,    50,    85,   105,
      89,   124,    90,    85,    85,    35,    36,    86,    37,   195,
     173,   106,    92,    55,   107,   203,   108,    28,    29,   204,
     109,    35,    36,    89,    37,    61,    94,     3,     4,     5,
       6,    85,     7,    62,    63,    64,   169,    65,    66,    67,
      61,    95,     3,     4,     5,     6,    98,     7,    62,    63,
      64,   121,    65,    66,    67,    98,    99,   125,    96,    68,
      69,   158,   126,   159,   160,   130,    62,    63,    64,    93,
      65,    66,    67,   138,    68,   122,    61,   139,     3,     4,
       5,     6,   175,     7,    62,    63,    64,   140,    65,    66,
      67,    61,    68,     3,     4,     5,     6,   141,     7,    62,
      63,    64,   147,    65,    66,    67,    96,   156,   157,   191,
      68,   127,   160,   192,    62,    63,    64,    51,    65,    66,
      67,    96,    90,   201,   205,    68,   167,    58,    87,    62,
      63,    64,    19,    65,    66,    67,   102,   103,    98,   104,
      68,   117,    91,   158,   163,   159,   160,   196,   129,    96,
       0,     0,     0,   105,     0,    68,   119,    62,    63,    64,
     174,    65,    66,    67,    96,   106,     0,     0,   107,     0,
     108,   131,    62,    63,    64,     0,    65,    66,    67,    96,
       0,     0,     0,    68,   161,     0,     0,    62,    63,    64,
       0,    65,    66,    67,    96,     0,     0,     0,    68,   162,
       0,     0,    62,    63,    64,     0,    65,    66,    67,   102,
     103,    98,   104,    68,   166,   102,   103,    96,   104,   158,
       0,   159,   160,     0,     0,     0,   105,     0,    68,   194,
       0,   158,   105,   159,   160,     0,   177,     0,   106,     0,
       0,   107,     0,   108,   106,     0,     0,   107,   202,   108,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
       0,   200,     0,   176,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,     1,     0,     0,     0,     0,     2,
       0,     3,     4,     5,     6,     0,     7,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,     2,     0,     3,
       4,     5,     6,     0,     7
};

static const yytype_int16 yycheck[] =
{
      50,     9,    64,    67,    34,     9,    12,    41,    70,    14,
      68,    45,    40,    19,    64,    65,    66,    75,    68,    34,
      70,    10,    11,     8,    13,    75,     0,    10,    11,    93,
      13,    89,    41,    94,    95,    40,    45,     8,    53,    89,
      55,   106,    50,   108,    41,    41,   107,   108,    45,    45,
      58,     0,    41,    41,    58,   105,   118,    45,    41,    40,
       9,   123,   126,    12,    42,   126,   128,    75,   118,     8,
      19,   129,     8,   123,    24,    43,    26,    27,   128,   129,
      42,    89,    44,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,    44,     4,     5,    46,   159,   160,
     130,    25,     6,     7,     8,     9,   168,     8,   172,    58,
      44,   173,   174,    24,   175,    26,    27,    42,   168,    23,
      42,   129,    44,   173,   174,    10,    11,    44,    13,    41,
      41,    35,     8,    45,    38,   200,    40,    44,    45,   201,
      44,    10,    11,    42,    13,     8,    40,    10,    11,    12,
      13,   201,    15,    16,    17,    18,    41,    20,    21,    22,
       8,    40,    10,    11,    12,    13,     8,    15,    16,    17,
      18,    44,    20,    21,    22,     8,     9,     8,     8,    42,
      43,    24,    14,    26,    27,    40,    16,    17,    18,    40,
      20,    21,    22,    17,    42,    43,     8,    44,    10,    11,
      12,    13,    40,    15,    16,    17,    18,    44,    20,    21,
      22,     8,    42,    10,    11,    12,    13,    44,    15,    16,
      17,    18,    44,    20,    21,    22,     8,    36,    37,    44,
      42,    43,    27,    44,    16,    17,    18,    44,    20,    21,
      22,     8,    44,    19,    44,    42,    43,    46,    53,    16,
      17,    18,     9,    20,    21,    22,     6,     7,     8,     9,
      42,    43,    55,    24,   126,    26,    27,   172,    89,     8,
      -1,    -1,    -1,    23,    -1,    42,    43,    16,    17,    18,
      41,    20,    21,    22,     8,    35,    -1,    -1,    38,    -1,
      40,    41,    16,    17,    18,    -1,    20,    21,    22,     8,
      -1,    -1,    -1,    42,    43,    -1,    -1,    16,    17,    18,
      -1,    20,    21,    22,     8,    -1,    -1,    -1,    42,    43,
      -1,    -1,    16,    17,    18,    -1,    20,    21,    22,     6,
       7,     8,     9,    42,    43,     6,     7,     8,     9,    24,
      -1,    26,    27,    -1,    -1,    -1,    23,    -1,    42,    43,
      -1,    24,    23,    26,    27,    -1,    41,    -1,    35,    -1,
      -1,    38,    -1,    40,    35,    -1,    -1,    38,    41,    40,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      -1,    25,    -1,    41,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,     3,    -1,    -1,    -1,    -1,     8,
      -1,    10,    11,    12,    13,    -1,    15,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,     8,    -1,    10,
      11,    12,    13,    -1,    15
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
      41,     8,    16,    17,    18,    20,    21,    22,    42,    43,
      60,    61,    62,    63,    66,    67,    68,    69,    71,    72,
      73,    74,    75,    76,    77,    83,    44,    54,    59,    42,
      44,    58,     8,    40,    40,    40,     8,    61,     8,     9,
      83,    83,     6,     7,     9,    23,    35,    38,    40,    44,
      78,    79,    80,    81,    82,    83,    84,    43,    60,    43,
      61,    44,    43,    60,    68,     8,    14,    43,    60,    67,
      40,    41,    64,    65,    78,    78,    82,    82,    17,    44,
      44,    44,    83,    79,    82,    79,    82,    44,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    24,    26,
      27,    43,    43,    63,    78,    82,    43,    43,    60,    41,
      57,    41,    45,    41,    41,    40,    41,    41,    79,    79,
      79,    79,    79,    79,    79,    79,    79,    79,    79,    82,
      82,    44,    44,    44,    43,    41,    65,    61,    61,    82,
      25,    19,    41,    79,    61,    44
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    46,    47,    47,    48,    48,    48,    48,    48,    49,
      50,    51,    51,    52,    52,    52,    53,    53,    54,    55,
      55,    56,    56,    56,    56,    56,    56,    56,    56,    57,
      57,    58,    59,    59,    59,    60,    60,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    62,    63,    63,    64,
      64,    65,    66,    66,    67,    67,    68,    69,    69,    69,
      69,    69,    69,    70,    70,    71,    71,    71,    72,    72,
      73,    74,    75,    75,    76,    76,    77,    78,    78,    78,
      78,    78,    78,    78,    78,    79,    80,    80,    81,    81,
      81,    81,    81,    81,    82,    82,    82,    82,    82,    82,
      83,    84,    84,    84
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     1,     2,     2,     1,     1,     1,     6,
       3,     1,     1,     6,     5,     6,     3,     1,     1,     2,
       1,     7,     9,     6,     8,     8,     7,     8,     7,     3,
       1,     2,     1,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     4,     3,     3,
       1,     1,     2,     3,     1,     2,     3,     1,     1,     1,
       1,     1,     1,     1,     3,     4,     4,     4,     7,     5,
       7,     5,     3,     2,     3,     3,     3,     3,     3,     3,
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
#line 47 "class_parser.y" /* yacc.c:1646  */
    { 	printf("global_decl_statement func_def_list ");
		}
#line 1470 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 50 "class_parser.y" /* yacc.c:1646  */
    { printf("\nfunc_def_list ");
                }
#line 1477 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 57 "class_parser.y" /* yacc.c:1646  */
    { printf("\nglobal_decl_statement func_decl");
		}
#line 1484 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 61 "class_parser.y" /* yacc.c:1646  */
    { 	printf("\nglobal_decl_statement var_decl");
  		}
#line 1491 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 64 "class_parser.y" /* yacc.c:1646  */
    {   	printf("\nvar_decl");
 		}
#line 1498 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 67 "class_parser.y" /* yacc.c:1646  */
    {   	printf("\nfunc_decl");
 		}
#line 1505 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 70 "class_parser.y" /* yacc.c:1646  */
    {	printf("\nclass_declaration_stat");
		}
#line 1512 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 76 "class_parser.y" /* yacc.c:1646  */
    { printf("\nCLASS");
		}
#line 1519 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 91 "class_parser.y" /* yacc.c:1646  */
    {  	printf("\ntype NAME '(' dummy_formal_param_list ')' ';'  ");
		}
#line 1526 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 94 "class_parser.y" /* yacc.c:1646  */
    { printf("\ntype NAME '('')' ';' ");
  			
		}
#line 1534 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 98 "class_parser.y" /* yacc.c:1646  */
    { 	printf("\ntype NAME '(' formal_param_list ')' ';'");
		}
#line 1541 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 104 "class_parser.y" /* yacc.c:1646  */
    { printf("\ndummy_formal_param_list ',' dummy_formal_param ");	
		}
#line 1548 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 107 "class_parser.y" /* yacc.c:1646  */
    { printf("\ndummy_formal_param"); }
#line 1554 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 112 "class_parser.y" /* yacc.c:1646  */
    { printf("\nparam_type");	
		}
#line 1561 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 118 "class_parser.y" /* yacc.c:1646  */
    { printf("\nfunc_def_list func_def");
		}
#line 1568 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 121 "class_parser.y" /* yacc.c:1646  */
    { printf("\nfunc_def");
		}
#line 1575 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 128 "class_parser.y" /* yacc.c:1646  */
    { printf("\n  type NAME '(' formal_param_list')'  '{' '}'");
		}
#line 1582 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 134 "class_parser.y" /* yacc.c:1646  */
    { printf("\n type NAME '(' formal_param_list')'  '{' var_decl_list  statement_list '}'");
		}
#line 1589 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 139 "class_parser.y" /* yacc.c:1646  */
    { printf("\n type NAME '(' ')'  '{'  '}'  ");
		}
#line 1596 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 146 "class_parser.y" /* yacc.c:1646  */
    { printf("\n type NAME '(' ')' '{' var_decl_list statement_list '}'   ");
		}
#line 1603 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 151 "class_parser.y" /* yacc.c:1646  */
    { printf("\ntype NAME '(' formal_param_list')' '{' var_decl_list '}'");
		}
#line 1610 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 156 "class_parser.y" /* yacc.c:1646  */
    { printf("\ntype NAME '(' ')' '{' var_decl_list '}'");
		}
#line 1617 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 162 "class_parser.y" /* yacc.c:1646  */
    { printf("\ntype NAME '(' formal_param_list')' '{' statement_list '}'");
		}
#line 1624 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 168 "class_parser.y" /* yacc.c:1646  */
    { printf("\ntype NAME '(' ')' '{' statement_list '}'");
		}
#line 1631 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 175 "class_parser.y" /* yacc.c:1646  */
    { printf("\nformal_param_list ',' formal_param ");
		}
#line 1638 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 178 "class_parser.y" /* yacc.c:1646  */
    { printf("\nformal_param");
		}
#line 1645 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 184 "class_parser.y" /* yacc.c:1646  */
    { printf("\nparam_type NAME ");
		}
#line 1652 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 190 "class_parser.y" /* yacc.c:1646  */
    { printf("\nINTEGER ");	
		}
#line 1659 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 193 "class_parser.y" /* yacc.c:1646  */
    { printf("\nFLOAT");
		}
#line 1666 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 196 "class_parser.y" /* yacc.c:1646  */
    { printf("\nBOOL");
		}
#line 1673 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 202 "class_parser.y" /* yacc.c:1646  */
    { 	printf("\nstatement_list statement ");
  		}
#line 1680 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 205 "class_parser.y" /* yacc.c:1646  */
    { printf("\nstatement");
	}
#line 1687 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 211 "class_parser.y" /* yacc.c:1646  */
    { 	printf("\nstatement");
           	}
#line 1694 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 214 "class_parser.y" /* yacc.c:1646  */
    { 	printf("\nstatement");
           	}
#line 1701 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 217 "class_parser.y" /* yacc.c:1646  */
    { 	printf("\nstatement");
           	}
#line 1708 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 220 "class_parser.y" /* yacc.c:1646  */
    { 	printf("\nstatement");
           	}
#line 1715 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 223 "class_parser.y" /* yacc.c:1646  */
    { 	printf("\nstatement");
           	}
#line 1722 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 226 "class_parser.y" /* yacc.c:1646  */
    { 	printf("\nstatement");
           	}
#line 1729 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 229 "class_parser.y" /* yacc.c:1646  */
    { 	printf("\nstatement");
           	}
#line 1736 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 232 "class_parser.y" /* yacc.c:1646  */
    { 	printf("\nstatement");
               	}
#line 1743 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 235 "class_parser.y" /* yacc.c:1646  */
    { 	printf("\nstatement");
                }
#line 1750 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 241 "class_parser.y" /* yacc.c:1646  */
    { 	printf("\nfunc_call ';'");
		}
#line 1757 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 247 "class_parser.y" /* yacc.c:1646  */
    { printf("\nNAME '(' non_empty_arg_list  ')'");
	}
#line 1764 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 250 "class_parser.y" /* yacc.c:1646  */
    { printf("\n NAME '(' ')'");
	}
#line 1771 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 257 "class_parser.y" /* yacc.c:1646  */
    { printf("\nnon_empty_arg_list ',' actual_arg ");	
		}
#line 1778 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 260 "class_parser.y" /* yacc.c:1646  */
    { 	printf("\nactual_arg ");
		}
#line 1785 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 266 "class_parser.y" /* yacc.c:1646  */
    { 	printf("\narith_expression");
  		}
#line 1792 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 272 "class_parser.y" /* yacc.c:1646  */
    { printf("\nRETURN ';'");
		}
#line 1799 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 276 "class_parser.y" /* yacc.c:1646  */
    { 	printf("\n RETURN arith_expression ';'");
		}
#line 1806 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 282 "class_parser.y" /* yacc.c:1646  */
    { printf("\nvar_decl ");	
  		}
#line 1813 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 285 "class_parser.y" /* yacc.c:1646  */
    { 	printf("\nvar_decl_list var_decl ");
		}
#line 1820 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 291 "class_parser.y" /* yacc.c:1646  */
    { printf("\ntype variables_list ';'");	
		}
#line 1827 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 297 "class_parser.y" /* yacc.c:1646  */
    { printf("\nINTERGER");
		}
#line 1834 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 300 "class_parser.y" /* yacc.c:1646  */
    { 	printf("\nFLOAT");
		}
#line 1841 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 303 "class_parser.y" /* yacc.c:1646  */
    { 	printf("\nVOID");
		}
#line 1848 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 306 "class_parser.y" /* yacc.c:1646  */
    { 	printf("\nSTRING");
		}
#line 1855 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 309 "class_parser.y" /* yacc.c:1646  */
    { 	printf("\nBOOL");
		}
#line 1862 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 312 "class_parser.y" /* yacc.c:1646  */
    { printf("\nNAME");
		}
#line 1869 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 318 "class_parser.y" /* yacc.c:1646  */
    { 	
	  	}
#line 1876 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 321 "class_parser.y" /* yacc.c:1646  */
    { 	
  		}
#line 1883 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 327 "class_parser.y" /* yacc.c:1646  */
    { 	
		}
#line 1890 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 330 "class_parser.y" /* yacc.c:1646  */
    { 	
		}
#line 1897 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 333 "class_parser.y" /* yacc.c:1646  */
    { 	
		}
#line 1904 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 339 "class_parser.y" /* yacc.c:1646  */
    { 	
  		}
#line 1911 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 342 "class_parser.y" /* yacc.c:1646  */
    { 	
  		}
#line 1918 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 348 "class_parser.y" /* yacc.c:1646  */
    { 	
		}
#line 1925 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 354 "class_parser.y" /* yacc.c:1646  */
    { 	
  		}
#line 1932 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 360 "class_parser.y" /* yacc.c:1646  */
    { 	
  		}
#line 1939 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 364 "class_parser.y" /* yacc.c:1646  */
    {
	}
#line 1946 "y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 370 "class_parser.y" /* yacc.c:1646  */
    { 	
		}
#line 1953 "y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 373 "class_parser.y" /* yacc.c:1646  */
    { 	
		}
#line 1960 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 379 "class_parser.y" /* yacc.c:1646  */
    { 	
		}
#line 1967 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 386 "class_parser.y" /* yacc.c:1646  */
    { 	
  		}
#line 1974 "y.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 389 "class_parser.y" /* yacc.c:1646  */
    { 	
  		}
#line 1981 "y.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 392 "class_parser.y" /* yacc.c:1646  */
    { 	
  		}
#line 1988 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 395 "class_parser.y" /* yacc.c:1646  */
    { 	
  		}
#line 1995 "y.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 398 "class_parser.y" /* yacc.c:1646  */
    { 	
  		}
#line 2002 "y.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 401 "class_parser.y" /* yacc.c:1646  */
    {  	
  		}
#line 2009 "y.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 404 "class_parser.y" /* yacc.c:1646  */
    { 	
  		}
#line 2016 "y.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 407 "class_parser.y" /* yacc.c:1646  */
    { 	
  		}
#line 2023 "y.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 413 "class_parser.y" /* yacc.c:1646  */
    { 	
  		}
#line 2030 "y.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 419 "class_parser.y" /* yacc.c:1646  */
    { 	
  		}
#line 2037 "y.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 422 "class_parser.y" /* yacc.c:1646  */
    { 	
  		}
#line 2044 "y.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 428 "class_parser.y" /* yacc.c:1646  */
    { 	
  		}
#line 2051 "y.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 431 "class_parser.y" /* yacc.c:1646  */
    { 	
  		}
#line 2058 "y.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 434 "class_parser.y" /* yacc.c:1646  */
    { 	
  		}
#line 2065 "y.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 437 "class_parser.y" /* yacc.c:1646  */
    { 	
  		}
#line 2072 "y.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 440 "class_parser.y" /* yacc.c:1646  */
    { 	
  		}
#line 2079 "y.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 443 "class_parser.y" /* yacc.c:1646  */
    { 	
  		}
#line 2086 "y.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 449 "class_parser.y" /* yacc.c:1646  */
    { 	
  		}
#line 2093 "y.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 452 "class_parser.y" /* yacc.c:1646  */
    { 	
		}
#line 2100 "y.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 455 "class_parser.y" /* yacc.c:1646  */
    { 	
		}
#line 2107 "y.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 458 "class_parser.y" /* yacc.c:1646  */
    { 	
  		}
#line 2114 "y.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 461 "class_parser.y" /* yacc.c:1646  */
    { 	
  		}
#line 2121 "y.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 464 "class_parser.y" /* yacc.c:1646  */
    { 	
  		}
#line 2128 "y.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 471 "class_parser.y" /* yacc.c:1646  */
    { 	
  		}
#line 2135 "y.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 479 "class_parser.y" /* yacc.c:1646  */
    { 	
  		}
#line 2142 "y.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 482 "class_parser.y" /* yacc.c:1646  */
    { 	
  		}
#line 2149 "y.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 485 "class_parser.y" /* yacc.c:1646  */
    { 	
  		}
#line 2156 "y.tab.c" /* yacc.c:1646  */
    break;


#line 2160 "y.tab.c" /* yacc.c:1646  */
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
