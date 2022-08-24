**PROCEDURE: main
**BEGIN: Three Address Code Statements
	a_ = 5
	b_ = 3
	read  c_
	read  d_
	temp0 = a_ > b_
	temp11 =  ! temp0
	if(temp11) goto Label4
	temp1 = c_ > d_
	temp4 =  ! temp1
	if(temp4) goto Label0
	temp2 = a_ * b_
	temp5 = temp2
	goto Label1
Label0:
	temp3 = a_ - b_
	temp5 = temp3
Label1:
	temp12 = temp5
	goto Label5
Label4:
	temp6 = d_ > c_
	temp9 =  ! temp6
	if(temp9) goto Label2
	temp7 = a_ + 3
	temp10 = temp7
	goto Label3
Label2:
	temp8 = b_ * 10
	temp10 = temp8
Label3:
	temp12 = temp10
Label5:
	a_ = temp12
	write  a_
**END: Three Address Code Statements
