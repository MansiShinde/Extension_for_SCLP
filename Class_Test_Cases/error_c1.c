class Compiler {

private: int var;
	string scan;

private: bool is_tac_generated();
	 void create_rtl();

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

	a = l.var; 
}

//var is private. Hence cannot be accessed in main function
