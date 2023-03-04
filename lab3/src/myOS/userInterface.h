#ifndef __USERINTERFACE_H__
#define __USERINTERFACE_H__

/* 内核提供给用户程序的函数接口 */

int myPrintf(int color, const char *format, ...);
int myPrintk(int color, const char *format, ...);

#endif
