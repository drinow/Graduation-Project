#ifndef __QUEUE_H
#define __QUEUE_H

#include "including.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct queue
{
        int queuesize;   //数组大小
        int head, tail;  //头尾下标
        int *q;          //数组头指针
}Queue;

void InitQueue(Queue *Q,u8 size);
void EnQueue(Queue *Q, int key);
int DeQueue(Queue *Q);
int IsQueueEmpty(Queue *Q);
int IsQueueFull(Queue *Q);

#endif
