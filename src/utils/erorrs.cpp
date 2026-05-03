#include "util.h"
void printerr(char* errname){
	printc("\n[-ERORR-] : ",12);
	printc(errname,12);
	return;
}

void printwarning(char* errname){
	printc("\n[WARNING] : ",14);
	printc(errname,14);
	return;
}

void printBerr(char* errmsg ){ //big erorr
	printc("\n[!ERORR!] : ",12<<8);
	printc(errmsg,12<<8);
	printc("\n",12);
}


void paink(char* msg ){
	TTLclear();
	for(u32 i = 0 ; i < TTLWidth*TTLHigth ; i++){
		print_char(219, 12);
	}
	char* errstr = "-KERNEL PAINIC-";

	set_print_pos((u8)((TTLWidth/2)-(sizeof(errstr)/2)), 0);
	printc(errstr,12<<4 );
	
	u32 msglen = strlen(msg);
	u32 px = ((TTLWidth/2)-(msglen/2))>0?((TTLWidth/2)-(msglen/2)):0;
	
	
	set_print_pos((u8)px, (TTLHigth/2)-5);
	printc(msg,12<<4);	

	while (1) {
	
	}
}
