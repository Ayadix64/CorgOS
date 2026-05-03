#ifndef TIMER_H
#define TIMER_H
#include "../utils/util.h"
extern "C" u64 TimerClock;
extern "C" u64 Seconds;

typedef struct RSDPDescriptor {
    char signature[8];
    u8 checksum;
    char oemid[6];
    u8 revision;
    u32 rsdt_address; // if ACPI version 1.0
    // below is only valid for revision >= 2
    u32 length;
    u64 xsdt_address;
    u8 extended_checksum;
    u8 reserved[3];
}__attribute__ ((packed));

typedef struct acpi_sdt_header {
    char signature[4];       // "RSDT" أو "XSDT"
    u32 length;         // طول الجدول بالكامل
    u8 revision;
    u8 checksum;
    char oem_id[6];
    char oem_table_id[8];
    u32 oem_revision;
    u32 creator_id;
    u32 creator_revision;
} __attribute__((packed));

typedef struct HPET {
    char signature[4];     // "HPET"
    u32 length;
    u8 revision;
    u8 checksum;
    char oem_id[6];
    char oem_table_id[8];
    u32 oem_revision;
    u32 creator_id;
    u32 creator_revision;
    u8 hardware_rev_id;
    u8 comparator_count : 5;
    u8 counter_size     : 1;
    u8 reserved         : 1;
    u8 legacy_replacement : 1;
    u16 pci_vendor_id;
    struct {
        u8 address_space_id; // 0 for system memory
        u8 register_bit_width;
        u8 register_bit_offset;
        u8 reserved;
        u64 address;
    } address;
    u8 hpet_number;
    u16 minimum_tick;
    u8 page_protection;
} __attribute__((packed));

typedef struct hpet_t {
    u64 capabilities;         // 0x00
    u64 reserved0;
    u64 configuration;        // 0x10
    u64 reserved1;
    u64 interrupt_status;     // 0x20
    u64 reserved2[24];
    u64 main_counter;         // 0xF0
} __attribute__((packed));

void set_timer_f(u32 freq);
void timer_init();
void sleep(u32 milisecends);
void pit_sleep(u32 ms);
void hpet_sleep_ms(u64 ms);
#endif