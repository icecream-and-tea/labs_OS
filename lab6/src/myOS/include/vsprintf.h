#ifndef __VSPRINTF_H__
#define __VSPRINTF_H__

#include <stdarg.h>
#include "mystring.h"
#include "mymath.h"
int vsprintf(char *buf, const char *fmt, va_list args);

#endif
