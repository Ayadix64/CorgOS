#ifndef ATA_DISK_H
#define ATA_DISK_H
#include "../utils/util.h"

void ATARead(u32 lba,u32 count, u8* buffer, u32 disk) ;
u32 ATAGetDiskErr();
void ATAWrite(u32 lba,u32 count, u8* buffer, u32 disk);
u32 ATAchaeck(u32 disk);
#endif
