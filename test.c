int fib(int a){
    if(a == 1)
        return 1;
    return fib(a-1) + fib(a-2);
}
int factorial(int n){
    return n*factorial(n-1);
}
int main()
{
    int i = 0;
    i += 5;

    while (i > 0)
    {
        int y = 8;
        break;
    }
    double d = 2.0;
    int a = 31;
    int q = a % 3;
    q = 1 % 2;
    int b = deneme(a);
    int c = deneme(a,deneme(b));

    printf("abc\n");

    printf("integer: %d\n", i);
}