class Compiler {

private: int var;
	string scan;

private: bool is_tac_generated();
	 void create_rtl();
	int get_scan();

public: int get_var();
	string get_scan(); //class function already declared once

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

