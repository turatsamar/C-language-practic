#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ハッシュテーブルのノード */
struct htnode {
    char keyname[32];  /* キーとなる文字列(名前) */
    int  height;     /* 関連するデータ(身長) */
    struct htnode *next; /* 次のポインタ */
};

/* ハッシュテーブル */
struct ht {
    int size;    /* ハッシュテーブルの要素数 */
    struct htnode **table; /* ハッシュテーブル */
};

/* ハッシュテーブルのノードを確保 */
struct htnode *htnode_alloc(void){
    struct htnode *ret = NULL;
    ret = (struct htnode *)malloc(sizeof(struct htnode));
    if(ret != NULL){
        memset(ret, 0, sizeof(struct htnode));
    }
    return ret;
}

/* ハッシュテーブルを確保 */
struct ht *ht_alloc(int size){
    struct ht *ret = NULL;
    ret = (struct ht *)malloc(sizeof(struct ht));

    /* malloc成功ならば */
    if(ret != NULL){
        ret->size = size;  /* 要素数を代入 */
        ret->table = (struct htnode **)malloc
                (sizeof(struct htnode *) * size);
        /* malloc失敗 */
        if(ret->table == NULL){
            free(ret);
            return NULL;
        }
        memset(ret->table, 0, sizeof(struct htnode *) * size);
    }
    return ret;
}

/* ハッシュテーブルとノードすべてを解放 */
void ht_free(struct ht *htp){
    int i;
    for(i = 0; i < htp->size; i++){
        while(htp->table[i] != NULL){
            struct htnode *n = htp->table[i];
            htp->table[i] = n->next;
            free(n);
        }
    }
}

/* htp->table 用の hash 関数 */
unsigned int hash(struct ht *htp, char *key){
    char *s = key;
    unsigned int ret = 0;
    /* 終端文字でない限り */
    while(*s != '\0'){
        ret = ret * 37 + *s;
        s++;
    }

    /* htp->table の要素数で割った余りを取る */
    return ret % htp->size;
}

/*
    ht_put 関数:
    ・ノードをハッシュテーブルに登録
    ・すでに同じキーのノードが登録されている場合は、
    入れ替えて、古いノードを返す。
 */
struct htnode *ht_put(struct ht *htp, struct htnode *node){
    int hashval = hash(htp, node->keyname); /* ハッシュ値を計算 */
    struct htnode *p = htp->table[hashval];

    node->next = NULL;    /* nodeのnext を初期化 */

    /* htp->table[hashval] が NULL ならば */
    if(p == NULL){
        /* htp->table[hashval]に代入 */
        htp->table[hashval] = node;
        return NULL;  /* NULLをreturn */
    }

    /* 先頭のキーが同じかチェック */
    else if(strcmp(p->keyname, node->keyname) == 0){
        node->next = p->next;  
        htp->table[hashval] = node;
        p->next = NULL;
        return p;
    }

    /* ほかのノードが同じキーを持っているかチェック */
    for(; p->next != NULL; p = p->next){
        struct htnode *n = p->next;
        
        /* 同じキーかチェック */
        if(strcmp(n->keyname, node->keyname) == 0){
            /* 入れ替え */
            p->next = node;
            node->next = n->next;
            n->next = NULL;
            return n;
        }
    }

    /* 同じキーのノードはテーブルに存在しない */
    p->next = node;

    return NULL;
}

/* 
    ht_del 関数
    ・key で指定されたキーを持つノードをハッシュテーブルから削除し、そのノードを返す。
    ・ノードがなければ NULL を返す。 
 */
struct htnode *ht_del(struct ht *htp, char *key){
    int hashval = hash(htp, key);  /* ハッシュ値を計算 */
    struct htnode *p = htp->table[hashval];

    /* htp->table[hashval] が NULL ならば */
    if(p == NULL){
        return NULL;
    }
    /*  htp->table[hashval] のキーが同じならば */
    else if(strcmp(key, p->keyname) == 0){
        htp->table[hashval] = p->next;
        p->next = NULL;
        return p;
    }

    for(; p->next != NULL; p = p->next){
        struct htnode *n = p->next;

        /* 同じキーかチェック */
        if(strcmp(n->keyname, key) == 0){
            /* 削除 */
            p->next = n->next;
            n->next = NULL;
            return n;
        }
    }
    return NULL;
}

/*
    ht_get 関数:
    ・key で指定されたキーを持つノードを返す。
    ・なければ NULL を返す。
 */
struct htnode *ht_get(struct ht *htp, char *key){
    int hashval = hash(htp, key);  /* ハッシュ値を計算 */
    struct htnode *p = htp->table[hashval];

    /* pがNULLでない限り繰り返し */
    while(p != NULL){
        /* key が同じならば、while 文から脱出 */
        if(strcmp(p->keyname, key) == 0){
            break;
        }
        p = p->next;  /* 次のノード */
    }
    return p;
}


/* ハッシュテーブルのデータをすべて表示 */
void ht_dump(struct ht *htp){
    int i;
    for(i = 0; i < htp->size ; i++){
        struct htnode *n = htp->table[i];
        printf("ht[%d]: ", i);

        while(n != NULL){
            printf("-> (%s %d) ", n->keyname, n->height);
            n = n->next;
        }
        printf("-> NULL\n");
    }
}


int main(void){
    struct ht *htp = NULL;
    struct htnode *np = NULL;
    char name[32];
    int height;

    /* 要素数 5 のハッシュテーブルを確保 */
    htp = ht_alloc(5);
    if(htp == NULL){
        fprintf(stderr, "ht_alloc failed.\n");
        return 1;
    }
    
    /* 名前と身長を読み込み */
    while(scanf("%s%d", name, &height) != EOF){
        struct htnode *old = NULL;

        np = htnode_alloc();  /* メモリ割り当て */
        if(np == NULL){
            fprintf(stderr, "htnode_alloc failed.\n");
            return 1;
        }
        strcpy(np->keyname, name);  /* nameをコピー */
        np->height = height;     /* heightをコピー */

        old = ht_put(htp, np);      /* 登録 */
        /* 同じキーの古いノードがあれば */
        if(old != NULL){
            free(old);      /* 古いノードを解放 */
        }
    }

    /* ハッシュテーブルの状態をダンプ */
    ht_dump(htp);

    /* ハッシュテーブルを解放 */
    ht_free(htp);
    return 0;
}
