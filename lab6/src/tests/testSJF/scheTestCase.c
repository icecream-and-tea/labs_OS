#include "scheTestCase.h"

extern int system_ticks; // one tick : 1ms 

void sleep(int time){
    //沉睡几毫秒
    int start = system_ticks;
    while(system_ticks - start < time);
}

void test1(){
    for(int i = 0; i < 10; i++){
        myPrintf(0x1, "test1---------->loop:%d\n", i);
        sleep(20);
    }
    tskEnd();
}

void test2(){
    for(int i = 0; i < 10; i++){
        myPrintf(0x2, "test2---------->loop:%d\n", i);
        sleep(20);
    }
    tskEnd();
}

void test3(){
    for(int i = 0; i < 10; i++){
        myPrintf(0x3, "test3---------->loop:%d\n", i);
        sleep(20);
    }
    tskEnd();
}

void test4(){
    for(int i = 0; i < 10; i++){
        myPrintf(0x4, "test4---------->loop:%d\n", i);
        sleep(20);
    }
    tskEnd();
}