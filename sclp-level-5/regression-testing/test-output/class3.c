//This testcase has: multiple classes declaration, its objects creation and object assignments

class Sample
{
public: string abc;
	int a;
	void printname();
private:
	int b;
	float x;
	
};

class Sample1
{
public: float abc;
	int printname1();
private:
	int b;
	bool x;
	
};

void main()
{
 Sample x;
 Sample1 y;
 float a;
 int i;
 a = y.abc;
 i = x.c;
}
