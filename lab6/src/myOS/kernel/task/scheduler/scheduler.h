#include "../../../include/task.h"

#define FCFS 1
#define RR 2
#define SJF 3

struct scheduler {
    unsigned int type;
    
    void (*nextTsk_func)(void);
    void (*enqueueTsk_func)(Queue *queue, myTCB *tsk);
    void (*tick_hook)(void); //if set, tick_hook will be called every tick
};

void nextTsk(void);
void tskEnqueue_sche(Queue *queue, myTCB *tsk); // 只有这一个需要挂钩
void scheduler_tick(void); // run every tick
void schedule(void); //对外的统一调度接口

// 用户接口，设置不同的调度器
void setSysScheduler(unsigned int what);




