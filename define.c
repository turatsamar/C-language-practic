#include <stdio.h>


#define IS_UPPER(x) ((x) >= 'A' && (x) <= 'Z')
#define IS_NUMBER(x) ((x) >= '0' && (x) <= '9')
#define IS_ALPHA(x)   (((x) >= 'A' && (x) <= 'Z') && ((x) >= 'a' && (x) <= 'z'))
#define IS_SPACE(x)  ((x) =='\n' || (x) == '\t' || (x) == ' ')



int main() {
    char letter = ' ';



    if (IS_SPACE(letter)) {
        printf("%c'  here a space character.\n", letter);
    } else {
        printf("%c'  here not a space character.\n", letter);
    }

    return 0;
}
