#ifndef   _PRINTF_H
#define   _PRINTF_H

int printf (const char *format, ...);

#include "types.h"
#define PANIC(msg) panic(msg, __FILE__, __LINE__);

extern void panic(const char *message, const char *file, u32 line);
#endif
