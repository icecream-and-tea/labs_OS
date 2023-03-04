#ifndef __TSKPARA__
#define __TSKPARA__

#define PRIORITY 1 
#define EXETIME 2 
#define ARRTIME 3

typedef struct tskPara { 
    unsigned long priority; 
    unsigned long arrTime; 
    unsigned long exeTime; 
} tskPara;

void initTskPara(tskPara *buffer);
void setTskPara(unsigned int option, unsigned int value, tskPara *buffer); 
void getTskPara(unsigned int option, unsigned int *para);

#endif