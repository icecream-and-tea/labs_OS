#ifndef __WALLCLOCK_H__
#define __WALLCLOCK_H__

#include "myPrintk.h"
#include "vga.h"
typedef unsigned short int int16;
void setWallClock(int HH,int MM,int SS);
void getWallClock(int *HH,int *MM,int *SS);
void oneTickUpdateWallClock(int HH, int MM, int SS);
void setWallClockHook(void (*func)(int, int, int));
int getTimesegment();
int transChartoInt(int16 ch);

#endif
