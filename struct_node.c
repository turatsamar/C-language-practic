


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* node 構造体と変数 head と変数 tail */
struct node {
    char name[32];        /* 名前 */
    int  height;        /* 身長 */
    struct node *next;    /* 次のノードへのポインタ */
} *head = NULL, *tail = NULL;

/* 
    node_alloc 関数
    node 構造体分を malloc し、malloc に成功した場合だけ初期化する。
    mallocの返り値を返す。
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
        /* 名前が同じかチェック */
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
    struct node *q = head;

    printf("-> rem_node(): %s: %d\n", p->name, p->height);

    /* p が先頭のノードの場合 */
    if(p == head){
        head = head->next;
        /* p が末尾でもある場合 */
        if(p == tail){
            tail = NULL;
        }
        p->next = NULL; 
        return;
    }

    /* p が先頭以外のノードの場合 */
    for(; q->next != NULL; q = q->next)
        if(q->next == p){
            q->next = p->next;
            /* p が末尾でもある場合 */
            if(p == tail){
                tail = q;
            }
            p->next = NULL;
            return;
        }
}

/*  
    add_tail 関数
    同じ名前のノードがあれば外し、新たな node を末尾に連結
*/
struct node *add_tail(struct node *p){
    struct node *old;

    p->next = NULL;        /* p->next に NULL を代入 */

    /* もしも名前の同じノードがあるならば */
    if((old = find_node(p->name)) != NULL)
        rem_node(old);    /* リストから外す */

    if(head == NULL){
        head = tail = p;
    }
    else {
        tail->next = p;    /* 現在の tail の next に p を代入 */
        tail = p;        /* tail に p を代入 */
    }

    printf("-> add_tail(): %s: %d\n", p->name, p->height);
    return old;
}

/*
    print_nodes 関数
    head から順にノードを表示
 */
void print_nodes(void){
    struct node *p;
    printf("-> print_nodes(): start.\n");
    for(p = head; p != NULL; p = p->next){
        printf("%s: %d\n", p->name, p->height);
    }
    printf("-> print_nodes(): end.\n");
}

/*
    free_nodes 関数
    すべてのノードを解放
 */
void free_nodes(void){
    while(head != NULL){
        struct node *p = tail;
        rem_node(p);    /* リストから外す */
        free(p);    /* メモリを解放 */
    }
}

int main(void){
    struct node *np = NULL;
    char name[32];
    int height;

    /* 名前と身長を読み込み */
    while(scanf("%s%d", name, &height) != EOF){
        struct node *old = NULL;

        np = node_alloc();    /* メモリ割り当て */
        if(np == NULL){
            fprintf(stderr, "node_alloc failed.\n");
            return 1;
        }
        strcpy(np->name, name);    /* name をコピー */
        np->height = height;        /* height をコピー */
        
        old = add_tail(np);        /* 追加 */
        /* 同じ名前の古いノードがあれば */
        if(old != NULL){
            free(old);        /* 古いノードを free */
        }
    }

    /* すべてのノードを表示 */
    print_nodes();

    /* すべてのノードを解放 */
    free_nodes();

    return 0;
}
