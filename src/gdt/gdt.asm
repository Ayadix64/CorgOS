[bits 32]
[global loadgdt]
loadgdt:
    lgdt [eax]
    mov ax , 0x10
	mov ds, ax	
	mov es, ax
	mov fs, ax
	mov gs ,ax
	mov ss, ax
    jmp 0x8:func
    ret



func:
    ;nothing mater , just to do a far jump
    xor eax, eax 
    ret