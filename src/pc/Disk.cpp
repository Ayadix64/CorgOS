#include "../cpu/pci.h"
#include "../mem/paging.h"
#include "../utils/util.h"
#include "../idt/idt.h"
#include "Disk.h"
#include "timer.h"
#include "ata.h"
/************************************detact strage devaces*************************************/
Device ATAs[12];
u32 ATAsnum;
Device SATAs[23];
u32 SATAsnum;
Device USBs[64];
u32 USBsnum;


DISK SATADisks[128];
u32 StatsDisksnum=0;

DISK ATADISK[4];
u32 ATADISKn = 0;

void diskhand(regs* reg){
    printc("\nDisk Handler",10);
    
    //asm("hlt");
    return;
}

void detact_streg(){
    install_irq(0xb,diskhand,0);
    ATAsnum=find_pci_devace((u8)1,(u8)5).num_of_devc;
    u32 *find=(u32*)find_pci_devace((u8)1,(u8)5).devace;
    if(ATAsnum){
        printc("\nwill , thay are a ATAs\n",15);
        for(int i = 0 ; i < ATAsnum ; i++){
            ATAs[i]=Devaces[find[i]];
        }
    }
    SATAsnum=find_pci_devace((u8)1,(u8)6).num_of_devc;
    find=(u32*)find_pci_devace((u8)1,(u8)6,1).devace;
    if(SATAsnum){
        printc("\nwill , thay are a SATAs controlors\n",15);
        for(int i = 0 ; i < SATAsnum ; i++){
            SATAs[i]=Devaces[find[i]];
        }
    }
    USBsnum=find_pci_devace((u8)0xc,(u8)3,(u8)0x20).num_of_devc;
    find=(u32*)find_pci_devace((u8)0xc,(u8)3,(u8)0x20).devace;
    if(USBsnum){
        printc("\nwill , thay are a USBs controlrs",15);
        printc(Hex(USBsnum),14);
        printc("\n",10);
        for(int i = 0 ; i < USBsnum ; i++){
            USBs[i]=Devaces[find[i]];
            u32 pciin=rd_pci(USBs[i].bus,USBs[i].device,USBs[i].func,0xc0);

            pciin|=(1<<24);
            pciin&=~(1<<16);

            wr_pci(USBs[i].bus,USBs[i].device,USBs[i].func,0xc0,pciin);
            pciin=rd_pci(USBs[i].bus,USBs[i].device,USBs[i].func,0xc0);
            int ee = 0x100000;
            while((!rd_pci(USBs[i].bus,USBs[i].device,USBs[i].func,0xc0)&((1<<24)))&&
                (rd_pci(USBs[i].bus,USBs[i].device,USBs[i].func,0xc0)&(1<<16)) && ee ){
                //pciin|=(1<<24);
                //pciin&=~(1<<16);

                //wr_pci(USBs[i].bus,USBs[i].device,USBs[i].func,0xc0,pciin);
                //pciin=rd_pci(USBs[i].bus,USBs[i].device,USBs[i].func,0xc0);
                ee--;
            }
            if(!ee){
                printc("\nfail to disabel BIOS USB owner ship .\n",12);
            }
            printc(Bin(pciin),(14<<4)|0);
            printc("\n",12);
        }

    }
}

/***********************************Uncool AHCI data structs************************/
typedef volatile struct {

    u32 clb;		// 0x00, command list base address, 1K-byte aligned
	u32 clbu;		// 0x04, command list base address upper 32 bits
	u32 fb;		// 0x08, FIS base address, 256-byte aligned
	u32 fbu;		// 0x0C, FIS base address upper 32 bits
	u32 is;		// 0x10, interrupt status
	u32 ie;		// 0x14, interrupt enable
	u32 cmd;		// 0x18, command and status
	u32 rsv0;		// 0x1C, Reserved
	u32 tfd;		// 0x20, task file data
	u32 sig;		// 0x24, signature
	u32 ssts;		// 0x28, SATA status (SCR0:SStatus)
	u32 sctl;		// 0x2C, SATA control (SCR2:SControl)
	u32 serr;		// 0x30, SATA error (SCR1:SError)
	u32 sact;		// 0x34, SATA active (SCR3:SActive)
	u32 ci;		// 0x38, command issue
	u32 sntf;		// 0x3C, SATA notification (SCR4:SNotification)
	u32 fbs;		// 0x40, FIS-based switch control
	u32 rsv1[11];	// 0x44 ~ 0x6F, Reserved
	u32 vendor[4];	// 0x70 ~ 0x7F, vendor specific
} HBA_PORT __attribute__((packed));;

typedef volatile struct 
{
    u8 CommandFIS:5;
    u8 ATAPI:1;
    u8 read_write:1;//0 read, 1 write
    u8 Pref:1;

    u8 reset:1;
    u8 Bist:1;
    u8 clearbusy:1;
    u8 rsv0:1;
    u8 multiport:4;
    u16 tabel_length;

    u32 trasferbyte;
    u32 Command_tabel_adrrlow;
    u32 Command_tabel_adrrhi;
    u32 rsv1[4];
}HBA_COMMAND __attribute__((packed));;

typedef volatile struct {
    volatile u32 cap;		// 0x00, Host capability
	volatile u32 ghc;		// 0x04, Global host control
	volatile u32 is;		// 0x08, Interrupt status
	volatile u32 pi;		// 0x0C, Port implemented
	volatile u32 vs;		// 0x10, Version
	volatile u32 ccc_ctl;	// 0x14, Command completion coalescing control
	volatile u32 ccc_pts;	// 0x18, Command completion coalescing ports
	volatile u32 em_loc;		// 0x1C, Enclosure management location
	volatile u32 em_ctl;		// 0x20, Enclosure management control
	volatile u32 cap2;		// 0x24, Host capabilities extended
	volatile u32 bohc;		// 0x28, BIOS/OS handoff control and status

	// 0x2C - 0x9F, Reserved
	u8  rsv[116];

	// 0xA0 - 0xFF, Vendor specific registers
	u8  vendor[96];

	// 0x100 - 0x10FF, Port control registers
    volatile HBA_PORT ports[32]; 
} HBA_MEM __attribute__((packed));;


typedef volatile struct 
{
    
    
    u8  fis_type=0x27;    // FIS_TYPE_REG_D2H
	u8  pmport:4;    // Port multiplier
	u8  rsv0:2;      // Reserved
	u8  i:1;         // Interrupt bit
    u8  c:1;      // Reserved

	u8  comand;      // Status register
	u8  error;       // Error register
	
	  // DWORD 1
	u8  lba0;        // LBA low register, 7:0
  	u8  lba1;        // LBA mid register, 15:8
	u8  lba2;        // LBA high register, 23:16
	u8  device;      // Device register

	// DWORD 2
	u8  lba3;        // LBA register, 31:24
	u8  lba4;        // LBA register, 39:32
	u8  lba5;        // LBA register, 47:40
	u8  rsv2;        // Reserved

	// DWORD 3
  	u8  countl;      // Count register, 7:0
	u8  counth;      // Count register, 15:8
	u8  rsv3[2];     // Reserved

	// DWORD 4
	u8  rsv4[4];     // Reserved
}HBA_FIS_H2D __attribute__((packed));;

typedef volatile struct 
{
	// DWORD 0
	u8  fis_type;    // FIS_TYPE_REG_D2H

	u8  pmport:4;    // Port multiplier
	u8  rsv0:2;      // Reserved
	u8  i:1;         // Interrupt bit
	u8  rsv1:1;      // Reserved

	u8  comand;      // Status register
	u8  error;       // Error register
	
	// DWORD 1
	u8  lba0;        // LBA low register, 7:0
	u8  lba1;        // LBA mid register, 15:8
	u8  lba2;        // LBA high register, 23:16
	u8  device;      // Device register

	// DWORD 2
	u8  lba3;        // LBA register, 31:24
	u8  lba4;        // LBA register, 39:32
	u8  lba5;        // LBA register, 47:40
	u8  rsv2;        // Reserved

	// DWORD 3
	u8  countl;      // Count register, 7:0
	u8  counth;      // Count register, 15:8
	u8  rsv3[2];     // Reserved

	// DWORD 4
	u8  rsv4[4];     // Reserved
} HBA_FIS_D2H __attribute__((packed));;

 // Reserved
typedef volatile struct {
     u32 bufferl;//the data location wich is have write data or read data
     u32 bufferhi;
     u32 rsv0;
     u32 bytecount:22;//up to 4MB most be N-1
     u16 rsv:9;
     u8 interupt_on_complite:1;
} HBA_PRDT_ENTRY __attribute__((packed));;



typedef volatile struct 
{
     u8 CFIS[64];
     u8 atapi_cmd[16];
     u8 rsv[48];
     HBA_PRDT_ENTRY entry[1];
}HBA_COMMAND_TABEL __attribute__((packed));;


/****************************************SATA Interface********************************************/
volatile HBA_MEM **HBAp;//[32];
volatile void*FISBA=0;
volatile void*hbacomlist_loc=0;
volatile void*hbacomtabel_loc=0;


void DiskInit(){
    FISBA=malloc_aligned(0x1000,1024);
    hbacomlist_loc=malloc_aligned(0x1000,1024);
    hbacomtabel_loc=malloc_aligned(0x1000,1024);
    HBAp = (HBA_MEM**)malloc(SATAsnum);
    //printc(Hex(SATAsnum),15);
    
    for(int i = 0 ; i < SATAsnum ; i++){
        //for(int ii = SATAs[i].base ; ii < 0xffffffff ; ii++){
            
        //}
	    

	//one time to minimaze the proces time and to avoid any memory fail in the futer
	
	/*i was think that you shold link it, but in the exprince , you shlodnt, yeah, you need a froge to eat lama with space shipe*/
	    
	//volatile void* ad1 = malloc_aligned(0x1000,0x1000);//malloc(0x1000);
        //void* ad2 = malloc(0x1000);
        //void* ad3 = malloc(0x1000);
        //link_page((u32)ad1,SATAs[i].bases[5]);
        //link_page((u32)ad2,SATAs[i].bases[5]+0x1000);
        //link_page((u32)ad3,SATAs[i].bases[5]+0x2000);
	
        HBAp[i]=(HBA_MEM*)SATAs[i].bases[5];
        if(HBAp[i]->cap2&0x1){
		printc("\nAHCI BIOS handoff ...",15);
		HBAp[i]->bohc|=1<<1; // geting OS owner
		u32 c = 0x100000;
		while (HBAp[i]->bohc&1<<4 && c) c--;
		if(c){
			printc("[OK].",10);
		}else{
			printc("[ERORR]",12);
		}
	}else{
		printc("\nNon BIOS/OS handoff.",15);
	}
	printc("\nSATA # ",15);
        printc(Int_To_STR(i,10),15);
        printc(" was setup in ",15);
        printc(Hex(SATAs[i].base),15);
        printc(" and linked secufuly by paging.",15);
    }
    
    prtcmem((void*)HBAp, sizeof(HBA_MEM)*32);
    SATAD_Update();
}



bool stopcmd(u32 sata , u32 drave){
    u32 i = 0;
    u32 maxi=0x10000;
    volatile HBA_PORT * port = &HBAp[sata]->ports[drave];
    while(port->tfd&0x88){i++;if(i>=maxi){printc("\nDisk read/write ERORR !!!", 12);return 0;}}
    i=0;
    port->cmd &= ~(1 << 0); // CLE=0
    port->cmd &= ~(0x10); // FRE=0

    while ((port->cmd & ((1 << 15)|(1 << 14)))) {i++;if(i==maxi){printc("\nDisk read/write ERORR !!!", 12);return 0;}} // CR=1?
    //while ((port->cmd & (1 << 14))) {} // FR=1?
    i=0;
    while (port->cmd &0x8000){i++;if(i==maxi){printc("\nDisk read/write ERORR !!!", 12);return 0;}}
    return 1;
}

bool startcmd(u32 sata , u32 drave){
    u32 i = 0;
    u32 maxi=0x10000;
    volatile HBA_PORT * port = &HBAp[sata]->ports[drave];
    while(port->tfd&0x88){i++;if(i>=maxi){printc("\nDisk read/write ERORR !!!", 12);return 0;}}
    i=0;
    while (port->cmd &0x8000){i++;if(i>=maxi){printc("\nDisk read/write ERORR !!!", 12);return 0;}}
    i=0;
    port->cmd |= (1 << 4); // FRE=1
    port->cmd |= (1 << 0); // CLE=1

    while (port->ci & (1<<drave)) {
        i++;
        if (port->is & (1 << 30)) { // TFES (Transmit FIS Error)
            printc("\nDisk read/write ERORR !!!", 12);
            return false;
        }
        if(i==maxi){
            printc("\nDisk read/write ERORR !!!", 12);
            return false;
        }

    }
    if (port->is & (1 << 30)) { // TFES (Transmit FIS Error)
        printc("\nDisk read/write ERORR !!!", 12);
        return false;
    }
    i=0;
    while(port->tfd&0x88) {i++;if(i>=maxi){printc("\nDisk read/write ERORR !!!", 12);return 0;}}
    return true;
}



//************************************************
//*TODO: VERY UNNSAFE MEMROY MANGMENT, get fix it*
//************************************************

//************************************************
//*TODO2: It is sleping 10 ms to get data, fix it*
//************************************************
bool read_sata(u64 lba , u16 cont , volatile u8*bufer , u32 sata , u32 drave){
    //sleep(200);
    if(!(HBAp[sata]->pi&(1<<drave))||sata>=SATAsnum){printc("\nthay are not disk!",12);return 0;}
    
    if(!stopcmd(sata,drave)){;return 0;}
    

    
    memset((u8)0,(void*)FISBA,0x1000);
    memset((u8)0,(void*)hbacomtabel_loc,0x1000);
    memset((u8)0,(void*)hbacomlist_loc,0x1000);
   // printc("\nthey are a disk!",10);
    
    
    volatile HBA_PORT *port = &HBAp[sata]->ports[drave];
    
    
    port->clb=(u32)(u64)physic_adress((u32)(u64)(hbacomlist_loc));
    port->fb=(u32)(u64)physic_adress((u32)(u64)(FISBA));
    HBAp[sata]->ghc|=0x80000000;

    volatile HBA_COMMAND *hbacomlist=(volatile  HBA_COMMAND *) port->clb;
    volatile HBA_COMMAND_TABEL *hbacommandtabel=(volatile  HBA_COMMAND_TABEL *)hbacomtabel_loc;
    
    volatile HBA_PRDT_ENTRY *hbaprentry = (volatile  HBA_PRDT_ENTRY *)&hbacommandtabel->entry[0];
    
    hbacomlist->Command_tabel_adrrlow=(u32)(u64)hbacommandtabel;
    hbacomlist->Command_tabel_adrrhi=0;
    
    hbacomlist->CommandFIS=sizeof(HBA_FIS_H2D)/4;
    hbacomlist->tabel_length= 1;
    hbacomlist->read_write=0;


    hbacommandtabel->entry[0].bufferl=(u32)(u64)physic_adress((u32)(u64)(bufer));
    hbacommandtabel->entry[0].bufferhi= 0;
    hbacommandtabel->entry[0].bytecount= cont*512 - 1;
    hbacommandtabel->entry[0].interupt_on_complite=1;

    HBA_FIS_H2D *hbacommfis = (HBA_FIS_H2D *)(u64)&hbacommandtabel->CFIS[0];
    hbacommfis->fis_type=0x27;
    hbacommfis->comand=0x25;
    hbacommfis->c=1;
    hbacommfis->lba0=(u8)lba&0xff;
    hbacommfis->lba1=(u8)(lba>>8)&0xff;
    hbacommfis->lba2=(u8)(lba>>16)&0xff;
    hbacommfis->lba3=(u8)(lba>>24)&0xff;
    hbacommfis->lba4=(u8)(lba>>32)&0xff;
    hbacommfis->lba5=(u8)(lba>>40)&0xff;

    hbacommfis->device=0x40;

    hbacommfis->countl=cont&0xff;
    hbacommfis->counth=(cont>>8)&0xff;

    
    //hbacommfis->pmport=0;
    //hbacommfis->i=1;
    //hbacommfis->rsv0=1;
    //port->sact=1;

    
    /*u8 *cfis =(u8*) hbacommandtabel->CFIS;
    cfis[0] = 0x27;
    cfis[1] = 0x80;
    cfis[2] = 0x25;
    cfis[7] = 0x40; // Device

    cfis[4] = (u8)lba;
    cfis[5] = (u8)(lba >> 8);
    cfis[6] = (u8)(lba >> 16);
    cfis[8] = (u8)(lba >> 24);
    cfis[9] = (u8)(lba >> 32);
    cfis[10] = (u8)(lba >> 40);

    cfis[12] = (u8)cont;
    cfis[13] = (u8)(cont >> 8);
    */

    if(startcmd(sata,drave)){
        port->ci=1<<drave;
        HBAp[sata]->ghc|=0x80000000;
        //sleep(5);
        //mfree((void*)FISBA,0x1000);
        //mfree((void*)hbacomtabel_loc,0x1000);
        //mfree((void*)hbacomlist_loc,0x1000);
        //asm volatile ("sti");
        return 1;
    }
    //mfree((void*)FISBA,0x1000);
    //mfree((void*)hbacomtabel_loc,0x1000);
    //mfree((void*)hbacomlist_loc,0x1000);
    //asm volatile ("sti");
    return 0;

}

bool write_sata(u64 lba , u16 cont , volatile u8*bufer , u32 sata , u32 drave){
    //sleep(200);
    if(!(HBAp[sata]->pi&(1<<drave))||sata>=SATAsnum){printc("\nthay are not disk!",12);return 0;}
    //asm volatile ("cli");
    if(!stopcmd(sata,drave)){return 0;}
    
    
    //volatile void*FISBA=malloc_aligned(0x1000,0x1000);//(u8*)((((u32)malloc(0x2000))&(~0xfff))+0x1000);//=((u8 *)(((u32)malloc(0x1000))&0x1000+0x1000));
    //volatile void*hbacomlist_loc=malloc_aligned(0x1000,0x1000);
    //volatile void*hbacomtabel_loc=malloc_aligned(0x1000,0x1000);
    memset((u8)0,(void*)FISBA,0x1000);
    memset((u8)0,(void*)hbacomtabel_loc,0x1000);
    memset((u8)0,(void*)hbacomlist_loc,0x1000);
    //printc("\nthey are a disk!",10);
    
    
    volatile HBA_PORT *port = &HBAp[sata]->ports[drave];
    
    
    port->clb=(u32)(u64)physic_adress((u32)(u64)(hbacomlist_loc));
    port->fb=(u32)(u64)physic_adress((u32)(u64)(FISBA));
    //HBAp[sata]->ghc|=0x80000000;

    volatile HBA_COMMAND *hbacomlist=(volatile  HBA_COMMAND *) port->clb;
    volatile HBA_COMMAND_TABEL *hbacommandtabel=(volatile  HBA_COMMAND_TABEL *)hbacomtabel_loc;
    
    volatile HBA_PRDT_ENTRY *hbaprentry = (volatile  HBA_PRDT_ENTRY *)&hbacommandtabel->entry[0];
    
    hbacomlist->Command_tabel_adrrlow=(u32)(u64)hbacommandtabel;
    hbacomlist->Command_tabel_adrrhi=0;
    
    hbacomlist->CommandFIS=sizeof(HBA_FIS_H2D)/4;
    hbacomlist->tabel_length= 1;
    hbacomlist->read_write=1;


    hbacommandtabel->entry[0].bufferl=(u32)(u64)physic_adress((u32)(u64)(bufer));
    hbacommandtabel->entry[0].bufferhi= 0;
    hbacommandtabel->entry[0].bytecount= cont*512 - 1;
    hbacommandtabel->entry[0].interupt_on_complite=1;

    HBA_FIS_D2H *hbacommfis = (HBA_FIS_D2H *)(u64)&hbacommandtabel->CFIS[0];
    hbacommfis->fis_type=0x27;
    hbacommfis->comand=0x35;
    hbacommfis->rsv1=1;
    hbacommfis->lba0=(u8)lba&0xff;
    hbacommfis->lba1=(u8)(lba>>8)&0xff;
    hbacommfis->lba2=(u8)(lba>>16)&0xff;
    hbacommfis->lba3=(u8)(lba>>24)&0xff;
    hbacommfis->lba4=(u8)(lba>>32)&0xff;
    hbacommfis->lba5=(u8)(lba>>40)&0xff;

    hbacommfis->device=0x40;

    hbacommfis->countl=cont&0xff;
    hbacommfis->counth=(cont>>8)&0xff;

    

    if(startcmd(sata,drave)){
        port->ci=1<<drave;
        HBAp[sata]->ghc|=0x80000000;
        sleep(5);
        //mfree((void*)FISBA,0x1000);
        //mfree((void*)hbacomtabel_loc,0x1000);
        //mfree((void*)hbacomlist_loc,0x1000);
        //asm volatile ("sti");
        return 1;
    }
    //asm volatile ("sti");
    //mfree((void*)FISBA,0x1000);
    //mfree((void*)hbacomtabel_loc,0x1000);
    //mfree((void*)hbacomlist_loc,0x1000);
    return 0;

}



/*
bool read_sata(u64 lba , u16 cont , volatile u8*bufer , u32 sata , u32 drave){
    //sleep(200);
    if(!(HBAp[sata]->pi&(1<<drave))||sata>=SATAsnum){printc("\nthay are not disk!",12);return 0;}
    
    if(!stopcmd(sata,drave)){return 0;}
    
    
    
    volatile u8 FISBA[1024] ALIGNED(256);//=malloc_aligned(0x1000,0x1000);//(u8*)((((u32)malloc(0x2000))&(~0xfff))+0x1000);//=((u8 *)(((u32)malloc(0x1000))&0x1000+0x1000));

    memset((u8)0,(void*)FISBA,512);

    //printc("\nthey are a disk!",10);
    
    
    volatile HBA_PORT *port = &HBAp[sata]->ports[drave];
    
    volatile HBA_COMMAND hbacomlist ALIGNED(1024);
    volatile HBA_COMMAND_TABEL hbacommandtabel ALIGNED(64);

    prtcmem((void*)FISBA, 512); 
    prtcmem((void*)&hbacomlist, 512); 
    prtcmem((void*)&hbacommandtabel, 512);


    port->clb=(u32)(u64)physic_adress((u32)(u64)(&hbacomlist));
    port->fb=(u32)(u64)physic_adress((u32)(u64)(FISBA));

    HBAp[sata]->ghc|=0x80000000;


    volatile HBA_PRDT_ENTRY *hbaprentry = (volatile  HBA_PRDT_ENTRY *)&hbacommandtabel.entry[0];
    
    hbacomlist.Command_tabel_adrrlow=(u32)(u64)&hbacommandtabel;
    hbacomlist.Command_tabel_adrrhi=0;
    
    hbacomlist.CommandFIS=sizeof(HBA_FIS_H2D)/4;
    hbacomlist.tabel_length= 1;
    hbacomlist.read_write=0;


    hbacommandtabel.entry[0].bufferl=(u32)(u64)physic_adress((u32)(u64)(bufer));
    hbacommandtabel.entry[0].bufferhi= 0;
    hbacommandtabel.entry[0].bytecount= cont*512 - 1;
    hbacommandtabel.entry[0].interupt_on_complite=1;

    HBA_FIS_H2D *hbacommfis = (HBA_FIS_H2D *)(u64)&hbacommandtabel.CFIS[0];
    hbacommfis->fis_type=0x27;
    hbacommfis->comand=0x25;
    hbacommfis->c=1;
    hbacommfis->lba0=(u8)lba&0xff;
    hbacommfis->lba1=(u8)(lba>>8)&0xff;
    hbacommfis->lba2=(u8)(lba>>16)&0xff;
    hbacommfis->lba3=(u8)(lba>>24)&0xff;
    hbacommfis->lba4=(u8)(lba>>32)&0xff;
    hbacommfis->lba5=(u8)(lba>>40)&0xff;

    hbacommfis->device=0x40;

    hbacommfis->countl=cont&0xff;
    hbacommfis->counth=(cont>>8)&0xff;

    
    //hbacommfis->pmport=0;
    //hbacommfis->i=1;
    //hbacommfis->rsv0=1;
    //port->sact=1;

    
    /*u8 *cfis =(u8*) hbacommandtabel->CFIS;
    cfis[0] = 0x27;
    cfis[1] = 0x80;
    cfis[2] = 0x25;
    cfis[7] = 0x40; // Device

    cfis[4] = (u8)lba;
    cfis[5] = (u8)(lba >> 8);
    cfis[6] = (u8)(lba >> 16);
    cfis[8] = (u8)(lba >> 24);
    cfis[9] = (u8)(lba >> 32);
    cfis[10] = (u8)(lba >> 40);

    cfis[12] = (u8)cont;
    cfis[13] = (u8)(cont >> 8);

    *

    if(startcmd(sata,drave)){
        port->ci=1<<drave;
        HBAp[sata]->ghc|=0x80000000;
        /*while(port->sact&1){
            asm("nop");
        }*
        sleep(50);

        return 1;
    }

    return 0;

}
/*
bool write_sata(u64 lba , u16 cont , volatile u8*bufer , u32 sata , u32 drave){
    //sleep(200);
    if(!(HBAp[sata]->pi&(1<<drave))||sata>=SATAsnum){printc("\nthay are not disk!",12);return 0;}
    //asm volatile ("cli");
    if(!stopcmd(sata,drave)){;return 0;}
    
    
    volatile u8 FISBA[1024] ALIGNED(256);//=malloc_aligned(0x1000,0x1000);//(u8*)((((u32)malloc(0x2000))&(~0xfff))+0x1000);//=((u8 *)(((u32)malloc(0x1000))&0x1000+0x1000));
    memset((u8)0,(void*)FISBA,512);

    //printc("\nthey are a disk!",10);
    
    
    volatile HBA_PORT *port = &HBAp[sata]->ports[drave];
    
    volatile HBA_COMMAND hbacomlist ALIGNED(1024);
    volatile HBA_COMMAND_TABEL hbacommandtabel ALIGNED(64);

    port->clb=(u32)(u64)physic_adress((u32)(u64)(&hbacomlist));
    port->fb=(u32)(u64)physic_adress((u32)(u64)(FISBA));
    //HBAp[sata]->ghc|=0x80000000;
    
    //it like to i have to protect momery even it is in compuler time, yeah
    prtcmem((void*)FISBA, 512); 
    prtcmem((void*)&hbacomlist, 512); 
    prtcmem((void*)&hbacommandtabel, 512);

    volatile HBA_PRDT_ENTRY *hbaprentry = (volatile  HBA_PRDT_ENTRY *)&hbacommandtabel.entry[0];
    
    hbacomlist.Command_tabel_adrrlow=(u32)(u64)&hbacommandtabel;
    hbacomlist.Command_tabel_adrrhi=0;
    
    hbacomlist.CommandFIS=sizeof(HBA_FIS_H2D)/4;
    hbacomlist.tabel_length= 1;
    hbacomlist.read_write=1;


    hbacommandtabel.entry[0].bufferl=(u32)(u64)physic_adress((u32)(u64)(bufer));
    hbacommandtabel.entry[0].bufferhi= 0;
    hbacommandtabel.entry[0].bytecount= cont*512 - 1;
    hbacommandtabel.entry[0].interupt_on_complite=1;

    HBA_FIS_D2H *hbacommfis = (HBA_FIS_D2H *)(u64)&hbacommandtabel.CFIS[0];
    hbacommfis->fis_type=0x27;
    hbacommfis->comand=0x35;
    hbacommfis->rsv1=1;
    hbacommfis->lba0=(u8)lba&0xff;
    hbacommfis->lba1=(u8)(lba>>8)&0xff;
    hbacommfis->lba2=(u8)(lba>>16)&0xff;
    hbacommfis->lba3=(u8)(lba>>24)&0xff;
    hbacommfis->lba4=(u8)(lba>>32)&0xff;
    hbacommfis->lba5=(u8)(lba>>40)&0xff;

    hbacommfis->device=0x40;

    hbacommfis->countl=cont&0xf;
    hbacommfis->counth=(cont>>8)&0xff;

    

    if(startcmd(sata,drave)){
        port->ci=1<<drave;
        HBAp[sata]->ghc|=0x80000000;
        /*while(port->sact&1){
            asm("nop");
        }*
        sleep(50);
        return 1;
    }
    return 0;

}
*/
bool get_sata_info(SATA_INDEFTY& indef,u32 sata , u32 drave){
    //SATA_INDEFTY indef;
    if(!(HBAp[sata]->pi&(1<<drave))||sata>=SATAsnum){printc("\nthay are not disk!",12);return 0;}
    if(!stopcmd(sata,drave)){return 0;}

    
    volatile void*FISBA=malloc_aligned(0x1000,0x1000);//(u8*)((((u32)malloc(0x2000))&(~0xfff))+0x1000);//=((u8 *)(((u32)malloc(0x1000))&0x1000+0x1000));
    volatile void*hbacomlist_loc=malloc_aligned(0x1000,0x1000);
    volatile void*hbacomtabel_loc=malloc_aligned(0x1000,0x1000);
    memset((u8)0,(void*)FISBA,0x1000);
    memset((u8)0,(void*)hbacomtabel_loc,0x1000);
    memset((u8)0,(void*)hbacomlist_loc,0x10000);
    //printc("\nthey are a disk!",10);
    
    
    volatile HBA_PORT *port = &HBAp[sata]->ports[drave];
    
    
    port->clb=(u32)(u64)(hbacomlist_loc);
    port->fb=(u32)(u64)(FISBA);
    HBAp[sata]->ghc|=0x80000000;
    volatile HBA_COMMAND *hbacomlist=(volatile  HBA_COMMAND *) port->clb;
    volatile HBA_COMMAND_TABEL *hbacommandtabel=(volatile  HBA_COMMAND_TABEL *)hbacomtabel_loc;
    volatile HBA_PRDT_ENTRY *hbaprentry = (volatile  HBA_PRDT_ENTRY *)&hbacommandtabel->entry[0];
    
    hbacomlist->Command_tabel_adrrlow=(u32)(u64)hbacommandtabel;
    hbacomlist->Command_tabel_adrrhi=0;
    
    hbacomlist->CommandFIS=sizeof(HBA_FIS_H2D)/4;
    hbacomlist->tabel_length= 1;
    hbacomlist->read_write=0;


    hbacommandtabel->entry[0].bufferl=(u32)(u64)(&indef);
    hbacommandtabel->entry[0].bufferhi= 0;
    hbacommandtabel->entry[0].bytecount= 512 - 1;
    hbacommandtabel->entry[0].interupt_on_complite=1;

    HBA_FIS_H2D *hbacommfis = (HBA_FIS_H2D *)(u64)&hbacommandtabel->CFIS[0];
    hbacommfis->fis_type=0x27;
    hbacommfis->comand=0xec;
    hbacommfis->c=1;
    hbacommfis->lba0=(u8)0;
    hbacommfis->lba1=(u8)0;
    hbacommfis->lba2=(u8)0;
    hbacommfis->lba3=(u8)0;
    hbacommfis->lba4=(u8)0;
    hbacommfis->lba5=(u8)0;

    hbacommfis->device=0x40;

    hbacommfis->countl=1;
    hbacommfis->counth=0;

    
    //hbacommfis->pmport=0;
    //hbacommfis->i=1;
    //hbacommfis->rsv0=1;
    //port->sact=1;

    
    /*u8 *cfis =(u8*) hbacommandtabel->CFIS;
    cfis[0] = 0x27;
    cfis[1] = 0x80;
    cfis[2] = 0x25;
    cfis[7] = 0x40; // Device

    cfis[4] = (u8)lba;
    cfis[5] = (u8)(lba >> 8);
    cfis[6] = (u8)(lba >> 16);
    cfis[8] = (u8)(lba >> 24);
    cfis[9] = (u8)(lba >> 32);
    cfis[10] = (u8)(lba >> 40);

    cfis[12] = (u8)cont;
    cfis[13] = (u8)(cont >> 8);

    */
    if(startcmd(sata,drave)){
        port->ci=1<<drave;
        HBAp[sata]->ghc|=0x80000000;
        mfree((void*)FISBA,0x1000);
        mfree((void*)hbacomtabel_loc,0x1000);
        mfree((void*)hbacomlist_loc,0x1000);
        return 1;
    }
    mfree((void*)FISBA,0x1000);
    mfree((void*)hbacomtabel_loc,0x1000);
    mfree((void*)hbacomlist_loc,0x1000);
    return 0;

}
/*************************detact disks lol***********************/


extern "C" DISK bootdisk  = {._ATA=0,._SATA=0,._USB=0};
extern "C" bool bootfind=false;
void SATAD_Update(){ 
    u8 buf[1024];
    StatsDisksnum=0;
    ATADISKn=0;
    bootfind=0;
    memset((u8)0,(void*)&bootdisk,sizeof(DISK));
    memset((u8)0,ATADISK,sizeof(ATADISK));
    memset((u8)0,SATADisks,sizeof(SATADisks));
    // if they are not a SATA controler it will while i = 0 ; i < 0 ,it will just pased
    for(int i = 0  ; i<SATAsnum ; i++){
        for(int ii = 0 ; ii< 30 ; ii++){
            if(HBAp[i]->pi&(1<<ii)){
                
		read_sata(BOOT_SEG_LBA, 1, (volatile u8 *)buf, i, ii);
		if(memcmp((void*)BOOT_SEGN,(void*)buf,BOOT_SEG_LONG) && !bootfind){
			bootfind=true;
			bootdisk._SATA=true;
			bootdisk.index1=i;
			bootdisk.index2=ii;
			printc("\nBoot Disk is SATA AHCI at port ",15);
			printc(Hex(ii),15);
	
		}
		else {
			SATADisks[SATAsnum]._SATA=true;
			SATADisks[StatsDisksnum].index1=i;
                	SATADisks[StatsDisksnum].index2=ii;
                	StatsDisksnum++;
		}
            }
        } } 
    for(int i = 0 ; i < 4 ; i++){
	u8 dev = ATAchaeck(i);
	if(dev&1 && !(dev&2)){
		ATARead(BOOT_SEG_LBA, 1, (u8 *)buf, i);
		if(memcmp((void*)BOOT_SEGN,(void*)buf,BOOT_SEG_LONG) && !bootfind){
			bootfind=true;
			bootdisk._ATA=true;
			bootdisk.index1=i;
			printc("\nBoot Disk is ATA IDE at port ",15);
			printc(Hex(i),15);
		}else{
			ATADISK[ATADISKn]._ATA=1;
			ATADISK[ATADISKn].index1=i;
			ATADISKn++;
		}

	}
    }
    printc("\n they are ",12);
    printc(Int_To_STR(StatsDisksnum,10),12);
    printc(" SATA disk .\n",12);
    
    printc("\n they are ",12);
    printc(Int_To_STR(ATADISKn,10),12);
    printc(" ATA IDE disk .\n",12);

    return;
}
/*finaly, it end , this hill it just let me think to crach my computer evry time i lock on it, heey!?*/
