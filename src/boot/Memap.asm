MemMap:
	mov word [0x5000],0
	mov di , 0x5002
	xor ebx, ebx		; ebx must be 0 to start
	xor bp, bp		; keep an entry count in bp
	mov edx, 0x0534D4150	; Place "SMAP" into edx
	mov eax, 0xe820
	mov [es:di + 20], dword 1	; force a valid ACPI 3.X entry
	mov ecx, 24		; ask for 24 bytes
	int 0x15  
	jc .fail
	.loop_e820:
		mov eax, 0xe820		; eax, ecx get trashed on every int 0x15 call
		mov [es:di + 20], dword 1	; force a valid ACPI 3.X entry
		mov ecx, 24		; ask for 24 bytes again
	
		int 0x15
		jc .fail             
		cmp edx, 0x534D4150
		jne .fail

		add di, 24             
		inc word [0x5000]

		cmp ebx, 0
		jne .loop_e820
	   	cmp word [0x5000],128
    		je .done
		jmp .done

	.fail:
    		mov bx , merr
    		call print
    		hlt
	.done:
		ret
merr: db'Mem Err',0,0
