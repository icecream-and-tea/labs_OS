#include "../../include/mem.h"
#include "../../include/myPrintk.h"

unsigned long malloc(unsigned long size){
    //调用实现的dPartition或者是ePartition的malloc
    //return eFPartitionAlloc(pMemHandler);
    return dPartitionAlloc(pMemHandler,size);

}

unsigned long free(unsigned long start){

    //调用实现的dPartition或者是ePartition的free
    //return eFPartitionFree(pMemHandler, start);
    return dPartitionFree(pMemHandler,start);
}