#include <stdio.h>
#include <unistd.h>

/* UNIX(Linux) の場合
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h> */
/* X_OK is check file can run*/
/*F_OK check file file existe*/

int main(int argc, char *argv[]){
    int result = 0;

    /* ファイル名が argv[1] のファイルが読み出し可能か判定 */
    result = access(argv[1], R_OK);
    printf("Read: %s\n", result == 0 ? "YES": "NO");

    /* ファイル名が argv[1] のファイルが書き込み可能か判定 */
    result = access(argv[1], W_OK);
    printf("Write: %s\n", (result == 0 ? "YES": "NO"));

    /* ファイル名が argv[1] のファイルが実行可能か判定 */
    result = access(argv[1], X_OK);
    printf("Execute: %s\n",(result == 0 ? "YES": "NO"));

    return 0;
}
