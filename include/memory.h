#ifndef MEMORY_FUNCTION
#define MEMORY_FUNCTION
#include "types.h"

typedef u16 ioport_t;

void outb(u16 port, u8 value);
u8 inb(u16 port);
u16 inw(u16 port);
void memcpy(u8 *dest, const u8 *src, u32 len);
void memset(u8 *dest, u8 val, u32 len);
static inline void in32 (ioport_t port, u32 *data)
{ asm volatile ("inl %%dx, %%eax" : "=a" (*data) : "d" (port)); }

static inline void out32 (ioport_t port, u32 data)
{ asm volatile ("outl %%eax, %%dx" : : "a" (data), "d" (port)); }

#endif
