#include "../utils/util.h"
#include "pci.h"

#define PCI_COMMAND 0xCF8
#define PCI_DATA 0xCFC

#define IDE_Controller      0x01	0x01	
#define SATA_Controller     0x01	0x06	
#define Ethernet_Controller 0x02	0x00	
#define VGA_Compatible      0x03	0x00	 //Display Controller
#define Host_Bridge         0x06	0x00	
#define USB_Controller      0x0C	0x03	
#define SD                  0x0C	0x05

Device *Devaces;
u32 devacenum=0;
PCI_Res_Find find;
/*char* devices_nams[13][7]{
    {"non defined","non defined","non defined","non defined","non defined","non defined","non defined"},
    {"SCSI","IDE","Floppy","IPI","RAID","ATA","SATA contreler"},
    {"Ethernet controler","Ethernet","Ethernet","Ethernet","Ethernet","Ethernet","Ethernet"},
    {"VGA_Compatible","XGA_Compatible","3D","Grafics","Grafics","Grafics","Grafics"},
    {"Video","Audio","Phone","Audio devace","non defined","non defined","non defined"},
    {"RAM","Flash","non defined","non defined","non defined","non defined","non defined"},
    {"Host_Bridge","ISA","EISA","MicroChanel","PCI-to-PCI","PCMCIA","NuBus"},
    {"Serial","Parallel","Multiport Serial","Modem","GPIB","SmartCard","non defined"},
    {"PCI","DMA","Timer","RTC","Hot-plug","SD Host","IOMMU"},
    {"Keyboard","Digitizer Pen","Mouse","Scanner","Gameport","non defined","non defined"},
    {"Generic","non defined","non defined","non defined","non defined","non defined","non defined"},
    {"cpu 386","cpu 486","cpu pentume","Alpha cpu","Power PC","MIPS","CO-Processor"},
    {"Extern Strage devace","Extern Strage devace","Extern Strage devace","USB_Controller","Extern Strage devace","SD Strage devace","Extern Strage devace"}
};*/

u32 rd_pci(u8 bus, u8 device , u8 func , u8 offset){
    u32 adress = (1<<31)
                 |(bus<<16)
                 |((device&0x1f)<<11)
                 |((func&7)<<8)
                 |(((u32)offset)&0x3f);
    outl(PCI_COMMAND,adress);
    return inl(PCI_DATA);
}

void wr_pci(u8 bus, u8 device , u8 func , u8 offset,u32 val){
    u32 adress = (1<<31)
                 |(bus<<16)
                 |((device&0x1f)<<11)
                 |((func&7)<<8)
                 |(((u32)offset)&0x3f);
    outl(PCI_COMMAND,adress);
    outl(PCI_DATA,val);
    return;
}


u32 device_info(u8 bus, u8 divice, u8 func ){
    return rd_pci(bus,divice,func,0);
}
void detact_divace(){
    devacenum=0;
    find.devace = (u32*)malloc(256*sizeof(u32));
    u32 max_dev = 0x1000/sizeof(Device);
    Devaces = (Device*)malloc(0x1000 / sizeof(Device));
    for(u16 bus = 0 ; bus < 256 ; bus ++){
        for(u8 device = 0 ; device < 32 ; device++){
            for(u8 func = 0 ; func < 8 ; func++){
                u16 vendor = rd_pci(bus,device,func,0)&0xffff;
                u16 dev = (rd_pci(bus,device,func,0)>>16)&0xffff;

                if(vendor!=0xffff){
		    if(devacenum>=max_dev){
			realloc((void*)Devaces, max_dev*sizeof(Device), (max_dev + 0x1000/sizeof(Device))*sizeof(Device));
		    	max_dev += 0x1000/sizeof(Device);
		    }
                    Devaces[devacenum].classcode= (rd_pci(bus, device, func, 0x08) >> 24) & 0xFF;
                    Devaces[devacenum].subclass_code = (rd_pci(bus, device, func, 0x08) >> 16) & 0xFF;
                    Devaces[devacenum].prog_if = (rd_pci(bus, device, func, 0x08) >> 8) & 0xFF;
                    Devaces[devacenum].IRQ=(rd_pci(bus, device, func, 0x3c) ) & 0xFF;

                    Devaces[devacenum].bus=bus;
                    Devaces[devacenum].device=device;
                    Devaces[devacenum].func=func;

                    Devaces[devacenum].DeviceID=dev;
                    Devaces[devacenum].VindorID=vendor;


                    for(int i = 0 ; i < 6 ; i++){
                        u32 adress=rd_pci(bus, device, func, 0x10+i*4)&0xffffffff;
                        if(adress&0x2){
                            Devaces[devacenum].adrasing64=true;
                        }
                        Devaces[devacenum].bases[i]=adress&0xfffffff0;
                        if(Devaces[devacenum].bases[i]!=0xfffffff0 &&
                           Devaces[devacenum].bases[i]!=0 &&
                          (Devaces[devacenum].bases[i]>Devaces[devacenum].base)){
                            Devaces[devacenum].base=adress&0xfffffff0;
                        }
                       
                        Devaces[devacenum].suport_MMIO=(!(adress&0x1));
                    }
                    if(Devaces[devacenum].base<0x10000000){//this is not good
                        //wr_pci(bus,device,func,0x10,0xffaa0000/*((u32)unloked_malloc_aligned(0x4000,0x1000,0x10000000,0xffffffff))*/|(Devaces[devacenum].base&0xf));
                        //Devaces[devacenum].base=rd_pci(bus,device,func,0x10)&0xfffffff0;
                        //Devaces[devacenum].bases[0]=rd_pci(bus,device,func,0x10)&0xfffffff0;
                        //(u32)unloked_malloc_aligned(0x4000,0x1000,0x10000000,0xffffffff);
                        //dont work :(
                    }
                    for(int i = 0 ; i < 6 ; i++){
                        prtcmem((void*)Devaces[devacenum].bases[i],0x4000);
                        
                    }
                    devacenum++;
                }
            }
        }
    }
}

void print_divaces(){
    for(int i = 0 ; i < devacenum ; i++){
        sleep_print("\nFind Device : ",10);
        sleep_print(devices_names[Devaces[i].classcode>=0x14?0:Devaces[i].classcode][Devaces[i].subclass_code>=0x10?0:Devaces[i].subclass_code],15);
        /*sleep_print(" (class code ",10);
        sleep_print(Int_To_STR(class_code,10),10);*/
        sleep_print("  Vendor ID ",10);
        sleep_print(Int_To_STR(Devaces[i].VindorID,10),10);
        sleep_print("  Device ID ",10);
        sleep_print(Int_To_STR(Devaces[i].DeviceID,10),10);
        //sleep_print("  Progif ",10);
        //sleep_print(Int_To_STR(Devaces[i].prog_if,10),10);

        sleep_print("\n",10);
        sleep_print("  base ",10);
        sleep_print(Hex(Devaces[i].base),10);
        sleep_print(Devaces[i].adrasing64?(char*)" --64bit-- ": (char*)" --32bit-- ",14);
        sleep_print(Devaces[i].suport_MMIO?(char*)" MMIO":(char*)" Port I/O",14);
        sleep_print("  IRQ# ",10);
        sleep_print(Hex(Devaces[i].IRQ),10);
        sleep_print("\n",10);
    }
}

PCI_Res_Find find_pci_devace(u8 classcode){
    find.num_of_devc=0;
    for(int i = 0 ; i < 256*32*8 ; i++){
       if(Devaces[i].classcode==classcode){
            find.devace[find.num_of_devc]=i;
            find.num_of_devc++;
       }
    }
    return find;
}

PCI_Res_Find find_pci_devace(u8 classcode , u8 subclasscode){
    find.num_of_devc=0;
    //PCI_Res_Find find;
    for(u32 i = 0 ; i < 256*32*8 ; i++){
       if(Devaces[i].classcode==classcode && Devaces[i].subclass_code==subclasscode){
            find.devace[find.num_of_devc]=i;
            find.num_of_devc++;
       }
    }
    return find;
}

PCI_Res_Find find_pci_devace(u8 classcode , u8 subclasscode , u8 ifprog){
    find.num_of_devc=0;
    //PCI_Res_Find find;
    for(u32 i = 0 ; i < 256*32*8 ; i++){
       if(Devaces[i].classcode==classcode && Devaces[i].subclass_code==subclasscode &&  Devaces[i].prog_if == ifprog){
            find.devace[find.num_of_devc]=i;
            find.num_of_devc++;
       }
    }
    return find;
}

PCI_Res_Find find_pci_devace(u16 vendorid , u16 devaceid){
    find.num_of_devc=0;
    //PCI_Res_Find find;
    for(u32 i = 0 ; i < 256*32*8 ; i++){
       if(Devaces[i].VindorID==vendorid && Devaces[i].DeviceID==devaceid ){
            find.devace[find.num_of_devc]=i;
            find.num_of_devc++;
       }
    }
    return find;
}

PCI_Res_Find find_pci_devace(u16 vendorid , u16 devaceid , u8 classcode , u8 subclasscode){
    find.num_of_devc=0;
    //PCI_Res_Find find;
    for(u32 i = 0 ; i < 256*32*8 ; i++){
       if(Devaces[i].VindorID==vendorid   &&
          Devaces[i].DeviceID==devaceid   &&
          Devaces[i].classcode==classcode && 
          Devaces[i].subclass_code==subclasscode ){
            find.devace[find.num_of_devc]=i;
            find.num_of_devc++;
       }
    }
    return find;
}

//pleas , if you wont more , pleas dont tell me
