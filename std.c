#include <stdio.h>

int main(int argc, char *argv[])
{
    int i;
    FILE *fp;  /* ストリーム用の変数 fp */

    /*  引数がなければ、使い方を教える */
    if(argc == 1){
        /* 標準エラー出力に使い方を表示 */
        fprintf(stderr, "use for: %s filename \n", argv[0]);
        return 1;  /* エラーによる終了 */
    }

    /* 1 から argc までの繰り返し */
    for(i = 1; i < argc ; i++){
        /* 読み出し専用で fopen */
        fp = fopen(argv[i], "r");

        printf("%s -> open!\n", argv[i]);
        /* エラーが起きたかチェック */
        if(fp == NULL){
            perror(argv[i]);  /* エラーを表示 */
            return 1; /* エラーによる終了 */
        }

        /* ストリームを閉じる */
        fclose(fp);
        printf("%s -> close!\n", argv[i]);
    }

    return 0; /* 正常終了 */
}
