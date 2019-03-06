#ifndef KEYBOARD_DEVICE
#define KEYBOARD_DEVICE


#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

void init_keyboard();

char get_keyboard_buffor();
#endif
