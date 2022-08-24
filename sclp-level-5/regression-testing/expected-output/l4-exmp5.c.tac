**PROCEDURE: main
**BEGIN: Three Address Code Statements
	read  local_
	global2_ = local_
	global1_ = 5
	temp0 = global2_ > global1_
	temp1 = local_ - 2
	temp2 = temp1 != 0
	temp3 = temp0 && temp2
	temp4 =  ! temp3
	if(temp4) goto Label0
	temp5 = 1
	goto Label1
Label0:
	temp5 = 0
Label1:
	ans_ = temp5
	temp6 = ans_ == 1
	temp10 =  ! temp6
	if(temp10) goto Label5
Label2:
	temp7 = global2_ > 0
	temp9 =  ! temp7
	if(temp9) goto Label3
	write  global2_
	temp8 = global2_ - 1
	global2_ = temp8
	goto Label2
Label3:
	goto Label4
Label5:
Label6:
	write  global2_
	temp12 = global2_ + 1
	global2_ = temp12
	temp11 = global2_ < 10
	if(temp11) goto Label6
Label4:
**END: Three Address Code Statements
