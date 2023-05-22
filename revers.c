#include <stdio.h>

/*
   strlength 関数
   引数に渡された char 型の配列を文字列だと考えて長さを
   調べる。その結果を返す。
 */
int strlength(char str[]){
    int len = 0;

    /* 終端文字でないかぎり */
    while(str[len] != '\0')
        len++; /* len をインクリメント */

    /* 長さを返す */
    return len;
}

/* 
   printrevers 関数
   引数に渡された char 型の配列を文字列だと考え、
   逆さまにして、引数に渡されたストリームに表示する。
 */
void printrevers(char str[], FILE *fp){
    int i = 0;
    int eol = 0;  /* 最後に改行があるなら 1、ないなら 0 */
    int len = strlength(str); /* 長さ */

    /* 最後が改行文字だったら */
    if(str[len - 1] == '\n'){
        /* 終端文字を代入 */
        str[len - 1] = '\0';
        len--;  /* 文字列の長さを 1 減らす */
        eol = 1; /* 改行があったことを記す */
    }

    /* 文字列の末尾から、先頭まで繰り返し */
    for(i = len - 1; i >= 0; i--)
        fputc(str[i], fp);  /* 表示 */

    /* 改行文字があったならば */
    if(eol == 1)
        fputc('\n', fp);  /* 改行を表示 */
}


/*
   main 関数
   コマンド ライン引数にとったファイル名を開いて、
   それぞれの行を逆さまにして表示する。
 */
int main(int argc, char *argv[])
{
    int i;
    FILE *fp;  /* ストリーム */
    char buf[1024];

    /* 引数をチェック */
    if(argc == 1){
        fprintf(stderr, "使い方: %s ファイル名\n", argv[0]);
        return 1; /* エラーによる終了 */
    }

    /*  引数のファイル名すべてに対して繰り返し */
    for(i = 1 ; i < argc; i++){
        /* ファイルを読み出し専用で open */
        fp = fopen(argv[i], "r");

        /* エラーかどうか確認 */
        if(fp == NULL){
            perror(argv[i]);  /* エラーの原因を表示 */
            return 1; /* エラーによる終了 */
        }

        /* fp から 1 行読み込む、NULL でないかぎり繰り返し */
        while(fgets(buf, 1024, fp) != NULL)
             /* 逆にして stdout に書き込む */
             printrevers(buf, stdout);
    
        /*　ストリームを閉じる */
        fclose(fp);
    }
    return 0;
}
