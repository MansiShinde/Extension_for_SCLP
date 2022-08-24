**PROCEDURE: main
**BEGIN: Three Address Code Statements
	a_ = 5
	b_ = 1
Label2:
	temp1 = a_ + 1
	a_ = temp1
	temp2 = b_ < 2
	temp4 = ! temp2
	if(temp4) goto Label1
	temp3 = b_ + 1
	b_ = temp3
	goto Label0
Label1:
	temp5 = b_ - 1
	b_ = temp5
Label0:
	temp0 = a_ < 10
	if(temp0) goto Label2
	write  a_
**END: Three Address Code Statements
