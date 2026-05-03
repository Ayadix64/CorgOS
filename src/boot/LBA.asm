
LBAKernelRead:
 
    mov ax, KERNEL_SEG    
    mov es, ax
    xor ax, ax
  ;  mov ds, ax

    mov bx, KERNEL_LOCATION  
    mov si, disk_packetK1
    mov ah, 0x42           
    ;mov dl, [BOOT_DISK] 
    int 0x13

    mov ax, es
    add ax, 0xfc0           
    mov es, ax             
    xor ax, ax

    mov bx, KERNEL_LOCATION         
    mov si, disk_packetK2
    mov ah, 0x42           
    ;mov dl, [BOOT_DISK]  
    int 0x13
    
    xor ax, ax
    mov es, ax
 ;   mov ds, ax

    ret

LBARStage2:


    ;xor ax, ax        
    ;mov es, ax             
    
    mov bx, STAGE2LOC        
    mov si, disk_packetS2	
    mov ah, 0x42           
    mov dl, 0x80;[BOOT_DISK]  
    int 0x13
    ;jc DERR

    ret

disk_packetS2:
    db 0x10               
    db 0x00               
    dw STAGE2SIZ
    dw STAGE2LOC
    dw 0x000
    dd STAGE2LBA-1
    dd 0x00000000


disk_packetK1:
    db 0x10               
    db 0x00               
    dw 126
    dw KERNEL_LOCATION             
    dw KERNEL_SEG    
    dd (KERNEL_START-1)        
    dd 0x00000000         

disk_packetK2:
    db 0x10               
    db 0x00               
    dw 126             
    dw KERNEL_LOCATION             
    dw (KERNEL_SEG+0x0fc0) 
    dd (KERNEL_START-1+126)   
    dd 0x00000000         

