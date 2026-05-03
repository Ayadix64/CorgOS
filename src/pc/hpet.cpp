#include "acpi.h"
#include "../idt/idt.h"
#include "../utils/util.h"

struct HPET_{
	ACPIDTH hader;
	
	u8 hardware_rev_id;
	u8 comparator_count:5;
	u8 counter_size:1;
	u8 reserved:1;
	u8 legacy_replacement:1;
	u16 pci_vendor_id;
	struct {
		u8 address_space_id;    // 0 - system memory, 1 - system I/O
		u8 register_bit_width;
		u8 register_bit_offset;
		u8 reserved;
		u64 address;
	} address __attribute__ ((packed));
	u8 hpet_number;
	u16 minimum_tick;
	u8 page_protection;
}__attribute__ ((packed));

HPET_* HPET = (HPET_*)0;

void hept_init(){
	void* adress = acpi_rsd_rd("HPET",4);
	if((u32)adress){
		HPET = (HPET_*)adress;
	}else {
		printc("",0);
	}
}
