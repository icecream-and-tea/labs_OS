// 任务就绪队列
#include "../../include/taskQueue.h"
#include "../../include/myPrintk.h"

#define myTCB struct myTCB

extern Queue newQueue;

int initQueue(Queue* queue){
    queue->head = (void *)0;
    queue->tail = (void *)0;
}

// 判断队列是否为空
int queueEmpty(Queue* queue){
    if (queue->head  == (void *)0 && queue->tail == (void *)0){
        return 1;
    }
    return 0;
}

// 任务入队
void tskEnqueue(Queue* queue, myTCB* tsk){
    if(queueEmpty(queue)){
        queue->head = tsk;
    }
    else{
        queue->tail->next = tsk;
    }
    queue->tail = tsk;
    tsk->next = (void *)0;
}

// 任务出队
myTCB* tskDequeue(Queue* queue){
    myTCB* tsk = queue->head;
    queue->head = tsk ->next;

    if(tsk == queue->tail){
        queue->tail = (void *)0;
    }

    return tsk;
}

// 找到对应的id的任务删除
myTCB* tskRemove(Queue* queue, unsigned long tid){
    myTCB *p = queue->head;
    myTCB *pre = p;
    while(p != (void*) 0 && p->tid != tid){
        pre = p;
        p = p->next;
    }

    //log: 没有调整head && tail !!!

    //删除
    if(p == queue->head){
        queue->head = p->next;
        //只有这一个任务
        if(queue->head == (void*)0){
            queue->tail = (void*)0;
        }
    }
    else{
        pre->next = p->next;
        p->next = (void *)0; //脱离
        // 改尾指针
        if(p == queue->tail){
            queue->tail = pre;
        }
    }

    return p;//有可能没找到，返回NULL
}

//输入id 找到对应的TCB块
myTCB* tskFind(Queue* queue, unsigned long tid){
    myTCB *p = queue->head;
    while(p != (void*) 0 && p->tid != tid){
        p = p->next;
    }
    return p;
}

void queueTraverse(Queue* queue){
    myTCB *p = queue->head;
    while(p != (void*)0){
        myPrintf(0x7, "The task id is %d, str is 0x%x\n", p->tid, p->stackPtr);
        p = p->next;
    }
}