/* #include "IO/pci.h" */


#include "printf.h"
#include "screen.h"
#include "types.h"
#include "descriptor_tables.h"
#include "dev/keyboard.h"


void main(void)
{
  initialize_descriptor_tables();
  /* PCIScan(); */

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
  PCIScan();

  /* asm volatile("int $0x3"); */
  /* asm volatile("int $0x4"); */

  /* asm volatile("int $0x8"); */
  /* asm volatile("int $0xA"); */
  /* asm volatile("int $0xB"); */
  /* asm volatile("int $0xC"); */
  /* asm volatile("int $0xD"); */
  /* asm volatile("int $0xE"); */
  asm volatile("sti");
  /* init_keyboard(); */
}

