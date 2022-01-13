int int_var = 10;
double double_var = 4.20;
string str_var = "ali türk";
bool bool_var = false;

int test(){
    int t = 13;
    int test_data = -(int_var + 15 / 74 * (t % 3));
    bool test_flag = !bool_var && (test_data > (15-t));
    return test_data + t;
}

void main(){
    int res = test();
    printf(res);
}