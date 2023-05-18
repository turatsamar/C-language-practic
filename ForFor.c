#include <stdio.h>

int main(){

    int i, j;

    for(i=0;i<5;i++){
        printf("OUT  i = %d  \n", i);

        for(j=0;j<5;j++){
            printf("INNER  j = %d \n",j);
        }

        printf("why\n");
    }
    return 0;
}
