[bits 32]
%macro ISR 1
global isr%1
isr%1:
    cli
    push long 0
    push long %1
    ;mov eax, cr0
    ;;xor eax, 0x8000000
    ;mov cr0, eax
    ;jmp isr_common
%endmacro

%macro ISR_ERR 1
global isr%1
isr%1:
    cli
    ;the erorr code pushed automaticly
    push long %0
    ;mov eax, cr0
    ;xor eax, 0x8000000
    ;mov cr0, eax

    jmp isr_common
%endmacro

%macro IRQ 1
global irq%1
irq%1:
    cli
    ;mov eax, cr0
    ;xor eax, 0x8000000
    ;mov cr0, eax

    push long 0
    push long %1+32
    jmp irq_common
%endmacro


ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR_ERR 8
ISR 9
ISR_ERR 10
ISR_ERR 11
ISR_ERR 12
ISR_ERR 13
ISR_ERR 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19
ISR 20
ISR 21
ISR 22
ISR 23
ISR 24
ISR 25
ISR 26
ISR 27
ISR 28
ISR 29
ISR 30
ISR 31
;ISR 32
;ASM file ... auto creat!

IRQ 0
IRQ 1
IRQ 2
IRQ 3
IRQ 4
IRQ 5
IRQ 6
IRQ 7
IRQ 8
IRQ 9
IRQ 10
IRQ 11
IRQ 12
IRQ 13
IRQ 14
IRQ 15
IRQ 16
IRQ 17
IRQ 18
IRQ 19
IRQ 20
IRQ 21
IRQ 22
IRQ 23
IRQ 24
IRQ 25
IRQ 26
IRQ 27
IRQ 28
IRQ 29
IRQ 30
IRQ 31
IRQ 32
IRQ 33
IRQ 34
IRQ 35
IRQ 36
IRQ 37
IRQ 38
IRQ 39
IRQ 40
IRQ 41
IRQ 42
IRQ 43
IRQ 44
IRQ 45
IRQ 46
IRQ 47
IRQ 48
IRQ 49
IRQ 50
IRQ 51
IRQ 52
IRQ 53
IRQ 54
IRQ 55
IRQ 56
IRQ 57
IRQ 58
IRQ 59
IRQ 60
IRQ 61
IRQ 62
IRQ 63
IRQ 64
IRQ 65
IRQ 66
IRQ 67
IRQ 68
IRQ 69
IRQ 70
IRQ 71
IRQ 72
IRQ 73
IRQ 74
IRQ 75
IRQ 76
IRQ 77
IRQ 78
IRQ 79
IRQ 80
IRQ 81
IRQ 82
IRQ 83
IRQ 84
IRQ 85
IRQ 86
IRQ 87
IRQ 88
IRQ 89
IRQ 90
IRQ 91
IRQ 92
IRQ 93
IRQ 94
IRQ 95
IRQ 97
IRQ 98
IRQ 99
IRQ 100
IRQ 101
IRQ 102
IRQ 103
IRQ 104
IRQ 105
IRQ 106
IRQ 107
IRQ 108
IRQ 109
IRQ 110
IRQ 111
IRQ 112
IRQ 113
IRQ 114
IRQ 115
IRQ 116
IRQ 117
IRQ 118
IRQ 119
IRQ 120
IRQ 121
IRQ 122
IRQ 123
IRQ 124
IRQ 125
IRQ 126
IRQ 127
IRQ 128
IRQ 129
IRQ 130
IRQ 131
IRQ 132
IRQ 133
IRQ 134
IRQ 135
IRQ 136
IRQ 137
IRQ 138
IRQ 139
IRQ 140
IRQ 141
IRQ 142
IRQ 143
IRQ 144
IRQ 146
IRQ 147
IRQ 148
IRQ 149
IRQ 150
IRQ 151
IRQ 152
IRQ 153
IRQ 154
IRQ 155
IRQ 156
IRQ 157
IRQ 158
IRQ 159
IRQ 160
IRQ 161
IRQ 162
IRQ 163
IRQ 164
IRQ 165
IRQ 166
IRQ 167
IRQ 168
IRQ 169
IRQ 170
IRQ 171
IRQ 172
IRQ 173
IRQ 174
IRQ 175
IRQ 176
IRQ 177
IRQ 178
IRQ 179
IRQ 180
IRQ 181
IRQ 182
IRQ 183
IRQ 184
IRQ 185
IRQ 186
IRQ 187
IRQ 188
IRQ 189
IRQ 190
IRQ 191
IRQ 192
IRQ 193
IRQ 194
IRQ 195
IRQ 196
IRQ 197
IRQ 198
IRQ 199
IRQ 200
IRQ 201
IRQ 202
IRQ 203
IRQ 204
IRQ 205
IRQ 206
IRQ 207
IRQ 208
IRQ 209
IRQ 210
IRQ 211
IRQ 212
IRQ 213
IRQ 214
IRQ 215
IRQ 216
IRQ 217
IRQ 218
IRQ 219
IRQ 220
IRQ 221
IRQ 222
IRQ 223



ISR 128
ISR 177
;global syscall1
;global syscall2
;syscall1:
;    cli
    ;hlt
;    push byte 0
;    push byte 128
;    push eax
;    mov eax, 0xB8000
;    mov byte [eax], 'S'
;    pop eax
;    jmp isr_common;system call

;syscall2:
;    cli
;    hlt
;    push byte 0
;    push byte 177
;    jmp isr_common;system call

extern isr_handler

isr_common:

    pusha
    push ds
    push es
    push fs
    push gs
    
    mov eax, cr4
    push eax
    mov eax, cr3;becuse some risen , intel and amd never do a regester call cr1 , if you traye to read frome it , you pc will restart
    push eax
    mov eax, cr2
    push eax
    mov eax, cr0
    push eax

    mov ax, 0x10;data segment
    mov ds , ax
    mov es , ax
    mov fs , ax
    mov gs , ax

    mov eax, esp
    push eax
    ;mov eax, cr0
    ;xor eax, 0x8000000
    ;mov cr0, eax
 
    call isr_handler

    pop eax
    xor eax , eax

    pop eax;cr0
    mov cr0, eax
    pop eax;cr2
    mov cr2, eax
    pop eax;cr3
    mov cr3, eax
    pop eax;cr4
    mov cr4, eax

    pop gs
    pop fs
    pop es
    pop ds

    popa
    add esp, 16 ; clean up our errcode and isrnum..... look, it wase 8, but this dosnt crach if system call alert, probaly a bad thing,heh
    sti
    ;mov eax, cr0
    ;or eax, 0x8000000
    ;mov cr0, eax
    iret ;pop cs, eip , eflags , ss and esp
    




extern irq_handler
irq_common:
    pusha
    push ds
    push es
    push fs
    push gs
    mov eax, cr4
    push eax
    mov eax, cr3;becuse some risen , intel and amd never do a regester call cr1 , if you taye to read frome it , you pc will restart
    push eax
    mov eax, cr2
    push eax
    mov eax, cr0
    push eax

    mov ax, 0x10;data segment
    mov ds , ax
    mov es , ax
    mov fs , ax
    mov gs , ax

    mov eax, esp
    push eax
    
    ;mov eax, cr0
    ;xor eax, 0x8000000
    ;mov cr0, eax
 
    call irq_handler
    pop eax

    pop eax;cr0
    mov cr0, eax
    pop eax;cr2
    mov cr2, eax
    pop eax;cr3
    mov cr3, eax
    pop eax;cr4
    mov cr4, eax

    pop gs
    pop fs
    pop es
    pop ds

    popa
    add esp, 8 ; clean up our errcode and isrnum.....
    sti
    ;mov eax, cr0
    ;or eax, 0x8000000
    ;mov cr0, eax 
    iret ;pop cs, eip , eflags , ss and esp






;not menchen with inturupt, i just too lazy to creat a new file lol
;global loadreg
;_esp:dd 0
;_eax:dd 0
;loadreg: 
;    mov [_esp],esp
;    mov [_eax],eax
;    mov eax , 0
;    push eax
;    push eax
;    push eax
;    push eax
;    push eax
;    push eax
;    mov eax ,[_eax]
;
;    pusha
;    push ds
;    push es
;    push fs
;    push gs
;    mov eax, cr4
;    push eax
;    mov eax, cr3
;    push eax
;    mov eax, cr2
;    push eax
;    mov eax, cr0
;    push eax
;    mov eax, [_esp+4]
;    mov esp, [_esp]
;    ret
    



