#include "../utils/util.h"
#include "../gdt/gdt.h"
#include "../cpu/CPU.h"
#include "../idt/idt.h"
#include "../pc/timer.h"
#include "../pc/keyboard.h"
#include "../mem/paging.h"
#include "../cpu/pci.h"
#include "../pc/Disk.h"
#include "../pc/Speker.h"
#include "../filesys/1fat16.h"
#include "../multitask/multitask.h"
#include "../pc/ps2mouse.h"
#include "../cpu/smp.h"
using namespace FAT12;



void Shell();

extern "C" void main(){
	*(char*)0xb8000='A';
   
    TTLclear();
    printc("plz press Enter whan you ready ....\n",14);
    while(inb(0x60)!=0x9c){
	//outb(0xe5,'A');
    }
    while(!inb(0x60)&0x80){}
    
    for(int i = 0 ; i < 1000 ; i++){}
    printc("*****START32******\n",10);
    printc("Interuupt test ....\n",14);
     
    asm("cli");
    idt_init();
    DMem_init();
    gdt_init();
     
    //int syscrash = 1/0;
    //u64 cpu_frq = CPU_info::get_cpu_freqence();
    //printc("CPU speed : ",10);
    //printc(Hex((u64)cpu_frq),10);
    asm("sti");
    
    paging_init();
    initCors();

    keybrard_init();
    timer_init(); 
    mouse_init();

    u8 ff=inb(0x60);//if we not doing this, and the user press key at time is the intrupts ignored, the key boeard will stop do interupt to inbyte his port
    detact_divace();
    detact_streg();
    ff=inb(0x60);
    DiskInit();
    //USBInit();
    

    //printc((char*)(0x10000+127*512),(u32)512*50,12);
    printc("\nCPU name : ",10);
    printc(CPU_info::get_cpu_name(),(u32)48,14);
    //sleep(1000);

    printc("\nInteruupt test done ....\n",14);
   	printc("\nMemory (RAM) size install : ",15);
	printc(Int_To_STR(get_mem_info().Extend2,10),10);
	printc(" byte installed  .\n",15);
    printc("Warning : ",12);
    printc("ths system is 32bit, so it can use just the first 4GB of RAM, no more!\n",15);
       

    printc("\ninstalled ",14);
    printc(Hex(devacenum),14);
    printc("\nboot seg ",14);
    printc(Hex(*(u64*)(0x7e00)),14);
    
    Shell();

    printc("\nSystem Helted : out of Shell",12);
    asm volatile("hlt");
    while (1) {}
}



