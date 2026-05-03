#include "../utils/util.h"

//TODO: do somthing
u32 VGAWidth=640;
u32 VGAHeithg=480;
u32 TTLWidth=80;
u32 TTLHigth=25;

#define SVGA_INDEX  0x1CE
#define SVGA_DATA   0x1CF

void setvgareg(u8 reg , u16 data){
	outb(SVGA_INDEX,reg);
	outb(SVGA_DATA,data);
}
