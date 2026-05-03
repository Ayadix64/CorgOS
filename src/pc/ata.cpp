#include "../utils/util.h"

#define IDE_DATA 0x1F0       // منفذ البيانات
#define IDE_SECTOR_COUNT 0x1F2 // عدد القطاعات
#define IDE_LBA_LOW 0x1F3      // LBA Low Byte
#define IDE_LBA_MID 0x1F4      // LBA Mid Byte
#define IDE_LBA_HIGH 0x1F5     // LBA High Byte
#define IDE_DRIVE_SELECT 0x1F6 // اختيار القرص
#define IDE_COMMAND 0x1F7      // منفذ الأوامر
#define IDE_STATUS 0x1F7       // منفذ الحالة

#define DRIVER_SLOT(x)\
       (x>1)*0x80

#define DRIVER_SLECT(x)  (0xa0 + 0x10*(x%2)) | 0x40


#define ATA_READ 0x20          // أمر القراءة
#define ATA_WRITE 0x30
#define ATA_ENDF 0xec
#define SECTOR_SIZE 512        // حجم القطاع (512 بايت)


bool DiskErr = false;
u32 DiskErrSector = 0;
u32 ATAGetDiskErr(){
    if(DiskErr){
        return DiskErrSector;
    }
    return 0;
}

bool ATAwaitDrive(u32 hd) {
   u32 ii = 0x10000; 
   while ((inb(IDE_STATUS-(hd>=2?0x80:0)) & 0x80 )&& ii)ii--;;
   if(!ii){
	printc("\nIDE Draver halted. ",12);
	return 0;
   }
   return 1;
}

int ATAGetDiskErorr(int lba,u32 hd){
    if (inb(IDE_STATUS) & 0x01) {  // إذا كانت هناك مشكلة في العملية (مثل الخطأ)
        printc("Disk ERORR",12);
        //printc((int)DiskErrSector,12);
        return 1;
    }
    return 0;
}

void ATARead(u32 lba,u32 count, u8* buffer, u32 hd) {
	//return;
    for(int ii = 0 ; ii < count ; ii++){
        if(!ATAwaitDrive(hd)){
		printc("IDE Read Err",12);
		return;
	};

        outb(IDE_SECTOR_COUNT, 1);     
        outb(IDE_LBA_LOW, lba & 0xFF);
        outb(IDE_LBA_MID, (lba >> 8) & 0xFF); 
        outb(IDE_LBA_HIGH, (lba >> 16) & 0xFF);
        outb(IDE_DRIVE_SELECT, DRIVER_SLECT(hd) | ((lba >> 24) & 0x0F));

        outb(IDE_COMMAND, ATA_READ);
        if(ATAGetDiskErorr(lba,hd)){DiskErr=true;DiskErrSector=lba;return;};
       	if(!ATAwaitDrive(hd)){
		printc("IDE Read Err",12);
		return;
	}; 
        for (int i = 0; i < SECTOR_SIZE/2; i++) {
            ((u16*)buffer)[i+ii*(SECTOR_SIZE/2)] = inw(IDE_DATA);
        }
        lba++;
    }
}


void ATAWrite(u32 lba,u32 count, u8* buffer, u32 hd) {
    //return;
    for(int ii = 0 ; ii < count ; ii++){
        if(!ATAwaitDrive(hd)){
		printc("IDE Write Err",12);
		return;

	};

        outb(IDE_SECTOR_COUNT, 1);     
        outb(IDE_LBA_LOW, lba & 0xFF);
        outb(IDE_LBA_MID, (lba >> 8) & 0xFF); 
        outb(IDE_LBA_HIGH, (lba >> 16) & 0xFF);
        outb(IDE_DRIVE_SELECT, DRIVER_SLECT(hd) | ((lba >> 24) & 0x0F));

        outb(IDE_COMMAND, ATA_WRITE);
        if(ATAGetDiskErorr(lba,hd)){DiskErr=true;DiskErrSector=lba;return;};
        if(!ATAwaitDrive(hd)){
		printc("IDE Write Err",12);
		return;

	};

        for (int i = 0; i < SECTOR_SIZE/2; i++) {
            outw(IDE_DATA,((u16*)buffer)[i+ii*(SECTOR_SIZE/2)]);
	    
        }
        lba++;
    }
}

u32 ATAchaeck(u32 disk){
    //	return 1;
    ATAwaitDrive(disk);
    outb(IDE_SECTOR_COUNT-DRIVER_SLOT(disk) ,1);
    outb(IDE_LBA_LOW -DRIVER_SLOT(disk), 0);
    outb(IDE_LBA_MID -DRIVER_SLOT(disk), 0);
    outb(IDE_LBA_HIGH-DRIVER_SLOT(disk), 0);
    outb(IDE_DRIVE_SELECT-DRIVER_SLOT(disk),  DRIVER_SLECT(disk)/*0xE0+ 1 * (disk%2)*/ );
    outb(IDE_COMMAND-DRIVER_SLOT(disk), ATA_ENDF);
    ATAwaitDrive(disk);
    u8 status = inb(IDE_STATUS-DRIVER_SLOT(disk));
    //ATAwaitDrive(disk);
    if(!status)
        return 0;

    if(!ATAwaitDrive(disk)){
	printc("Non IDE.",12);
	return 0;

    }
    status = inb(IDE_STATUS-DRIVER_SLOT(disk));
    for (int i = 0; i < SECTOR_SIZE/2; i++) {
        int r = inw(IDE_DATA-DRIVER_SLOT(disk));
    }

    if(status&0x1){
        printc("\nATA Disk Err.\n",15);
        return 2;
    }
    else
        return 1;

    return 1;

}
