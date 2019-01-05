#ifndef ISR_REGISTERS
#define ISR_REGISTERS
#include "types.h"

// A few defines to make life a little easier
#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

struct registers
{
  u32 DS;
  u32 EDI, ESI, EBP, ESP, EBX, EDX, ECX, EAX;
  u32 interruption_number, error_code;
  u32 EIP, CS, eflags, userESP, SS;
};
typedef struct registers registers_table;


typedef void (*isr_t)(registers_t);
void register_interrupt_handler(u8 n, isr_t handler);

#endif
