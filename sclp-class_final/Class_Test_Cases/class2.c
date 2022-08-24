
int func(int n);

class A{

public: int a, b;
	int func(int n);
	A(int c, int d);
};

class B{

public: string abc;
	int func(int n);
	B(string a);
};

A :: A(int c, int d)
{
	a  = c;
	b = d;
}

B :: B(string a)
{
	abc = a;
}


int A:: func(int n)
{
	return n;
}

int B:: func(int n)
{
	return n;
}

int func(int n)
{
	return n;
}

void main()
{
 string abc;
 B r;
int z;

z  = func(10);
 
abc = r.abc;


 abc = "Hello";

}


