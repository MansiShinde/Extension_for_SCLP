//Shadowing of parameter by a declaration inside a function should not be allowed

int main(int argc, char const *argv[])
{
    int argc;
}
