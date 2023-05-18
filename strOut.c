#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1000

void findStringInFile(const char* filename, const char* searchString) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strstr(line, searchString) != NULL) {
            printf("%s", line);
        }
    }

    fclose(file);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <search_string> <filename>\n", argv[0]);
        return 1;
    }

    char* searchString = argv[1];
    char* filename = argv[2];

    findStringInFile(filename, searchString);

    return 0;
}
