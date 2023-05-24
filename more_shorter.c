#include <stdio.h>

#define P(x) printf("%s = %p : (%s + 1) = %p\n",#x,x,#x, (x+1));
int main(){

    int *int_pointer=NULL;
    char *char_point=NULL;
    long *long_point=NULL;
    short *short_poi=NULL;

    P(int_pointer); P(char_point); P(long_point); P(short_poi);
    return 0;
}
