// 获取下一个任务，使用上下文切换运行此任务
#include "../../../include/task.h"
#include "scheduler.h"

#define FCFS 1

//调用时间：每个任务结束时
void nextTsk_FCFS(void){
    ready2running();
}

void tskEnqueue_FCFS(Queue* queue, myTCB* tsk){
    if(queueEmpty(queue)){
        queue->head = tsk;
    }
    else{
        queue->tail->next = tsk;
    }
    queue->tail = tsk;
    tsk->next = (void *)0;
}

struct scheduler scheduler_FCFS = {
    FCFS, nextTsk_FCFS, tskEnqueue_FCFS, NULL
};
