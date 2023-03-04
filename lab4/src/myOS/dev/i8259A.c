#include "io.h"

#define PORT_M0 0x20
#define PORT_M1 0x21
#define PORT_S0 0xA0
#define PORT_S1 0xA1

void init8259A(void){
	//屏蔽中断源
	// outb(PORT_M1, 0XFF);
	// outb(PORT_S1, 0XFF);
	//初始化主片
	outb(PORT_M0, 0X11);
	outb(PORT_M1, 0X20);
	outb(PORT_M1, 0X04);
	outb(PORT_M1, 0X3);
	//初始化从片
	outb(PORT_S0, 0X11);
	outb(PORT_S1, 0X28);
	outb(PORT_S1, 0X02);
	outb(PORT_S1, 0X01);
}
