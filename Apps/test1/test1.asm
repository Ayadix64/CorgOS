[ORG 0x100000]
[BITS 32]
NEWLINE equ 0xa
start_:
pusha
;clear
mov eax, 0x2
int 0x80
;prnint
mov eax, 0x1
mov dword ebx, msg
mov ecx , 14
mov edx, msgend-msg
int  0x80
xor eax, eax
popa
ret

;mov eax, 0x1
;mov ebx, msg
;mov ecx, 10
;int 0x80


msg:db "Hello to the AWSOME APP, is do nothing BTW, and yeah, pain lol " ,0xa, "do you know that i use arch btw , and hyprland btw, and btw a custom it by my self btw" , 0xa , "and do you know that i do my owne os btw, and btw that wath you are runnig btw, btw , i made this app to btw, yeah btw",0,0
msgend:

