#include "printf.h"
#include "screen.h"

#include "types.h"
#include "descriptor_tables.h"
#include "memory.h"
#include "isr.h"

extern isr_t interrupt_handlers[];

extern void idt_flush(u32);
extern void gdt_flush(u32);

static void init_IDT();
static void init_GDT();
static void set_IDT_gate(u8, u32, u16, u8);
static void set_GDT_gate(s32, u32, u32, u8, u8);

gdt_entry_table   GDT_Entries[5];
gdt_pointer       pointer_to_GDT;

idt_entry_table   IDT_Entries[256];
idt_pointer       pointer_to_IDT;


void store_gdt_desc(gdt_pointer *  location) {
  asm volatile ("sgdt %0" : : "m"(*location) : "memory");}

void store_idt_desc(idt_pointer *  location) {
  asm volatile ("sidt %0" : : "m"(*location) : "memory");}


void initialize_descriptor_tables()
{
  gdt_pointer desc;
  store_gdt_desc(&desc);
  printf("[BEFORE] GDT base: 0x%X, limit: 0x%X\n", desc.base, desc.limit);

  idt_pointer idtdesc;
  store_idt_desc(&idtdesc);
  printf("[BEFORE] IDT base: 0x%X, limit: 0x%X\n", idtdesc.base, idtdesc.limit);

  init_GDT();

  //gdt_pointer desc;
  store_gdt_desc(&desc);
  printf("GDT base: 0x%X, limit: 0x%X\n", desc.base, desc.limit);

  init_IDT();

  //idt_pointer idtdesc;
  store_idt_desc(&idtdesc);
  printf("IDT base: 0x%X, limit: 0x%X\n", idtdesc.base, idtdesc.limit);

  memset(&interrupt_handlers, 0, sizeof(isr_t)*256);

}


static void init_GDT()
{
  pointer_to_GDT.limit = ((sizeof(gdt_entry_table)) * 5) -1;
  pointer_to_GDT.base  = (u32) &GDT_Entries;

  set_GDT_gate(0, 0, 0, 0, 0);                /* null GDT segment */
  set_GDT_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); /* Code segment */
  set_GDT_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); /* Data segment */
  set_GDT_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); /* User mode code segment */
  set_GDT_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); /* User mode data segment */
  gdt_flush((u32)&pointer_to_GDT);
  /* __asm__ volatile ("lgdt %0" */
  /*                   : */
  /*                   : "m" (pointer_to_GDT)); */

  /* asm("   movw $0x10, %ax    \n \ */
  /*           movw %ax, %ds    \n \ */
  /*           movw %ax, %es    \n \ */
  /*           movw %ax, %fs    \n \ */
  /*           movw %ax, %gs    \n \ */
  /*           ljmp $0x08, $next    \n \ */
  /*           next:        \n"); */
  //reloadSegments();

}


static void set_GDT_gate(s32 num, u32 base, u32 limit, u8 access, u8 gran)
{
  GDT_Entries[num].base_low       = (base & 0xFFFF);
  GDT_Entries[num].base_middle    = (base >> 16) & 0xFF;
  GDT_Entries[num].base_high      = (base >> 24) & 0xFF;

  GDT_Entries[num].limit_low      = (limit & 0xFFFF);
  GDT_Entries[num].granularity    = (limit >> 16) & 0x0F;

  GDT_Entries[num].granularity    |= gran & 0xF0;
  GDT_Entries[num].access         = access;
}


static void init_IDT()
{
  pointer_to_IDT.limit = (sizeof(idt_entry_table) * 256) -1;
  pointer_to_IDT.base = (u32) &IDT_Entries;

  /* The interrupt description table filled */
  /* space constant type of byte */
  memset(&IDT_Entries, 0, sizeof(idt_entry_table)*256);

  /* Remap the irq table  */
  outb(0x20, 0x11);
  outb(0xA0, 0x11);
  outb(0x21, 0x20);             /* decimal 32 on master   */
  outb(0xA1, 0x28);             /* decimal 40 on slave  */
  outb(0x21, 0x04);
  outb(0xA1, 0x02);
  outb(0x21, 0x01);
  outb(0xA1, 0x01);
  outb(0x21, 0x0);
  outb(0xA1, 0x0);

  set_IDT_gate(0,  (u32)isr0,  0x08, 0x8E);
  set_IDT_gate(1,  (u32)isr1,  0x08, 0x8E);
  set_IDT_gate(2,  (u32)isr2,  0x08, 0x8E);
  set_IDT_gate(3,  (u32)isr3,  0x08, 0x8E);
  set_IDT_gate(4,  (u32)isr4,  0x08, 0x8E);
  set_IDT_gate(5,  (u32)isr5,  0x08, 0x8E);
  set_IDT_gate(6,  (u32)isr6,  0x08, 0x8E);
  set_IDT_gate(7,  (u32)isr7,  0x08, 0x8E);
  set_IDT_gate(8,  (u32)isr8,  0x08, 0x8E);
  set_IDT_gate(9,  (u32)isr9,  0x08, 0x8E);
  set_IDT_gate(10, (u32)isr10, 0x08, 0x8E);
  set_IDT_gate(11, (u32)isr11, 0x08, 0x8E);
  set_IDT_gate(12, (u32)isr12, 0x08, 0x8E);
  set_IDT_gate(13, (u32)isr13, 0x08, 0x8E);
  set_IDT_gate(14, (u32)isr14, 0x08, 0x8E);
  set_IDT_gate(15, (u32)isr15, 0x08, 0x8E);
  set_IDT_gate(16, (u32)isr16, 0x08, 0x8E);
  set_IDT_gate(17, (u32)isr17, 0x08, 0x8E);
  set_IDT_gate(18, (u32)isr18, 0x08, 0x8E);
  set_IDT_gate(19, (u32)isr19, 0x08, 0x8E);
  set_IDT_gate(20, (u32)isr20, 0x08, 0x8E);
  set_IDT_gate(21, (u32)isr21, 0x08, 0x8E);
  set_IDT_gate(22, (u32)isr22, 0x08, 0x8E);
  set_IDT_gate(23, (u32)isr23, 0x08, 0x8E);
  set_IDT_gate(24, (u32)isr24, 0x08, 0x8E);
  set_IDT_gate(25, (u32)isr25, 0x08, 0x8E);
  set_IDT_gate(26, (u32)isr26, 0x08, 0x8E);
  set_IDT_gate(27, (u32)isr27, 0x08, 0x8E);

  set_IDT_gate(28, (u32)isr28, 0x08, 0x8E);
  set_IDT_gate(29, (u32)isr29, 0x08, 0x8E);
  set_IDT_gate(30, (u32)isr30, 0x08, 0x8E);
  set_IDT_gate(31, (u32)isr31, 0x08, 0x8E);
  /* IRQ lines */
  set_IDT_gate(32, (u32)irq0,  0x08, 0x8E);
  set_IDT_gate(33, (u32)irq1,  0x08, 0x8E);
  set_IDT_gate(34, (u32)irq2,  0x08, 0x8E);
  set_IDT_gate(35, (u32)irq3,  0x08, 0x8E);
  set_IDT_gate(36, (u32)irq4,  0x08, 0x8E);
  set_IDT_gate(37, (u32)irq5,  0x08, 0x8E);
  set_IDT_gate(38, (u32)irq6,  0x08, 0x8E);
  set_IDT_gate(39, (u32)irq7,  0x08, 0x8E);
  set_IDT_gate(40, (u32)irq8,  0x08, 0x8E);
  set_IDT_gate(41, (u32)irq9,  0x08, 0x8E);
  set_IDT_gate(42, (u32)irq10, 0x08, 0x8E);
  set_IDT_gate(43, (u32)irq11, 0x08, 0x8E);
  set_IDT_gate(44, (u32)irq12, 0x08, 0x8E);
  set_IDT_gate(45, (u32)irq13, 0x08, 0x8E);
  set_IDT_gate(46, (u32)irq14, 0x08, 0x8E);
  set_IDT_gate(47, (u32)irq15, 0x08, 0x8E);

  idt_flush((u32) &pointer_to_IDT);
  /* __asm__ volatile ("lidt %0" */
  /*                   : */
  /*                   : "m" (pointer_to_IDT)); */
}


static void set_IDT_gate(u8 num, u32 base, u16 idt_selector, u8 flags)
{
  IDT_Entries[num].base_low   = base & 0xFFFF;
  IDT_Entries[num].base_high  = (base >> 16) & 0xFFFF;
  IDT_Entries[num].selector   = idt_selector;
  IDT_Entries[num].zero       = 0;
  IDT_Entries[num].flags      = flags;
}
