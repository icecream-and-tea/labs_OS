#include "io.h"
#include "vga.h"

#define VGA_BASE 0xB8000 // vga 显存起始地址
#define VGA_SCREEN_WIDTH 80 // vga 屏幕宽度（可容纳字符数）
#define VGA_SCREEN_HEIGHT 25 // vga 屏幕高度

#define CURSOR_LINE_REG 0xE // 行号寄存器
#define CURSOR_COL_REG 0xF // 列号寄存器
#define CURSOR_INDEX_PORT 0x3D4 // 光标行列索引端口号
#define CURSOR_DATA_PORT 0x3D5 // 光标数据端口号

typedef unsigned short int int16;

//vga properties
struct VGA{
    int16* head;
    int16* tail;
    int16 row;
    int16 column;
    int16* cursor;
}vga = {(int16*)VGA_BASE, (int16*)(VGA_BASE + 2 * VGA_SCREEN_WIDTH * VGA_SCREEN_HEIGHT), (int16)VGA_SCREEN_WIDTH , (int16)VGA_SCREEN_HEIGHT, (int16*)VGA_BASE };


/* 将光标设定到特定位置*/
void set_cursor_pos(void) {
    //实现思路：用outb将行列分时送入
    int16 pos = vga.cursor - vga.head;
    unsigned char line_pos, col_pos;
    line_pos = pos / 256; //高8位是行数
    col_pos = pos % 256;  //低8位是列数
    //设置行
    outb(CURSOR_INDEX_PORT, CURSOR_LINE_REG);
    outb(CURSOR_DATA_PORT, line_pos);
    //设置列
    outb(CURSOR_INDEX_PORT, CURSOR_COL_REG);
    outb(CURSOR_DATA_PORT, col_pos);
}

/* 获取光标当前所在位置 */ 
int16 get_cursor_pos(void) {
    int16 pos;
    unsigned char line_pos, col_pos;
    //读取行
    outb(CURSOR_INDEX_PORT, CURSOR_LINE_REG);
    line_pos = inb(CURSOR_LINE_REG);
    //读取列
    outb(CURSOR_INDEX_PORT, CURSOR_COL_REG);
    col_pos = inb(CURSOR_COL_REG);
    pos = line_pos << 8 + col_pos;
    return pos;

}

/* 滚屏，vga 屏幕满时使用。丢弃第一行内容，将剩余行整体向上滚动一行
*要保留最后一行显示时钟和其他内容*/
void scroll_screen(void) {
    int i;
    int16* p = vga.head;
    //move data
    for(; p < vga.tail - 2* vga.row; p ++){ 
        *p = *(p + vga.row); 
    }
    //set last row
    for(; p < vga.tail - vga.row; p++){
        *p = 0x0700;
    }
    //set cursor to outset of last row
    vga.cursor = vga.tail - 2 * vga.row;
    set_cursor_pos();
}

/* 向 vga 的特定光标位置 pos 输出一个字符*/
void put_char2pos(unsigned char c, int color) {
    if(c == '\n'){
        //set the cursor to the outset of next row
        vga.cursor = vga.head + ((vga.cursor - vga.head)/vga.row + 1) * vga.row;
    }
    else{
        *(vga.cursor) = (int16)(color<<8 | c);
        vga.cursor ++;
    }
    //full screen
    if(vga.cursor >= vga.tail - vga.row){
        scroll_screen();
    }
}

/* 清除屏幕上所有字符，并将光标位置重置到顶格 */
void clear_screen(void) {
    int16* p;
    for(p = vga.head; p < vga.tail; p++){
        *p = 0x0700;
    }
    //reset the cursor
    vga.cursor = vga.head;
    set_cursor_pos();
}

/* 向 vga 的当前光标位置输出一个字符串，并移动光标位置到串末尾字符的下一位 */
void append2screen(char *str, int color) { 
    int i = 0;
    while(str[i] != '\0'){
        put_char2pos(str[i++], color);
        //update cursor
        set_cursor_pos();
    }
}

/*向VGA 指定位置输出信息，如时钟等 */
void showMessage(int pos_row, int pos_col, int color, char* str)
{
    int i = 0;
    int16* p = vga.head + (pos_row * vga.row + pos_col);
    while(str[i] != '\0')
    {
        *(p) = (int16)(color<<8 | str[i]);
        p++;
        i++;
    }
}

int16 getMessage(int pos_row, int pos_col)
{
    int16* p = vga.head + (pos_row * vga.row + pos_col);
    return (*p);
}