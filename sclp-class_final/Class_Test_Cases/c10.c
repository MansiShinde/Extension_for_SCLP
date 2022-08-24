
class A {

public: float b;
	int d;
	int f(int n);
	int func(int n);
};

class Compiler {

public: int var;
	string scan;

private: int f(int n);
	Compiler(int var, string scan);
};


class Empty {
};

int A::f(int n)
{
	return n;
}

int Compiler :: f(int n)
{
	return n;
}

Compiler::Compiler(int v, string s)
{
	scan = s;
	var = v;
}

int A::func(int n){
return n;
}


void main() 
{
A x,y;
Compiler l;
int a;
int m;

x.d = 5;
m = x.d + y.d;
m = x.d ;

a = l.var + x.d;

a = l.var;

}





