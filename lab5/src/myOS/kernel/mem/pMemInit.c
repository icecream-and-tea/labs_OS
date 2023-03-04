#include "../../include/myPrintk.h"
#include "../../include/mem.h"

#define int16 unsigned short int
unsigned long pMemStart;//可用的内存的起始地址
unsigned long pMemSize;//可用的大小


void memTest(unsigned long start, unsigned long grainSize){
	/*本函数需要实现！！！*/
	/*功能：检测算法
		这一个函数对应实验讲解ppt中的第一大功能-内存检测。
		本函数的功能是检测从某一个地址开始的内存是否可用，具体算法就可以用ppt上说的写了读看是否一致。
	注意点两个：
	1、开始的地址要大于1M，需要做一个if判断。
	2、grainsize不能太小，也要做一个if判断
	*/
	// 内存检测从1M以上开始
	int16* p = (int16*)start;
	int16 tmp;
	if(start < 0x100000)
		myPrintk(0x7, "start should > 1M\n");
	// grainsize 不能小于 512字节
	if(grainSize <= 0x200)
		myPrintk(0x7, "grainSize should > 512B");
	//开始检测
	do{
		tmp = *p;
		//检测前两个字节
		*p = 0xAA55;
		if(*p != 0xAA55){
			*p = tmp;
			break;
		}
		*p = 0x55AA;
		if(*p != 0x55AA){
			*p = tmp;
			break;
		}
		*p = tmp; // 还原数据

		//检测后两个字节
		
		p += ((grainSize >>1) - 1);

		tmp = *p;
		*p = 0xAA55; 
		if(*p != 0xAA55){
			*p = tmp;
			p -= ((grainSize >>1) - 1); //退出前要回退
			break;
		}
		*p = 0x55AA;
		if(*p != 0x55AA){
			*p = tmp;
			p -= ((grainSize >>1) - 1);
			break;
		}
		// 还原数据
		*p = tmp;
		p += 1;
	}while(1);
	// 设置全局变量
	pMemStart = start;
	pMemSize = (unsigned long)p -start;
	myPrintk(0x7,"MemStart: 0x%x  \n", pMemStart);
	myPrintk(0x7,"MemSize:  0x%x  \n", pMemSize);
	
}

extern unsigned long _end;
void pMemInit(void){
	unsigned long kernelSpaceSize = 0x50000;

	unsigned long _end_addr = (unsigned long) &_end;
	memTest(0x100000,0x1000);
	myPrintk(0x7,"_end:  0x%x  \n", _end_addr);
	if (pMemStart <= _end_addr) {
		pMemSize -= _end_addr - pMemStart;
		pMemStart = _end_addr;
	}
	//先从简单的等大小测试，块大小设置为4k
	//pMemHandler = eFPartitionInit(pMemStart, 0x1000, pMemSize/(0x1000) - 1);

	//进行动态内存管理测试
	//内核态和用户态
	kpMemHandler = dPartitionInit(pMemStart, kernelSpaceSize); 
	pMemHandler  = dPartitionInit(pMemStart + kernelSpaceSize, pMemSize - kernelSpaceSize);	

}
