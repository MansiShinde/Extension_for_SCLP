**PROCEDURE: f1_
**BEGIN: Three Address Code Statements
	temp2 = b_ + c_
	d_ = temp2
	temp3 = d_ + 2
	temp0 = temp3
	goto Label0
Label0:
	 return temp0
**END: Three Address Code Statements
**PROCEDURE: g_
**BEGIN: Three Address Code Statements
	temp1 = b_
	goto Label1
Label1:
	 return temp1
**END: Three Address Code Statements
**PROCEDURE: main
**BEGIN: Three Address Code Statements
	a_ = 3
	temp4 = f1_(a_, a_)
	a_ = temp4
	write  a_
**END: Three Address Code Statements
