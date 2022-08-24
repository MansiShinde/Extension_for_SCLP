**PROCEDURE: main
**BEGIN: Three Address Code Statements
	read  num1_
	read  num2_
	temp0 = num1_ >= num2_
	temp1 = num1_ + num2_
	temp2 = num1_ * num2_
	temp3 = temp2 > temp1
	temp4 = temp0 || temp3
	temp5 =  ! temp4
	if(temp5) goto Label0
	temp6 = num1_
	goto Label1
Label0:
	temp6 = num2_
Label1:
	greater_ = temp6
	write  greater_
**END: Three Address Code Statements
