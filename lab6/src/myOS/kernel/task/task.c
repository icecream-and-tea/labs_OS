/* state:  null, new, ready, waiting, terminal, running */
#include "../../include/task.h"
#include "scheduler/scheduler.h"
#include "../../include/myPrintk.h"

extern void idleTsk(void);
extern unsigned long *IdleTskStkPtr;

Queue rdyQueue;
Queue waitQueue;
Queue newQueue;

myTCB* prevTsk;
myTCB* currentTsk;

void CTX_SW(void);
// 上下文切换
void contextSwitch(unsigned long **prevTskStkAddr, unsigned long *nextTskStk) { 
    prevTSK_StackPtrAddr = prevTskStkAddr; 
    nextTSK_StackPtr = nextTskStk; 
    CTX_SW(); 
}

void CTX_NEXT(void);
// 不进行保护现场的上下文切换
void setNextTsk(unsigned long *nextTskStk){
    nextTSK_StackPtr = nextTskStk;
    CTX_NEXT();
}

// 创建一个进程时，就将一些内容压入栈中
// 栈的初始化
void stackInit(unsigned long **stk, void (*task)(void)) { 

    *(*stk)-- = (unsigned long)0x08; // CS 
    *(*stk)-- = (unsigned long)task; // eip 

    // pushf 
    *(*stk)-- = (unsigned long)0x0202; // flag registers 

    // pusha 
    *(*stk)-- = (unsigned long)0xAAAAAAAA; // eax 
    *(*stk)-- = (unsigned long)0xCCCCCCCC; // ecx 
    *(*stk)-- = (unsigned long)0xDDDDDDDD; // edx 
    *(*stk)-- = (unsigned long)0xBBBBBBBB; // ebx 
    *(*stk)-- = (unsigned long)0x44444444; // esp 
    *(*stk)-- = (unsigned long)0x55555555; // ebp 
    *(*stk)-- = (unsigned long)0x66666666; // esi 
    **stk = (unsigned long)0x77777777;     // edi 
}

// 生成任务ID
unsigned long getTid(){
    static unsigned long i = 1;
    return i++;
}

// 创建新任务
unsigned long null2new(void (*tskBody)(void), tskPara* para){
    unsigned long start = kmalloc(sizeof(myTCB)); 

    myTCB newTsk;
    // 初始化基本参数
    newTsk.tid = getTid();
    newTsk.state = NEW; 
    newTsk.pc = (unsigned long*)tskBody;
    newTsk.next = (void*) 0;
    newTsk.stackPtr = (unsigned long *)(kmalloc(STACK_SIZE) + STACK_SIZE - 4); //反向增长
    // 初始化拓展参数
    newTsk.arrTime = para->arrTime;
    newTsk.exeTime = para->exeTime;
    newTsk.priority = para->priority;
    // 初始化栈
    stackInit(&(newTsk.stackPtr), tskBody); 
    //log：栈低指针出现错误，意外的修改了EMB

    memcpy((void*)start, &newTsk, sizeof(myTCB));

    // 插入队列，若达到时间为0，应立即启动
    if(newTsk.arrTime == 0){
        ((myTCB*)start)->state = READY;
        tskEnqueue_sche(&rdyQueue, (myTCB*)start);
    }
    else{
        tskEnqueueSort(&newQueue, (myTCB*)start); //插入新建队列
    }
    return newTsk.tid;
}

// 启动任务
/* lab6 要求根据到达时间实现任务就绪动态化 */
void new2ready(unsigned long tid){
    //在new队列删除，插入wait队列
    myTCB *tsk;
    tsk = tskRemove(&newQueue, tid);
    tsk->state = READY;
    tskEnqueue_sche(&rdyQueue, tsk); // 挂钩函数
}

// 运行任务
void ready2running(){
    prevTsk = currentTsk;
    currentTsk = tskDequeue(&rdyQueue);
    currentTsk->state = RUNNING;
    // //debug
    // myPrintf(0x3, "prevTsk id = %d, pc = 0x%x, str = 0x%x\n", prevTsk->tid, prevTsk->pc, prevTsk->stackPtr);
    // myPrintf(0x3, "nextTsk id = %d, pc = 0x%x, str = 0x%x\n", currentTsk->tid, currentTsk->pc, currentTsk->stackPtr);
    // 如果该进程已经结束，则不必保护现场
    if(prevTsk->state == TERMINAL){
        setNextTsk(currentTsk->stackPtr);
    }
    else{//若进程未结束，想要保护上下文
        contextSwitch(&(prevTsk->stackPtr), currentTsk->stackPtr);
    }
}

// 销毁任务，释放栈空间
void ready2terminal(unsigned long tid){
    //在ready队列删除，释放栈空间
    myTCB *tsk;
    tsk = tskRemove(&rdyQueue, tid);
    kfree((unsigned long)tsk->stackPtr);
}

// 结束任务
void running2terminal(){
    currentTsk->state = TERMINAL;
    if(currentTsk->tid != 1){
        kfree((unsigned long)currentTsk->stackPtr);
    }
    else{//对于Idle特殊处理，将其进程栈空间保留，并重新初始化
        currentTsk->stackPtr = IdleTskStkPtr;
        stackInit(&(currentTsk->stackPtr), idleTsk);
    }
}

// 释放TCB空间
void terminal2null(myTCB *tsk){
    //释放进程块本身
    kfree((unsigned long) tsk);
}

/* lab6 新增代码 */
// 用于RR：将运行任务插入就绪队列
void running2ready(){
    tskEnqueue(&rdyQueue, currentTsk);
}

// 任务动态到达
void tskDynArr(unsigned long sysTime){
    if(newQueue.head->arrTime <= sysTime && newQueue.head->tid != 1){
        tskStart(newQueue.head->tid);
    }
}

/*-------------------------------以下都是接口函数-------------------------*/

// 任务创建
unsigned long createTsk(void (*tskBody)(void), tskPara* para){
    null2new(tskBody, para);
}

// 任务销毁
void destroyTsk(unsigned long tid){
    ready2terminal(tid);
}

// 任务启动
void tskStart(unsigned long tid){
    //myPrintf(0x4, "Start task %d\n", tid);
    new2ready(tid);
}

// 任务结束
void tskEnd(){
    running2terminal();
    schedule(); // 为接口，使用不同的调度策略
}