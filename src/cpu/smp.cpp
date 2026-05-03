#include "../utils/util.h"
#include "../idt/idt.h"
#include "../pc/timer.h"
#include "../pc/acpi.h"
//extern "C"  void APInit();

void initCors(){
	void* madt = acpi_rsd_rd("APIC", 4);
	printc("\n@",15);
	printc(Hex((u32)madt),15);
	printc("\n",15);
	printc((char*)madt,(u32)150,15);

	/*while (1) {
	
	}*/
}
