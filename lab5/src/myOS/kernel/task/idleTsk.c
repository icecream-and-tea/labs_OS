// 闲置任务
#include "../../include/task.h"
#include "../../include/taskQueue.h"

extern Queue rdyQueue;
extern Queue newQueue;
extern myTCB* currentTsk;

void idleTsk(void){
    // 不断调用 schedule
    while(queueEmpty(&rdyQueue)); //无任务一直运行
    // 将idle的TCB块再放入new队列，姑且就先这样吧
    tskEnqueue(&newQueue, currentTsk);
    schedule(); //队列不空时进行调度
}