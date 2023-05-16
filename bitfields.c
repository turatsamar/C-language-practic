#include <stdio.h>

int main()
{
    struct name{
        int a:4;
        int s:5;
    };
    
    struct name1{
        int y:1;
        int :0;
        int i:2;
    };
    
    printf("Size of struct name %lu byte\n ",sizeof(struct name));
    printf("Size of struct name1 %lu byte\n ",sizeof(struct name1));

    return 0;
}
