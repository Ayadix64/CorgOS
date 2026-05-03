#include "../utils/util.h"
#include "gdt.h"
extern "C" void loadgdt(u32 eax);
GDTSegment CodeSegment(){
    GDTSegment code;
    code.base_low=0x0;
    code.base_mid=0;
    code.base_hi=0;
    
    code.limit_low=0xffff;
    code.flag2_limit_hi=(0b1100<<4/* *=4kb , 32bit , 64bit , avl  */)|0xf/*limithi*/;

    code.pps_flag1=((0b1001/*present, privlage (ring0) --> 0,type(1==>code or data)*/)<<4)
    |(0b1110/*exetabel(this is code so yes),acssed by ring3 (at this moment , we will do multi tasking , so yes),
            readbel(make us read constans so yes), acssed (CPU will change it,)*/);
    return code;
}

GDTSegment DataSegment(){
    GDTSegment data;
    data.base_low=0x0;
    data.base_mid=0;
    data.base_hi=0;
    
    data.limit_low=0xffff;
    data.flag2_limit_hi=(0b1100<<4/* *=4kb , 32bit , 64bit , avl  */)|0xf/*limithi*/;

    data.pps_flag1=((0b1001/*present, privlage (ring0 tow bits),type(1==>code or data)*/)<<4)
    |(0b0010/*exetabel(this is data so no),diraction(not good thing),
            writabel(this is data , so yes), acssed (CPU will change it)*/);
    return data;
}

void gdt_init(){
    GDTEntry* gdt;
    GDTp* pgdt;
    gdt->codesegment=CodeSegment();
    gdt->datasegment=DataSegment();

    
    pgdt->size=sizeof(GDTEntry)-1;
    pgdt->pointer=(u32)gdt;

    /*hey , this is the end , just this , have a nice day*/
    loadgdt((u32)pgdt);
    /*parfect , */
}
