**PROCEDURE: main
**BEGIN: Three Address Code Statements
	a_ = 5
	b_ = 3
	read  c_
	read  d_
	temp0 = a_ > b_
	temp9 = ! temp0
	if(temp9) goto Label4
	temp1 = c_ > d_
	temp4 = ! temp1
	if(temp4) goto Label0
	temp2 = a_ * b_
	stemp0 = temp2
	goto Label1
Label0:
	temp3 = a_ - b_
	stemp0 = temp3
Label1:
	stemp2 = stemp0
	goto Label5
Label4:
	temp5 = d_ > c_
	temp8 = ! temp5
	if(temp8) goto Label2
	temp6 = a_ + 3
	stemp1 = temp6
	goto Label3
Label2:
	temp7 = b_ * 10
	stemp1 = temp7
Label3:
	stemp2 = stemp1
Label5:
	a_ = stemp2
	write  a_
**END: Three Address Code Statements
