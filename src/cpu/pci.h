#ifndef PCI_H
#define PCI_H
#include "../utils/util.h"



static char* devices_names[0x14][0x10] = {
    // 0x00 - Unclassified
    {"Unclassified", "VGA-Compatible", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown"},
    // 0x01 - Mass Storage Controller
    {"SCSI", "IDE", "Floppy", "IPI", "RAID", "ATA", "SATA", "SAS", "NVM", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown"},
    // 0x02 - Network Controller
    {"Ethernet", "Token Ring", "FDDI", "ATM", "ISDN", "WorldFip", "PICMG", "Infiniband", "Fabric", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown"},
    // 0x03 - Display Controller
    {"VGA", "XGA", "3D", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown"},
    // 0x04 - Multimedia Controller
    {"Video", "Audio", "Phone", "Audio Device", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown"},
    // 0x05 - Memory Controller
    {"RAM", "Flash", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown"},
    // 0x06 - Bridge Device
    {"Host", "ISA", "EISA", "MicroChannel", "PCI-to-PCI", "PCMCIA", "NuBus", "CardBus", "RACEway", "PCI-to-PCI (semi-transparent)", "InfiniBand-to-PCI", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown"},
    // 0x07 - Communication Controller
    {"Serial", "Parallel", "Multiport Serial", "Modem", "GPIB", "Smart Card", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown"},
    // 0x08 - System Peripheral
    {"PIC", "DMA", "Timer", "RTC", "Hot-Plug", "SD Host", "IOMMU", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown"},
    // 0x09 - Input Device
    {"Keyboard", "Digitizer Pen", "Mouse", "Scanner", "Gameport", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown"},
    // 0x0A - Docking Station
    {"Generic", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown"},
    // 0x0B - Processor
    {"386", "486", "Pentium", "Alpha", "PowerPC", "MIPS", "Co-Processor", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown"},
    // 0x0C - Serial Bus Controller
    {"FireWire", "ACCESS Bus", "SSA", "USB Controller", "Fibre Channel", "SMBus", "InfiniBand", "IPMI", "SERCOS", "CANbus", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown"},
    // 0x0D - Wireless Controller
    {"iRDA", "Consumer IR", "RF", "Bluetooth", "Broadband", "802.1a", "802.1b", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown"},
    // 0x0E - Intelligent I/O Controller
    {"I2O", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown"},
    // 0x0F - Satellite Communication
    {"TV", "Audio", "Voice", "Data", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown"},
    // 0x10 - Encryption Controller
    {"Network", "Entertainment", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown"},
    // 0x11 - Signal Processing Controller
    {"DPIO", "Performance Counter", "Communication Sync+", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown"},    
    // 0x12 to 0x13 - Reserved or unknown
    {"Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown"},
    {"Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown"},
};



typedef struct Device{
    u8 bus;
    u8 device;
    u8 func;
    u16 VindorID;
    u16 DeviceID;
    u8 classcode;
    u8 subclass_code;
    u8 prog_if;
    u32 base ;
    u32 bases[6];
    bool adrasing64=false;
    bool suport_MMIO=true;
    u8 IRQ;
};
typedef struct PCI_Res_Find {
    u32 *devace;//if i do u32 , it will take 200kb from ram !!!!!!!!
    u32 num_of_devc; 
};
extern "C" Device* Devaces;//yeah this is not smart [256*32*8]; //maxemum of dvaces can be in one pc
extern "C" u32 devacenum;//how many devace thar are in the pc

u32 rd_pci(u8 bus, u8 device , u8 func , u8 offse);
void wr_pci(u8 bus, u8 device , u8 func , u8 offset,u32 val);
u32 device_info(u8 bus, u8 divice, u8 func );

void detact_divace();
void print_divaces();



PCI_Res_Find find_pci_devace(u8 classcode);
PCI_Res_Find find_pci_devace(u8 classcode , u8 subclasscode);
PCI_Res_Find find_pci_devace(u8 classcode , u8 subclasscode , u8 ifprog);
PCI_Res_Find find_pci_devace(u16 vendorid , u16 devaceid);
PCI_Res_Find find_pci_devace(u16 vendorid , u16 devaceid , u8 classcode , u8 subclasscode);


#endif
