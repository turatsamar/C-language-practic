#include <stdio.h>

#define PR(x) printf("Address of %s = %p\n",#x, &x);

int main(){

    char c = 1;
    int i = 2;
    short s= 1;
    long l= 5;
    PR(c); PR(i); PR(s); PR(l);

    return 0;  
    
}
