
class A {

public: float b;
	int d;
};


int retInt(){
return 10;
}

float retFloat(){
return 5.5;
}


void main() 
{
A x,y;

int a;
float p;
int m;

a = retInt();
p = retFloat();

x.d = retInt();
x.b = retFloat();

m = x.d + y.d;
m = x.d ;

}





