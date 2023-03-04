#include "../../include/taskQueue.h"
#include "../../include/task.h"
#include "../../include/state.h"
#include "../../include/myPrintk.h"
#include "../../include/mem.h"

extern myTCB* currentTsk;
extern Queue rdyQueue;

unsigned long BspContextBase[STACK_SIZE] ;
unsigned long *BspContext;

//启动多任务运行
void startMultitask(void) { 
    myTCB *tsk;
    BspContext = BspContextBase + STACK_SIZE - 1; 
    tsk = tskDequeue(&rdyQueue); // 手动使init函数出队
    currentTsk = tsk;
    contextSwitch(&(BspContext), currentTsk->stackPtr);
}


extern Queue rdyQueue;
extern Queue newQueue;
extern Queue waitQueue;
extern myTCB* currentTsk;

extern void idleTsk(void);
extern void initTskBdy(void);

// 任务初始化原语
void tskManagerInit(void){
    // 三个队列的初始化
    initQueue(&newQueue);
    initQueue(&rdyQueue);
    initQueue(&waitQueue);

    // 创建两个任务
    myPrintf(0x7, "create idleTsk\n");
    createTsk(idleTsk); //id 为 1
    myPrintf(0x7, "create initTsk\n");
    createTsk(initTskBdy); // id 为 2
    myPrintf(0x7,"\n");

    tskStart(2); // 将初始化任务转入就绪队列

    //进入多任务模式
    myPrintf(0x2, "START MULTITASK...\n");
    startMultitask();
    myPrintf(0x2, "STOP MULTITASK.......SHUTDOWN\n");

}