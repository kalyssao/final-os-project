#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct __element {
    char name[256];
    struct __element * next;
} node;

// initialising an array of pointers to type node
node *hashTable[1000];

// size of hash table
int hashTableSize = 10;

// the hash function, which
int stringToKey(char name[]){

}