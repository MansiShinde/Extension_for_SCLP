//parameterised constructor added

class Sample
{
public: string abc;
	int m, n, l;
	void printname();
	int getVolume();
	Sample(int m,int n);
	
private:
	int b;
	float x;
};

Sample:: Sample(int a, int b)
	{
		m = a;
		n = b;
	}

void Sample::printname() 
{
   m = getVolume();
   print m;
}

int Sample:: getVolume()
{
  return 5;
}

void main()
{
int y;
int x;
int a;
  if(x>y) {
	 a=x;
}
 else {
       a=y; 
	
}

}
