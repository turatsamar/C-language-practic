#include <stdio.h> // fprintf koldonobuz
#include <fcntl.h> //file control header
#include <sys\stat.h>

int main(int argc, char *argv[]){

    int fd = -1; //-1 ishtetebiz
    char buf[256]; //buffer
    int read_size; //read size

    /*argument jok bolso katta bolot*/
    if( argc != 2){
        printf("pakany korsot!!.\n");
        return 1;
    }

    /*okuu katary achabyz*/
    fd=open(argv[1], O_RDONLY);

    /*papka achaalbay kalsa kata chygar*/
    if(fd < 0){
        fprintf(stderr,"%s: papka achylgan jok.\n");
        return 1;
    }

    printf("%s: papka achyldy.\n", argv[1]);

    close(fd);
    return 0;
}
