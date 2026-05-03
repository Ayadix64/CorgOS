#include "../utils/util.h"
#include "../idt/idt.h"
#include "acpi.h"
#define RSDPLOWR 0xE0000
#define RSDPHIGR 0xFFFFF

/*struct RSDP{
	char Signature[8];
	u8 Checksum;
	char OEMID[6];
	u8 Revision;
	u32 RsdtAddress;      // deprecated since version 2.0

	u32 Length;
	u64 XsdtAddress;
	u8 ExtendedChecksum;
	u8 reserved[3];
} __attribute__ ((packed));
*/
struct RSDT { //rsd Tabel
	char Signature[4];
	u32 Length;
	u8 Revision;
	u8 Checksum;
	char OEMID[6];
	char OEMTableID[8];
	u32 OEMRevision;
	u32 CreatorID;
	u32 CreatorRevision;
}__attribute__ ((packed));

struct RSD {
	RSDT rsdt;
	u32 entries[];
}__attribute__ ((packed));

struct XSD {
	RSDT xsdt;
	u64 entries[];
}__attribute__ ((packed));



void * rsd_ptr(){
	void * ret = memfind((void*)RSDPLOWR, (void *)"RSD PTR ", 8, (RSDPHIGR-RSDPLOWR));
	if(!((u32)ret)){
		printc("\nThe ACPI.RSD (ROOT SYSTEM DECRIPTOR) NONFOUND !!! //This is probaly bad",12);
	}
	return ret;
}

void* acpi_rsd_rd(char* ent, u32 lng){
	RSDP* rsdp = (RSDP*)rsd_ptr();
	//void *rsdt = (void*)0; 
	u32 length = 0xff;
	
	bool xsdt_seport = false;
	RSD* rsdt = (RSD*)0;
	XSD* xsdt = (XSD*)0;
	
	if(!(u32)rsdp)return (void*)0;;
	if(rsdp->Revision>=2){
		xsdt_seport=true;
		if(rsdp->XsdtAddress>=0xffffffff && SYSTEM_ARCHITACTER < 64){
			printc("\nACPI.XSDT is over 4Gb reagen, and this is 32 bit OS.",12);
			return (void*)0;
		}else {
			xsdt = (XSD*)rsdp->XsdtAddress;
			length = xsdt->xsdt.Length - sizeof(RSDT);
		}
	}else {
		rsdt = (RSD*)rsdp->RsdtAddress;
		length = rsdt->rsdt.Length - sizeof(RSDT);	
	}
	

	void *find = (void*)(u32)0;
	for(u32 i = 0 ; i < (length/(xsdt_seport?8:4)) ; i++){
		if(!xsdt_seport){
			if (memcmp((void*)((u32)rsdt->entries[i]),(void*)ent,lng)){
				find = (void*)rsdt->entries[i];
				break;
			}
			
		
		}else if(xsdt_seport){
			if(xsdt->entries[i]>=0xffffffff && SYSTEM_ARCHITACTER < 64){
				printc("\nA entry of ACPI.XSDT is over 4Gb reagen, and this is 32 bit OS.",12);	
				
			}else {
				if (memcmp((void*)((u32)(u64)xsdt->entries[i]),(void*)ent,lng)){
					find = (void*)((u32)(u64)xsdt->entries[i]);
					break;
				}
			}
		}

	}
	
	
	if(!(u32)find){
		printc("ACPI.(R/X)SDT.",12);
		printc(ent,(u32)lng,12);
		printc(" NONFOUND !!!",12);
	}
	return find;
}
u32 acpi_vertion(){
	RSDP* rsdp = (RSDP*)rsd_ptr();
	if(!((u32)rsdp))return 0;;
	return  rsdp->Revision;
}
