#include <stdio.h>

enum Year{
    not_insert,
    jan,
    feb,
    mar,
    apr,
    may,
    jul,
    juy,
    avg,
    sep,
    oct,
    nov,
    dec
};

int main(){
    enum Year month;
    month=nov;
    printf("last month of the year %d", month);
    return 0;
}
