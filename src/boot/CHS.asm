CHSKernelRead:
	pusha
	xor ax, ax
	mov es, ax
	mov ds, ax
	;mov bp, 0x7e00;0xffff
	;mov sp, bp

	xor ax, ax
	xor dh, dh
	mov cl , KERNEL_START
	rdker:
		push ax
		
		mov ah, 2
		mov al, 126
		mov bx, KERNEL_LOCATION
		mov ch, 0
		mov dl,[BOOT_DISK]
		int 0x13
		jc DERR

		add dh, 0x2
		mov ax, es
		add ax, 0xfc0
		mov es, ax

		pop ax
		inc ax
	
		cmp ax, KERNEL_LONG
		jl rdker
		xor ax, ax
		mov es, ax
		popa
		ret


CHSWbootSeg:
	pusha
	mov ah , 0x3 ;write...
	mov al , 1
	mov ch , 0
	mov dh , 0
	mov cl , SEGLBA
	mov dl , [BOOT_DISK]
	mov bx  , boot_dev_sig
	int 0x13
	popa
	ret

CHSRboot0x8000:
	
	mov ah , 0x2;read the boot again in 0x8000 because the 7c00 havnt the boot segnater
	mov al , 1  ;so you cant install it in a other disk
	mov ch , 0
	mov dh , 0
	mov cl , 1
	mov dl , [BOOT_DISK]
	mov bx , 0x8000
	int 0x13
	ret

CHSRStage2:
	mov ah , 0x2
	mov al , STAGE2SIZ
	mov ch , 0
	mov dh , 0
	mov cl , STAGE2LBA
	mov dl , [BOOT_DISK]
	mov bx , STAGE2LOC
	int 0x13
	ret

