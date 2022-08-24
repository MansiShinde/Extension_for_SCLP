**PROCEDURE: f1_
**BEGIN: Three Address Code Statements
	temp1 = b_ + c_
	d_ = temp1
	temp2 = d_ + 2
	temp0 = temp2
	goto Label0
Label0:
	 return temp0
**END: Three Address Code Statements
**PROCEDURE: g_
**BEGIN: Three Address Code Statements
	temp3 = b_
	goto Label1
Label1:
	 return temp3
**END: Three Address Code Statements
**PROCEDURE: main
**BEGIN: Three Address Code Statements
	a_ = 3
	temp4 = f1_(a_, a_)
	a_ = temp4
	write  a_
**END: Three Address Code Statements
