#include "vga.h"
#include "i8253.h"
#include "i8259A.h"
#include "tick.h"
#include "myPrintk.h"
#include "mem.h"

extern void tskManagerInit(void);

void osStart(void)
{
	init8259A();
	init8253();
	tick();
	enable_interrupt();
	
    clear_screen();

    pMemInit();
    
    myPrintk(0x2, "START RUNNING......\n");
    tskManagerInit();
    myPrintk(0x2, "STOP RUNNING......ShutDown\n");
    while (1);
}
