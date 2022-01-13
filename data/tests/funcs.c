int sum(int a, int b){
    return a+b;
}
bool isEven(int a){
    return !(a%2);
}

void main(){
    int v1 = 13;
    int v2 = v1*2-10;
    int res = sum(a,b) / 2;
    printf(res);
    bool v2_even = isEven(v2);
    printf(v2_even);
}