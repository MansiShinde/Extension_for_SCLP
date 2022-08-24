ASGN=A4
rm -rf $ASGN-Resources*
RESOURCE=$ASGN-Resources
IMPL=$RESOURCE/reference-implementations
EXMP=$RESOURCE/example-programs
mkdir $RESOURCE
mkdir $IMPL
mkdir $EXMP
for i in L1 L2 L3 L4
do
	echo "Bulding sclp for level " $i;
	sh build.sh -l$i -pasm
	strip sclp
	mv sclp $IMPL/$i-sclp
done
sh build.sh -lL4 -ptac
strip sclp
mv sclp $IMPL/$ASGN-sclp
cp -r regression-testing/test-cases/level1 $EXMP
cp -r regression-testing/test-cases/level2 $EXMP
cp -r regression-testing/test-cases/level3 $EXMP
cp -r regression-testing/test-cases/level4 $EXMP
cp README.$ASGN $RESOURCE
tar cvfz $RESOURCE.tgz $RESOURCE
