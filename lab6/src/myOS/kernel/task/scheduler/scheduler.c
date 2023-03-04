#include "scheduler.h"
#include "../../../include/myPrintk.h"
#include "../../../include/task.h"

extern int queueEmpty(Queue* queue);
extern Queue rdyQueue;


extern struct scheduler scheduler_FCFS;
extern struct scheduler scheduler_SJF;
extern struct scheduler scheduler_RR;

void (*nextTsk_hook)(void);
void (*enqueueTsk_hook)(Queue *queue, myTCB *tsk);
void (*tick_hook)(void);

void nextTsk(void){
    if(nextTsk_hook)
        nextTsk_hook();
}

void tskEnqueue_sche(Queue *queue, myTCB *tsk){
    if(enqueueTsk_hook)
        enqueueTsk_hook(queue, tsk);
}

void scheduler_tick(void){
    if(tick_hook)
        tick_hook();
} // run every tick


// 调度函数
void schedule(void){
    if(queueEmpty(&rdyQueue)){
        // 手动启用idle任务
        myPrintf(0x7, "Idle start...\n");
        tskStart(1); // idle的tid为1
        schedule();
    }
    else{
        nextTsk();
    } 
}

// 将相应调度器里的调度接口与顶层的统一调度接口进行对接
void setScheFunc(struct scheduler sche) {
	nextTsk_hook = sche.nextTsk_func;
    enqueueTsk_hook = sche.enqueueTsk_func;
    tick_hook = sche.tick_hook;
}

void setSysScheduler(unsigned int what){
    switch (what)
    {
    case FCFS:
        setScheFunc(scheduler_FCFS);
        break;
    case SJF:
        setScheFunc(scheduler_SJF);
        break;
    case RR:
        setScheFunc(scheduler_RR);
        break;
    
    default:
        break;
    }
}
