[GLOBAL gdt_flush]

gdt_flush:
  mov eax, [esp+4]
  lgdt [eax]

  mov ax, 0x10                  ; offset to data segment Descriptor 16-bite offset
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  jmp 0x08:.flush

.flush:
  ret


[GLOBAL idt_flush]

idt_flush:
  mov eax, [esp+4]
  lidt [eax]
  ret                           ; return addres to caller function
