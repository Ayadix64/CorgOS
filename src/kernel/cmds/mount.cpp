#include "../shell.h"



void mount(int argc, char* arg){
	while (1) {
		printc("\nType of disk [BOOT.0,ATA.1,SATA.2]: ",15);
		u32 type = getint(14);
		if(!type){
			if(bootfind){
				//MainDisk=bootdisk;
				if(isitfat12(bootdisk)){
					MainDisk = bootdisk;
				}else{
					printc("\nNon suported file system or non file system in the target disk.",12);
					continue;
				}
			}else{
				printc("\n*boot disk non found, set it to Main disk know*",12);
				//disk=MainDisk;
				continue;
			}
			break;
		}else if(type==1){
			printc("\nThey are ",15);
			printc(Int_To_STR(ATADISKn, 10),15);
			printc(" ATA Disk",15);
			printc("\nslect one : ",15);
			u32 i = getint(14);
			if(i>ATADISKn || i==0){
				printc("\nnon Valiad opetion",15);
				continue;
			}
			//else
			if(isitfat12(ATADISK[i-1])){
				MainDisk = ATADISK[i-1];
			}else{
				printc("\nNon suported file system or non file system in the target disk.",12);
				continue;
			}
			break;
		}
		else if(type==2){
			printc("\nThey are ",15);
			printc(Int_To_STR( StatsDisksnum, 10),15);
			printc(" SATA Disk",15);
			printc("\nslect one : ",15);
			u32 i = getint(14);
			if(i>StatsDisksnum || i==0){
				printc("\nnon Valiad opetion",15);
				continue;
			}
			//else
			if(isitfat12(SATADisks[i-1])){
				MainDisk = SATADisks[i-1];
			}else{
				printc("\nNon suported file system or non file system in the target disk.",12);
				continue;
			}
			//MainDisk = SATADisks[i-1];
			break;
		}else{
			printc("\nNon Option.",12);
			if((LCtrl||RCtrl)&&(key=='c'||key=='C'))return;;
		}
		
	}
	for(int i = ldirlng ; i > 1 ; i--,ldirlng--){
		ldir[i-1]=0;
	}
	return;
}
