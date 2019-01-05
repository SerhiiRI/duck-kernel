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


[GLOBAL reloadSegments]
reloadSegments:
	   ; Reload CS register containing code selector:
	   JMP   0x08:.reload_CS ; 0x08 points at the new code selector
	.reload_CS:
	   ; Reload data segment registers:
	   MOV   AX, 0x10 ; 0x10 points at the new data selector
	   MOV   DS, AX
	   MOV   ES, AX
	   MOV   FS, AX
	   MOV   GS, AX
	   MOV   SS, AX
	   RET


[GLOBAL idt_flush]

idt_flush:
  mov eax, [esp+4]
  lidt [eax]
  ret                           ; return addres to caller function
