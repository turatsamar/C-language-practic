#include <stdio.h>

int main()
{
char c = 'A';
short s = 50;
int i = -100;
unsigned  int ui = 128;
float f = 0.5f;
double d = 2568.2;
long double ld = 1.258E-25;
size_t sz;

sz = sizeof c;
printf("size  of char = %zu byte \n",  sz);

sz = sizeof s;
printf("size of short = %zu byte \n", sz);

sz = sizeof i;
printf("size of int %d byte \n", sz);

sz = sizeof ui;
printf("size of  unsigned int %d  byte \n", sz);

sz = sizeof f;
printf("size of float %d byte \n", sz);

sz = sizeof d;
printf("size of double %d byte \n", sz);

sz = sizeof ld;
printf("size of long double %d byte \n", sz);

sz = sizeof(size_t);
printf("size of size_t %d byte \n", sz);
return 0;
}