**PROCEDURE: main
**BEGIN: Three Address Code Statements
	a_ = 5
	b_ = 1
Label1:
Label0:
	temp2 = b_ + 1
	b_ = temp2
	temp1 = b_ < 5
	if(temp1) goto Label0
	temp3 = a_ + 1
	a_ = temp3
	temp0 = a_ > 10
	if(temp0) goto Label1
**END: Three Address Code Statements
