**PROCEDURE: main
**BEGIN: Three Address Code Statements
	global1_ = 5
	read  local_
	temp0 = local_ + 1
	global2_ = temp0
	temp1 =  - global1_
	temp2 = global2_ * temp1
	temp3 =  - local_
	temp4 = temp3 + temp2
	local_ = temp4
	temp5 = local_ / 2
	global2_ = temp5
	write  global1_
	write  global2_
	write  local_
**END: Three Address Code Statements
