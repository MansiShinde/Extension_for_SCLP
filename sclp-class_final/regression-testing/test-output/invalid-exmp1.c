int x;

void main () 
{
	int a;
	bool y; 
	x = 5;
	a = 6;
	y = (x > a) < (x < a); //should not allow comparison (with LT or GT) of boolean data types
}
