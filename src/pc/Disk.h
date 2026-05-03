#ifndef DISK_H
#define DISK_H
#include "../utils/util.h"
typedef struct SATA_INDEFTY{
    u16 config;
    u16 model[40];
    u32 maxlba28;
    u64 maxlba48;
    u8 rsv[458];
};

typedef struct Disks
{
    u8 AHCIcontrolnum;
    u8 portnum;
};

typedef struct DISK{
	bool _ATA=false;
	bool _SATA=false;
	bool _USB=false;
	u32 index1 = 0;
	u32 index2 = 0;
	u32 index3 = 0;	
};

extern "C" DISK SATADisks[128];/*how will have that number ? , i dont know but this number is good
i mean , thay are 32 port in the controler , sooo, this number have to have 4 AHCI controlors withe full ports*/
extern "C" u32 StatsDisksnum;

extern "C" DISK ATADISK[4];//aka the max of disk number in ide
extern "C" u32 ATADISKn ;

extern "C" DISK bootdisk;
extern "C" bool bootfind;

void detact_streg();
void DiskInit();
bool read_sata(u64 lba , u16 cont , volatile u8*bufer , u32 sata , u32 drave);
bool write_sata(u64 lba , u16 cont , volatile u8*bufer , u32 sata , u32 drave);
bool get_sata_info(SATA_INDEFTY& indef,u32 sata , u32 drave);
void SATAD_Update();
void USBInit();
#endif 
