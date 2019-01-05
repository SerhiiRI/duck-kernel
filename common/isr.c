#include "isr.h"
#include "printf.h"
#include "types.h"


isr_t interrupt_handlers[256];

void register_interrupt_handler(u8 n, isr_t handler)
{
  interrupt_handlers[n] = handler;
}


void isr_handler(registers_table regs)
{
  printf("recieved interrupt: %d | error code: %d\n"
         , regs.interruption_number
         , regs.error_code);

  if (interrupt_handlers[regs.interruption_number] != 0)
    {
      isr_t handler = interrupt_handlers[regs.interruption_number];
      handler(regs);
    }


}

void irq_handler(registers_table regs)
{
  /* Send an EOI (end of interrupt) signal to the PICs. */
  /* If this interrupt involved the slave. */
  if (regs.interruption_number >= 40)
    {
      // Send reset signal to slave.
      outb(0xA0, 0x20);
    }
  // Send reset signal to master. (As well as slave, if necessary).
  outb(0x20, 0x20);

  if (interrupt_handlers[regs.interruption_number] != 0)
    {
      isr_t handler = interrupt_handlers[regs.interruption_number];
      handler(regs);
    }

}
