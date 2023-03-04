#include "mystring.h"

void strzero(char *str, int len)
{
    for (int i = 0; i < len; i++)
        str[i] = '\0';
}

int strcmp(char *str1, char *str2)
{
    while ((*str1) && (*str1 == *str2))
    {
        str1++;
        str2++;
    }

    if (*(unsigned char *)str1 > *(unsigned char *)str2)
    {
        return 1;
    }
    else if (*(unsigned char *)str1 < *(unsigned char *)str2)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

char *strcpy(char *dst, char *src)
{
    char *str = dst;
    while ((*str++ = *src++) != '\0')
        ;
    return dst;
}

int strlen(char *str)
{
    int len = 0;
    while (*(str++) != '\0')
    {
        len++;
    }
    return len;
}

char *strend(char *str)
{
    while (*str != '\0')
        str++;
    return str;
}

char *extend(char *dst, char *src)
{
    while (*src != '\0')
        *(dst++) = *(src++);
    *dst = '\0';
    return dst;
}

char *strcat(char *dst, char *src)
{
    extend(strend(dst), src);
    return dst;
}

char *move(char *str, int n, char ch)
{
    int length = strlen(str);
    char *p = str + length;
    while (p >= str)
    {
        *(p + n) = *p;
        p--;
    }
    for (int i = 0; i < n; i++)
    {
        *(str + i) = ch;
    }
    return str;
}

char *fill(char *str, char ch, int width, int align, int relative)
{
    char *p;
    int count = relative ? width : width - strlen(str);
    if (count <= 0)
        return str;
    if (align == LEFT)
    {
        p = strend(str);
        while (count-- > 0)
            *(p++) = ch;
        *p = '\0';
    }
    else
    {
        move(str, count, ch);
    }
    return str;
}

char *remove(char *dst, char *src, char ch1, char ch2)
{
    char *p1, *p2 = dst;
    while (*src != ch2 && *src != '\0')
        *(dst++) = *(src++);
    p1 = src;
    while (*(--dst) == ch1)
        *dst = '\0';
    dst++;
    while (*p1 != '\0')
        *(dst++) = *(p1++);
    return p2;
}

int find(char *str, char ch)
{
    for (int i = 0; str[i] != '\0'; i++)
        if (str[i] == ch)
            return TRUE;
    return FALSE;
}

void *memcpy(void *dst, const void *src, unsigned n)
{
    char *dsts = (char *)dst;
    char *srcs = (char *)src;
    while (n--)
    {
        *dsts++ = *srcs++;
    }
    return dst;
}
