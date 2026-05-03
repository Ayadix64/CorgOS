#include "../idt/idt.h"
#include "../utils/util.h"

int msx=0,msy=0;


void mouse_handler(regs* reg){
    static char ms_data[3];
    static u32 si = 0;
    
    char status = (char)inb(0x64);  
    if ((status & 0x20)){
    char in = inb(0x60);
    ms_data[si]=in;
    si++;
    si%=3;
    
    
    if(si==0){
        if((msx+ms_data[1])<79 && (msx+ms_data[1])>=0){
		msx+=ms_data[1]/3;
        }else if(msx+ms_data[1]>=80){
		msx=79;
	}else {
		msx=0;
	}
        if((msy-ms_data[2])<24 && (msy-ms_data[2])>=0){
            msy-=ms_data[2]/3;
        }else if((msy-ms_data[2])>=25){
		msy=24;
	}else {
		//msy=0;
	}
        //printc("\nx:",10);
        //printc(Int_To_STR(msx,10),10);
        //printc("\ny:",10);
        //printc(Int_To_STR(msy,10),10);
        //*(short*)(0xb8000+msy*80+msx)=('0'<<8)|15;
    }}
    return;

}
void mouse_init(){
    	outb(0x64, 0xA8);
	outb(0x64, 0x20);
	u8 status = inb(0x60) | 2;
	outb(0x64, 0x60);
	outb(0x60, status);
	
	outb(0x64,0xD4);
	outb(0x60,0xf6);
	//mouse_write(0xF6);
	
	//mouse_read();
	inb(0x60);

	outb(0x64,0xD4);
	outb(0x60,0xf4);
	//mouse_write(0xF4);
    	inb(0x60);
    	msx = 0;
    	msy = 0;
    	install_irq(12,mouse_handler,0);

}

