#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread/pthread.h>

#define MAX_HASH  1027
#define NUMTHREADS 2
pthread_t t1, t2;

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
void deleteItem(HashItem *table[], const char *key, pthread_mutex_t lock) {
    HashItem **link = &table[hashCode(key)];

    while (*link) {
        HashItem *tmp = *link;
        pthread_mutex_lock(&lock);
        if (strcmp(tmp->key, key) == 0) {
            *link = tmp->next;  // unlink the list node
            pthread_mutex_unlock(&lock);
            freeItem(tmp);
            break;
        } else {
            link = &(*link)->next;
            pthread_mutex_unlock(&lock);
        }
    }
}

// function for inserting item into the table - acquires lock 
void insertItem(HashItem *table[], const char *key, const char *value, pthread_mutex_t lock) {
    unsigned int code = hashCode(key);
    HashItem *item = malloc(sizeof(*item));
    if (item != NULL) {
        pthread_mutex_lock(&lock);
        item->key = strdup(key);
        item->value = strdup(value);
        item->next = table[code];
        table[code] = item;
        pthread_mutext_unlock(&lock);
    }
}

// displaying hash table elements
void displayHashTable(HashItem *table[]) {
    for (int i = 0; i < MAX_HASH; i++) {
        for (HashItem *tmp = table[i]; tmp; tmp = tmp->next) {
            printf("%s: %s\n", tmp->key, tmp->value);
        }
    }
}

// testing thread 1
void *myThreadOne(void *arg){
    pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
    insertItem((HashItem **)arg, "Bart", "Simpson", lock1);
    HashItem * table = (HashItem *)arg;
    deleteItem((HashItem **)arg, "Lisa", lock1);
    return 0;
}

// testing thread 2
void *myThreadTwo(void *arg){
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    insertItem((HashItem **)arg, "Lisa", "Simpson", lock);
    HashItem * table = (HashItem *)arg;
    return 0;
}

int main(int argc, char const *argv[]) {
    HashItem *table[MAX_HASH] = { 0 };

    pthread_create(&t1, NULL, myThreadTwo, table);
    pthread_create(&t2, NULL, myThreadOne, table);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    displayHashTable(table);
    return 0;
}