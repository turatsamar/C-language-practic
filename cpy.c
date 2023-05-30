#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    int srcfd;  /* コピー元ファイル ディスクリプタ */
    int dstfd;  /* コピー先ファイル ディスクリプタ */

    int nread = 0;
    int nwrite = 0;
    char buf[1024];

    /* もしも引数の数が合わなければ */
    if(argc != 3){
        fprintf(stderr, "use: %s file_copy1.c file_copy2.c \n", argv[0]);
        return 1;
    }

    /* コピー元ファイルを読み込み専用で open */
    srcfd = open(argv[1], O_RDONLY);
    if(srcfd < 0){
        fprintf(stderr, "%s: open failed\n", argv[1]);
        return 1;
    }

    /* コピー先ファイルを書き込み専用で open */
 dstfd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(dstfd < 0){
        fprintf(stderr, "%s: open failed\n", argv[2]);
        return 1;
    }
    

    /* コピー元ファイルからの読み出しができるかぎり繰り返し */
    while((nread = read(srcfd, buf, 1024)) > 0){
        /* 読み込んだ nread バイトだけ write */
        nwrite = write(dstfd, buf, nread);

        /* 書き込みに失敗しているかチェック */
        if(nwrite < 0){
            fprintf(stderr, "write failed.\n");
            return 1;
        }
    }

    /* 読み込みに失敗したかチェック */
    if(nread < 0){
        fprintf(stderr, "read failed.\n");
        return 1;
    }

    /* 開いたファイル ディスクリプタを閉じる */
    close(srcfd);
    close(dstfd);

    return 0;
}
