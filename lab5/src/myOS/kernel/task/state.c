/* state:  null, new, ready, waiting, terminal, running */
#include "../../include/state.h"

extern void idleTsk(void);


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

//创建新任务
unsigned long null2new(void (*tskBody)(void)){
    unsigned long start = kmalloc(sizeof(myTCB)); 

    myTCB newTsk;
    newTsk.tid = getTid();
    newTsk.state = NEW; 
    newTsk.pc = (unsigned long*)tskBody;
    newTsk.next = (void*) 0;
    newTsk.stackPtr = (unsigned long *)(kmalloc(STACK_SIZE) + STACK_SIZE - 4); //反向增长
    
    // 初始化栈
    stackInit(&(newTsk.stackPtr), tskBody); 

    //log：栈低指针出现错误，意外的修改了EMB

    memcpy((void*)start, &newTsk, sizeof(myTCB));
    tskEnqueue(&newQueue, (myTCB*)start); //插入新建队列

    return newTsk.tid;
}

// 启动任务
void new2ready(unsigned long tid){
    //在new队列删除，插入wait队列
    myTCB *tsk;
    tsk = tskRemove(&newQueue, tid);
    tsk->state = READY;
    tskEnqueue(&rdyQueue, tsk);
}

// 销毁任务，释放栈空间
void ready2terminal(unsigned long tid){
    //在ready队列删除，释放栈空间
    myTCB *tsk;
    tsk = tskRemove(&rdyQueue, tid);
    kfree((unsigned long)tsk->stackPtr);
}

//运行任务
void ready2running(){
    prevTsk = currentTsk;
    currentTsk = tskDequeue(&rdyQueue);
    currentTsk->state = RUNNING;
    contextSwitch(&(prevTsk->stackPtr), currentTsk->stackPtr);
    
}

// 释放TCB空间
void terminal2null(myTCB *tsk){
    //释放进程块本身
    kfree((unsigned long) tsk);
}

// 结束任务
void running2terminal(){
    prevTsk->state = TERMINAL;
    kfree((unsigned long)prevTsk->stackPtr);
}
