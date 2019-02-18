#include "multiboot.h"
#include "printf.h"
#include "screen.h"
#include "types.h"
#include "stdlib/task.h"
#include "descriptor_tables.h"
#include "dev/keyboard.h"
#include "stdlib/paging.h"
#include "stdlib/kernelheap.h"


u32 stack_esp;

void main(struct multiboot * header, u32 stack_pointer)
{
  stack_esp = stack_pointer;
  initialize_descriptor_tables();
	clear_screen();

  printf("\n                   +-----------------+\n");
  printf("      ,~~.         |     KWA         |\n");
  printf("     (  9 )-_,    <        -KWA!     | \n");
  printf("(\\___ )=='-'       +-----------------+\n");
  printf(" \\ .   ) )\n");
  printf("  \\ `-' /\n");
  printf("  `~j-'\n");
  printf("    \"=:)\n");
  printf("\n");
  //PCIScan();
  asm volatile("sti");
  init_keyboard();
  //initialise_timer(50);
  initialise_paging();
  initialise_tasking();

  int ret = fork();
  task_switch();
  printf("RET -> %X | PID -> %X\n", ret, get_pid());
  printf("Thread function");

}


