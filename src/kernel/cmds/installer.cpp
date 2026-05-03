#include "../shell.h"

void installer(int argc , char* arg){//hack yeah!,not realy
	TTLclear();
	printc("Hello to Ayadi OS installer , will , this program is -ALPHA and is very likly to fail, Before you do any thing, pleaz read this:",15);
	printc("\nWARNING:"
	       "\n========\n\n",14);
	printc("\n1. This programe install the Ayadi OS kernel in the slecting disk, if you have already a Ayadi OS on your disk , it mey you will lose your data,",15);
	printc("else ..., ",14);
	printc("YOU WILL LOST ALL YOUR DATA!!!\n",12);
 	printc("\n2. This OS is -Alpha, so mybe you will not h2. This OS is -Alpha, so mybe you will heve the most smoth exprince (at list if you have one)\n",15);
	printc("\n3. The system not seporting partichinig yet, soo, if you do a partion just for ayadi is (wich is not pemaly), it will not work, it just will just install it and you will probaly lost all your data",15);
	bool agree = false;
	
	for(;;){
		printc("\nAre you agree ? [y,n] : ",15);
		u8 In  = 0;
		getcin((char*)&In, 1, 14);
		while (key) ;
		if(In=='y' || In == 'Y'){
			agree=true;
			break;
		}else if(In=='n' || In == 'N'){
			agree=false;
			break;
		}else{
			printc(" ???",14);
		}
	}
	if(agree){
		TTLclear();
		DISK disk;
		while (1) {
			printc("\nType of disk [ATA.0,SATA.1]: ",15);
			u32 type = getint(14);
			 if(type==0){
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
				disk = ATADISK[i-1];
				break;
			}
			else if(type==1){
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
				disk = SATADisks[i-1];
				break;
			}else{
				if((LCtrl|RCtrl )&&(key=='c'|key=='C'))return;
				printc("\nNon Option.",12);

			}
		}
		FAT12BOOT boot; 
		readD(0, (u8 *)&boot, 1, disk);
		if(memcmp((void*)&boot,(void*)0x8000,512)){
			printc("\nIt look that you already have Ayadi OS in the target disk .\n",14);
			for(;;){
				printc("\nContinu? [y,n] : ",15);
				u8 In  = 0;
				getcin((char*)&In, 1, 14);
				if(In=='y' || In == 'Y'){
					agree=true;
					break;
				}else if(In=='n' || In == 'N'){
					agree=false;
					break;
				}else{
					printc(" ???",14);
				}
			}
			if(agree){
				TTLclear();
				printc("**installing Ayadi OS in the ",15);
				printc(disk._ATA?(char*)"ATA IDE PORT#":disk._SATA?(char*)"SATA PORT#":disk._USB?(char*)"USB":(char*)"UNKOWN",15);
				printc(disk._ATA?(Int_To_STR(disk.index1, 10)):disk._SATA?Int_To_STR(disk.index1, 10):(char*)"NON",15);
				printc("**",15);
				for(int i = KERNEL_START_LBA ; i < KERNEL_LONG ; i++){
					writD(i, (u8 *)(0x7c00+(i-KERNEL_START_LBA+1)*512), 1, disk);
					set_print_pos(0, 2);
					u64 cur = ((u64)((((double)(u64)i)/((double)KERNEL_LONG))*60.0));
        				printc("Instaling... |",15);
					for(int i = 0 ; i < 60 ; i++){
        				    if(i<cur){
				                print_char(219,10);
				            }else{
        				        print_char(176,15);
        				    }
					}
					    
					printc("|  ",15);
					printc(Int_To_STR((i*100)/KERNEL_LONG, 10),15);
					printc("%",15);
        			}
				printc("\nComplite!",10);
				
			}else{
				printc("\n Install cancled.",15);
				return;	
			}	
		}else {
			printc("\nIt look that your disk haven't Ayadi OS on it, that will cause that you will lost all your data.\n",14);
			for(;;){
				printc("\nContinu? [y,n] : ",15);
				u8 In  = 0;
				getcin((char*)&In, 1, 14);
				if(In=='y' || In == 'Y'){
					agree=true;
					break;
				}else if(In=='n' || In == 'N'){
					agree=false;
					break;
				}else{
					printc(" ???",14);
				}
			}
			if(agree){
				TTLclear();
				printc("**installing Ayadi OS in the ",15);
				printc(disk._ATA?(char*)"ATA IDE PORT#":disk._SATA?(char*)"SATA PORT#":disk._USB?(char*)"USB":(char*)"UNKOWN",15);
				printc(disk._ATA?(Int_To_STR(disk.index1, 10)):disk._SATA?Int_To_STR(disk.index1, 10):(char*)"NON",15);
				printc("**",15);
				for(int i = KERNEL_START_LBA ; i < KERNEL_LONG ; i++){
					writD(i, (u8 *)(0x7c00+(i-KERNEL_START_LBA+1)*512), 1, disk);
					set_print_pos(0, 2);
					u64 cur = ((u64)((((double)(u64)i)/((double)KERNEL_LONG))*60.0));
        				printc("Instaling... |",15);
					for(int i = 0 ; i < 60 ; i++){
        				    if(i<cur){
				                print_char(219,10);
				            }else{
        				        print_char(176,15);
        				    }
					}
					printc("|  ",15);
					printc(Int_To_STR((i*100)/KERNEL_LONG, 10),15);
					printc("%",15);
        			}
				writD(0, (u8 *)0x7c00, 1, disk);
				printc("\nFile System ",15);
				u32 fat = getFatAT(boot);
				u32 fatlong = getFatLong(boot);
				u8 zero[512];
				memset((u8)0,(void*)zero,512);
				for(int i = fat ; i < fat+fatlong ; i++){
					writD(i, (u8 *)zero, 1, disk);
					printc(".",15);
				}
				u32 entres = getrootdir(boot);
				u32 entlng = getrootlong(boot);
				for(int i = entres ; i < entres+entlng ; i++){
					writD(i, (u8 *)zero, 1, disk);
					printc(".",15);
				}
				printc("\nComplite!",10);	
				}
			else{
				printc("\n-->Install cancled.",15);
				return;	
			}
		}
		
	}else{
		printc("\n-->Install cancled.",15);
		return;
	}

}
