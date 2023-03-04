#include "wallClock.h"

typedef unsigned short int int16;

int col;

void (*wallClock_hook)(int, int, int) = 0;

void oneTickUpdateWallClock(int HH, int MM, int SS){
	if(wallClock_hook) wallClock_hook(HH,MM,SS);
}

//决定设置墙钟所用的函数
void setWallClockHook(void (*func)(int, int, int)) {
	wallClock_hook = func;
}

void setWallClock(int HH,int MM,int SS){
	//向VGA的右下角输出时钟
	//要将显示格式设置为00：00：00
	if(HH <= 9)
		showMsg(24, 71, 0x4, "0%d:",HH);
	else	
		showMsg(24, 71, 0x4, "%d:",HH);
	if(MM <= 9)
		showMsg(24, 74, 0x4, "0%d:",MM);
	else	
		showMsg(24, 74, 0x4, "%d:",MM);
	if(SS <= 9)
		showMsg(24, 77, 0x4, "0%d",SS);
	else	
		showMsg(24, 77, 0x4, "%d",SS);

}

//获得时间的一部分，如S
int getTimesegment()
{
	int16 ch;
	int n1, n2;
	ch = getMessage(24, col++);
	n1 = transChartoInt(ch);
	ch = getMessage(24, col++);
	n2 = transChartoInt(ch);
	return (n1*10 + n2);
}

//将从vga获得的信息转换为int
int transChartoInt(int16 ch)
{
	//取低8位
	char n = ch % 256;
	return (int) (n - '0');
}

//获得墙钟时间
void getWallClock(int *HH,int *MM,int *SS){
	//获取时钟
	col = 71;
	(*HH) = getTimesegment();
	col++;
	(*MM) = getTimesegment();
	col++;
	(*SS) = getTimesegment();
}


