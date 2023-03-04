#include "irqs.h" 

void ignoreIntBody(void){
    char* str;
    char msg[100] = "Unknown interrupt1\0";
    str = msg;
    showMsg(24, 0, 0x4, str);
    //showMessage(24, 0, 0x4, str);
}
