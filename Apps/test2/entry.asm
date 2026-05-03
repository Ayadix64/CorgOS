[bits 32]
section .entry
[extern main]
[global _start]
_start:
    call main
    ret
    ;jmp $
section .text
section .data
