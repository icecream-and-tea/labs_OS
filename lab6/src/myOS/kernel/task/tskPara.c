#include "../../include/tskPara.h"
#include "../../include/myPrintk.h"

void initTskPara(tskPara *buffer){
    buffer->priority = 0;
    buffer->arrTime = 0;
    buffer->exeTime = 0;
}

void setTskPara(unsigned int option, unsigned int value, tskPara *buffer){
    switch (option)
    {
    case PRIORITY:
        buffer->priority = value;
        break;
    case ARRTIME:
        buffer->arrTime = value;
        break;
    case EXETIME:
        buffer->exeTime = value;
        break;
    default:
        myPrintf(0x7, "option error !!!\n");
        break;
    }
}

void getTskPara(unsigned int option, unsigned int *para){
    // switch (option)
    // {
    // case PRIORITY:
    //     buffer->priority = value;
    //     break;
    // case ARRTIME:
    //     buffer->arrTime = value;
    // case EXETIME:
    //     buffer->exeTime = value;
    // default:
    //     myPrintf(0x7, "option error !!!\n");
    //     break;
    // }
}