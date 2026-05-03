#include "../utils/util.h"
#include "../mem/paging.h"
#include "../pc/timer.h" 
#include "../pc/keyboard.h"
#include "idt.h"

void system_call(regs* reg){
	//setreg(Kerenlregs)	;
	//printc("\nsystem call !!",12);
	//sleep(1000);
	//return;
	//asm("popa");
		
	if (reg->eax==1) {
			//printc("-->print",15);
			char* out = (char*)physic_adress(/*(u32*)reg->cr3,*/reg->ebx);
			u8 cl = reg->ecx;

			printc((char*)reg->ebx,(u32)reg->edx,cl);
	}
	else if (reg->eax==2){
			TTLclear();
			
			//printc("-->clear",15);
	}
	else if (reg->eax == 3){
		reg->eax = key;
	}
	else if(reg->eax==4){
		reg->eax=TimerClock;
	}
	else {
			printc("\nnon menchen system call.\n",12);
			
	}
	//reg->eip++;
	//while (1) {}
	return;
}
