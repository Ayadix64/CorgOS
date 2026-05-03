PM:
	CODE_SEG equ GDT_code - GDT_start
	DATA_SEG equ GDT_data - GDT_start

	cli
	lgdt [GDT_descriptor]
	;lidt [idt]
	nop
	nop
	;nop

	mov eax, cr0
	or eax, 1
	mov cr0, eax

	jmp 0x8:start_protected_mode
	nop

	jmp $
GDT_start:
    GDT_null:
        dd 0x0
        dd 0x0

    GDT_code:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10011010
        db 0b11001111
        db 0x0

    GDT_data:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10010010
        db 0b11001111
        db 0x0

GDT_end:

GDT_descriptor:
    dw GDT_end - GDT_start - 1
    dd GDT_start
idt:
	dw 0
	dd 0


[bits 32]
start_protected_mode:
	mov ax, 0x10
	mov ds, ax	
	mov es, ax
	mov fs, ax
	mov gs ,ax
	mov ss, ax

	mov esp, 0x900000
	mov ebp, esp 
	mov eax, cr0
or eax, 0x60000000     ; enable caching again
mov cr0, eax

	mov al, 'B'
	mov ah, 0x0e; white on black
	mov[0xb8000], ax ;VidioMemory
	;hlt
	jmp dword 0x8:KERNEL_LOCATION

                                     




