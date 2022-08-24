set -
OUTPUT=regression-testing/test-output
EXPECTED=regression-testing/expected-output
SOURCE=regression-testing/test-cases/level*
INVALID=regression-testing/invalid-test-cases/
OPTIONS="-ast -tac -cfg -rtl"
ERR_LOG=expected-error-messages.log
#OPTIONS="-rtl"
#OPTIONS="-sa-parse"
mkdir -p $OUTPUT
rm -f $OUTPUT/*
STRICT=$1
total_tests=0
failed_tests=0
total_compile=0
failed_compile=0
for file in `ls $SOURCE/*.c`
do
	cp $file $OUTPUT
	FILE=`echo $file | cut -f4 -d"/"`
	./sclp $OPTIONS $OUTPUT/$FILE 
	if [ $? = 0 ]
	then
		for i in $OPTIONS
		do
			if [ $i = "-ast" ] 
			then
				echo "Comparing AST for " $FILE
				diff $STRICT $OUTPUT/$FILE.ast $EXPECTED/$FILE.ast
				total_tests=`expr $total_tests + 1`
				failed_tests=`expr $failed_tests + $?`
			elif [ $i = "-tac" ] 
			then
				echo "Comparing TAC for " $FILE
				diff $STRICT $OUTPUT/$FILE.tac $EXPECTED/$FILE.tac
				total_tests=`expr $total_tests + 1`
				failed_tests=`expr $failed_tests + $?`
			elif [ $i = "-cfg" ] 
			then
				echo "Comparing CFG for " $FILE
				diff $STRICT $OUTPUT/$FILE.cfg $EXPECTED/$FILE.cfg
				total_tests=`expr $total_tests + 1`
				failed_tests=`expr $failed_tests + $?`
			elif [ $i = "-rtl" ] 
			then
				echo "Comparing RTL for " $FILE
				diff $STRICT $OUTPUT/$FILE.rtl $EXPECTED/$FILE.rtl
				total_tests=`expr $total_tests + 1`
				failed_tests=`expr $failed_tests + $?`
			fi
		done
		echo "Comparing ASM for " $FILE
		diff  $OUTPUT/$FILE.spim $EXPECTED/$FILE.spim
		total_tests=`expr $total_tests + 1`
		failed_tests=`expr $failed_tests + $?`
		total_compile=`expr $total_compile + 1`
	else
		echo "Error compiling " $FILE 
		total_compile=`expr $total_compile + 1`
		failed_compile=`expr $failed_compile + 1`
	fi
done
invalid_compile=0
passed_invalid_tests=0
rm $ERR_LOG -f
for file in `ls $INVALID/*.c`
do
	cp $file $OUTPUT
	FILE=`echo $file | cut -f4 -d"/"`
	echo $FILE
	echo $FILE >> $ERR_LOG
	./sclp $OUTPUT/$FILE 2>> $ERR_LOG
	if [ $? = 0 ]
	then
		echo $FILE " invalidity check failed"
		invalid_compile=`expr $invalid_compile + 1`
	else
		echo $FILE 
		invalid_compile=`expr $invalid_compile + 1`
		passed_invalid_tests=`expr $passed_invalid_tests + 1`
	fi
done
passed_compile=`expr $total_compile - $failed_compile`
passed_tests=`expr $total_tests - $failed_tests`
echo "***** Regresssion test summary *****"
echo "Attempted compilations = " $total_compile
echo "Successful compilations = " $passed_compile
echo "IR output comparisons = " $total_tests
echo "Successful comparisons = " $passed_tests
echo "Attempted compilations of invalid programs = " $invalid_compile
echo "Successfully detected invalid programs = " $passed_invalid_tests
