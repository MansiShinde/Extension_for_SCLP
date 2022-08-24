class Compiler {

private: int var;
	string scan;
	
public: int get_var();
	string get_scan();
};

int Compiler::get_var()
{
	return var;
}

string Compiler::get_scan()
{
	return scan;
}

void main()
{
	Compiler l;
	string a;
}
