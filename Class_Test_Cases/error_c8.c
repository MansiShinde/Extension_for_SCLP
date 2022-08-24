class A {

public: float b;
	int d;
	int f(int n);
	int func(int n);
};


class Compiler {

public: int var;
	string scan;

public: int get_var();
	string get_scan();
};

void main()
{
	A n;
	Compiler l;
	int fe, m;
	string s;

	fe = A.d; //This is wrong
}


