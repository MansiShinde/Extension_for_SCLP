
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

AST_OBJ = ast.o tac-gen.o tac.o rtl.o rtl-gen.o type-checking.o
PROG_OBJ = program.o program-compile.o
PROC_OBJ = procedure.o procedure-compile.o
SYMTAB_OBJ = symbol-table.o symbol-table-compile.o
TOKEN_OBJ = store-tokens.o
OBJ_CORE = error-display.o main.o user-options.o reg-alloc.o
OBJ_GEN = scanner.o parser.tab.o

HEADERS_CORE = error-display.hh user-options.hh ast.hh symbol-table.hh program.hh procedure.hh rtl.hh reg-alloc.hh common-classes.hh store-tokens.hh tac.hh

HEADERS = $(HEADERS_CORE)
OBJ = $(OBJ_GEN) $(OBJ_CORE) $(AST_OBJ) $(PROC_OBJ) $(PROG_OBJ) $(SYMTAB_OBJ) $(TOKEN_OBJ) 	

CPP = g++
BISON = bison
FLEX = flex
#TGT = test/sclp
TGT = sclp


CFLAGS = -g

$(TGT): $(OBJ) 
	$(CPP) --std=c++0x  $(OBJ) -o $(TGT) -ly

# removed -lfl option from the above command and added %option noyywrap in scanner.l file.

scanner.o: scanner.c $(HEADERS)
	$(CPP) --std=c++0x  $(CFLAGS) -c $<

parser.tab.o:parser.tab.c $(HEADERS)
	$(CPP) --std=c++0x  $(CFLAGS) -c -fpermissive $<

%.o: %.cc $(HEADERS)
	$(CPP) --std=c++0x  $(CFLAGS) -c $<

scanner.c : scanner.l parser.tab.h
	$(FLEX) -l --yylineno -o scanner.c scanner.l 

parser.tab.c parser.tab.h : parser.y
	$(BISON) -dv  parser.y

check_compiler:
	@# create .toks .ast .eval files	
	@for file in `ls testcompiler/*.cs316.c`; do \
		./sclp $$file -tokens; \
		mv $$file.toks $$file.toks.new; \
		./sclp $$file -ast; \
		mv $$file.ast $$file.ast.new; \
		./sclp $$file -icode; \
		mv $$file.ic $$file.ic.new; \
		./sclp $$file -symtab; \
		mv $$file.sym $$file.sym.new; \
		./sclp $$file -compile; \
		mv $$file.spim $$file.spim.new; \
	done
	
	@# compare .toks .ast .eval files
	@for clpfile in $(shell find -iname '2test[1-9].cs316.c' | cut -f3 -d"/"); do \
		diff -w -b -B testcompiler/$$clpfile.toks.new test_compiler_output/$$clpfile.toks.old; \
		RETVAL=$$?; \
		if [ $$RETVAL -eq 0 ]; then \
			echo correct $$clpfile -toks; \
		else \
			echo failed $$clpfile - toks; \
		fi; \
		diff -w -b -B testcompiler/$$clpfile.ast.new test_compiler_output/$$clpfile.ast.old; \
		RETVAL=$$?; \
		if [ $$RETVAL -eq 0 ]; then \
			echo correct $$clpfile - ast; \
		else \
			echo failed $$clpfile - ast; \
		fi; \
		diff -w -b -B testcompiler/$$clpfile.ic.new test_compiler_output/$$clpfile.ic.old; \
		RETVAL=$$?; \
		if [ $$RETVAL -eq 0 ]; then \
			echo correct $$clpfile -ic; \
		else \
			echo failed $$clpfile - ic; \
		fi; \
		diff -w -b -B testcompiler/$$clpfile.sym.new test_compiler_output/$$clpfile.sym.old; \
		RETVAL=$$?; \
		if [ $$RETVAL -eq 0 ]; then \
			echo correct $$clpfile -symtab; \
		else \
			echo failed $$sclpfile - sym; \
		fi; \
		diff -w -b -B testcompiler/$$clpfile.spim.new test_compiler_output/$$clpfile.spim.old; \
		RETVAL=$$?; \
		if [ $$RETVAL -eq 0 ]; then \
			echo correct $$clpfile -spim; \
		else \
			echo failed $$clpfile - spim; \
		fi; \
	done
	@rm -f testcompiler/*.toks testcompiler/*.toks.new
	@rm -f testcompiler/*.ast testcompiler/*.ast.new
	@rm -f testcompiler/*.ic testcompiler/*.ic.new
	@rm -f testcompiler/*.spim testcompiler/*.spim.new
	@rm -f testcompiler/*.sym testcompiler/*.sym.new
	
check_interpreter:
	@# create .eval files	
	@for file in `ls testinterpret/*.cs316.c`; do \
		./sclp $$file -eval; \
		mv $$file.eval $$file.eval.new; \
	done
	
	@# compare .toks .ast .eval files
	@for clpfile in $(shell find -iname '2test*i.cs316.c' | cut -f3 -d"/"); do \
		diff -w -b -B testinterpret/$$clpfile.eval.new test_interpret_output/$$clpfile.eval.old; \
		RETVAL=$$?; \
		if [ $$RETVAL -eq 0 ]; then \
			echo correct $$clpfile -eval; \
		else \
			echo failed $$sclpfile -eval; \
		fi; \
	done
	
	
	@rm -f testinterpret/*.eval testinterpret/*.eval.new

help:
	@echo " "
	@echo "		make -f Makefile [options]"
	@echo " "
	@echo "				Compiles the code using source files"
	@echo "				from the present directory."
	@echo "				Creates an executable " $(TGT)"."
	@echo " "
	@echo " \toptions:" 
	@echo " "
	@echo "	option=-DCOMPILE  	Compiles the code with macro COMPILE as"
	@echo "				defined."
	@echo "				Creates an executable " $(TGT) "for compilation."
	@echo " "
	@echo "	check_interpreter	Executes .c files using " $(TGT) "executable."
	@echo "				Compares them with the corresponding files"
	@echo "		 		in test_files_output files."
	@echo " "
	@echo "	check_compiler		Executes .c files using " $(TGT) "executable."
	@echo "				Compares them with the corresponding files"
	@echo "				in test_files_output files."
	@echo " "
	@echo "	clean		Cleans the current directory, restoring it"

clean :
	rm -f *.o 
	rm -f $(TGT) *.output *.out *.spim *.toks *.ast *.sym *.icode *.tac *.rtl
	rm -rf parser.tab.c lex.yy.c parser.tab.h scanner.c
