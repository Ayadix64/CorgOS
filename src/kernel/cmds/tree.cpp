#include "../shell.h"

void tree(int argc, char* arg){
	FAT12BOOT boot;
	RDBOOT(boot, MainDisk);
	FATFILE * fils = (FATFILE*)malloc(getrootlong(boot)*512);
	mfree(fils, getrootlong(boot)*512);
	
	FATFILE folder;
	folder.firstclusterlow=fdir(ldir, fils, 0xffff, MainDisk);
	
	Stack<int> is;
	Stack<FATFILE> folders;
	//memset((u8)0,fils,getrootlong(boot)*512);
	//folders.push(folder);
	//read_file(folder,(char*)fils,MainDisk);
	printc("\n",15);
	for(int i = 0 ; i < getrootenris(boot) ; i++){
		
		if(fils[i].Attributs==0x10){
			if(!((memcmp((void*)fils[i].Name ,(void*) ".                  ",11)||
			      memcmp((void*)fils[i].Name , (void*)"..                 ",11)))  )
			{
				sleep(200);
				for (int i = 0 ; i< is.size()  ; i++) printc("  ",15);		
				printc("-->",14);
				printc((char*)fils[i].Name,(u32)11,14<<4);
				printc("\n",15);

				folders.push(folder);
				is.push(i);
				folder=fils[i];
				memset((u8)0,fils,getrootlong(boot)*512);
				read_file(folder,(char*)fils,MainDisk);
				i=2;
				continue;

			}else{
				//i++;
				continue;
			}
		
		}else if(fils[i].Attributs==0x20){
			for (int i = 0 ; i< is.size()  ; i++) printc("  ",15);		
			printc("---",14);
			printc((char*)fils[i].Name,(u32)11,15);
			printc("\n",15);

		}
		if(i==(getrootenris(boot)-1)){
			if(is.size()){
				//for(int ii = 0 ; ii < is.size() ; ii++){
				//	printc(Int_To_STR(is[ii], 10),15);
				//	printc("\n",15);
				//}
				folder=folders.pop();
				read_file(folder,(char*)fils,MainDisk);
				memset((u8)0,fils,getrootlong(boot)*512);
				i=is.pop();
			}else {

				is.end();
				folders.end();
				return;
			}
		}
	}
	//folders.push(folder);
	/*u32 ret = 1;	
	printc("\n\n",12);
	bool done = false;
	//is.push(0);
		
	for(int i = 0 ; i < getrootenris(boot) ; i++){
		//sleep(200);
		//done=true;
		if(fils[i].Attributs==0x10){
			if(memcmp((void*)fils[i].Name ,(void*) ".                 ",11)||memcmp((void*)fils[i].Name , (void*)"..               ",11))
			{
				done=false;
				continue;
			}else{
				printc("-->",15);
				printc((char*)fils[i].Name,(u32)11,14<<4);
				printc("\n",15);
				sleep(200);
				is.push(i);
				folders.push(folder);
				done=false;
				folder=fils[i];
				read_file(folder,(char*)fils,MainDisk);
				//i=0;
				ret++;
				
				continue;
			}
			


		}
		//if(i==getrootenris(boot)-1){done=true;}
		if(i==getrootenris(boot)-1){
			if(is.size()|folders.size()){
				folder=folders.pop();
				read_file(folder,(char*)fils,MainDisk);
				i=is.pop();
			}else return;
			//ret>0?ret--:ret=ret;
			//if(!ret && i)return;
			//continue;		
				
		}
	
		if(fils[i].Attributs==0x20){ for(int ii = 0 ; ii < is.size() ; ii++)printc("  ",15); printc("--",15);
			printc((char*)fils[i].Name,(u32)11,15);
			printc("\n",15);
			sleep(100);
		}
		
			
	}*/

}
