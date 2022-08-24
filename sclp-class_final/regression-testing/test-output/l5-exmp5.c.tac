**PROCEDURE: func1_
**BEGIN: Three Address Code Statements
	i2_ = 5
	f2_ = 5.00
**END: Three Address Code Statements
**PROCEDURE: func2_
**BEGIN: Three Address Code Statements
	temp3 = a_ + 10
	a_ = temp3
	i1_ = 5
	temp4 = a_ + i1_
	temp5 = temp4 + i2_
	temp6 = temp5 + 2
	temp0 = temp6
	goto Label0
Label0:
	 return temp0
**END: Three Address Code Statements
**PROCEDURE: func3_
**BEGIN: Three Address Code Statements
	temp7 = b_ + f2_
	b_ = temp7
	temp8 = c_ + 10.50
	c_ = temp8
	temp9 = b_ + c_
	temp1 = temp9
	goto Label1
Label1:
	 return temp1
**END: Three Address Code Statements
**PROCEDURE: func4_
**BEGIN: Three Address Code Statements
	i1_ = 4
	temp10 = i1_ + i1_
	temp2 = temp10
	goto Label2
Label2:
	 return temp2
**END: Three Address Code Statements
**PROCEDURE: main
**BEGIN: Three Address Code Statements
	func1_()
	temp12 = func4_()
	i2_ = temp12
	temp13 = func2_(i1_, f2_, i2_)
	i2_ = temp13
	temp14 = func3_(i1_, f1_, f2_)
	f2_ = temp14
	write  f1_
	write  f2_
**END: Three Address Code Statements
