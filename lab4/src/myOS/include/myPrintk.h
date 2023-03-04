#ifndef __MY_PRINTK_H__
#define __MY_PRINTK_H__

int myPrintk(int color, const char* format, ...);
int myPrintf(int color, const char* format, ...);
int showMsg(int pos_row, int pos_col, int color, const char *format, ...);

#endif
