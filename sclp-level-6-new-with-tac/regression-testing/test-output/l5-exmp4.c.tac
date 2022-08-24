**PROCEDURE: f_
**BEGIN: Three Address Code Statements
	temp1 = n_ == 0
	temp2 = ! temp1
	if(temp2) goto Label1
	ret_ = 1
	temp0 = ret_
	goto Label0
	goto Label1
Label1:
	temp3 = n_ - 1
	k_ = temp3
	temp4 = f_(k_)
	ret_ = temp4
	temp5 = n_ * ret_
	ret_ = temp5
	temp0 = ret_
	goto Label0
Label0:
	 return temp0
**END: Three Address Code Statements
**PROCEDURE: main
**BEGIN: Three Address Code Statements
	b_ = 5
	temp6 = f_(b_)
	a_ = temp6
	write  a_
	m_ = 2
	write  m_
**END: Three Address Code Statements
