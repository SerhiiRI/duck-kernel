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
  clear_screen();
  printf("[+] Scanning PCI\n");
  PCIScan();

  printf("[+] keyboard initialization\n");
  init_keyboard();
  asm volatile("sti");
  printf("[+] memory initialization.\n");
  initialise_paging();
  printf("[+] timer initialization\n");
  printf("[+] tasking initialization\n");



  initialise_timer(50);
  initialise_tasking();
  int ret = fork();
  //task_switch();
  print_task_info();

}


