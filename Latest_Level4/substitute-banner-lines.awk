BEGIN{
bannerstring1 = "\
/*******************************************************************************************************\n\
\n\
                                sclp : A C Language Processor\n\
                                --------------------------------\n\
\n\
       About:\n\
\n\
       Originally implemented by Tanu Kanvar and Uday Khedker (http://www.cse.iitb.ac.in/~uday)\n\
       for the courses cs302+cs316 Language Processors (theory and lab) at IIT Bombay.\n\
\n\
       Initial release date Jan 15, 2013.\n\
\n\
       Updated by N. Venkatesh in Jan 2019. The main updates included\n\
       - Changing the input language from a basic-block-based GCC GIMPLE dump to a subset of C.\n\
       - Replacing the flexc++ and bisonc++ scripts by flex and bison.\n\
       - Supporting print operation for variables.\n\
       \n\
       Updated by Uday Khedker with the help of Nisha Biju and Saari Rajan in Jan 2020. The main\n\
       updates include\
";
bannerstring2 = "\
       - Introduction of three address code (TAC) phase between abstract syntax tree (AST)\n\
         and Register Transfer Language (RTL) which was originally called icode.\n\
       - Introduction of string data type with constant strings.\n\
       - Support for read operation for integer and floating point variables, and print\n\
         operation for all variables (including string variables) and string constants. \n\
       - Significant changes in command line switches for printing each representation (tokens,\n\
         AST, TAC, RTL, and assembly) as well as stopping after each phase (scanning, parsing,\n\
	 AST construction (i.e. semantic analysis), TAC generation and RTL generation).\n\
       - Substantial clean up of the code including removal of AST to icode translation,\n\
         and interpretation (which was buggy and had not been tested thorougly after the front\n\
	 end and the input langauge changed in 2019). \n\
\n\
       Copyrights reserved  by Uday Khedker. This  implemenation has been made available purely  \n\
       for academic purposes without any warranty of any kind.\n\
\n\
*******************************************************************************************************/\
"
ignore = 0;
}
{
	if ($0 == "// BEGIN_BANNER")
	{
		ignore = 1;
	       	printf "/* BEGIN_BANNER */\n";
	}
	if (ignore == 0)
		print $0;
	if ((ignore == 1) && ($0 != "// END_BANNER"))
		;
	if ($0 == "// END_BANNER")
	{
		ignore = 0;
		print bannerstring1;
		print bannerstring2;
	       	printf "/* END_BANNER */\n";
	}
}
