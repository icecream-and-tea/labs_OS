#include "scheTestCase.h"

void FCFSTestCase(void){
    tskPara para[4];

    setTskPara(PRIORITY, 0, &para[0]);
    setTskPara(EXETIME, 1000, &para[0]);
    setTskPara(ARRTIME, 5, &para[0]);
    
    setTskPara(PRIORITY, 0, &para[1]);
    setTskPara(EXETIME, 1000, &para[1]);
    setTskPara(ARRTIME, 10, &para[1]);
    
    setTskPara(PRIORITY, 0, &para[2]);
    setTskPara(EXETIME, 1000, &para[2]);
    setTskPara(ARRTIME, 15, &para[2]);
    
    setTskPara(PRIORITY, 0, &para[3]);
    setTskPara(EXETIME, 1000, &para[3]);
    setTskPara(ARRTIME, 20, &para[3]);

    createTsk(test1, &para[0]);
    createTsk(test2, &para[1]);
    createTsk(test3, &para[2]);
    createTsk(test4, &para[3]);

}