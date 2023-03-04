#include "wallClock.h"
#include "tick.h"

// 宏参来代表HMS分别需要多少tick
#define H_TICKNUM 360000
#define M_TICKNUM 6000
#define S_TICKNUM 100
int system_ticks = 0;
int HH,MM,SS;

/*由 i8253 引起的时钟中断而引起的中断子程序 
time_interrupt 处理中调用tick 函数*/

void tick(void){
	int temp1, temp2;
	system_ticks++;
	//对HH,MM,SS进行处理
	if(system_ticks == 24 * H_TICKNUM)
		system_ticks = 0;
	HH = system_ticks / H_TICKNUM;
	temp1 = system_ticks % H_TICKNUM;
	MM = temp1 / M_TICKNUM;
	temp2 = temp1 % M_TICKNUM;
	SS = temp2 / S_TICKNUM;
	oneTickUpdateWallClock(HH,MM,SS);
	return;
}

