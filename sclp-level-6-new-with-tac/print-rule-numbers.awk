{
	spacecount= 110 - length($0);
	if (($1==":")||($1=="|"))
	{
		printf "%s",$0;
		for (i=0; i<spacecount; i++)
			printf " ";
		printf "(rule %d)\n",count++;
	}
	else
		printf ("%s\n",$0);
}
