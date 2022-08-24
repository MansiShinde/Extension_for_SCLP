**PROCEDURE: main
**BEGIN: Three Address Code Statements
	test_ = 0
	loop_ = 10
Label2:
	temp3 =  ! loop_
	if(temp3) goto Label3
	temp0 = loop_ - 1
	loop_ = temp0
	temp1 = loop_ == 0
	temp2 =  ! temp1
	if(temp2) goto Label1
	write  loop_
	goto Label0
Label1:
	write  test_
Label0:
	goto Label2
Label3:
Label4:
	temp5 = loop_ + 1
	loop_ = temp5
	temp4 = loop_ < 10
	if(temp4) goto Label4
	write  loop_
**END: Three Address Code Statements
