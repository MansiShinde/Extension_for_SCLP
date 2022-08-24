// parameterized constructor not defined properly!! .....

class ptr{

public : int a,b,c[20];
	 string cc;
	 int f1();
	 ptr(int a);

};

 ptr :: ptr()
{
  a = 10;
}

int ptr::f1()
{
	int *x,*y,**z;
	int d,e,f[20];

	x = &d;
	y = &e;
	**z = f[3];
	*y = **z;
	return 2;
}

void main() {

	ptr s;
	int z, j, *k;
	k = &z;
}




