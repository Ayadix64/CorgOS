[BITS 16]
[ORG 0x7C00]        
jmp  start
nop
nop
nop
nop
nop
bdb_oem:                    db 'DOSOS7.1'           ; 8 bytes
bdb_bytes_per_sector:       dw 512
bdb_sectors_per_cluster:    db 1
bdb_reserved_sectors:       dw 255
bdb_fat_count:              db 2
bdb_dir_entries_count:      dw 0E0h
bdb_total_sectors:          dw 2880*4                 ; 2880 * 512 = 1.44MB
bdb_media_descriptor_type:  db 0F8h                 ; F0 = 3.5" floppy disk
bdb_sectors_per_fat:        dw 9*4                    ; 9 sectors/fat
bdb_sectors_per_track:      dw 63 ;18
bdb_heads:                  dw 2
bdb_hidden_sectors:         dd 0x00000000
bdb_large_sector_count:     dd 2880*4 ;????

; extended boot record
ebr_drive_number:           db 0x80                    ; 0x00 floppy, 0x80 hdd, useless
                            db 0                    ; reserved
ebr_signature:              db 29h
ebr_volume_id:              db 12h, 34h, 56h, 78h   ; serial number, value doesn't matter
ebr_volume_label:           db 'Ayadi OS   '        ; 11 bytes, padded with spaces
ebr_system_id:              db 'FAT16   '           ; 8 bytes
;thans to Nanobyte
;
nop
nop
nop
KERNEL_LOCATION equ 0x10000
KERNEL_SEG equ 0x1000
KERNEL_LONG equ 2 ;*126
KERNEL_START equ 4

;STAGE2LOC equ 0x7e00
STAGE2LBA equ 2
STAGE2SIZ equ 1

SEGLBA equ 3

start:
	;mov [BOOT_DISK], dl
	xor eax, eax
	mov eax , cr0
    	and eax, 0xffffefff
    	mov cr0,eax
	mov ax , 0x2401
	int 0x15	
	xor ax, ax
	mov ds, ax
	mov es, ax

	
;	xor ax, ax
;	mov es, ax
;	mov fs, ax
;	mov ds, ax
;	mov sp, 0x8a00
;	mov bp, sp
;	call  StartSeg	
	;call LBASepport
	;cmp ax, ax
	;je SEPORT
	;jne NONSEPORT

SEPORT:
	
	call  LBARStage2
	call  LBAKernelRead
	
;	mov bx, STAGE2LOC        
;    	mov si, disk_packetS2	
;    	mov ah, 0x42           
;    	;mov dl, 0x80;[BOOT_DISK]  
;    	int 0x13
;	
;	mov ah, 0xe
;	mov al, [0x7e00]
;	int 0x10
;	mov ah, 0xe
;	mov al, [0x7e01]
;	int 0x10
;	mov ax, KERNEL_SEG    
;    	mov es, ax
;    	xor ax, ax
;  	;  mov ds, ax
;
;    	mov bx, 0x0 
;    	mov si, disk_packetK1
;    	mov ah, 0x42           
;    	;mov dl, [BOOT_DISK] 
;    	int 0x13
;   	 mov ax, es
;   	 add ax, 0xfc0           
;   	 mov es, ax             
;   	 xor ax, ax
;
;   	 mov bx, 0x0;KERNEL_LOCATION         
;   	 mov si, disk_packetK2
;   	 mov ah, 0x42           
   	 ;mov dl, [BOOT_DISK]  
;   	 int 0x13
   	    	 xor ax, ax
   	 mov es, ax


	;call  LBAWbootSeg
	;call<F6>  LBARboot
	jmp CONTINU
NONSEPORT:
	mov bx, LBAERR
	mov cx, 18
	call print
	hlt
	;call  CHSKernelRead
	;call  CHSRStage2
	;call  StartSeg
	;call  LBAWbootSeg
	;call  CHSRboot0x8000
	;jmp CONTINU
	;jne CHSKernelRead
	
	;cmp byte [SELBA], 1
	;pusha
		;jne CHSRStage2

CONTINU:
		call MemMap
	call PM




;%include"./boot/CHS.asm"

DERR:   
    mov ah , 0x0e
    mov al , 'D'
    int 0x10
    hlt
StartSeg:
	pusha
;	xor ax , ax
;	mov es, ax
;	mov ds, ax
;	mov es, ax
;	mov fs, ax
	
	mov ah , 0x2
	int 0x1a
	mov [houre] , ch
	mov [mint]  , cl
	mov [sec]   , dh

	mov ah , 0x4
	int 0x1a
	mov  [year]  , cl
	mov  [month] , dh
	mov  [day]   , dl  
	popa
	ret
print:
    pusha
    jmp .prn
    .prn:
        mov ah, 0xe
        mov al , [bx]
        int 0x10
        inc bx
        cmp word cx, [bx]
        jl .prn
        jmp .end
    .end:
        popa
        ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;disk_packetS2:
;    db 0x10               
;    db 0x00               
;    dw 0x0001
;;    dw STAGE2LOC
;    dw 0x000
 ;   dd 0x00000001
 ;   dd 0x00000000
;
;
;disk_packetK1:
;    db 0x10               
;    db 0x00               
;    dw 126
;    dw 0x0;KERNEL_LOCATION             
;;    dw KERNEL_SEG    
 ;   dd (KERNEL_START-1)        
;    dd 0x00000000         

;disk_packetK2:
;    db 0x10               
;    db 0x00               
;    dw 126             
;    dw 0x0;KERNEL_LOCATION             
;    dw (KERNEL_SEG+0x0fc0) 
;    dd (KERNEL_START-1+126)   
;    dd 0x00000000         
;
;disk_packetWS:
;    db 0x10               
;;    db 0x00               
 ;   dw 126             
 ;   dw 0x0;KERNEL_LOCATION             
 ;   dw (KERNEL_SEG+0x0fc0) 
 ;   dd (KERNEL_START-1+126)   
 ;   dd 0x00000000         
;
;
%include"./boot/LBA.asm"
LBAERR:db'LBA is not Seport!',0,0 ;the CHS take a lot of the boot secter, and  i am lazy tbh
BOOT_DISK: db 0
;derr: db'Disk Err',0,0
;msghello:db 'Loading Ayadi OS ...',0,0,0

times 510-($-$$) db 0              
dw 0xaa55
STAGE2LOC:
db 'S2'
%include"./boot/Memap.asm"
%include"./boot/pm.asm"

times 1024-($-$$) db 0 
boot_dev_sig:
sec:db 0
mint:db 0
houre:db 0
day:db 0
month:db 0
year:db 0;no , this is a boot segnatur , dont worry
times 1536-($-$$) db 0
SECEND_BOOT:
;times 512 db 0
