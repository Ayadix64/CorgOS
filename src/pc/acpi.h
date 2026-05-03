#ifndef BASIC_ACPI
#define BASIC_ACPI
#include "../utils/util.h"

#define RSDPLOWR 0xE0000
#define RSDPHIGR 0xFFFFF

struct RSDP{
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

struct ACPIDTH { //rsd Tabel
	char Signature[4];
	u32 Length;
	u8 Revision;
	u8 Checksum;
	char OEMID[6];
	char OEMTableID[8];
	u32 OEMRevision;
	u32 CreatorID;
	u32 CreatorRevision;
};

void * rsd_ptr();
void* acpi_rsd_rd(char* ent, u32 lng);
u32 acpi_vertion();
#endif
