#ifndef __STATE_H__
#define __STATE_H__


#include "./taskQueue.h"
#include "./kmalloc.h"
#include "./mystring.h"
#include "./task.h"

# define NEW 0
# define READY 1
# define WAITING 2
# define TERMINAL 3
# define RUNNING 4


unsigned long **prevTSK_StackPtrAddr; 
unsigned long *nextTSK_StackPtr; 

void contextSwitch(unsigned long **prevTskStkAddr, unsigned long *nextTskStk);
unsigned long getTid();
unsigned long null2new(void (*tskBody)(void));
void new2ready(unsigned long tid);
void ready2terminal(unsigned long tid);
void ready2running();
void running2terminal();

#endif