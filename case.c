#include <stdio.h>

int main()
{
    int n;
    printf("1-5ge cheiin san jaz!");
    scanf("%d",&n);

    switch (n)
    {
    case 1:
        printf("1 di tandagan uchun saga choon ragmat\n");
        break;
     case 2:
        printf("2ni tandadynby))  tugoyn bat tabasyn\n")  ; 
        break;
    case 3:
        printf("3 tu tandadynby)! sen kanday baktyluu adamsyn\n");
        break;
    case 4:
        printf("4 senin jakwy korgon sanyn turbayby!!\n");
        break;
    case 5:
        printf("5 akyrky san bolchu!! sen baktyluusun\n");            
    default:
        printf("1-5ge cheiin san jaz!");
        break;
    }

    printf("buttu!!\n");

   return 0;
}
