.text 
.code32

.global prevTSK_StackPtrAddr
.global nextTSK_StackPtr

.global CTX_SW
.global CTX_NEXT

CTX_SW: 
    cli
    # 保护现场
    pushf                                     # 旧进程的标志寄存器入栈
    pusha                                     # 旧进程的通用寄存器入栈
    movl prevTSK_StackPtrAddr,%eax            # eax是指针
    movl %esp, (%eax)                         # () 是访存的标志
    movl nextTSK_StackPtr, %esp               # 新进程的栈顶指针值存入 esp 寄存器
    # 恢复现场
    popa                                      # 新进程的通用寄存器出栈
    popf                                      # 新进程的标志寄存器出栈
    sti
    ret

CTX_NEXT:
    cli
    movl nextTSK_StackPtr, %esp               # 新进程的栈顶指针值存入 esp 寄存器
    # 恢复现场
    popa                                      # 新进程的通用寄存器出栈
    popf                                      # 新进程的标志寄存器出栈
    sti
    ret