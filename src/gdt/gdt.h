#ifndef GDT_H
#define GDT_h
#include "../utils/util.h"
typedef struct GDTp{
    u16 size;
    u32 pointer;
}__attribute__ ((packed));

typedef struct GDTSegment{
    u16 limit_low;
    u16 base_low;
    u8 base_mid;
    u8 pps_flag1;
    u8 flag2_limit_hi;
    u8 base_hi;
}__attribute__ ((packed));

typedef struct GDTEntry{
    u64 zeros = 0;
    GDTSegment codesegment;
    GDTSegment datasegment;
}__attribute__ ((packed));

void gdt_init();
#endif