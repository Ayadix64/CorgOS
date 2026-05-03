#ifndef MULTITASKING
#define MULTITASKING
#include "../utils/util.h"
#include "../idt/idt.h"
#include "../mem/paging.h"
/*<lol>
 * 	you have to alloc the process and realloc them if thry are a new 
 * 	procec and alloc page table and realoc it evry time meory taking change
 * </lol>*/
typedef struct Task{
	u8  Type=0;
	u64 pid;
	u32 TakeMem=0;
	regs reg;
	PageDir pd;
	u32* pt;
};
void multitk_init();
void MlutiSw(regs* r);
void loadtask(void* adrr ,u32 size, u8 type);
#endif
