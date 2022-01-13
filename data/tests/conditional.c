int a = 3;

bool flag = false;

void main(){
    int i = a*2;
    if(i%2 == 0 && !flag){
        printf(i);
        if(i < 10){
            printf("big i!");
        }
    }
    else{
        flag = !flag;
        printf("flag switched!");
    }

    for(int i = a; i< 2*a; i++){
        printf(a);
    }

    while(i > 0){
        if(i % 2){
            printf("Odd: ",i);
        }
        else{
            printf("Even: ",i);
        }
        i = i-2;
    }

}