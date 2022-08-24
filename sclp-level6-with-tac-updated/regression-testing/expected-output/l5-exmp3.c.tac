**PROCEDURE: f_
**BEGIN: Three Address Code Statements
	temp0 = 3.50
	goto Label0
Label0:
	 return temp0
**END: Three Address Code Statements
**PROCEDURE: main
**BEGIN: Three Address Code Statements
	r_ = 2.00
	temp1 = f_(2, 3)
	r_ = temp1
**END: Three Address Code Statements
