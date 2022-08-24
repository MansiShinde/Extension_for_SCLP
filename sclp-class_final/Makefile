
#############################################################################################
#
#                                sclp : A C Language Processor
#                                --------------------------------
#
#           About:
#
#           Originally implemented in 2012 by Tanu Kanvar and Uday Khedker 
#           (http://www.cse.iitb.ac.in/~uday) for the courses cs302+cs316
#           Language Processors (theory and lab) at IIT Bombay.
#
#           Initial release date Jan 15, 2013.
#
#           Updated by N. Venkatesh in Jan 2019.
#
#           Updated by Uday Khedker with the help of Nisha Biju and Saari
#           Rajan in Jan 2020.
#
#
#           Copyrights reserved  by Uday Khedker. This  implemenation has
#           been made available purely  for academic purposes without any
#           warranty of any kind.
#
#
#
###############################################################################################

AST_OBJ = ast.o tac-gen.o tac.o rtl.o rtl-gen.o type-checking.o type-expressions.o
# --------------- Changed on 25-02-2020 ---------
BB_OBJ = basic-block.o 
CFG_OBJ = cfg.o cfg-gen.o
LV_OBJ = lv-dfv.o lv-dfv-gen.o#Rnd Code
AV_OBJ = av-dfv.o av-dfv-gen.o#Rnd Code
SSA_OBJ = ssa-gen.o
# --------------- till here ---------------------
PROG_OBJ = program.o program-compile.o versioning.o
PROC_OBJ = procedure.o procedure-compile.o
SYMTAB_OBJ = symbol-table.o symbol-table-compile.o
TOKEN_OBJ = store-tokens.o
OBJ_CORE = error-display.o main.o user-options.o reg-alloc.o #stacktrace.o
OBJ_GEN = scanner.o parser.tab.o


# --------------- Changed on 25-02-2020 ---------
HEADERS_CORE = error-display.hh user-options.hh ast.hh symbol-table.hh program.hh procedure.hh rtl.hh reg-alloc.hh common-classes.hh store-tokens.hh tac.hh versioning.hh basic-block.hh cfg.hh 

HEADERS_EXTRA = lv-dfv.hh av-dfv.hh

HEADERS = $(HEADERS_CORE) $(HEADERS_EXTRA)

# --------------- Changed on 25-02-2020 ---------
OBJ = $(OBJ_GEN) $(OBJ_CORE) $(AST_OBJ) $(PROC_OBJ) $(PROG_OBJ) $(SYMTAB_OBJ) $(TOKEN_OBJ) $(BB_OBJ) $(CFG_OBJ) $(LV_OBJ) $(AV_OBJ)

CPP = g++
BISON = bison
FLEX = flex
TGT = sclp

# In order to create a reference implementation for level li, choose parameters 
# (li, phase_asm).  For example, for creating full compiler for level L2, use 
# the options "-e L=L2 -e P=asm". 

L = L5
P = phase_asm

VERSION = -DLEVEL=$(L) -DPHASE=$(P)
CFLAGS = -g -rdynamic

$(TGT): $(OBJ) 
	$(CPP) --std=c++0x $(CFLAGS) $(OBJ) -o $(TGT) -ly

# removed -lfl option from the above command and added %option noyywrap in scanner.l file.

scanner.o: scanner.c $(HEADERS)
	$(CPP) --std=c++0x  $(CFLAGS) -c $<

parser.tab.o:parser.tab.c $(HEADERS)
	$(CPP) --std=c++0x  $(CFLAGS) -c -fpermissive $<

main.o: main.cc $(HEADERS)
	$(CPP) --std=c++0x  $(CFLAGS) $(VERSION) -c main.cc

%.o: %.cc $(HEADERS)
	$(CPP) --std=c++0x  $(CFLAGS) -c $<

%.o: %.cxx $(HEADERS)
	$(CPP) --std=c++14  $(CFLAGS) -c $<

scanner.c : scanner.l parser.tab.h
	$(FLEX) -l --yylineno -o scanner.c scanner.l 

parser.tab.c parser.tab.h : parser.y
	$(BISON) -dv  parser.y
clean :
	rm -f *.o 
	rm -f $(TGT) *.output *.spim *.toks *.ast *.sym *.icode *.tac *.rtl *.log
	rm -rf parser.tab.c lex.yy.c parser.tab.h scanner.c
	rm -rf html latex
