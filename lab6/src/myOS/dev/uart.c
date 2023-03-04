#include "io.h"
#include "uart.h"

#define UART_PORT 0x3F8 // 串口端口号

unsigned char uart_get_char(void)
{
	while (!(inb(UART_PORT+5)&1));
	return inb(UART_PORT);
}

/* 向串口输出一个字符
 * 使用封装好的 outb 函数 */
void uart_put_char(unsigned char ch) {
    if(ch == '\n'){
        outb(UART_PORT, '\r');
        outb(UART_PORT, '\n');
    } 
    else{
        outb(UART_PORT,ch); 
    } 
}

/* 向串口输出一个字符串
 * 此函数接口禁止修改 */
void uart_put_chars(char *str) {
    int i;
    for(i = 0; str[i] != '\0'; i++){
        uart_put_char(str[i]);
    }
}
