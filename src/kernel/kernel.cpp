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
    //print_divaces();
    DiskInit();
    //read_sata(131,70,(volatile u8*)(0x18000),0,0);
    //USBInit();
    

    //printc((char*)(0x10000+127*512),(u32)512*50,12);
    printc("\nCPU name : ",10);
    printc(CPU_info::get_cpu_name(),(u32)48,14);
    //sleep(1000);

    printc("\nInteruupt test done ....\n",14);
    
    //printc("\npit test , sleep 1 s...\n",10);
    //pit_sleep(1000);
    //hpet_sleep_ms(1);
    //printc("\npit test end\n",10);
    //u32 x=0;
    //multitk_init();
    /*while(1){
      //  printc("\r",10);
       // printc((char*)(char[2]){"\\|/-"[x%4],0},10);
        x++;
        
        //char l=inb(0x60);
    }*/

    //start_read_sector8(KERNEL_START_LBA,KERNEL_LONG,(u8*)KERNEL_POINTER);
    /*HARD_DISK_INFO* dinfo;
    //disk_info((HARD_DISK_INFO*)dinfo);
    printc("Device_statue : ",15);
	printc(Int_To_STR(dinfo[0].General_Config,10),14);

	printc("\nCylinders number : ",15);
	printc(Int_To_STR(dinfo[0].Cylinders,10),14);

	printc("\nHeads nummber : ",15);
	printc(Int_To_STR(dinfo[0].Heads,10),14);

	printc("\nSectors per track : ",15);
	printc(Int_To_STR(dinfo[0].Track_Sectors,10),14);

	printc("\nSrial number : ",15);
	/*for(int i = 0 ; i <8 ; i++){
		if(dinfo[0].Serial_number[i]==0)dinfo[0].Serial_number[i]=' ';
	}*
	printc((char*)dinfo[0].Serial_Number,14);

	
	printc("\nATA vertion : ",15);
	printc(Int_To_STR(dinfo[0].ATA_Version_Major,10),14);
	

	printc("\nModel name : ",15);
	/*for(int i = 0 ; i <16 ; i++){
		if(dinfo[0].Model_name[i]==0)dinfo[0].Model_name[i]=' ';
	}*
	printc((char*)dinfo[0].Model_Name,14);
	

	printc("\nSector size : ",15);
	printc(Int_To_STR(dinfo[0].Sector_Bytes,10),14);
	
	printc("\nLBA count : ",15);
	printc(Int_To_STR(dinfo[0].LBA_28bit_Count,10),14);

    printc("\n\n\nReading the disk end .....",11);*/

    /*Mem*/	
    
    
	printc("\nMemory (RAM) size install : ",15);
	printc(Int_To_STR(get_mem_info().Extend2,10),10);
	printc(" byte installed  .\n",15);
    printc("Warning : ",12);
    printc("ths system is 32bit, so it can use just the first 4GB of RAM, no more!\n",15);
    u8* Aray=(u8*)malloc(4096);
    printc("\n\nDaynamick MEM ....\nallocating a araay in adress ",15);
    printc(Int_To_STR((u32)(Aray),10),10);
    if(!Aray){
    	printc(" .FAIL!!!",12);
    }
    
    u8* Aray2=(u8*)malloc(4096);
    printc("\nallocating a araay in adress ",15);
    printc(Int_To_STR((u32)(Aray2),10),10);
    if(!Aray2){
	printc(" .FAIL!!!",12);
    }
    u8* Aray3=(u8*)malloc(4094);
    printc("\nallocating a araay in adress ",15);
    printc(Int_To_STR((u32)(Aray3),10),10);
    if(!Aray3){
	printc(" .FAIL!!!",12);
    }
    mfree((void*)Aray2,4096);
    
    //u8 hi = inb(0x60);

    

    printc("\ninstalled ",14);
    printc(Hex(devacenum),14);
    printc("\nboot seg ",14);
    printc(Hex(*(u64*)(0x7e00)),14);

   /* printc("\nhey , this is a sata read test , hope the best",10);
    printc("\n***************** ",14);
    sleep_print("At first, reading Indfinty data from SATA#1",10);
    printc(" *****************\n",14);
    SATA_INDEFTY indefinty;
    get_sata_info(indefinty,0,0);
    sleep_print("Model Name : ",10);
    sleep_print((char*)indefinty.model,14,80,5);
    sleep_print("\nLBA : ",10);
    /*if(indefinty.maxlba48){
       sleep_print(Hex(indefinty.maxlba48),14);
    }* if(indefinty.maxlba28)
    {
        u32 lab= indefinty.maxlba28;
        for(int i = 0 ; i < 2  ; i++){
            ((u16*)&lab)[i]=switche_byte16(((u16*)&indefinty.maxlba28)[i]);
        }
        sleep_print(Hex(indefinty.maxlba28),14);
    }
    
    sleep_print("\n*******************************************************************************\n",14);
    //sleep(5000);
    u8 bufer[513];
    u8 bufer2[513];
    //sleep(100);
    
    
    if(read_sata(0,1,(volatile u8*)bufer,0,0)){
        for(int i  = 0 ; i < 512/8 ; i++){
            printc(Hex(((u64*)bufer)[i]),10);
        }
        for(int i  = 0 ; i < 512 ; i++){
            print_char(bufer[i],10);
        }
        //sleep(2000);
    }
    for(int i = 0 ; i < 512 ; i++){
        bufer[i]='_';
    }
    write_sata(2,1,(volatile u8*)bufer,0,0);
    sleep(100);
    for(int i = 0 ; i < 512 ; i++){
        bufer[i]=0;
    }

    //sleep(1000);
    if(read_sata(2,1,(volatile u8*)bufer2,0,0)){
        //Speker_Note(10000);
        //start_read_sector8(0,1,(u8*)bufer);
        for(int i  = 0 ; i < 512/8 ; i++){
            printc(Hex(((u64*)bufer2)[i]),10);
        }
        for(int i  = 0 ; i < 512 ; i++){
            print_char(bufer2[i],10);
        }
    }*/
    u8* buf =(u8*)malloc_aligned(5000,0x1000) ;
    u8* bufli = (u8*)malloc_aligned(0x1000,0x1000);
    //memset((u8)'R',buf,5000);
    /*link_page((u32)buf,(u32)0xfeee0000);
    memset((u8)'R',buf,5000);
    printc("\nthe real adrss of virtual adress  ",14);
    printc(Hex((u32)0xfeee0000),15);
    printc(" is ",14);
    printc(Hex((u32)physic_adress((u32)0xfeee0000)),10);
    printc((char*)0xfeee0000,(u32)1000,15); a paging test , you probaly dont wont to do this in real machine , at list with this regen*/
    u8 pk;
    /*int o = 0;
    while(o < 10){
        sleep(1000);
        printc("\n",10);
        printc(Hex(o),10);
        o++;
    }*/
    //sleep(10000);
    /*paink("BOOM, your pc will explude in 3s"); */
    Shell();
    //asm volatile("jmp $0x8200");
    //goto KERNEL_POINTER;
    printc("\nSystem Helted : out of Shell",12);
    asm volatile("hlt");
    while (1) {}
}



