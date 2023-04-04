#include <stdio.h>
#include <string.h>

int main (void)
{
    char str [30] = "ZHAMALBEKOV Turatbek";
    char st[30] = "Japan_Osaka";


    printf(" Length string of  «%s«  -->  %lu simbol\n", str, strlen(str) );

    printf("length of string «%s« -- %lu simbol\n", str, strlen(st) );

    return 0;
}