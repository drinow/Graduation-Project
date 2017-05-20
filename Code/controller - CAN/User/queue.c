#include "queue.h"

void InitQueue(Queue *Q,u8 size)
{
        Q->queuesize = size;
        Q->q = (int *)malloc(sizeof(int) * Q->queuesize); //����ͷָ��
        Q->tail = 0;
        Q->head = 0;
}

void EnQueue(Queue *Q, int key)
{
        int tail = (Q->tail+1) % Q->queuesize; //ȡ�ౣ֤,��quil=queuesize-1ʱ,��ת��0
        if (tail == Q->head)                   //������
        {
            printf("the queue has been filled full!");
        }
        else
        {
            Q->q[Q->tail] = key;
            Q->tail = tail;
        }
}

int DeQueue(Queue *Q)
{
        int tmp;
        if(Q->tail == Q->head)     //�����Ƿ��
        {
            printf("the queue is NULL\n");
        }
        else
        {
            tmp = Q->q [Q->head];
            Q->head = (Q->head+1) % Q->queuesize;
        }
        return tmp;
}

int IsQueueEmpty(Queue *Q)
{
        if(Q->head == Q->tail)
        {
            return 1;
        }
        else
        {
            return 0;
        }
}

int IsQueueFull(Queue *Q)
{
    if((Q->tail+1)% Q->queuesize == Q->head)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

