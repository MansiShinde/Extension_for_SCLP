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

string Compiler:: scan() //scan() should be first declared then defined
{
	return scan;
}

void main()
{
	Compiler l;
	string a;
}


