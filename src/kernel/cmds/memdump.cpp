#include "../shell.h"

void memdump(int argc, char* arg){
	//TTLclear();
	if(memcmp((void*)arg,(void*)"find",4)) {
		char* find = (char*)malloc(0x1000);
		
		while (1) {
			printc("\nEnter Adress(in Hex) : ",15);
			u8* adress =(u8*)gethex(14);
			while (key=='\n') ;
			printc("\nTo Adress : ",15);
			u32 lng = gethex(14);
			while (key=='\n') ;
			printc("\nFind : ",15);
			
			u32 findlng = getcin(find,0x1000,14);
			while (key=='\n') ;
			for(u32 i = 0 ; i < lng ; i++){
				if(memcmp((void*)((u32)adress+i),(void*)find,findlng) && findlng){
					printc("\nFind @",15);
					printc(Hex((u64)(adress+i)),14);
					i+=findlng;
				}
				if((LCtrl || RCtrl)&&(key=='c' || key=='C')){break;}
				/*printc(BHex(adress[i]),15);
				printc(" ",15);
				if(i && !((i+1)%8)){
					printc("       ",15);
					for(int ii = i-8 ; ii < i ; ii++){
						print_char(adress[ii], 15);
					}
					printc("\n",15);
					printc(DHex((u64)(adress+i)),14);
					printc(" : ",14);
				}*/
			}
			if(((LCtrl || RCtrl)&&(key=='c' || key=='C'))  ){break;}
			printc("\nPress Q to exit or any key to continu.",9);
			while(!KeyPress){}
			if(key=='Q' || key == 'q'){
				break;
			}

		}
		mfree((void*)find, 0x1000);
	}

	else {while(1){
		printc("\nEnter Adress(in Hex) : ",15);
		u8* adress =(u8*)gethex(14);
		while (key=='\n') ;
		printc("\nEnter Long : ",15);
		u32 lng = getint(14);
		while (key=='\n') ;
		printc("\n********************************************************************************",15);
		//TTLclear();
		u32 I = 0;
		printc(DHex((u64)adress),14);
		printc(" : ",14);
		for(int i = 0 ; i < lng ; i++){
			printc(BHex(adress[i]),15);
			printc(" ",15);
			if(i && !((i+1)%8)){
				printc("       ",15);
				for(int ii = i-8 ; ii < i ; ii++){
					print_char(adress[ii], 15);
				}
				printc("\n",15);
				printc(DHex((u64)(adress+i)),14);
				printc(" : ",14);
			}
		}
		printc("\nPress Enter to Continu or Q to Quit",15);
		while (1){
			if(key=='\n'){
				break;
			}else if (key=='q' || key=='Q') {
				return;
			}
		}
	}}

}

