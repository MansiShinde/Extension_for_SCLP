**PROCEDURE: f1_
**BEGIN: Three Address Code Statements
	temp0 = p_
	goto Label0
Label0:
	 return temp0
**END: Three Address Code Statements
**PROCEDURE: main
**BEGIN: Three Address Code Statements
	temp1 = 10 > 5
	x_ = temp1
	temp2 = f1_(x_)
	x_ = temp2
**END: Three Address Code Statements
