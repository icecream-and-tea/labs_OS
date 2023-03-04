#ifndef __VGA_H__
#define __VGA_H__

void append2screen(char *str, int color);
void clear_screen(void);
void showMessage(int pos_row, int pos_col, int color, char* str);
unsigned short int getMessage(int pos_row, int pos_col);

#endif
