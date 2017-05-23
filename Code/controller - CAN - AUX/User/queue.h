#ifndef __QUEUE_H
#define __QUEUE_H

#include "including.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct queue
{
        int queuesize;   //�����С
        int head, tail;  //ͷβ�±�
        int *q;          //����ͷָ��
}Queue;

void InitQueue(Queue *Q,u8 size);
void EnQueue(Queue *Q, int key);
int DeQueue(Queue *Q);
int IsQueueEmpty(Queue *Q);
int IsQueueFull(Queue *Q);

#endif
