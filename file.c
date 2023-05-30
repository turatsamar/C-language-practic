#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[]){
    int  c;
    FILE *src;    /* 読み出し側 */
    FILE *dst;    /* 書き込み側 */
    char ifilename[128];  /* 読み出しファイル名 */
    char ofilename[128];  /* 書き込みファイル名 */

    if(argc != 2){
        fprintf(stderr, "利用方法: %s filename\n", argv[0]);
        return 1;
    }

    /* ifilename に argv[1] をコピー */
    strcpy(ifilename, argv[1]);

    /* ofilename に ifilename に .tmp を付けたものを出力 */
    sprintf(ofilename, "%s.tmp", ifilename);


    src =fopen(ifilename, "r");
    if(src == NULL) {
        printf("ファイル%sのオープンに失敗しました\n", ifilename);
        return 1;    
    }    
    dst =fopen(ofilename, "w");
    if(dst == NULL) {
        printf("ファイル%sのオープンに失敗しました\n", ofilename);
        return 1;
    }
    
    
    /* EOF でないかぎり、src から 1 文字読み出し */
    while((c = fgetc(src)) != EOF){
        if(isupper(c)){
            c = tolower(c);  /* 大文字は小文字に変換 */
        }
        else if(islower(c)){
            c = toupper(c);  /* 小文字は大文字に変換 */
        }

        /* dst に 1 文字書き込み */
        fputc(c, dst);
    }

    /* ファイルを閉じる */
    fclose(src);
    fclose(dst);

    /* ifilename を削除 */
    if(rename(ifilename) != 0){
        fprintf(stderr, "ファイル%sを削除できませんでした。\n", 
ifilename);
        return 1;
    }

    /* 出力したファイルの名前を、ofilename から ifilename に変更 */
    if(remove(ofilename, ifilename) != 0){
        fprintf(stderr, "ファイル%sの名前を変更できませんでした\n", ofilename);
        return 1;
    }
    return 0;
}




