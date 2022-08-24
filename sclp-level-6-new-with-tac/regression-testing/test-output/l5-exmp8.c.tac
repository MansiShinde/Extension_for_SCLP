**PROCEDURE: f1_
**BEGIN: Three Address Code Statements
	temp0 = s_
	goto Label0
Label0:
	 return temp0
**END: Three Address Code Statements
**PROCEDURE: main
**BEGIN: Three Address Code Statements
	s1_ = "Hello"
	temp1 = f1_(s1_)
	s2_ = temp1
	write  s1_
**END: Three Address Code Statements
