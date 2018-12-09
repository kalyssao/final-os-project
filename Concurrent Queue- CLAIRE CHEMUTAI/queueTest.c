// Author Claire Chemutai
// Reference: Three Easy Pieces
#include "queue.h"
#include <stdio.h>
#include <assert.h>
#include <pthread.h>



void first_test(int argc, char const *argv[]){
    printf("Thread 1 created\n");
    struct __queue_t q;

    Queue_Init(&q);
    printf("Queue in thread 1 initialized successfully\n");
    Queue_Enqueue(&q, 1);
    printf("T1:Added 1 \n");
    Queue_Enqueue(&q, 2);
    printf("T1:Added 2 \n");
    Queue_Enqueue(&q, 3);
    printf("T1:Added 3 \n");
    Queue_Dequeue(&q, 2);
    printf("T1:Removed 2 \n");


    return;

}
void second_test(int argc, char const *argv[]){
    printf("Thread 2 created\n");
    struct __queue_t q;

    Queue_Init(&q);
    printf("Queue in thread 2 initialized successfully\n");
    Queue_Enqueue(&q, 4);
    printf("T2:Added 4 \n");
    Queue_Enqueue(&q, 5);
    printf("T2:Added 5 \n");
    Queue_Enqueue(&q, 6);
    printf("T2:Added 6 \n");
    Queue_Dequeue(&q, 4);
    printf("T2:Removed 4 \n");

    return;

}

int main(int argc, char const *argv[])
{
    pthread_t p1, p2;
    printf("main: begin\n");
    pthread_create(&p1, NULL, first_test, "A");
    pthread_create(&p2, NULL, second_test, "B");

    //join the threads
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    printf("Main: End\n");


    return 0;
}




