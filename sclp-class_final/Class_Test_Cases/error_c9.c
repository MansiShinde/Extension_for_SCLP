class Compiler {

private: int var;
	string scan;

private: bool is_tac_generated();
	 void create_rtl();

public: int get_var();
	string get_scan();
};

int Compiler1::get_var() //Wrong - it will give Compiler1_get_var needs to be declared!!, whereas Compiler1 class is not defined
{			 // its so because, first whether function is declared in its class is checked then class name is checked
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

