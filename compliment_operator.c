#include <stdio.h>

int main(){
    

    short a=12, b=100, c= -50;

    printf("%d solgo 1 bit jyldyr!! ---%d\n", a, a<<1);
    printf("%d --%d\n",a, a<<2);
    printf("%d --%d\n",a, a<<3);
    printf("%d --%d\n",a, a<<4);
    printf("%d --%d\n",a, a<<5);

    printf("%d ongo jyldyr%d\n",b, b>>1);
    printf("%d --\n",b>>2);
    printf("%d --\n",b>>3);
    printf("%d --\n",b>>4);
    return 0;
}
