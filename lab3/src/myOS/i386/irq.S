.globl enable_interrupt
.globl disable_interruput

enable_interrupt:
    sti 
    ret

    
disable_interruput:
    cli
    ret