#ifndef IDT_H
#define IDT_H

#include "../utils/util.h"
struct idt_entry {
    u16 offset_low;
    u16 selector;
    u8 zero;
    u8 type_attr;
    u16 offset_high;
} __attribute__((packed));

struct idt_ptr {
    u16 limit;
    u32 base;
} __attribute__((packed));
typedef struct regs
{
    unsigned int cr0,cr2, cr3, cr4, gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
} __attribute__ ((packed)) ;

extern "C" regs Kerenlregs; 

void out_apic(u32 offset, u32 data);
u32 in_apic(u32 offset);

void out_apic_io(u8 reg, u32 val);
u32 in_apic_io(u8 reg);

void EOI(regs* reg);

//void system_call(regs* regs);

void install_irq(u32 irq , void (*irq_hand)(regs* reg),u32 CPU);
void uninstall_irq(u32 irq);

void install_isr(u32 isr , void (*isr_hand)(regs* reg));
void uninstall_isr(u32 isr);

regs loadreg();
void setreg(regs& r);
#endif
