**PROCEDURE: main
**BEGIN: Three Address Code Statements
	write  "Enter your marks (out of 100). "
	read  mrks_
	temp0 = mrks_ >= 40
	temp1 =  ! temp0
	if(temp1) goto Label0
	temp2 = "pass"
	goto Label1
Label0:
	temp2 = "fail"
Label1:
	res_ = temp2
	write  res_
**END: Three Address Code Statements
