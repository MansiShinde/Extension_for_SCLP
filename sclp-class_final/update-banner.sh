for i in `ls *.h *.c *.hh *.cc *.y *.l`
#for i in temp
do
	echo $i
	mv $i yy
	awk -f substitute-banner-lines.awk < yy > $i 
done
