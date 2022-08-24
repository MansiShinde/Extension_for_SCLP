**PROCEDURE: f_
**BEGIN: Three Address Code Statements
	temp0 = a_
	goto Label0
Label0:
	 return temp0
**END: Three Address Code Statements
**PROCEDURE: g_
**BEGIN: Three Address Code Statements
	temp1 = 0
	goto Label1
Label1:
	 return temp1
**END: Three Address Code Statements
**PROCEDURE: main
**BEGIN: Three Address Code Statements
	temp2 = f_()
	temp3 = g_()
**END: Three Address Code Statements
