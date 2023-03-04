.globl start
/*此处，按照multiboot协议要求定义必要的参数*/
/*格式：XXX_ITEM_NAME=value*/
/*此处开始，按协议标准来定义必须的multiboot header*/
        /* magic */
magic_ITEM_NAME = 0x1BADB002
        /*flags*/
flags_ITEM_NAME = 0x00000000
        /*checksum*/
checksum_ITEM_NAME = -0x1BADB002

.section ".multiboot_header"
/*使用.long和前面定义的参数构建头结构，每次32位，格式为：.long XXX_ITEM_NAME*/
.align 4
.long magic_ITEM_NAME
.long flags_ITEM_NAME
.long checksum_ITEM_NAME

.text /*进入代码段*/
.code32 /*32位代码*/
        start: /*与第一行的声明要一致*/
        /*VGA输出*/
        movl $0x2f652f68, 0xB8000 # he
        movl $0x2f6c2f6c, 0xB8004 # ll
        movl $0x2f2c2f6f, 0xB8008 # o,
        movl $0x2f422f50, 0xB800C # PB
        movl $0x2f302f32, 0xB8010 # 20
        movl $0x2f372f30, 0xB8014 # 07
        movl $0x2f342f31, 0xB8018 # 14
        movl $0x2f362f34, 0xB801C # 46
        movl $0x2f7a2f5f, 0xB8020 # _z
        movl $0x2f612f68, 0xB8024 # ha
        movl $0x2f712f6f, 0xB8028 # oq
        movl $0x2f6e2f69, 0xB802C # in
        movl $0x2f692f6c, 0xB8030 # li
        movw $0x2f6e, 0xB8034 # n
        nop
        /*串口输出*/
        movb $0x7a, %al 
        movw $0x3F8, %dx
        outb %al, %dx  # putchar z
        movb $0x71, %al
        outb %al, %dx  # putchar q
        movb $0x6c, %al
        outb %al, %dx  # putchar l
        nop
        /*实验结束，使用hlt指令使计算机停机*/
        hlt

/*使用qemu-system-i386 -kernel multibootHeader.bin -serial stdio；来进行运行*/