#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread/pthread.h>

#define MAX_HASH  1027
#define NUMTHREADS 2
pthread_t t [ NUMTHREADS ];

typedef struct HashItem {
    char *key;
    char *value;
    struct HashItem *next;
    pthread_mutex_t lock;
} HashItem;

// generate hash code
unsigned int hashCode(const char *key) {
    unsigned int sum = 0;

    for (int i = 0; key[i]; i++) {
        sum += (unsigned char)key[i] * (i + 1);
    }
    return sum % MAX_HASH;
}

// free item
void freeItem(HashItem *item) {
    free(item->key);
    free(item->value);
    free(item);
}

// function for deleting item from hash table
void deleteItem(HashItem *table[], const char *key) {
    HashItem **link = &table[hashCode(key)];

    while (*link) {
        HashItem *tmp = *link;
        //pthread_mutex_init(&->lock, NULL)
        if (strcmp(tmp->key, key) == 0) {
            *link = tmp->next;  // unlink the list node
            freeItem(tmp);
            break;
        } else {
            link = &(*link)->next;
        }
    }
}

// function for inserting item into the table
void insertItem(HashItem *table[], const char *key, const char *value) {
    unsigned int code = hashCode(key);
    HashItem *item = malloc(sizeof(*item));
    if (item != NULL) {
        item->key = strdup(key);
        item->value = strdup(value);
        item->next = table[code];
        table[code] = item;
    }
}

// displaying items
void displayHashTable(HashItem *table[]) {
    for (int i = 0; i < MAX_HASH; i++) {
        for (HashItem *tmp = table[i]; tmp; tmp = tmp->next) {
            printf("%s: %s\n", tmp->key, tmp->value);
        }
    }
}

void *myThreadOne(void *arg){
    insertItem((HashItem *)arg, "Bart", "Simpson");
    HashItem * table = (HashItem *)arg;
}

void *myThreadTwo(void *arg){
    insertItem((HashItem *)arg, "Lisa", "Simpson");
    HashItem * table = (HashItem *)arg;
}

int main(int argc, char const *argv[]) {
    HashItem *table[MAX_HASH] = { 0 };

    pthread_create(&t[0], NULL, myThreadOne, (void *)table);
    pthread_create(&t[1], NULL, myThreadTwo, (void *)table);
    
    deleteItem(table, "Lisa");
    pthread_exit(NULL);
    //insertItem(table, "Bart", "Simpson");
    displayHashTable(table);
    return 0;
}