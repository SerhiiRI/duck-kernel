#ifndef MEMORY_FUNCTION
#define MEMORY_FUNCTION
#include "types.h"

void outb(u16 port, u8 value);
u8 inb(u16 port);
u16 inw(u16 port);
void memcpy(u8 *dest, const u8 *src, u32 len);
void memset(u8 *dest, u8 val, u32 len);

#endif
