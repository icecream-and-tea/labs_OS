#include "../myOS/userInterface.h"   //interface from kernel

#include "shell.h"
#include "memTestCase.h"

//void mySetWallClock(int HH, int MM, intSS) //允许用户在userApp中重新定义SetWallClock

void myMain(void){    
    //可以在userApp中自定义新的mysetWallClock （机制与策略相分离）
    //并将setWallClockHook中的参数从setWallClock默认的内核策略修改成mySetWallClock用户定义的新策略
    setWallClockHook(setWallClock); 

    initShell();
    // myPrintf(0x7, "initshell successd!\n");
    memTestCaseInit();
    // myPrintf(0x7, "memtestcaseinit successd!\n");

    // 创建后立马启动
    tskStart( createTsk(startShell) );
    // myPrintf(0x7, "start shell!\n");
    
    tskEnd(); //main函数结束，shell开始
    return;
}


