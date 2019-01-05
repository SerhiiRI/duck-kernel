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
						kernel.o

all: kernel.bin
rebuild: clean all


# image:
# 				@echo "Creating hdd.img"
# 				@dd if=/dev/zero of=./hdd.img bs=512 count=16065 1>/dev/null 2>&1

# 				@echo "Creating bootable first FAT32 partition"
# 				@losetup /dev/loop1 ./hdd.img
# 				@(echo c; echo u; echo n; echo p; echo 1; echo ;  echo ; echo a; echo 1; echo t; echo c; echo w;) | fdisk /dev/loop1 1>/dev/null 2>&1 || true

# 				@echo "Mounting partition... to dev loop2"
# 				@losetup /dev/loop2 ./hdd.img --offset `echo \`fdisk -lu /dev/loop1 | sed -n 10p | awk '{print $$3}'\`*512 | bc` \
# 																			--sizelimit `echo \`fdisk -lu /dev/loop1 | sed -n 10p | awk '{print $$4}'\`*512 | bc`
# 				@echo "Format partition..."
# 				@mkdosfs /dev/loop2

# 				@echo "Copy kernel and grub files on partition..."
# 				@mkdir -p tempdir
# 				@mount /dev/loop2 tempdir
# 				@mkdir tempdir/boot
# 				@cp -r grub tempdir/boot/
# 				@cp kernel.bin tempdir/
# 				@sleep 1
# 				@umount /dev/loop2
# 				@rm -r tempdir
# 				@losetup -d /dev/loop2

# 				@echo "Installing GRUB..."
# 				@echo "device (hd0) hdd.img \n \
# 	      			 root (hd0,0)         \n \
# 	       			 setup (hd0)          \n \
# 	       			 quit\n" | grub --batch 1>/dev/null
# 				@echo "Done!"

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

.c.o:
				$(CC) -Iinclude $(CFLAGS) -o $@ -c $<

kernel.bin: $(OBJFILES)
				$(LD) $(LDFLAGS) -T linker.ld -o $@ $^
				cp $@ $@.gdb
				strip $@

clean:
				@rm -f $(OBJFILES)
				@rm kernel.bin kernel.bin.gdb
