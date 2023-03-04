#include "mymath.h"

static const double power_of_ten[] = {1e-15, 1e-14, 1e-13, 1e-12, 1e-11, 1e-10,
                                      1e-9, 1e-8, 1e-7, 1e-6, 1e-5, 1e-4,
                                      1e-3, 1e-2, 1e-1, 1e0, 1e1, 1e2,
                                      1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9,
                                      1e10, 1e11, 1e12, 1e13, 1e14};

int logarithm(int base, double num)
{
    int count = 0;
    num = num > 0 ? num : -num;
    if (num == 0)
    {
        return 0;
    }
    while (num >= base)
    {
        num /= base;
        count++;
    }
    while (num < 1)
    {
        num *= base;
        count--;
    }
    return count;
}

double power(int base, int n)
{
    double num = 1;
    int flag = 1;
    if (base == 10)
    {
        if (-15 <= n && n <= 15)
            return power_of_ten[n + 15];
        if (n > 15)
        {
            num = power_of_ten[29];
            while ((n--) - 15 >= 0)
            {
                num *= 10;
            }
        }
        else
        {
            num = power_of_ten[0];
            while ((n++) + 15 < 0)
            {
                num /= 10;
            }
        }
    }
    else
    {
        if (n < 0)
        {
            flag = 0;
            n = -n;
        }
        while ((n--) > 0)
        {
            num *= base;
        }
        if (flag == 0)
        {
            num = 1 / num;
        }
    }
    return num;
}

double rounding(double num, int n)
{
    int sign = num > 0 ? 1 : -1;
    return sign * (sign * num + 5 * power(10, -n - 1));
}
