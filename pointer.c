#include <stdio.h>

int main(int argt, char *argd[])
{
 int t = 20;

 int *pi = &t;
 int **ppi = &pi;
 int ***pppi = &ppi;


 printf("%d \n", t);
 *pi = 30;
 printf("%d \n", t);
 (*(*ppi)) = 40;
 printf("%d \n", t);
 ***pppi = 60;
 printf("%d \n", t);
 return 0;
}