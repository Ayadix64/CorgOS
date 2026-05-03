#include "../utils/util.h"
#include "../gdt/gdt.h"
#include "../cpu/CPU.h"
#include "../idt/idt.h"
#include "../pc/timer.h"
#include "../pc/RTC.h"
#include "../pc/keyboard.h"
#include "../mem/paging.h"
#include "../cpu/pci.h"
#include "../pc/Disk.h"
#include "../pc/Speker.h"
#include "../filesys/1fat16.h"
#include "../utils/stack.h"
#include "multitask.h"
Task* tasks;
u32 taskn = 0;
u32 tasksmaxn = 0x1000/sizeof(Task);
u32 sicn = 0;//sicel cnounter
u32 workin = 0;
u64 pid_cont = 0;

void multitk_init(){
	tasks = (Task*)malloc(0x1000);	
}


void MlutiSw(regs* r){
	if(taskn){
		//tasks[workin].reg=r[0];
		//workin ++;
		//workin %= taskn;
		//install_pagedir(tasks[workin].pd);
		//tasks[workin].reg.cr3=(u32)tasks[workin].pd;
		//r[0]=tasks[workin].reg;
			
		//print_char('.', 15);
		//void (*fn)() = (void(*)())tasks[workin].reg.eip;
		//fn();
		//memcp((void*)&tasks[workin].reg, (void *)r, sizeof(regs));
		//setreg(tasks[workin].reg);
		
	}
	return;
}

void loadtask(void* adrr ,u32 size, u8 type){
	u32 stack_panding = 0x1000;
	install_mainpd();
	/*if(!taskn){
		regs reg=loadreg();
		Kerenlregs=reg;
	}
	printc("load a task ...",15);*/
	//realloc((void*)tasks, taskn*sizeof(Task), (taskn+1)*sizeof(Task));
	u32 emp = 0;
	for(u32 i = 0 ; tasks[i].pid!=0 &&  i < tasksmaxn ; i++,emp++);;
	
	if(emp == tasksmaxn-1){
		realloc((void*)tasks, tasksmaxn*sizeof(Task), (tasksmaxn+0x100)*sizeof(Task));
		tasksmaxn+=0x100;
		emp=taskn;
	}
	prtcmem(adrr, size);
	pid_cont++;
	tasks[emp].pid = pid_cont;
	tasks[emp].Type = type;
	tasks[emp].TakeMem=size;
	tasks[emp].pt=(u32*)malloc_aligned(size,1024);

	
	//PageDir pd__;

	//init_pagedir(/*tasks[emp].pd*pd__, tasks[emp].pt, (size+0xfff)/0x1000);
	//for(int i = 0 ; i < (size+0xfff)/0x1000 ; i++){
	//link_page(/*tasks[emp].pt, *//*(i*0x1000)+*/(u32)adrr,0x100000);
	//}
	//u32 rdd = (u32)physic_adress((u32)adrr);
	regs r = loadreg();

	u32 stacksize = r.ebp-r.esp;
	tasks[emp].reg.ebp = ((u32)malloc(stacksize+stack_panding)) + (stacksize+stack_panding);//becuse the intel ingenures went to be the stack revrs, like not esp ++ but esp --, think about that
	tasks[emp].reg.esp = tasks[emp].reg.ebp - stacksize;
	memcp((void*)r.esp,(void*)tasks[emp].reg.esp , stacksize);
	
	////r.eip=(u32) physic_adress((u32)adrr);
	//r.cr3=(u32)tasks[emp].pd;
	//tasks[emp].reg=r;
	//setreg(r);
	//u32 eip  = (u32)adrr;//(u32)physic_adress((u32)adrr);
	//install_pagedir(pd__);
	//asm("pushl %0"::"r"(eip));
	//asm("movl %0 , %%esp"::"r"((u32)malloc(0x2000)));
	//asm("jmpl %0"::"r"(0));
	//install_mainpd();
	//asm("movl %0 , %%esp"::"r"((u32)Kerenlregs.esp));
	//printc(Hex((u32)physic_adress(0x100000)),15);
	
	//for(int i = 0 ; i < size ; i++)print_char(*(u8*)(0x100000+i), 15);
	//void (*ll)()=(void(*)())(/*(u32)physic_adress(0x0)*/);
	//ll();
	//taskn++;
	
}
