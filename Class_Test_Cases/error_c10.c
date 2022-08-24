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
	return a;  //variable a_ is not present in its class 
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


