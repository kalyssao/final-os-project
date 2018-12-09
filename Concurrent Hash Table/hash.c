#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

#define BUCKETS (101)

typedef struct __node_t {
    int key;
    struct __node_t *next;
} node_t;


// basic list structure (one used per list)
typedef struct __list_t {
    node_t *head;
    pthread_mutex_t     lock;
} list_t;


void List_Init(list_t*L) {
    L->head = NULL;
    pthread_mutex_init(&L->lock, NULL);
}

int List_Insert(list_t*L, int key) {
    pthread_mutex_lock(&L->lock);
    node_t*new = malloc(sizeof(node_t));
    if (new == NULL) {
        perror("malloc");
        pthread_mutex_unlock(&L->lock);
        return -1; // fail
    }
    new->key  = key;
    new->next = L->head;
    L->head   = new;
    pthread_mutex_unlock(&L->lock);
    return 0; // success
}

int List_Lookup(list_t*L, int key) {
    int rv = -1;
    pthread_mutex_lock(&L->lock);
    node_t*curr = L->head;
    while (curr) {
        if (curr->key == key) {
            rv = 0;
            break;
        }
        curr = curr->next;
    }    
    pthread_mutex_unlock(&L->lock);
    return rv; // now both success and failure
}

int List_Delete(list_t*L, int key){
    node_t*curr = L->head;
    
    while(curr->key != NULL) {
        if (curr->key == key){
            pthread_mutex_lock(&L->lock);
            node_t*temp;
            temp = curr->next;
            free(curr);
            pthread_mutex_unlock(&L->lock);
        }
        curr = curr->next;
    }
    return NULL;
}

typedef struct __hash_t {
    list_t lists[BUCKETS];
    pthread_mutex_t lock;
} hash_t;

void Hash_Init(hash_t* H) {
    pthread_mutex_init(&H->lock, NULL);
    int i;
    for (i = 0; i < BUCKETS; i++) {
        List_Init(&H->lists[i]);
    }
}

int Hash_Insert(hash_t*H, int key) {
    pthread_mutex_lock(&H->lock);
    int bucket = key % BUCKETS;
    pthread_mutex_unlock(&H->lock);
    return List_Insert(&H->lists[bucket], key);
}
    
int Hash_Lookup(hash_t*H, int key) {
    pthread_mutex_lock(&H->lock);
    int bucket = key % BUCKETS;
    pthread_mutex_unlock(&H->lock);
    return List_Lookup(&H->lists[bucket], key);
}

int Hash_Delete(hash_t*H, int key)  {
    pthread_mutex_lock(&H->lock);
    int bucket = key % BUCKETS;
    List_Delete(&H->lists[bucket], key);
    pthread_mutex_unlock(&H->lock);
}

void Print_Hash(hash_t*H) {
    for(int i; i<BUCKETS; i++){
        if(&H->lists[i] != NULL){
            printf("d\n", &H->lists[i]);
        }
        continue;
    }
}

void ThreadOne(void){
    struct __hash_t hash;
    Hash_Init(&hash);
    Hash_Insert(&hash, 27);
}

void ThreadTwo(void){
    struct __hash_t hash;
    Hash_Init(&hash);
    Hash_Insert(&hash, 300);
    Print_Hash(&hash);

}


int main(int argc, char const *argv[]) {

    pthread_t t1, t2;

    pthread_create(&t1, NULL, ThreadTwo, NULL);
    pthread_create(&t2, NULL, ThreadOne, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    
    return 0;
}