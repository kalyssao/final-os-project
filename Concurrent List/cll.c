
#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */ 
#include <errno.h>      /* Errors */
#include <stdio.h>      /* Input/Output */
#include <stdlib.h>     /* General Utilities */
#include <pthread.h>    /* POSIX Threads */
#include <string.h>     /* String handling */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h> 

// basic node structure
 typedef struct __node_t {
 int key;
 struct __node_t *next;
 } node_t;

 // basic list structure (one used per list)
 typedef struct __list_t {
 node_t *head;
 pthread_mutex_t lock;
 } list_t;


void List_Init(list_t *L);
void List_Insert(list_t *L, int key);
int List_Lookup(list_t *L, int key);
int List_Delete(list_t *L, int key);
int pop(node_t * head);
void delete_list(node_t *head);
void print_list(node_t * head);


void *myThreadOne(void *vargp) 
{ 
    //list_t *list2= static_cast<list_t*>(vargp);
    //List_Init(list);
    for( int a = 1; a < 20; a = a + 1 ){
      List_Insert((list_t *)vargp, a);
   }
    
    list_t * list2 = (list_t *)vargp;
    printf("PThread 1 List \n");
    List_Delete(list2, 3);
    print_list(list2->head);
    return NULL; 
} 

void *myThreadTwo(void *vargp) 
{  
   
    //list_t *list2= static_cast<list_t*>(vargp);
    //List_Init(list);
    for( int a = 1; a < 20; a = a + 1 ){
      List_Insert((list_t *)vargp, a);
   }
    
    list_t * list2 = (list_t *)vargp;
    printf("PThread 2 List \n");
    List_Delete(list2, 3);
    print_list(list2->head);
    return NULL; 
} 

int main(int argc, char *argv[]) {
    
    list_t * list2 =malloc(sizeof(list_t));
    node_t * test_node = malloc(sizeof(node_t));
    list2->head = test_node;
 	test_node->key = 0;
    
    pthread_t thread_id;
    pthread_t thread_id2; 
    printf("Before Threads\n"); 
    pthread_create(&thread_id, NULL, myThreadOne, (void *)list2);
    pthread_create(&thread_id2, NULL, myThreadTwo, (void *)list2);
    pthread_join(thread_id, NULL);
    pthread_join(thread_id2, NULL);
    printf("After Threads\n");
    
	//node_t * test_list = malloc(sizeof(node_t));
	//list_t * list =malloc(sizeof(list_t));
	
	
	
    // List_Insert(list, 3);
    // List_Insert(list, 2);
    // List_Insert(list, 1);
    
    //List_Delete(&test_list, 3);

    //print_list(list->head);
    /*if (List_Lookup(list, 6) == 0){
        printf("Item found\n");
    } else{
        printf("Item Not Found\n");
    }*/
    
    //print_list(list->head);
    //delete_list(test_list);

    return EXIT_SUCCESS;
}

void List_Init(list_t *L) {
 L->head = NULL;
 pthread_mutex_init(&L->lock, NULL);
 }
 
 void List_Insert(list_t *L, int key) {
	 // synchronization not needed
	 node_t *new = malloc(sizeof(node_t));
	 if (new == NULL) {
	 perror("malloc");
	 return;
	 }
	 new->key = key;
	
	 // just lock critical section
	 pthread_mutex_lock(&L->lock);
	 new->next = L->head;
	 L->head = new;
	 pthread_mutex_unlock(&L->lock);
 }
 
int List_Lookup(list_t *L, int key) {
	 int rv = -1;
	 pthread_mutex_lock(&L->lock);
	 node_t *curr = L->head;
	 while (curr) {
	 if (curr->key == key) {
	 rv = 0;
	 break;
	 }
	 curr = curr->next;
	 }
	 pthread_mutex_unlock(&L->lock);
	 return rv; //now both success and failure
}

int List_Delete(list_t *L, int key){
	 int rv = -1;
	 
	 pthread_mutex_lock(&L->lock);
	 
	 	if ((L->head) == NULL) {
	 		pthread_mutex_unlock(&L->lock);
	        return -1;
	    }
	
	    if ((L->head)->key == key) {
	    	pthread_mutex_unlock(&L->lock);
	        //return pop(head);
	        printf("head is key");
	        return 0;
	    }
	    
	 node_t *curr = L->head;
	 node_t *prev = L->head;
	 while (curr) {
	 //other
	 if (curr->key == key) {
            prev->next = curr->next;
            free(curr);
            rv = 0;
            break;
        }

        prev = curr;
        curr  = curr->next;
	 }
	 pthread_mutex_unlock(&L->lock);
	 return rv; //now both success and failure
}
 

void print_list(node_t * head) {
    node_t * current = head;

    while (current != NULL) {
        printf("%d\n", current->key);
        current = current->next;
    }
}

void delete_list(node_t *head) {
    node_t  *current = head, 
            *next = head;

    while (current) {
        next = current->next;
        free(current);
        current = next;
    }
}
