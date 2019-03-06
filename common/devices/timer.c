#include "types.h"
#include "isr.h"
#include "dev/timer.h"
#include "stdlib/task.h"
#include "memory.h"

u32 tick = 0;

static void timer_callback(registers_table regs)
{
  tick++;
  task_switch();
}



void initialise_timer(u32 frequency)
{
  register_interrupt_handler(IRQ0, &timer_callback);
  /* 1.193182 MHz osciallator frequency */
  u32 divisor = 1193180 / frequency;

  outb(0x43, 0x36);             /* 0x36 bitmap - 0011.
                                   0110 */
  /*
    0043	r/w	PIT  mode port, control word register for counters 0-2

		 bit 7-6 = 00  counter 0 select
             = 01  counter 1 select	  (not PS/2)
             = 10  counter 2 select
		 bit 5-4 = 00  counter latch command
             = 01  read/write counter bits 0-7 only
             = 10  read/write counter bits 8-15 only
             = 11  read/write counter bits 0-7 first, then 8-15
		 bit 3-1 = 000 mode 0 select
             = 001 mode 1 select - programmable one shot
             = x10 mode 2 select - rate generator
             = x11 mode 3 select - square wave generator
             = 100 mode 4 select - software triggered strobe
             = 101 mode 5 select - hardware triggered strobe
		 bit 0	 = 0   binary counter 16 bits
             = 1   BCD counter
  */
  u8 l = (u8) (divisor & 0xFF);
  u8 h = (u8) ((divisor>>8) & 0xFF);

  outb(0x40, l);
  outb(0x40, h);
}
