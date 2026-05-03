#ifndef FAT12_H
#define FAT12_H
#include "../pc/ata.h"
#include "../pc/timer.h"
#include "../utils/util.h"


/*NOTE: VIRY IMPORTONT
    when i do data struct wiche it most have 16 bit long it haze 28, so mybe, just mybe, if remove the struct valu in USB data structs
    ,mybe it will work
*/
typedef struct PACKED FAT12BOOT 
{
    u8 jumpshortnop[3];
    u8 bdb_oem[8];
    
    u16 bdb_bytes_per_sector;      
    
    u8 bdb_sectors_per_cluster;
    
    u16 bdb_reserved_sectors;
    
    u8 bdb_fat_count;
    
    u16 bdb_dir_entries_count;
    u16 bdb_total_sectors;
    
    u8 bdb_media_descriptor_type;
    
    u16 bdb_sectors_per_fat;
    u16 bdb_sectors_per_track;
    u16 bdb_heads;
    
    u32 bdb_hidden_sectors;
    u32 bdb_large_sector_count;

    u8 ebr_drive_number;
    u8 rev;;
    u8 ebr_signature;
    u32 ebr_volume_id;
    u8 ebr_volume_label[11];
    u8 ebr_system_id[8];
    u8 rsv1[512];
} ;
typedef struct FATTIME{
    u8 hour:5;
    u8 mint:6;
    u8 secnd:5;
}PACKED;
typedef  struct FATDATE{
    u8 year:7;
    u8 month:4;
    u8 day:5;
}PACKED;
typedef struct FATFILE
{
    u8 Name[11];
    u8 Attributs;
    u8 rsv;
    u8 creationtimeinmil;
    /*struct {
        u8 hour:5;
        u8 mint:6;
        u8 secnd:5;//*=2
    }creastiontime PACKED;;
    struct{
        u8 year:7;
        u8 month:4;
        u8 day:5;
    }creastiondate PACKED;;
    struct{
        u8 year:7;
        u8 month:4;
        u8 day:5;
    }lastacsessdate PACKED;*/
    FATTIME creastiontime;
    FATDATE creastiondate;
    FATDATE lastacesdate;

    u16 firstclusterhei;//used by fat32
    
    /*struct{
        u8 hour:5;
        u8 mint:6;
        u8 secnd:5;//*=2
    }lastmodifiatime PACKED;;
    struct{
        u8 year:7;
        u8 month:4;
        u8 day:5;
    }lastmodifiadate PACKED;;*/
    FATTIME lastmodifiatime;
    FATDATE lastmodifiadate;

    u16 firstclusterlow;

    u32 size;//the size of file in bytes, if you remamber, that is way fat32 cant acssept more than 4GB file
}PACKED;
void readD(u64 lba ,u8* data ,u32 count , DISK disk);
void writD(u64 lba ,u8* data ,u32 count , DISK disk);

#define RDBOOT(boot, disk)\
	readD(0,(u8*)&boot,1,disk);


namespace FAT12
{
static const char* file_attributs[0x22]{
    "Unkown",
    "Read Only",
    "Hidden",
    "Unkown",
    "System",
    "Unkown",
    "Unkown",
    "Unkown",
    "Volume ID",
    "Unkown",
    "Unkown",
    "Unkown",
    "Unkown",
    "Unkown",
    "Unkown",
    "Unkown",
    "Directory",
    "Unkown",
    "Unkown",
    "Unkown",
    "Unkown",
    "Unkown",
    "Unkown",
    "Unkown",
    "Unkown",
    "Unkown",
    "Unkown",
    "Unkown",
    "Unkown",
    "Unkown",
    "Unkown",
    "Unkown",
    "Archive",
    "Unkown",
    //READ_ONLY=0x01 HIDDEN=0x02 SYSTEM=0x04 VOLUME_ID=0x08 DIRECTORY=0x10 ARCHIVE=0x20 
};
    bool creatfile(char* path,char* filename,DISK disk);
    bool read_file(FATFILE file,char* data, DISK disk);
    bool read_file(FATFILE file,char* data,u32 size, DISK disk);
    bool write_to_file(FATFILE &file, char* data, u32 size , DISK disk);
    bool mkdir(char* filename,char* path, DISK disk);
    bool rmfile(FATFILE& file,DISK disk);
    bool removefile(char* path, char*filen , DISK disk); 
    bool save(char* path, char* filen , u8* data, u32 size,DISK disk);

    u32 nextcluster(u32 closter,FAT12BOOT& boot,DISK disk);
    u32 getempitycluster(FAT12BOOT&  boot , DISK disk);
    bool writecluster(u32 closter,u32 in, FAT12BOOT &boot, DISK disk);
    
    u32 getFatAT(FAT12BOOT& boot);
    u32 getFatLong(FAT12BOOT& boot);
    u32 getrootdir( FAT12BOOT& boot);
    u32 getrootlong(FAT12BOOT&  boot);
    u32 getfatdata(FAT12BOOT& boot);
    u32 getrootenris(FAT12BOOT&  boot);
    u32 getvolumeid(FAT12BOOT&  boot);
    bool fat12readboot(DISK disk, FAT12BOOT& boot);

    bool isitfat12(DISK disk);
    bool rootdir(FATFILE* fils, DISK disk);
    u32 getvolumeid(DISK disk);
    u32 getrootentris(DISK disk);

    u32 fdir(char* dir,FATFILE* fils,u32 entrisn, DISK disk);
    
} // namespace FAT12


#endif
