set -
OUTPUT=regression-testing/test-output
EXPECTED=regression-testing/expected-output
SOURCE=regression-testing/test-cases/level*
#OPTIONS="-ast -tac -rtl"
OPTIONS="-sa-ast"
mkdir -p $OUTPUT
rm -f $OUTPUT/*
for file in `ls $SOURCE/*.c`
do
	cp $file $OUTPUT
	FILE=`echo $file | cut -f4 -d"/"`
	echo $FILE
	./sclp $OPTIONS $OUTPUT/$FILE
	if [ $? = 0 ]
	then
		for i in $OPTIONS
		do
			if [ $i = "-ast" ] 
			then
				diff $OUTPUT/$FILE.ast $EXPECTED/$FILE.ast
			elif [ $i = "-tac" ] 
			then
				diff $OUTPUT/$FILE.tac $EXPECTED/$FILE.tac
			elif [ $i = "-rtl" ] 
			then
				diff $OUTPUT/$FILE.rtl $EXPECTED/$FILE.rtl
			fi
		done
	fi
done
