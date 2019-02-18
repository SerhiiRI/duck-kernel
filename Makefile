CC			= gcc
CFLAGS	= -m32 -Wall -fno-builtin -nostdinc -nostdlib -ggdb3 -fno-stack-protector
LD			= ld
LDFLAGS = -m elf_i386
AS  		= nasm
ASFLAGS	= -felf

 OBJFILES 	= loader.o \
						common/printf.o \
						common/screen.o	\
						common/interrupt.o \
						common/descriptor_tables.o \
						common/isr.o \
						common/gdt.o \
						common/memory.o \
						common/PCI/pci.o \
						common/devices/keyboard.o \
						common/devices/timer.o \
						common/stdlib/paging.o \
						common/stdlib/kernelheap.o \
						common/stdlib/ordered_array.o \
						common/stdlib/task.o \
						common/stdlib/process.o \
						kernel.o

all: kernel.bin
rebuild: clean all

image: kernel.bin
					@echo -e "\033[92mCreating image \033[1mDuckOS.img\033[0m"
					@cp ./kernel.bin ./img/boot/
					@grub2-mkrescue -o duck.img img

cleanimage:
					@rm -f ./img/boot/kernel.bin

loader.o: loader.s
				$(AS) $(ASFLAGS) -o $@ $<

common/interrupt.o: common/interrupt.s
				$(AS) $(ASFLAGS) -o $@ $<

common/gdt.o: common/gdt.s
				$(AS) $(ASFLAGS) -o $@ $<

common/stdlib/process.s: common/stdlib/process.s
				$(AS) $(ASFLAGS) -o $@ $<

.c.o:
				$(CC) -Iinclude $(CFLAGS) -o $@ -c $<

kernel.bin: $(OBJFILES)
				$(LD) $(LDFLAGS) -T linker.ld -o $@ $^
				cp $@ $@.gdb
				strip $@

clean:
				@rm -f $(OBJFILES)
				@rm kernel.bin kernel.bin.gdb
