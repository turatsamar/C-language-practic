#include <stdio.h>
#include <stdlib.h>

int main() {
    int a,b,c;
    int d,res1,res2,MAX_NUMBER;
    
    // Write C code here
    printf("Input number X:");
    scanf("%d", &a);
    printf("Input number Y:");
    scanf("%d", &b);
    printf("Input number Z:");
    scanf("%d", &c);
    printf("Input number W:");
    scanf("%d", &d);
    
    res1=(a+b+abs(a-b))/2;
    res2=(res1+c+abs(res1-c))/2;
    MAX_NUMBER=(res2+d+abs(res2-d))/2;
    
    printf("%d\n", MAX_NUMBER);
    

    return 0;
}
