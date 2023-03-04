// 获取下一个任务，使用上下文切换运行此任务
#include "../../../include/task.h"
#include "scheduler.h"
#include "../../../include/myPrintk.h"
#define RR 2

extern myTCB* currentTsk;
extern Queue rdyQueue;

static int i = 0;

//调用时间：每个时间片
void nextTsk_RR(void){
    i = 0;
    ready2running(); // 从就绪队列选取下一个任务运行
}

void tskEnqueue_RR(Queue* queue, myTCB* tsk){
    if(queueEmpty(queue)){
        queue->head = tsk;
    }
    else{
        queue->tail->next = tsk;
    }
    queue->tail = tsk;
    tsk->next = (void *)0;
}

void tickHook_RR(){
    //当开始多任务运行，且就绪队列不为空，且现在不为Idle任务运行时才轮转
    if(i++ >= 40 && currentTsk && !queueEmpty(&rdyQueue) && currentTsk->tid != 1){ //40ms 为一个时间片
        i = 0;
        running2ready(); // 将现在运行的任务放回就绪队列
        nextTsk_RR();
    }
}

struct scheduler scheduler_RR = {
    RR, nextTsk_RR, tskEnqueue_RR, tickHook_RR
};