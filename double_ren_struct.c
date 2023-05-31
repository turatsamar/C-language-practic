#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* node 構造体と変数 head と変数 tail */
struct node {
    char name[32];        /* 名前 */
    int  height;         /* 身長 */
    struct node *next;    /* 次のノードへのポインタ */
    struct node *prev;    /* 前のノードへのポインタ */
} *head = NULL, *tail = NULL;

/* 
    node_alloc 関数
    node 構造体分を malloc し、malloc に成功した場合だけ初期化する。
    malloc の返り値を返す。
*/
struct node *node_alloc(void){
    struct node *ret = NULL;
    ret = (struct node *)malloc(sizeof(struct node));

    if(ret != NULL){
        memset(ret, 0, sizeof(struct node));
    }
    return ret;
}

/* 
    find_node 関数
    指定された名前のノードを探して、そのポインタを返す。
    なければ NULL を返す。
*/
struct node *find_node(char *name){
    struct node *p;
    for(p = head; p != NULL; p = p->next){
        /* 名前が同じかどうかチェック */
        if(strcmp(name, p->name) == 0){
            return p;
        }
    }
    return NULL;
}


/*
    rem_node 関数
    ポインタ p で指定されたノードをリストから外す
 */
void rem_node(struct node *p){
    printf("-> rem_node(): %s: %d\n", p->name, p->height);

    /* 前のノードがあるならば */
    if(p->prev != NULL){
        p->prev->next = p->next;
    }
    else{  /* ない場合は p は head */
        head = p->next;   /* headをp->nextにする */
    }

    /* 次のノードがあるならば */
    if(p->next != NULL){
        p->next->prev = p->prev;
    }
    else{  /* ない場合は p は tail */
        tail = p->prev;  /* tail を p->prev にする */
    }
    p->next = p->prev = NULL;
}

/*  
    add_tail 関数
    同じ名前のノードがあれば外し、新たな node を末尾に連結
*/
struct node *add_tail(struct node *p){
    struct node *old;

    p->prev = NULL;        /* p->prev に NULL を代入 */
    p->next = NULL;        /* p->next に NULL を代入 */

    /* もしも名前の同じノードがあるならば */
    if((old = find_node(p->name)) != NULL){
        rem_node(old);    /* リストから外す */
    }
    if(head == NULL){
        head = tail = p;
    }
    else {
        tail->next = p;  /* 現在の tail の next に pを代入 */
        p->prev = tail;  /* 現在の tail を p->prev に代入 */
        tail = p;    /* tail に p を代入 */
    }

    printf("-> add_tail(): %s: %d\n", p->name, p->height);
    return old;
}


/*
    print_nodes_head 関数
    head から順にノードを表示
 */
void print_nodes_head(void){
    struct node *p;
    printf("-> print_nodes_head(): start.\n");
    for(p = head; p != NULL; p = p->next){
        printf("%s: %d\n", p->name, p->height);
    }
    printf("-> print_nodes_head(): end.\n");
}


/*
    print_nodes_tail 関数
    tail から逆順にノードを表示
 */
void print_nodes_tail(void){
    struct node *p;
    printf("-> print_nodes_tail(): start.\n");
    for(p = tail; p != NULL; p = p->prev){
        printf("%s: %d\n", p->name, p->height);
    }
    printf("-> print_nodes_tail(): end.\n");
}
/*
    free_nodes 関数
    すべてのノードを解放
 */
void free_nodes(void){
    while(head != NULL){
        struct node *p = tail;
        rem_node(p);   /* リストから外す */
        free(p);       /* メモリを解放 */
    }
}

int main(void){
    struct node *np = NULL;
    char name[32];
    int height;
    
    /* 名前と身長を読み込み */
    while(scanf("%s%d", name, &height) != EOF){
        struct node *old = NULL;

        np = node_alloc();  /* メモリ割り当て */
        if(np == NULL){
            fprintf(stderr, "node_alloc failed.\n");
            return 1;
        }
        strcpy(np->name, name);  /* name をコピー */
        np->height = height;     /* height をコピー */

        old = add_tail(np);     /* 追加 */
        /* 同じ名前の古いノードがあれば */
        if(old != NULL){
            free(old);    /* 古いノードを free */
        }
    }

    /* すべてのノードを head から順に表示 */
    print_nodes_head();

    /* すべてのノードを tail から逆順に表示 */
    print_nodes_tail();

    /* すべてのノードを解放 */
    free_nodes();

    return 0;
}
