**PROCEDURE: main
**BEGIN: Three Address Code Statements
	test_ = 0
	loop_ = 10
Label2:
	temp0 = loop_ > 1
	temp4 = ! temp0
	if(temp4) goto Label3
	temp1 = loop_ - 1
	loop_ = temp1
	temp2 = loop_ == 0
	temp3 = ! temp2
	if(temp3) goto Label1
	write  loop_
	goto Label0
Label1:
	write  test_
Label0:
	goto Label2
Label3:
Label4:
	temp6 = loop_ + 1
	loop_ = temp6
	temp5 = loop_ < 10
	if(temp5) goto Label4
	write  loop_
**END: Three Address Code Statements
