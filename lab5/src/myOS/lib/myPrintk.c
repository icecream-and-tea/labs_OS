#include "vga.h"
#include "uart.h"
#include "vsprintf.h"

/* 内核 print 函数*/
char kBuf[400];
int myPrintk(int color, const char *format, ...) {
    va_list args;
    
    va_start(args, format);
    int cnt = vsprintf(kBuf, format, args);
    va_end(args);
    kBuf[cnt] = '\0';
    /*利用串口进行输出*/
    uart_put_chars(kBuf);
    /*利用VGA进行输出*/
    append2screen(kBuf,color);
    return cnt;
}

/* 用户 print 函数 */
char uBuf[400];
int myPrintf(int color, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int cnt = vsprintf(uBuf, format, args);
    uBuf[cnt] = '\0';
    va_end(args);

    /*利用串口进行输出*/
    uart_put_chars(uBuf);
    /*利用VGA进行输出*/
    append2screen(uBuf,color);
    return cnt;
}

/*特定位置显示字符*/
char msgBuf[200];
int showMsg(int pos_row, int pos_col, int color, const char *format, ...){
    va_list args;
    va_start(args, format);
    int cnt = vsprintf(msgBuf, format, args);
    msgBuf[cnt] = '\0';
    va_end(args);
    //vga中真正实现
    showMessage(pos_row, pos_col, color, msgBuf);
    return cnt;
}
