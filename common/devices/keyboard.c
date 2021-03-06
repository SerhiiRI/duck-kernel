#include "isr.h"
#include "printf.h"
#include "memory.h"
#include "dev/keyboard.h"
#include "dev/scan_codes.h"

char oneSymbolBuffor = 0;

static void write_to_buffor(char code){
  oneSymbolBuffor = code;
}

char get_keyboard_buffor(){
  return oneSymbolBuffor;
}

static void keyboard_press_callback(registers_table regs)
{
  unsigned char status;
	char keycode;


	status = inb(KEYBOARD_STATUS_PORT);
	/* Lowest bit of status will be set if buffer is not empty */
	if (status & 0x01) {
		keycode = inb(KEYBOARD_DATA_PORT);

		if(keycode < 0)
      return;

		if(keycode == 0x1C) {
			printf("\n");
      return;
		}
    write_to_buffor(keycode);
    printf("%c", keyboard_codes[keycode]);
	}
}

void init_keyboard()
{
  register_interrupt_handler(IRQ1, &keyboard_press_callback);
  //printf("Initialize keyboard\n");
}
