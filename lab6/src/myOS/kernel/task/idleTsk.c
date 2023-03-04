// 闲置任务
#include "../../include/task.h"
#include "../../include/taskQueue.h"
#include "../../include/myPrintk.h"
#include "scheduler/scheduler.h"

extern Queue rdyQueue;
extern Queue newQueue;
extern myTCB* currentTsk;

int emptyFlag = 1;

void idleTsk(void){
    // 不断调用 schedule
    int j = 1;
    while(emptyFlag){
        //每隔一段时间检测一次
        for(int i = 0; i <= 100000000; i++){
            if(i == 100000000){
                myPrintf(0x7, "Idle loop-------------->%d\n",j);
                if(!queueEmpty(&rdyQueue)){
                    emptyFlag = 0;
                    break;
                }
                i = 0;
                j++;
            }  
        }
        if(!emptyFlag){
            emptyFlag = 1;
            break;
        }
    };
    tskEnqueue(&newQueue, currentTsk);
    //无任务一直运行
    // 将idle的TCB块再放入new队列，栈要再次初始化
    tskEnd(); //队列不空时进行调度
}