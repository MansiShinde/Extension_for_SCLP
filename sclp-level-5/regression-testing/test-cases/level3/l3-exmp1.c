void main()
{
    int num2,greater;
    int num1;
    read num1;
    read num2;
    greater = num1>=num2 || num1*num2 > num1+num2 ? num1 : num2;    
    print greater;
}
