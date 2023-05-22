#include <stdio.h>

int main(int argc, char *argv[]){

    FILE *src; 
    FILE *dst;
    int c;

    if(argc == 1){
        fprintf(stderr, "write it: FileName1 Filename2\n", argv[]);
        fprintf(stderr, "we copy File1 to File2\n");
        return 1;
    }

    //open a copy file for "read"!!
    src=fopen(argv[1],"r");

    //check error file
    if(src == NULL){
        perror(argv[1])
        return 1;
    }

    dst=fopen(argv[2], "w");

    // check the error dst file or no!
    if(dst == NULL)
    perror(argv[2]);
    return 1;

    //read alphabet from src stop if EOG !!

    while ((c=fgets(src)) != EOF){
        fputs(c, dst);

    }
   
    fclose(src);
    fclose(dst)

    return 0;

}
