
class A{

public: int a, b;
	int func(int n);
	A(int c, int d);
};

class B{

public: string abc;
	int func(int n);
};

A::A(int c, int d)
{
	a = c;
	b = d;	
}

int B :: func(int a)
{
  return a;
}

int A :: func(int a)
{
  return a;
}

void main()
{
 string abc;
 B r;
 A q; 
int z;
 
abc = r.abc;

q.A(2,3);

z = r.func(5);



 abc = "Hello";

}
