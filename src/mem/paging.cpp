#include "../utils/util.h"
#include "../idt/idt.h"
#include "../cpu/CPU.h"
#include "paging.h"
/*PS=0-->4kb
  PS=1-->4mb
todo*/
//volatile u32 PD[1024] __attribute__((aligned(4096)));
//volatile u32 PT[1024*1024] __attribute__((aligned(4096*1024))) ;
#define PT_FLAGS 0b000010111 //10011
#define PD_FLAGS 0b10111
//PageDir PD;
//PageTabel PT;
volatile u32 *PD;
volatile u32 *PT;
void exc1(regs* reg){
    //reg->eip+=2;
    printc("\ncr2 ",12);
    printc(Hex(reg->cr2),12);
    return;
}

void paging_init(){
    install_isr(11,exc1);
    //prtcmem((void*)(&PD), sizeof(u32)*1024);
    //prtcmem((void*)(&PT), sizeof(u32)*1024*1024);
    PD=(u32*)malloc_aligned(1024*sizeof(u32),0x1000);
    PT=(u32*)malloc_aligned(1024*1024*sizeof(u32),0x1000);
    u32 adrss=0x0000;
    for(u32 i = 0 ; i < 1024 ; i++){
        for(u32 ii = 0 ; ii<1024 ; ii++){
            //PT[i][ii].adrr1=((adrss>>12)<<4)&0xff;
            //PT[i][ii].adrr2=((adrss>>14))&0xffff;
            PT[i*1024+ii]=(adrss&0xfffff000)|PT_FLAGS;
            adrss+=0x1000;//4kb
        }
        PD[i]=((u32)&PT[i*1024])&(0xfffff000)|PD_FLAGS;
    }

    //printc("\npaging done!",10);
    //asm volatile("invlpg [%0]" :: "r"((u32)PD) /*: "memory"*/); 
    asm volatile ("movl %0 , %%cr3"::"r"((u32)PD));
    //asm volatile (
    //"mov %cr0, %eax\n"
    //"or 0x80000001 , %eax\n"
    //"mov %eax , %cr0");
    u32 cr0=0;
    asm("mov %%cr0 , %0":"=r"(cr0));
    cr0|=0x80000001;
    asm("mov %0 , %%cr0"::"r"(cr0));     
    //mov eax, cr0
    //or eax, 0x60000000     ; enable caching again
    //mov cr0, eax
    asm("mov %%cr0 , %0":"=r"(cr0));
    cr0|=0x60000000;
    asm("mov %0 , %%cr0"::"r"(cr0));     

    return;
}

void install_mainpd(){
    asm volatile("movl %0 , %%cr3"::"r"((u32)PD));
    u32 cr0;
    asm volatile("mov %%cr0 , %0":"=r"(cr0));
    cr0|=0x8000001;
    asm volatile("mov %0 , %%cr0"::"r"(cr0));
    
    return;
}

void install_pagedir(PageDir & pd ){
	asm volatile("movl %0 , %%cr3"::"r"((u32)pd));
    	u32 cr0=0;
	asm volatile("mov %%cr0 , %0":"=r"(cr0));
        cr0|=0x8000000;
        asm volatile("mov %0 , %%cr0"::"r"(cr0));

	return;
}

void init_pagedir(PageDir & pd , u32* pt , u32 pagecount){
	for(u32 i = 0 ; i < pagecount ; i++){
     	   pd[i]=((u32)&pt[i*1024])&(0xfffff000)|PD_FLAGS;
    	}
}


void link_page(u32 *pt , u32 wontadress , u32 pageadress){
    pt[(pageadress>>12)]=wontadress&(0xfffff000)|PT_FLAGS;
    return;
}

void link_page(u32 wontadress , u32 pageadress){
    PT[(pageadress>>12)]=wontadress&(0xfffff000)|PT_FLAGS;
    return;
}

void* physic_adress(u32* pt , u32 virtadrr){
    return (void*)(pt[virtadrr>>12]+virtadrr&0xfff);
}
void* physic_adress(u32 virtadrr){
    return (void*)(((PT[(virtadrr>>12)]&(0xfffff000))|(virtadrr&0xfff)));
}
