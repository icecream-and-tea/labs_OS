#ifndef __TASKQUEUE_H__
#define __TASKQUEUE_H__

#include "./task.h"

typedef struct Queue{
    struct myTCB *head;
    struct myTCB *tail;
}Queue;

int initQueue(Queue* queue);
int queueEmpty(Queue* queue);
void tskEnqueue(Queue* queue, struct myTCB* tsk);
void tskEnqueueSort(Queue* queue, struct myTCB* tsk);
struct myTCB* tskDequeue(Queue* queue);
struct myTCB* tskRemove(Queue* queue, unsigned long tid);
struct myTCB* tskFind(Queue* queue, unsigned long tid);
void queueTraverse(Queue* queue);
#endif