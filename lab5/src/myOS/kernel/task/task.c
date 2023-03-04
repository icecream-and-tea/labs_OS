#include "../../include/task.h"
#include "../../include/myPrintk.h"

extern Queue rdyQueue;
extern int FCFS(void);

/*-----------------创建-> 启动 -> 终止 -> 销毁---------------------*/
// 都是接口函数

// 调度函数
void schedule(){
    if(queueEmpty(&rdyQueue)){
        // 手动启用idle任务
        tskStart(1); // idle的tid为1
        schedule();
    }
    else{
        FCFS();
    } 
}

// 任务创建
unsigned long createTsk(void (*tskBody)(void)){
    null2new(tskBody);
}

// 任务销毁
void destroyTsk(unsigned long tid){
    ready2terminal(tid);
}

// 任务启动
void tskStart(unsigned long tid){
    new2ready(tid);
}

// 任务结束
void tskEnd(){
    schedule(); // 为接口，使用不同的调度策略
    //running2terminal(); // 无法释放进程栈
}