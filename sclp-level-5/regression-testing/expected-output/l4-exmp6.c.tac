**PROCEDURE: main
**BEGIN: Three Address Code Statements
	a_ = 5
	b_ = 1
	temp0 = 3 > 2
	temp1 =  ! temp0
	if(temp1) goto Label0
	temp2 = a_
	goto Label1
Label0:
	temp2 = 4
Label1:
	temp3 = temp2 > 5
	temp4 =  ! temp3
	if(temp4) goto Label3
	c_ = 10
	goto Label2
Label3:
	temp5 = a_ == 5
	temp6 =  ! temp5
	if(temp6) goto Label4
	c_ = 2
	goto Label4
Label4:
Label2:
**END: Three Address Code Statements
