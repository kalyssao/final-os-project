#include <stdlib.h>
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
// synchronization not needed
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

typedef struct __hash_t {
    list_t lists[BUCKETS];
} hash_t;

void Hash_Init(hash_t* H) {
    int i;
    for (i = 0; i < BUCKETS; i++) {
        List_Init(&H->lists[i]);
    }
}

int Hash_Insert(hash_t*H, int key) {
    int bucket = key % BUCKETS;
    return List_Insert(&H->lists[bucket], key);
}
    
int Hash_Lookup(hash_t*H, int key) {
    int bucket = key % BUCKETS;
    return List_Lookup(&H->lists[bucket], key);
}

int main(int argc, char const *argv[])
{
    
    List_Init(a_list *l);
    return 0;
}
