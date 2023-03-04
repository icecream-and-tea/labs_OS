// 获取下一个任务，使用上下文切换运行此任务
#include "../../../include/task.h"
#include "scheduler.h"

#define SJF 3

//调用时间：每个任务结束时
void nextTsk_SJF(void){
    ready2running();
}

void tskEnqueue_SJF(Queue* queue, myTCB* tsk){
    // 按到达时间排序
    myTCB* p = queue->head;
    myTCB* pre = p;
    if(queueEmpty(queue)){
        queue->head = tsk;
        queue->tail = tsk;
    }// 插头尾节点不同
    else{
        while(p != (void*)0 && tsk->exeTime >= p->exeTime){
            pre = p;
            p = p->next;
        }
        // 维护尾指针
        if(pre == queue->tail && p == (void*)0){
            queue->tail = tsk;
        }
        // 维护头指针
        if(p == queue->head){
            queue->head = tsk;
        }
        else{
            pre->next = tsk;
        }
        tsk->next = p;
    }
}

struct scheduler scheduler_SJF = {
    SJF, nextTsk_SJF, tskEnqueue_SJF, NULL
};

