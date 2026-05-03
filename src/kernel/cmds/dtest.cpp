#include "../shell.h"



void Dtest(int argc, char* arg){	
	DISK disk;
	TTLclear();
	while (1) {
		printc("\nType of disk [BOOT.0,ATA.1,SATA.2]: ",15);
		u32 type = getint(14);
		if(!type){
			if(bootfind){
				disk=bootdisk;
			}else{
				printc("\n*boot disk non found, set it to Main disk know*",12);
				disk=MainDisk;
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
			disk = ATADISK[i-1];
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
			disk = SATADisks[i-1];
			break;
		}else{
			printc("\nNon Option.",12);
			if((LCtrl|RCtrl )&&(key=='c'|key=='C'))return;
		}
	}
	
	bool lama=false;
	int i = 0;
	if(argc){
		i=strint(arg, argc-1);
	}
	TTLclear();
	u32 nonempty = 0;
	char sec[512];
	u8 zero[512];
	memset((u8)0,(void*)zero,512);
	for(;;){
		memset((u8)0,sec,512);
		readD(i, (u8 *)sec, 1,  disk); 
		set_print_pos(0, 0);
		printc("sector : ",15);
		printc(Int_To_STR(i, 10),14);
		printc("   non empty cluster : ",15);
		printc(Int_To_STR(nonempty, 10),14);
		printc("\n",15);
		printc(sec,(u32)512,15);
		/*if(!argc){i++;
		sleep(100);}*/
		sleep(100);
		set_print_pos(1, 1);
		if(!memcmp((void*)sec,(void*)zero,512))nonempty++;
		for(int ii = 0 ; ii < 80*24 ; ii++)printc(" ",1);
		if((LCtrl|RCtrl )&&(key=='c'|key=='C'))break;
		if(key=='+'){i++;while (key) {
		
		}}
		if (key=='-' && i > 0) i--;
		
	}
}

