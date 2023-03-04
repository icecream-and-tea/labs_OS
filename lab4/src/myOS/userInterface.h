#ifndef __USERINTERFACE_H__
#define __USERINTERFACE_H__

//general for both kernel and userApp
#include "include/mem.h"    
#include "include/uart.h"
#include "include/vga.h"
#include "include/wallClock.h"
#include "include/vsprintf.h"
#include "include/mystring.h"

//only for userApp
#include "include/malloc.h"
#include "include/kmalloc.h"

int myPrintf(int color,const char *format, ...);

#endif
