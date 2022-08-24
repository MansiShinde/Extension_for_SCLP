**PROCEDURE: main
**BEGIN: Three Address Code Statements
	a_ = 2
	D1234_ = 3
	temp1 = a_ > 5
	temp2 =  ! temp1
	if(temp2) goto Label2
	temp0 = 10
	goto Label0
	goto Label1
Label2:
	temp0 = a_
	goto Label0
Label1:
Label0:
	 return temp0
**END: Three Address Code Statements
