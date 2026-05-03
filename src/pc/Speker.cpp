#include "../utils/util.h"
#include "timer.h"

void Speker_Note(u32 frq){
    u32 d = 1193180 / frq;
    
    outb(0x43, 0xB6);
    //outb(0x42, (u8) (d & 0xFF));
    //outb(0x42, (u8) ((d >> 8) & 0xFF));
    outb(0x42, (u8)(d));
    outb(0x42, (u8)(d>>8));
    
    u8 t = inb(0x61);
    if (t != (t | 0x3)) {
        outb(0x61, t | 0x3);
    }

    return;
}