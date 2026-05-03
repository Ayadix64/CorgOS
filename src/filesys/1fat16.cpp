#include "../pc/Disk.h"
#include "../pc/ata.h"
#include "../pc/timer.h"
#include "../utils/util.h"
#include "../pc/RTC.h"
#include "../utils/util.h"
//<LeftMouse>#include <ctime>
//#include <cstdlib>
#include "1fat16.h"


//yeah, it actionly 16 bit, hahaha i am a lier
using namespace FAT12;

u32 FAT12::getFatAT(FAT12BOOT& boot){
    return boot.bdb_reserved_sectors;
}

u32 FAT12::getFatLong(FAT12BOOT& boot){
    return (boot.bdb_fat_count*boot.bdb_sectors_per_fat);
}
u32 FAT12::getrootdir( FAT12BOOT& boot){
    return (getFatAT(boot)+getFatLong(boot));
}
u32 FAT12::getrootlong(FAT12BOOT&  boot){
    return ((boot.bdb_dir_entries_count*32)+(boot.bdb_bytes_per_sector-1))/boot.bdb_bytes_per_sector;
}

u32 FAT12::getfatdata(FAT12BOOT& boot){
    return (getrootdir(boot)+getrootlong(boot));
}

u32 FAT12::getrootenris(FAT12BOOT&  boot){
    return boot.bdb_dir_entries_count;
}
u32 FAT12::getvolumeid(FAT12BOOT&  boot){
    return boot.ebr_volume_id;
}

#define FATLBA(boot)        (u32)(boot.bdb_reserved_sectors)
#define FATLONG(boot)       (u32)(boot.bdb_fat_count*boot.bdb_sectors_per_fat)
#define ROOTDIRLBA(boot)    (u32)(FATLBA(boot)+FATLONG(boot))
#define ROOTLONG(boot)      (u32)((((boot.bdb_dir_entries_count*32))/512))
#define DATALBA(boot)       (u32)(ROOTDIRLBA(boot)+ROOTLONG(boot))
#define ROOTENTR(boot)	    boot.bdb_dir_entries_count

#define CHEKIFFAT16(boot) (memcmp((void*)boot.ebr_system_id,(void*)"FAT16",5)| \
                         memcmp((void*)boot.ebr_system_id,(void*)"fat16",5)| \
                         memcmp((void*)boot.ebr_system_id,(void*)"Fat16",5))) 

#define ISITROOT(fils) !(memcmp((void*)fils[0].Name,(void*)".\0\0\0\0\0\0\0\0\0\0",11)|memcmp((void*)fils[0].Name,(void*)".           ",11))

#define CHEKRT(fn) if(!(fn))return 0;


void readD(u64 lba ,u8* data ,u32 count , DISK disk){
    if(disk._ATA && !disk._SATA && !disk._USB){
        ATARead(lba,  count, data, disk.index1);
    }
    if(disk._SATA&& !disk._ATA && !disk._USB){
        read_sata( lba,  count, data, disk.index1, disk.index2);
    }

    return;
}

void writD(u64 lba ,u8* data ,u32 count , DISK disk){
    if(disk._ATA){
        ATAWrite(lba,  count, data, disk.index1);
    }else if(disk._SATA){
        write_sata(lba,  count, (volatile u8 *)data, disk.index1, disk.index2);
    }
    return;
}


bool FAT12::fat12readboot(DISK disk, FAT12BOOT& boot){
    
    memset((u8)0,(void*)&boot,512);
    readD(0, (u8 *)&boot, 1,  disk);
    return 1;
}
bool FAT12::isitfat12(DISK disk){
    FAT12BOOT boot ;
    memset((u8)0,(void*)&boot,512);
    if(fat12readboot(disk,boot)){
        //printc((char*)boot.ebr_system_id,12);
        if(memcmp((void*)boot.ebr_system_id,(void*)"FAT16",5)|
           memcmp((void*)boot.ebr_system_id,(void*)"fat16",5)|
           memcmp((void*)boot.ebr_system_id,(void*)"Fat16",5)){
            return true;
        }
        else return false;
    }
    return false;
}


//READ_ONLY=0x01 HIDDEN=0x02 SYSTEM=0x04 VOLUME_ID=0x08 DIRECTORY=0x10 ARCHIVE=0x20 LFN=READ_ONLY|HIDDEN|SYSTEM|VOLUME_ID 
//(LFN means that this entry is a long file name entry)
#define  READ_ONLY 0x1
#define  HIDDEN    0x2
#define  SYSTEM    0x4
#define  VOLUME_ID 0x8
#define  DIRECTORY 0x10
#define  ARCHIVE   0x20



//TODO:Make This Programe do withe the macros



bool FAT12::rootdir(FATFILE* fils, DISK disk){
    //if(!isitfat12(disk))return 0;
    FAT12BOOT boot;
    RDBOOT(boot, disk);

    readD(ROOTDIRLBA(boot), (u8 *)fils, ROOTLONG(boot),  disk);
    return true;
}


u32 FAT12::getvolumeid(DISK disk){
    FAT12BOOT boot;
    if(!fat12readboot(disk,boot))return false;
    return getvolumeid(boot);
}

u32 FAT12::getrootentris(DISK disk){
    FAT12BOOT boot;
    if(!fat12readboot(disk,boot))return false;
    return getrootenris(boot);
}

u32 FAT12::nextcluster(u32 closter,FAT12BOOT& boot,DISK disk){

    u16 fat[1024*2];
    
    readD(FATLBA(boot)+((u32)(closter/256)), (u8*)fat, 1, disk);
    return fat[closter%256];
}

u32 FAT12::getempitycluster(FAT12BOOT & boot , DISK disk){
    u16 *fat=(u16*)malloc(1024);
    mfree(fat, 1024);
    readD((FATLBA(boot)),( u8*)fat,4,disk);
    for(u32 i = 0 ; i < boot.bdb_sectors_per_fat*512 ; i++){
        if(fat[i]==0){
            return i;
        }
        if(i%1024==0 && i){
            readD((FATLBA(boot)+((u32)(i/256))),(u8*)fat,4,disk);
        }
    }
    return 0xffffffff;
}
bool FAT12::writecluster(u32 closter,u32 in, FAT12BOOT &boot, DISK disk){
    printc("\nw...",12);
    u16* fats=(u16*)malloc(0x1000);
    mfree(fats, 0x1000);
    u64 fatlba = FATLBA(boot);
    //memset((u8)0,(void*)fats,512);
    readD((u64)(fatlba+(closter/256)),(u8*)(fats),1,disk);
    fats[(u32)(closter%256)]=(u16)in;
    writD((fatlba+(closter/256)),(u8*)(fats),1,disk);

    return 1;
}
bool FAT12::creatfile(char* path,char* filen,DISK disk){
    FAT12BOOT boot;
    RDBOOT(boot, disk);
		
    FATFILE* dir = (FATFILE*)malloc(ROOTLONG(boot)*512);
    mfree((void*)dir, ROOTLONG(boot)*512);

    fdir(path, dir, ROOTENTR(boot), disk);
    bool root = true;
    u32 clost= dir[0].firstclusterlow;
    if(!ISITROOT(dir)){
 	root=false;
    }

    u32 ll = 0;
    bool findit = false;
    for (; ll<ROOTENTR(boot); ll++) {    
       	if(memcmp((void*)filen , (void*)dir[ll].Name, 11)){
		if(dir[ll].Attributs&(HIDDEN|SYSTEM|VOLUME_ID|READ_ONLY)){
			printc("\nOperation non-allawed ",12);
			return 0;
		}
		findit=true;
		break;
	}

    }
    if(findit){
	printc("\nThey allready a file with name \"",12);
	printc(filen,12);
	printc(" \".",12);
		//printc("\nFile not found.",12);
	return 0;
    }
    
    
    for(int ii = 0 ; ii < 11 ; ii++){
        if(filen[ii]=='/'){
          printc("\nErr:cant file name have \'/\' symbol.\n",12);
            return 0;//we not wont that, it will cause problems
        }
    }
    

    bool theypl = false;
    
    FATFILE empty;
    memset((u8)0,&empty,sizeof(FATFILE));

    int i = 0 ;
    for(;i<ROOTENTR(boot);i++){
	if(memcmp((void*)&dir[i],(void*)&empty,sizeof(FATFILE) )){
		theypl=true;
		break;
	}	
    }
    if(!theypl){
	printc("\nThe diractory is Full.",12);
	return 0;
    }

   //memcp((void*)filen,(void*) dir[i].Name, 11);
    memset((u8)' ',(void*)dir[i].Name,11);
    if(!filen[0]){
	printc("\nNo name selected",12);
	return 0;
    }
    for(int ii = 0 ; ii<11 && filen[ii]; ii++){
	dir[i].Name[ii]=filen[ii];
    }
    u32 h, mi, sec;
    get_time(h,mi,sec);
    dir[i].creastiontime.hour=h;
    dir[i].creastiontime.mint=mi;
    dir[i].creastiontime.secnd=sec/2;

    u32 yr, mo , da;
    get_date(da,mo,yr);
    dir[i].Attributs=ARCHIVE;
    dir[i].creastiondate.day=da;
    dir[i].creastiondate.month=mo;
    dir[i].creastiondate.year=yr;
    
    u32 cluster = getempitycluster(boot,disk);
    writecluster(cluster,0xffff,boot,disk);
    //printc(Hex(cluster),12);
    if(cluster!=0xffffffff)dir[i].firstclusterlow=(cluster);
    else{
	    printc("\nNo more space in the disk",12);
	    return 0;
    }
    
     if(root){
	printc("you are in root",12);
        writD(ROOTDIRLBA(boot),(u8*) dir,ROOTLONG(boot) , disk);
    }else{

	FATFILE thedir=dir[0];
	write_to_file(thedir,(char*)dir,ROOTENTR(boot)*sizeof(FATFILE),disk);

    }    
    return 1;
}

bool FAT12::write_to_file(FATFILE &file, char* data, u32 size , DISK disk){//TODO:fix this
    FAT12BOOT boot;
    RDBOOT(boot, disk);
    //fat12readboot(disk,boot); 
    //u32 cc = 0x1000;
    //while(!RDBOOT(boot, disk)&&cc){cc--;};
    u8 *buffer=(u8*)malloc((boot.bdb_sectors_per_cluster)*512);
    mfree((void*)buffer,(boot.bdb_sectors_per_cluster)*512);

    
    u32 closter = file.firstclusterlow;
    u32 lastcloster = closter;
    
    //printc(Hex((boot.bdb_sectors_per_cluster)*512)/*data,size*/,12);
    
    u32 ptr = 0;
    u32 bytewrited = 0;
    u32 ncls = 0;
    closter= nextcluster(closter, boot, disk);
    while (closter!=0xffff) {
    	ncls=nextcluster(closter, boot, disk);
	writecluster(closter, 0,boot,  disk);
	closter=ncls;
    }
    closter = file.firstclusterlow;

    //rmfile(file,  disk);
    //return 1 ;
    while(size!=bytewrited){
        for(int i = 0 ; bytewrited < size && i < boot.bdb_sectors_per_cluster*512 ; i++){
            buffer[i]=(u8)data[i+ptr];
            bytewrited++;
            file.size++; 
        }
        writD(DATALBA(boot)+(closter*boot.bdb_sectors_per_cluster)-2,(u8*)buffer,
                                     boot.bdb_sectors_per_cluster,disk);
        u32 emp=getempitycluster(boot,disk);
        
        if(emp==0xffff){ 
            printc("\nErr: cant write more data, the FAT is done !!!\n",12);
            return 0;
        }
        writecluster(closter,emp,boot,disk);
        lastcloster=closter;
        closter=emp;
        ptr+=boot.bdb_sectors_per_cluster*512;
	memset((u8)0,buffer,boot.bdb_sectors_per_cluster*512);
    }
        writecluster(lastcloster,-1,boot,disk);
    //mfree((void*)buffer,/*(boot.bdb_sectors_per_cluster+1)*512*/0x1000);
    return true;

}


bool FAT12::save(char* path, char* filen , u8* data, u32 size,DISK disk){
	FAT12BOOT boot;
	RDBOOT(boot, disk);
	FATFILE* fils = (FATFILE*)malloc(ROOTLONG(boot)*512);
	mfree((void*)fils, ROOTLONG(boot)*512);
	fdir(path, fils, ROOTENTR(boot), disk);
	u32 fll=0;
	bool findit = false;
	for(;fll<ROOTENTR(boot);fll++){
		if(memcmp(filen,fils[fll].Name,11)){
			findit=true;
			break;
		}
	}
	if(!findit){
		printc("\nFile non found.",12);
		return 0;
	}
	fils[fll].size=0;
	///*CHEKRT(*/rmfile(fils[fll], disk)/*)*/;
	//fils[fll].size=size;
	write_to_file(fils[fll], (char*)data, size,disk);
	bool root = ISITROOT(fils);
	
	if(root){
		writD(ROOTDIRLBA(boot), (u8 *)fils, ROOTLONG(boot)*sizeof(FATFILE),  disk);
	}
	else{
		FATFILE folder = fils[0];
		write_to_file(folder, (char *)fils, ROOTENTR(boot), disk);
	}
	return true;



}

bool FAT12::mkdir(char* filen,char* path, DISK disk){

    FAT12BOOT boot;
    RDBOOT(boot, disk);
		
    FATFILE* dir = (FATFILE*)malloc(ROOTLONG(boot)*512);
    mfree((void*)dir, ROOTLONG(boot)*512);

    fdir(path, dir, ROOTENTR(boot), disk);
    bool root = true;
    u32 clost= dir[0].firstclusterlow;
    if(!ISITROOT(dir)){
 	root=false;
    }

    u32 ll = 0;
    bool findit = false;
    for (; ll<ROOTENTR(boot); ll++) {    
       	if(memcmp((void*)filen , (void*)dir[ll].Name, 11)){
		if(dir[ll].Attributs&(HIDDEN|SYSTEM|VOLUME_ID|READ_ONLY)){
			printc("\nOperation non-allawed ",12);
			return 0;
		}
		findit=true;
		break;
	}

    }
    if(findit){
	printc("\nThey allready a file with name \"",12);
	printc(filen,12);
	printc(" \".",12);
		//printc("\nFile not found.",12);
	return 0;
    }
    
    
    for(int ii = 0 ; ii < 11 ; ii++){
        if(filen[ii]=='/'){
          printc("\nErr:cant file name have \'/\' symbol.\n",12);
            return 0;//we not wont that, it will cause problems
        }
    }
    

    bool theypl = false;
    
    FATFILE empty;
    memset((u8)0,&empty,sizeof(FATFILE));

    int i = 0 ;
    for(;i<ROOTENTR(boot);i++){
	if(memcmp((void*)&dir[i],(void*)&empty,sizeof(FATFILE) )){
		theypl=true;
		break;
	}	
    }
    if(!theypl){
	printc("\nThe diractory is Full.",12);
	return 0;
    }

   //memcp((void*)filen,(void*) dir[i].Name, 11);
    memset((u8)' ',(void*)dir[i].Name,11);
    if(!filen[0]){
	printc("\nNo name selected",12);
	return 0;
    }
    for(int ii = 0 ; ii<11 && filen[ii]; ii++){
	dir[i].Name[ii]=filen[ii];
    }
    u32 h, mi, sec;
    get_time(h,mi,sec);
    dir[i].creastiontime.hour=h;
    dir[i].creastiontime.mint=mi;
    dir[i].creastiontime.secnd=sec/2;

    u32 yr, mo , da;
    get_date(da,mo,yr);
    dir[i].Attributs=DIRECTORY;
    dir[i].creastiondate.day=da;
    dir[i].creastiondate.month=mo;
    dir[i].creastiondate.year=yr;
    
    u32 cluster = getempitycluster(boot,disk);
    writecluster(cluster,0xffff,boot,disk);
    //printc(Hex(cluster),12);
    if(cluster!=0xffffffff)dir[i].firstclusterlow=(cluster);
    else{
	    printc("\nNo more space in the disk",12);
	    return 0;
    }
    
     if(root){
	printc("you are in root",12);
        writD(ROOTDIRLBA(boot),(u8*) dir,ROOTLONG(boot) , disk);
    }else{
        //writD(clost, (u8*)dir, ROOTLONG(boot), disk);
	FATFILE thedir=dir[0];
	write_to_file(thedir,(char*)dir,ROOTENTR(boot)*sizeof(FATFILE),disk);
	//write_sata(getfatdata(boot)+(closter*boot.bdb_sectors_per_cluster)-2,getrootlong(boot),(volatile u8*) fils , disk.AHCIcontrolnum , disk.portnum);
    }
    

    FATFILE dirs[512/sizeof(FATFILE)];
    memset((u8)0,dirs,512);

    memset((u8)' ',dirs[0].Name,11);
    memset((u8)' ',dirs[1].Name,11);

    dirs[0].Name[0]='.';

    dirs[0].Attributs=0x10;
    dirs[0].firstclusterlow=cluster;//dir[filel].firstclusterlow;
    
    dirs[1].Name[0]='.';
    dirs[1].Name[1]='.';
    dirs[1].Attributs=0x10;

    dirs[1].firstclusterlow=root?0:dir[0].firstclusterlow;


    //writD(DATALBA(boot)+(cluster*boot.bdb_sectors_per_cluster)-2 ,(u8 *)dirs,1,disk);
    FATFILE a_othr_file = dir[i];//i have a very awsome hadeackes with this
    write_to_file(a_othr_file, (char *)dirs,512,  disk);
    
    
    return 1;    

}



bool FAT12::read_file(FATFILE file,char* data, DISK disk){
    FAT12BOOT boot;
    //fat12readboot(disk,boot);
    RDBOOT(boot, disk);

    //if(!read_sata(getfatdata(boot)+((file.firstclusterlow)*boot.bdb_sectors_per_cluster)-2,
    //                        boot.bdb_sectors_per_cluster,
    //                        (volatile u8*)data,
    //                        disk.AHCIcontrolnum,
    //     		      disk.portnum))return 0;
    
    readD(DATALBA(boot)+((file.firstclusterlow)*boot.bdb_sectors_per_cluster)-2
		    , (u8 *)data, boot.bdb_sectors_per_cluster,disk);

    u32 ptr = boot.bdb_sectors_per_cluster*boot.bdb_bytes_per_sector;
    u32 closter = (file.firstclusterlow);
    //printc("\nfirst closter : ",14);
    //printc(Hex(file.firstclusterlow),14);

    while(closter!=0xffff&&closter!=0xfff8&&closter!=0){
        closter=nextcluster(closter,boot,disk);
        //printc("\nNext closter : ",14);
        //printc(Hex(closter),14);
        if(closter!=0xffff&&closter!=0xfff8){
            //if(!read_sata(getfatdata(boot)+(closter*boot.bdb_sectors_per_cluster)-2,
            //                    boot.bdb_sectors_per_cluster,
            //                    (volatile u8*)(data+ptr),
            //                    disk.AHCIcontrolnum,
            //                    disk.portnum))return 0;
            readD(DATALBA(boot)+(closter*boot.bdb_sectors_per_cluster)-2, (u8 *)(data+ptr), boot.bdb_sectors_per_cluster, disk);
	}
        ptr+=boot.bdb_sectors_per_cluster*boot.bdb_bytes_per_sector;
    }
    return 1;
}

bool FAT12::read_file(FATFILE file,char* data,u32 size, DISK disk){
    FAT12BOOT boot;
    //fat12readboot(disk,boot);
    RDBOOT(boot, disk);

        readD(DATALBA(boot)+((file.firstclusterlow)*boot.bdb_sectors_per_cluster)-2
		    , (u8 *)data, boot.bdb_sectors_per_cluster,disk);

    u32 ptr = boot.bdb_sectors_per_cluster*boot.bdb_bytes_per_sector;
    u32 closter = (file.firstclusterlow);
    printc("\nfirst closter : ",14);
    printc(Hex(file.firstclusterlow),14);

    u8 *buffer=(u8*)malloc(((size+512-1)/512)*512);//[0x1000];

    while(closter!=0xffff&&closter!=0xfff8&&closter!=0){
        
        closter=nextcluster(closter,boot,disk);
        printc("\nNext closter : ",14);
        printc(Hex(closter),14);
        if(closter!=0xffff&&closter!=0xfff8){
  	    readD(DATALBA(boot)+(closter*boot.bdb_sectors_per_cluster)-2, (u8 *)(buffer), boot.bdb_sectors_per_cluster, disk);
	}
        for(int ii = 0 ; ii<boot.bdb_sectors_per_cluster*boot.bdb_bytes_per_sector && size ; ii++){
            data[ii+ptr]=buffer[ii];
            size--;
        }
        ptr+=boot.bdb_sectors_per_cluster*boot.bdb_bytes_per_sector;
    }
    mfree(buffer,((size+512-1)/512)*512);
    return 1;
}


bool FAT12::rmfile(FATFILE& file,DISK disk){
	FAT12BOOT boot;
	RDBOOT(boot, disk)

	u32 closter = file.firstclusterlow;
	u32 pcloster = closter;
	writecluster(closter, 0,boot, disk);
	closter = nextcluster(pcloster, boot,  disk);

	while(closter&&closter!=0xffff){
		pcloster=closter;
		writecluster(closter, 0, boot, disk);
		closter=nextcluster(pcloster, boot, disk);
		if((closter&0xffff)==0xffff){
			return 0;
		}
	}
	return 1;
}

bool FAT12::removefile(char* path, char*filen , DISK disk){
	FAT12BOOT boot;
	RDBOOT(boot, disk);
	
	FATFILE* dir = (FATFILE*)malloc(ROOTLONG(boot)*512);
	mfree(dir, ROOTLONG(boot)*512);

	fdir(path, dir, ROOTENTR(boot), disk);
	bool root = ISITROOT(dir);
	u32 clost= dir[0].firstclusterlow;
	
	u32 ll = 0;
	bool findit = false;
	for (; ll<ROOTENTR(boot); ll++) {    
        	if(memcmp((void*)filen , (void*)dir[ll].Name, 11)){
			if(dir[ll].Attributs&(HIDDEN|SYSTEM|VOLUME_ID|READ_ONLY)){
				printc("\nOperation non-allawed ",12);
				return 0;
			}
			findit=true;
			break;
		}

	}
	if(!findit){
		printc("\nFile not found.",12);
		return 0;
	}
	
	rmfile(dir[ll], disk);
	FATFILE empty;
	memset((u8)0,&empty,sizeof(FATFILE));
	dir[ll]=empty;
	if(root){
		//printc("you are in root are you",12);
		writD(ROOTDIRLBA(boot), (u8 *)dir, ROOTLONG(boot), disk);
	}else{
		FATFILE thedir=dir[0];
		write_to_file(thedir,(char*)dir,ROOTENTR(boot),disk);
		//writD(clost, (u8 *)dir, ROOTLONG(boot), disk);
	}
	return 1;
}


u32 FAT12::fdir(char* dir,FATFILE* fils,u32 entr,DISK disk){
    FAT12BOOT boot;
    RDBOOT(boot, disk);
    //fat12readboot(disk,boot);
    //printc(dir,(u32)11,46);
    u32 closter = ROOTDIRLBA(boot);
    if(dir[0]!='/'){
	printc("\nDIR: \"",12);
	printc(dir,12);
	printc("\" is not a path.",12);
	return  (u32) -1;
    };
    readD(closter, (u8 *)fils, ROOTLONG(boot), disk);
    u32 i=1;//ignor the first '/'
    char name[12]; 
    memset((u8)0,name,12);
    while (1) {
    	memset((u8)' ',name,11);	
	bool nend=false;
	for(u32 ii = 0;(dir[i]!='/')&&dir[i] ;i++,ii++)
	{
		
		nend=true;
		if(ii==11){//To learg to be a name in fat16, will they are a way to make a longer-than 11 file name, but a am so lazy so late ckip it sampel
			printc("\n",12);
			printc(name,12);
			printc(" Too big to be a name in fat",12);
			return (u32) -1;
		}
		name[ii]=dir[i];
	}
	i++;
	if(!nend){
		return closter;
	}
	bool findit=false;
	FATFILE folder;
	for(int ii = 0 ; ii < ROOTENTR(boot) ; ii++){
		if(memcmp((void*)name,(void*)fils[ii].Name,11) && fils[ii].Attributs==DIRECTORY){
			findit=true;
			folder=fils[ii];
			break;
		}
	}
	if(!findit){
		printc("\nFile ",12);
		printc(name,12);
		printc(" non found.",12);
		return (u32)-1;
	}
	memset((u8)0,fils,ROOTENTR(boot)*sizeof(FATFILE));
	read_file(folder,(char*)fils,disk);
	/*if(ISITROOT(fils)){
		printc("\n the \".\" folder in the folder \"",12);
		printc(name,12);
		printc("\" was not found.",12);
		return (u32)-1;
	}*/
	closter=fils[0].firstclusterlow;
	if(!dir[i])break;

    }
    
    /*char nn[100][12]{{0}};
    memset((u8)' ',nn,1200);
    u32 indx1=0;
    u32 indx2=0;
    if(dir[0]!='/')return 0;//unkown file path
    for(int i = 1 ; dir[i]!=0 ; i++){
        if(dir[i]!='/'){
            if(indx2<11){
                nn[indx1][indx2]=dir[i];
                indx2++;
            }
        }else{
            
            indx1++;
            indx2=0;
            continue;
        }
    }
    /*if(!dir [1]){
        rootdir(fils,disk);
        return 1;
    }*\\
    //printc(nn[0],(u32)11,12);
    FATFILE *fils_=(FATFILE*)malloc(ROOTLONG(boot)*512);//[0x1000];

    //rootdir((FATFILE*)fils_,disk);
    //return  1;
    //read_sata(getrootdir(boot),getrootlong(boot),(volatile u8*)fils_,disk.AHCIcontrolnum,disk.portnum);
    readD(ROOTDIRLBA(boot), (u8 *)fils_, ROOTLONG(boot), disk);
    
    for(int i = 0; i < indx1 ; i++){
        bool ret = false;
	for(int ii = 0 ; ii < ROOTENTR(boot) ; ii++){
            /*if(!fils_[ii].Name[0]){
                ret=true;
		break;;;
            }*\
            if(memcmp((void*)fils_[ii].Name, (void*)nn[i],11) && fils_[ii].Attributs==0x10){
		FATFILE dir = fils_[ii];
		memset((u8)0,fils_,ROOTENTR(boot));
                read_file(dir,(char*)fils_,/*getrootenris(boot)*sizeof(FATFILE),*disk);
                //readD(DATALBA(boot)+(fils_[ii].firstclusterlow*boot.bdb_sectors_per_cluster)-2, (u8*)fils_, boot.bdb_sectors_per_cluster,  disk);
		break;
            }
            
            
            //printc(nn[i],(u32)11,33);
            if(ii==ROOTENTR(boot)-1){
                printc("\nErr:",12);
                printc(nn[i],(u32)11,12<<4);
                printc("not find diractory",12<<4);
		mfree(fils_, getrootenris(boot)*sizeof(FATFILE));
                return 0;
            }
            
        }
	if(ret)break;
        
    }
    mfree(fils_, getrootenris(boot)*sizeof(FATFILE));
    memcp((void*)fils_,(void*)fils,(entrisn)*sizeof(FATFILE));*/
    return closter;
}
