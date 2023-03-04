#ifndef __UART_H__
#define __UART_H__

unsigned char uart_get_char(void);
void uart_put_char(unsigned char ch);
void uart_put_chars(char *str);

#endif