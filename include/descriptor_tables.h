#ifndef _IDT_ENTRY
#define _IDT_ENTRY
#include "types.h"



void initialize_descriptor_tables();

/* GDT table structure */
/* +----------------------------------------+----------------------------------------+ */
/* |32                                    16|15                                     0| */
/* +----------------------------------------+----------------------------------------+ */
/* |                                        |                                        | */
/* |              Base 0:15                 |               Limit 0:15               | */
/* |                                        |                                        | */
/* +-------------+----------+---------------+--------------------+-------------------+ */
/* |63         56|55      52|51           48|47                40|39               32| */
/* +-------------+----------+---------------+--------------------+-------------------+ */
/* |             |          |               |                    |                   | */
/* |  Base 24:31 |  Flags   |  Limit 16:19  |     Access Byte    |    Base 16:23     | */
/* |             |          |               |                    |                   | */
/* +-------------+----------+---------------+--------------------+-------------------+ */



/* This structute contain GDT entry table */
/* attribute packed create non alignment */
/* bite structure */
struct gdt_entry_struct
{
  u16 limit_low;
  u16 base_low;
  u8  base_middle;
  u8  access;
  u8  granularity;
  u8  base_high;
} __attribute__ ((packed));
typedef struct gdt_entry_struct gdt_entry_table;

struct gdt_ptr_struct
{
  u16 limit;
  u32 base;
} __attribute__((packed));
typedef struct gdt_ptr_struct gdt_pointer;


/* IDT table entry */
/* +----------------------------------------+----------------------------------------+ */
/* |32                                    16|15                                     0| */
/* +----------------------------------------+----------------------------------------+ */
/* |                                        |                                        | */
/* |             Selector 0:15              |               Base 0:15                | */
/* |                                        |                                        | */
/* +----------------------------------------+--------------------+-------------------+ */
/* |63                                    48|47                40|39               32| */
/* +----------------------------------------+--------------------+-------------------+ */
/* |                                        |                    |                   | */
/* |              Base 16:32                |        flags       |       zero        | */
/* |                                        |                    |                   | */
/* +----------------------------------------+--------------------+-------------------+ */

/* Selector of the interrupt function. If DPL has to be 0 so the 'iret' */
/* instruction won`t throw a GP exeption when executed. */
/* Selector is a code segment of IDT or GDT */

/* Flags */
/* +-------------------------------+ */
/* |7                             0| */
/* +---+-------+---+---------------+ */
/* | P |  DPL  | S |    GateType   | */
/* +---+-------+---+---------------+ */

/* P    [1]  - Present. Set 0 to unsed interrupt */
/* DPL  [2]  - Ring 0 - 3 */
/* S    [1]  - Storage segment (0 for interrupt and trap gates) */
/* Type [3]  - IDT gate type (  0b0101 - 32bit task gate */
/*                            , 0b0110 - 16-bit interrupt gate */
/*                            , 0b0111 - 16-bit trap gate */
/*                            , 0b1110 - 32-bit interrupt gate */
/*                            , 0b1111 - 32-bit trap gate) */
struct idt_entry_struct
{
  u16 base_low;
  u16 selector;
  u8  zero;
  u8  flags;
  u16 base_high;
} __attribute__ ((packed));
typedef struct idt_entry_struct idt_entry_table;

struct idt_ptr_struct
{
  u16 limit;
  u32 base;
} __attribute__ ((packed));
typedef struct idt_ptr_struct idt_pointer;


extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void irq0 ();
extern void irq1 ();
extern void irq2 ();
extern void irq3 ();
extern void irq4 ();
extern void irq5 ();
extern void irq6 ();
extern void irq7 ();
extern void irq8 ();
extern void irq9 ();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

#endif
