#include "timer.h"
#include "../idt/idt.h"
#include "../utils/util.h"
#include "../cpu/CPU.h"
#include "../multitask/multitask.h"
#include "RTC.h"
#include "acpi.h"
u64 cpu_clock_f=1506269091;
u32 st_rtc = 0;

u32 timer_f=1;
u64 tcount = 0;
u64 secends = 0;

extern "C" u64 TimerClock = 0;
extern "C" u64 Seconds=0;

volatile hpet_t* hpet;



void timer_handler(regs *reg){
    //out_apic(0x320, 0x10000); // One-shot, Vector=0 (لا مقاطعة)
    //out_apic(0x3E0, 11);  // Divide=128
    //out_apic(0x380, 0xFFFFFFFF); // Initial Count
    
    tcount++;
    if((int)tcount%(int)timer_f==0){
        secends++;
	//printc(".",15);
     }
    TimerClock=tcount;
    Seconds=secends;

     //MlutiSw(reg);
    return;
}

void set_timer_f(u32 freq){
    !freq?freq=1:1;
    timer_f=freq;
    u32 dev = 128;
    u32 counter=((u32)cpu_clock_f)/(freq);
    out_apic(0x320,32|0x20000);
    out_apic(0x3e0,7);
    out_apic(0x380,counter);   
    return;
}

u32* get_hept_addr(){
    return (u32*)acpi_rsd_rd("HPET", 4);
    /*bool XSDT = false;
    RSDPDescriptor* find_rsdp = (RSDPDescriptor*)memfind((void*)0xE0000 , (void*)"RSD PTR " , 8, 0x1FFFF);
    if((void*)find_rsdp==(void*)0){
        return (unsigned long*)0;
    }
    if(find_rsdp->revision>=2)XSDT=true;
    if(XSDT){
        volatile acpi_sdt_header* xsdt=(volatile acpi_sdt_header*)(find_rsdp->xsdt_address);
        
        volatile HPET*hept_add =(volatile HPET*) memfind((void*)xsdt,(void*)"HPET",2,xsdt->length);

        return (unsigned long*)(u32)hept_add->address.address;

    }
    volatile acpi_sdt_header* xsdt=(volatile acpi_sdt_header*)(find_rsdp->rsdt_address);
        
    volatile HPET*hept_add =(volatile HPET*) memfind((void*)xsdt,(void*)"HPET",2,xsdt->length);

    return (unsigned long*)(u32)hept_add->address.address;
	*/
}

void timer_init(){
    TimerClock=0;	
    u32 dev = 0xffff;
    u32 count=dev;

    //asm ("cli"); 
    u32 t1,t2,sc;
    get_time(t1,t2,sc);
    u32 esc= sc;
    while (sc==esc) {
        get_time(t1,t2,sc);
    }
    count=dev;

    //asm("cli");//no intrupt , for best 
    //outb(0x43, 0x30);// 0x30 = 00110000 (Channel 0, Access low/high byte, Mode 0)

    out_apic(0x320, 0x10000);
    out_apic(0x3e0,7);
    out_apic(0x380,0xffffffff);
    
     
     get_time(t1,t2,sc);
     esc= sc;
    while (sc==esc) {
        get_time(t1,t2,sc);
    }
    //pit_sleep(1000);

    u32 start =  in_apic(0x390);
   
    
    

    u32 delta_ = 0xffffffff-start;
    u32 apic_freq = delta_;//*(1193182/0xffff);
    cpu_clock_f=apic_freq;
    
    set_timer_f(1000);
    install_irq(0,timer_handler,0);
    
    hpet= (volatile hpet_t*)get_hept_addr();
    printc("\nhept addres : ",10);
    printc(Hex((u32)hpet),10);

    printc("\nAPIC TIMER @ ",15);
    printc(Int_To_STR(cpu_clock_f, 10),15);
    printc(" Hz",15);
    
    return;
    	/*outb(0x43, 0x34); // One-shot
    outb(0x40, 0xFF); 

    // 2. ضبط APIC Timer
    out_apic(0x320, 0x10000); // One-shot, Vector=0 
    out_apic(0x3E0, 0b1011);  // Divide=128
    out_apic(0x380, 0xFFFFFFFF); // Initial Count

    // 3. انتظر حتى ينتهي PIT
    while ((inb(0x40) & 0x80) == 0) {}

    // 4. قراءة Current Count
    u32 current = in_apic(0x390);
    u32 apic_freq = (0xFFFFFFFF - current) * 100 / 128; 

    cpu_clock_f = apic_freq; /
    printc("\nspeed ",10);
    printc(Hex(apic_freq),10);
    */
    /*u32 t1,t2,sc;
    get_time(t1,t2,sc);
    u32 esc= sc+1;
    while (sc<esc) {
        get_time(t1,t2,sc);
    }*/
	
     /*outb(0x43, 0x30);// 0x30 = 00110000 (Channel 0, Access low/high byte, Mode 0)
    outb(0x40, dev&0xFF); 
    outb(0x40, (dev>>8)&0xFF);
    while(count!=0){/*a test on a real machine : mybe this will be good if i do ???*
        u8 low = inb(0x40);
        u8 hi = inb(0x40);
        count=(low<<8)|hi;
    }*/
     //outb(0x40, dev&0xFF); 
    //outb(0x40, (dev>>8)&0xFF);
    
    /*while(count!=0){
        u8 low = inb(0x40);
        u8 hi = inb(0x40);
        count=(low<<8)|hi;
    }*/
    //u32 t1,t2,sc;

    //asm("sti");
    /*while (sc<esc) {
        get_time(t1,t2,sc);
    }
    u32 start = rdtsc();
    esc = sc+1;
    while (sc<esc) {
        get_time(t1,t2,sc);
    }
    st_rtc=sc;
    */
}

void sleep(u32 milisecends){
    u64 secnd = tcount+milisecends;
    //pit_sleep(milisecends);
    while (tcount<secnd){
	//printc(Int_To_STR(secnd-tcount, 10),15);
	//printc("\n",15);
    }
    return;
}

void pit_sleep(u32 ms){
    u32 dev=ms*(1193182/1000);
    u32 trn = (dev/0xffff)+1;
    for(u32 i = 0 ; i < trn ; i++){
        outb(0x40, (dev-(0xffff*i))&0xFF); 
        outb(0x40, ((dev-(0xffff*i))>>8)&0xFF);
        u32 count = dev-(0xffff*i);
        while(count!=0){
            u8 low = inb(0x40);
            u8 hi = inb(0x40);
            count=(low<<8)|hi;
        }
    }
    return;
}

u64 hpet_get_frequency() {
    u64 cap = hpet->capabilities;
    printc("cap : ",10);
    printc(Hex(cap),14);
    printc("\nconfig",10);
    printc(Hex(hpet->configuration),14);
    printc("\nmaincounter : ",10);
    printc(Hex(hpet->main_counter),14);
    printc("\n",10);
    u64 femtoseconds = cap;//(u32)(cap >> 32);  // قيمة بالـ فيمتوثانية (1e-15s)
    return 1000000000000000ULL / femtoseconds;      // تحويل إلى Hz
}

void hpet_sleep_ms(u64 ms){
    hpet->configuration|=1;
    //hpet->main_counter=0;
    u64 frq = hpet_get_frequency();
    u64 time =(ms*frq)/1000;

    u64 end=hpet->main_counter+time;

    while(hpet->main_counter<end){}
    return;
}
