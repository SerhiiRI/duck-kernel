#ifndef   _PRINTF_H
#define   _PRINTF_H

int printf (const char *format, ...);

#include "types.h"

#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

extern void panic(const char *message, const char *file, u32 line);
extern void panic_assert(const char *file, u32 line, const char *desc);
#endif
