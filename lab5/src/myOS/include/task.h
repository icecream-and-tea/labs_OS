#ifndef __TASK_H__
#define __TASK_H__

#include "state.h"

# define STACK_SIZE 0x0300

// myTCB数据结构
typedef struct myTCB{
    unsigned long tid;
    unsigned long state;
    unsigned long *stackPtr;
    unsigned long *pc;

    struct myTCB *next;

    // struct tskPara{
    //     unsigned long arrTime;
    // };

}myTCB;

// 任务的四个原语
unsigned long createTsk(void (*tskBody)(void));
void destroyTsk(unsigned long tid);
void tskStart(unsigned long tid);
void tskEnd();

void schedule();

#endif