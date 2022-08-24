for i in l1 l2 l3 l4 l5
do
	for ext in ast tac rtl cfg spim
	do
		yes | cp ../test-output/$i-exmp*.$ext .
		echo "copied " $ext " files for level " $i
	done
done 
