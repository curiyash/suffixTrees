#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "suffixTree.h"
#include "queue.h"

void initQueue(queue *q){
    q->front = NULL;
    q->rear = NULL;
}

void enqueue(queue *q, node *n){
    qElem *elm = (qElem *) malloc(sizeof(qElem));
    if (!elm){
        printf("Queue overflow!\n");
    } else{
        elm->n = n;
        elm->next = NULL;
        if (!q->front){
            q->front=q->rear=elm;
        } else{
            q->rear->next = elm;
            q->rear = elm;
        }
    }
}

void dequeue(queue *q){
    if (q->front){
        qElem *ret = q->front;
        q->front = q->front->next;
        if (!q->front){
            q->rear = q->front;
        }
        free(ret);
    } else{
        printf("Queue underflow\n");
    }
}

node *front(queue q){
    if (q.front){
        return q.front->n;
    } else{
        printf("Queue is empty\n");
    }
}

int isEmptyQ(queue q){
    if (!q.front){
        return 1;
    } else{
        return 0;
    }
}
