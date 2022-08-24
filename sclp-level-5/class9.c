//while loop added 

class Sample
{
public: string abc;
	int m;
	void printname();
	Sample(int n);

	
private:
	int b;
	float x;
};

Sample:: Sample(int n)
	{
		m = n;
	}

int func(int m){
 return m;
}


void main()
{
int y;
int x;
int a;
a = func(2);
while(a < 5)
{
	if(x>y) {
	 a=x + y;
	}
	else {
	a=x - y; 
	}
}

}
