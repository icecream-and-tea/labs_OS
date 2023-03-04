#include "../../include/taskQueue.h"
#include "../../include/task.h"
#include "../../include/myPrintk.h"
#include "../../include/mem.h"
#include "../../include/tskPara.h"
#include "./scheduler/scheduler.h"

#define INFINITE 100000000

extern myTCB* currentTsk;
extern Queue rdyQueue;

unsigned long *IdleTskStkPtr;

//启动多任务运行
void startMultitask(void) { 
    myTCB *tsk;
    tsk = tskDequeue(&rdyQueue); // 手动使init函数出队
    currentTsk = tsk;
    setNextTsk(currentTsk->stackPtr); // 开始第一个任务
}


extern Queue rdyQueue;
extern Queue newQueue;
extern Queue waitQueue;
extern myTCB* currentTsk;

extern void idleTsk(void);
extern void initTskBdy(void);
extern int whichScheduler(void); //用户态设置调度器

// 任务初始化原语
void tskManagerInit(void){
    // 设置调度算法
    setSysScheduler(whichScheduler()); // 此处应该为Hook，去用户中寻找具体实现

    // 三个队列的初始化
    initQueue(&newQueue);
    initQueue(&rdyQueue);
    initQueue(&waitQueue);
    currentTsk = (void*)0;

    // 创建两个任务
    myPrintf(0x7, "create idleTsk\n");
    struct tskPara tmp;
    setTskPara(PRIORITY, 0, &tmp);
    setTskPara(EXETIME, INFINITE, &tmp);
    setTskPara(ARRTIME, INFINITE, &tmp);
    createTsk(idleTsk, &tmp); //id 为 1
    IdleTskStkPtr = newQueue.head->stackPtr + 10; // 要栈底指针
    
    myPrintf(0x7, "create initTsk\n");
    setTskPara(PRIORITY, 0, &tmp);
    setTskPara(EXETIME, 0, &tmp);
    setTskPara(ARRTIME, 0, &tmp);
    createTsk(initTskBdy, &tmp); // id 为 2
    myPrintf(0x7,"\n");

    //进入多任务模式
    myPrintf(0x2, "START MULTITASK...\n");
    startMultitask();
    myPrintf(0x2, "STOP MULTITASK.......SHUTDOWN\n");

}