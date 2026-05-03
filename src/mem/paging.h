#ifndef PAGING_H
#define PAGING_H
#include "../utils/util.h"
#include "../idt/idt.h"
#include "../cpu/CPU.h"
/*typedef struct PageDirectory{
    u8 config=0b11000000;
              //0-->present
              //1-->R/W
              //2-->User/Supervisor , 0 , acess by all , 1 only kernel
              //3-->Write-Through
              //4-->Cache Disabel
              //5-->Acssed, set it to 1 i think
              //6-->AVL do with it what you wont
              //7-->PS , 0 --> 4kb , 1-->4mb
    u8 adrr1;//(0-->3)AVL , waht you wont
             //(3-->7)Adrr
    u16 adrr2;
}__attribute__((packed));

typedef struct PageTabel{
    u8 config=0b11000000;
              //0-->present
              //1-->R/W
              //2-->User/Supervisor , 0 , acess by all , 1 only kernel
              //3-->Write-Through
              //4-->Cache Disabel
              //5-->Acssed, set it to 1 i think
              //6-->AVL do with it what you wont
              //7-->PS , 0 --> 4kb , 1-->4mb
    u8 adrr1;//(0-->3)AVL , waht you wont
             //(3-->7)Adrr
    u16 adrr2;
}__attribute__((packed));/*the same thing*
heh, mybe i will use it */

typedef volatile u32 PageDir[1024]    __attribute__((aligned(4096)));
typedef volatile u32 PageTabel[1024*1024]  __attribute__((aligned(4096*1024)));

void paging_init();

void install_mainpd();
void install_pagedir(PageDir & pd);
void init_pagedir(PageDir & pd , u32* pt , u32 pagecount);

void link_page(u32* pt , u32 wontadress , u32 pageadress);
void link_page(u32 wontadress , u32 pageadress);

void* physic_adress(u32* pt , u32 virtadrr);
void* physic_adress(u32 virtadrr);
#endif

