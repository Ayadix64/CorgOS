#include "../shell.h"


void memtest(int arc, char* arg){
    TTLclear();
    printc("Wellcom to Ayadi OS Dynamic Memory test\n"
           "This program test the softoar, but also 'mybe' the Hard ware.\n"
           "you can exit by do Ctrl-C\n\n",15);
    printc("Memory size : ",15);
    
    u32 Failcount = 0;
    u64 memsize = get_mem_info().Extend2;
    printc(Int_To_STR(memsize,10),15);
    printc(" byte ( ",15);
    printc(Hex(memsize),15);
    printc(" ).\n",15);
    u64 Lasttick = TimerClock;
    //char* mbuff = (char*)malloc(0x2000); 
    while(1){
        //TTLclear();
        set_print_pos(0,6);
        
        char* buffer;
        buffer=(char*)malloc(0x2000);
        
	char* buf2=(char*)malloc(0x2000);
	printc("bufer adress : ",15);
        printc(Hex((u64)(u32)buffer),14);
       	mfree(buffer,0x2000);
	mfree(buf2, 0x2000);
        if(!buffer){
            printc(" FAILD !!!",12);
            Failcount++;
            continue;
        }
       	printc("  time tack to alloc : ",15);
	printc(Int_To_STR(TimerClock-Lasttick, 10),15);
	Lasttick=TimerClock; 
        memset((u8)'A',buffer,0x2000);
        if(Failcount){
            printc(" fial count : ",15);
            printc(Int_To_STR(Failcount,10),12);
        }
        printc("\n\n\n          ",15);
	u64 cur = ((u64)((((double)(u64)buffer)/((double)memsize))*60.0));
        for(int i = 0 ; i < 60 ; i++){
            if(i<cur){
                //printc((char*)(u8[2]){178,0},14);
                print_char(219,14);
            }else{
                print_char(176,15);
            }
        }
        printc("\n\n\n",15);
	if(arg[0]=='S')sleep(1000);
        //printc(buffer,(u32)80,15);
        memset((u8)0,buffer,0x2000);

       
        
        //printc("\r",15);
        if((LCtrl|RCtrl )&&(key=='c'|key=='C'))return;
        
    }
    return;
}

