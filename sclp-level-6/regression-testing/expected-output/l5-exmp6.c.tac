**PROCEDURE: f2_
**BEGIN: Three Address Code Statements
	temp0 = p_
	goto Label0
Label0:
	 return temp0
**END: Three Address Code Statements
**PROCEDURE: f_
**BEGIN: Three Address Code Statements
	temp1 = p_
	goto Label1
Label1:
	 return temp1
**END: Three Address Code Statements
**PROCEDURE: main
**BEGIN: Three Address Code Statements
	r_ = 2.00
	temp2 = 2.00 + 3.00
	temp3 = r_ * temp2
	r_ = temp3
	temp5 = r_ * 2.00
	temp4 = f_(temp5, 3)
	r_ = temp4
**END: Three Address Code Statements
