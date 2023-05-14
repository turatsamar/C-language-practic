#include <stdio.h>

int main(){

    int age;
    int okuuchu;
    printf("Toigo kelgeniniske rahmat!!\n");
    printf("---------------------------\n");
    printf("Jash kuragynysty korsotunuz ");
    scanf("%d",&age);
    printf("Okuuchusuzbu?\n(Yes:1,No:0)");
    scanf("%d",&okuuchu);

    if(age<6 || okuuchu==1){
        printf("sizge beker!!");
    }

    else{
        printf("Kiruu akysy 1000 som\n");
    }
    printf("-----------------------------\n");
    printf("Jakshy konul achyp ketiniz!!\n");
    
    return 0;
}
