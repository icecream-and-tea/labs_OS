#include "../myOS/userInterface.h"   //interface from kernel
#include "shell.h"

extern  Queue rdyQueue;

extern void RRTestCase(void);

//void mySetWallClock(int HH, int MM, intSS) //允许用户在userApp中重新定义SetWallClock

int whichScheduler(void){
    return RR;
}

void myMain(void){  

    setWallClockHook(setWallClock); 

    RRTestCase();

    struct tskPara tmp;
    setTskPara(PRIORITY, 0, &tmp);
    setTskPara(EXETIME, 1000, &tmp);
    setTskPara(ARRTIME, 30, &tmp);
    createTsk(startShell, &tmp);
    initShell();
    tskEnd(); //main函数结束，shell开始
    return;
}