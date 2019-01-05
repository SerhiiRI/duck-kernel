#include "types.h"

void outb(u16 port, u8 value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

u8 inb(u16 port)
{
    u8 ret;
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

u16 inw(u16 port)
{
    u16 ret;
    asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

// Copy len bytes from src to dest.
void memcpy(u8 *dest, const u8 *src, u32 len)
{
    const u8 *sp = (const u8 *)src;
    u8 *dp = (u8 *)dest;
    for(; len != 0; len--) *dp++ = *sp++;
}

// Write len copies of val into dest.
void memset(u8 *dest, u8 val, u32 len)
{
    u8 *temp = (u8 *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}


