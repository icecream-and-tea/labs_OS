#ifndef __TASK_H__
#define __TASK_H__

#include "./taskQueue.h"
#include "./kmalloc.h"
#include "./mystring.h"
#include "./tskPara.h"

# define STACK_SIZE 0x0500
# define NEW 0
# define READY 1
# define WAITING 2
# define TERMINAL 3
# define RUNNING 4

// myTCB数据结构
typedef struct myTCB{
    //base_para
    unsigned long tid;
    unsigned long state;
    unsigned long *stackPtr;
    unsigned long *pc;

    struct myTCB *next;

    //expand_para
    unsigned long priority; 
    unsigned long arrTime; 
    unsigned long exeTime;
}myTCB;

unsigned long **prevTSK_StackPtrAddr; 
unsigned long *nextTSK_StackPtr; 

// 任务各种操作的具体实现
void contextSwitch(unsigned long **prevTskStkAddr, unsigned long *nextTskStk);
void setNextTsk(unsigned long *nextTskStk);

unsigned long getTid();
unsigned long null2new(void (*tskBody)(void), tskPara* para);
void new2ready(unsigned long tid);
void ready2terminal(unsigned long tid);
void ready2running();
void running2ready();
void running2terminal();
void tskDynArr(unsigned long sysTime);

// 任务的四个原语
unsigned long createTsk(void (*tskBody)(void), tskPara* para);
void destroyTsk(unsigned long tid);
void tskStart(unsigned long tid);
void tskEnd();

#endif