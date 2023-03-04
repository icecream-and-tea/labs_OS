#include "vsprintf.h"

#define TRUE 1
#define FALSE 0
#define LEFT 1
#define RIGHT 0

static char upper_case[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static char lower_case[] = "0123456789abcdefghijklmnopqrstuvwxyz";

char *dstr(char *str, long num, int radix, int precision, char *prefix, int sign, char upper)
{
    // If sign == -2, num represents an unsigned type.
    char *p = str, *letter = upper ? upper_case : lower_case;
    long *pt = &num;
    int n;
    if (num == 0)
    {
        n = 1;
    }
    else
    {
        if (sign != -2)
            n = logarithm(radix, *pt) + 1;
        else
            n = logarithm(radix, (unsigned long)*pt) + 1;
    }

    // prefix
    p = extend(p, prefix);

    // sign
    if (sign != -2)
    {
        if (sign || *pt < 0)
            *(p++) = *pt < 0 ? '-' : '+';
        *pt = *pt > 0 ? *pt : -*pt;
    }

    // precision
    for (int i = precision - n; i > 0; i--)
    {
        *(p++) = '0';
    }

    // int to str
    for (int i = n; i > 0; i--)
    {
        if (sign != -2)
        {
            *(p++) = letter[(int)(*pt / power(radix, i - 1))];
            *pt %= (long)power(radix, i - 1);
        }
        else
        {
            *(p++) = letter[(int)((unsigned long)*pt / power(radix, i - 1))];
            *pt %= (unsigned long)power(radix, i - 1);
        }
    }

    *p = '\0';
    return str;
}

char *ustr(char *str, unsigned long num, int radix, int precision, char *prefix, int sign, char upper)
{
    return dstr(str, num, radix, precision, prefix, -2, upper);
}

char *fstr(char *str, double num, int radix, int precision, char *prefix, int sign, char upper)
{
    // precision: keep n decimal places.
    // if precision == -1, do not output decimal point.
    char tmp[20];
    char *p = str;
    int symbol = num > 0 ? 1 : -1;
    int n = logarithm(radix, num);
    num *= symbol;

    // precision
    num = rounding(num, precision > 0 ? precision : 0);

    // prefix
    p = extend(p, prefix);

    // sign
    if (sign || symbol < 0)
        *(p++) = symbol < 0 ? '-' : '+';

    // integral part to str
    if (n < 0)
        *(p++) = '0';
    for (int i = n; i >= 0; i--)
    {
        *(p++) = (int)(num / power(radix, i)) + '0';
        num -= (*(p - 1) - '0') * power(radix, i);
    }

    // decimal point to str
    if (precision >= 0)
        *(p++) = '.';

    // fractional part to str
    while ((precision--) > 0)
    {
        num *= 10;
        *(p++) = (int)num + '0';
        num -= (int)num;
    }

    *p = '\0';
    return str;
}

char *estr(char *str, double num, int radix, int precision, char *prefix, int sign, char upper)
{
    // precision: keep n decimal places.
    // if precision == -1, do not output decimal point.
    char tmp[20];
    char *p = str, *letter = (char *)(upper > 0 ? "E" : "e");
    int n = logarithm(radix, num);
    num /= power(radix, n);

    p = extend(p, fstr(tmp, num, radix, precision, prefix, sign, upper));
    p = extend(p, dstr(tmp, n, radix, 3, letter, TRUE, TRUE));

    *p = '\0';
    return str;
}

char *gstr(char *str, double num, int radix, int precision, char *prefix, int sign, char upper)
{
    int n = logarithm(radix, num);

    if (n < -4 || n > precision - 1)
        return estr(str, num, radix, precision == -1 ? -1 : (precision - 1 > 0 ? precision - 1 : 0), prefix, sign, upper);
    else
        return fstr(str, num, radix, precision == -1 ? -1 : (precision - (n + 1) > 0 ? precision - (n + 1) : 0), prefix, sign, upper);
}

char *cstr(char *str, char ch)
{
    str[0] = ch;
    str[1] = '\0';
    return str;
}

char *sstr(char *str, char *src)
{
    return strcpy(str, src);
}

int vsprintf(char *buf, const char *fmt, va_list args)
{
    char *str = buf;
    char tmp[100], tmp2[100], *p;

    char flags, length, type;
    int width, prec, have_prec;
    char _flags[] = "-+#0 ", _length[] = "hl", _digit[] = "0123456789";

    long lnum;
    double fnum;
    char ch;
    char *string;

    while (*fmt != '\0')
    {
        if (*fmt != '%')
            *(str++) = *(fmt++);
        else
        {
            // %
            if (*(++fmt) == '%')
            {
                *str++ = *fmt++;
                continue;
            }
            // flags
            flags = '\0';
            if (find(_flags, *fmt))
            {
                flags = *fmt;
                fmt++;
            }
            // width
            width = 0;
            while (find(_digit, *fmt))
            {
                width = width * 10 + *fmt - '0';
                fmt++;
            }
            // prec
            prec = 0, have_prec = FALSE;
            if (*fmt == '.')
            {
                fmt++;
                have_prec = TRUE;
                while (find(_digit, *fmt))
                {
                    prec = prec * 10 + *fmt - '0';
                    fmt++;
                }
            }
            // length
            length = '\0';
            if (find(_length, *fmt))
            {
                length = *fmt;
                fmt++;
            }
            switch (type = *fmt)
            {
            case 'i':
            case 'd':
                if (length == '\0' || length == 'h')
                    lnum = va_arg(args, int);
                else
                    lnum = va_arg(args, long);
                p = dstr(tmp, lnum, 10, prec, (char *)"", flags == '+' ? TRUE : FALSE, TRUE);
                break;
            case 'o':
                lnum = va_arg(args, unsigned long);
                p = ustr(tmp, lnum, 8, prec, (char *)(flags == '#' ? "0" : ""), FALSE, TRUE);
                break;
            case 'u':
                lnum = va_arg(args, unsigned long);
                p = ustr(tmp, lnum, 10, prec, (char *)"", FALSE, TRUE);
                break;
            case 'x':
                lnum = va_arg(args, unsigned long);
                p = ustr(tmp, lnum, 16, prec, (char *)(flags == '#' ? "0x" : ""), FALSE, FALSE);
                break;
            case 'X':
                lnum = va_arg(args, unsigned long);
                p = ustr(tmp, lnum, 16, prec, (char *)(flags == '#' ? "0X" : ""), FALSE, TRUE);
                break;
            case 'p':
                lnum = (unsigned long)va_arg(args, void *);
                p = ustr(tmp, lnum, 16, 16, (char *)"", FALSE, TRUE);
                break;
            case 'f':
                prec = have_prec ? prec : 6;
                prec = prec == 0 ? (flags == '#' ? 0 : -1) : prec;
                fnum = va_arg(args, double);
                p = fstr(tmp, fnum, 10, prec, (char *)"", flags == '+' ? TRUE : FALSE, TRUE);
                break;
            case 'e':
                prec = have_prec ? prec : 6;
                prec = prec == 0 ? (flags == '#' ? 0 : -1) : prec;
                fnum = va_arg(args, double);
                p = estr(tmp, fnum, 10, prec, (char *)"", flags == '+' ? TRUE : FALSE, FALSE);
                break;
            case 'E':
                prec = have_prec ? prec : 6;
                prec = prec == 0 ? (flags == '#' ? 0 : -1) : prec;
                fnum = va_arg(args, double);
                p = estr(tmp, fnum, 10, prec, (char *)"", flags == '+' ? TRUE : FALSE, TRUE);
                break;
            case 'g':
                prec = have_prec ? prec : 6;
                prec = prec == 0 ? (flags == '#' ? 0 : -1) : prec;
                fnum = va_arg(args, double);
                p = gstr(tmp, fnum, 10, prec, (char *)"", flags == '+' ? TRUE : FALSE, FALSE);
                strzero(tmp2, 100);
                if (flags != '#')
                {
                    p = remove(tmp2, p, '0', 'e');
                    p = remove(tmp2, p, '.', 'e');
                }
                break;
            case 'G':
                prec = have_prec ? prec : 6;
                prec = prec == 0 ? (flags == '#' ? 0 : -1) : prec;
                fnum = va_arg(args, double);
                p = gstr(tmp, fnum, 10, prec, (char *)"", flags == '+' ? TRUE : FALSE, TRUE);
                strzero(tmp2, 100);
                if (flags != '#')
                {
                    p = remove(tmp2, p, '0', 'E');
                    p = remove(tmp2, p, '.', 'E');
                }
                break;
            case 'c':
                ch = (unsigned char)va_arg(args, int);
                p = cstr(tmp, ch);
                break;
            case 's':
                string = va_arg(args, char *);
                p = sstr(tmp, string);
                break;
            default:
                p = (char *)"";
                break;
            }
            p = fill(p, flags == '0' ? '0' : ' ', width, flags == '-' ? LEFT : RIGHT, FALSE);
            if (flags == ' ' && (type == 'i' || type == 'd') && *p != '-')
                p = fill(p, ' ', 1, RIGHT, TRUE);
            str = extend(str, p);
            fmt++;
        }
    }
    return str - buf;
}

int sprintf(char *buf, const char *fmt, ...)
{
    int n;
    va_list args;
    va_start(args, fmt);
    n = vsprintf(buf, fmt, args);
    va_end(args);
    return n;
}

/*=================code end==============================*/
