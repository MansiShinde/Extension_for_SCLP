**PROCEDURE: main
**BEGIN: Three Address Code Statements
	a_ = 3.23
	j_ = 5.68
	temp0 =  - j_
	temp1 =  - j_
	temp2 = a_ * temp1
	temp3 = temp2 / a_
	temp4 = temp3 + temp0
	a_ = temp4
	temp5 =  - j_
	temp6 = a_ >= temp5
	temp10 =  ! temp6
	if(temp10) goto Label0
	temp7 = a_ + 1.00
	temp11 = temp7
	goto Label1
Label0:
	temp8 =  - a_
	temp9 = temp8 - 1.00
	temp11 = temp9
Label1:
	s_ = temp11
**END: Three Address Code Statements
