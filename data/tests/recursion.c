int fib(int n){
    if(n == 1){
        return n;
    }
    return n + fib(n-1);
}
void main(){
    int res = fib(10);
    printf(res);
}