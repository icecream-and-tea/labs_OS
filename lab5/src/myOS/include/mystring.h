#ifndef __STRING_H__
#define __STRING_H__

#define NULL ((void *)0)

#define TRUE 1
#define FALSE 0
#define LEFT 1
#define RIGHT 0

void strzero(char *str, int len);
int strcmp(char *str1, char *str2);
char *strcpy(char *dst, char *src);
int strlen(char *str);
char *strend(char *str);
char *extend(char *dst, char *src);
char *strcat(char *dst, char *src);
char *move(char *str, int n, char ch);
char *fill(char *str, char ch, int width, int align, int relative);
char *remove(char *dst, char *src, char ch1, char ch2);
int find(char *str, char ch);
void *memcpy(void *dst, const void *src, unsigned n);

#endif