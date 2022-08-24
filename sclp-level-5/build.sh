## This script reads to arguments and then decides whether or not to invoke make
if [ $# != 2 ]
then
	echo "This script needs 2 argument with -l<level> and -p<phase> options"
	exit 1
fi
level="l"
phase="p"
type1=`echo $1 | cut -c 1,2`
type2=`echo $2 | cut -c 1,2`
option1=`echo $1 | cut -c 3-`
option2=`echo $2 | cut -c 3-`
#echo "Type 1 and option 1 = " $type1 " " $option1
#echo "Type 2 and option 2 = " $type2 " " $option2
if [ $type1 = "-l" ]
then
	level=$option1
	if [ $type2 = "-p" ]
	then
		phase=$option2
	fi
elif [ $type1 = "-p" ]
then
	phase=$option1
	if [ $type2 = "-l" ]
	then
		level=$option2
	fi
fi
if [ $level = "l" ] || [ $phase = "p" ]
then
	echo "Missing level or phase"
	exit 1
fi
#echo "level is " $level
#echo "phase is " $phase
touch .version
oldlevel=`cat .version | awk '{print $1;}'`
oldphase=`cat .version | awk '{print $2;}'`
#echo $oldlevel
#echo $oldphase
if [ $level != $oldlevel ] || [ $phase != $oldphase ] 
then
	echo "Need to rebuild make"
	rm -f main.o
fi
make -e L=$level -e P="phase_"$phase
echo $level " " $phase >| .version
