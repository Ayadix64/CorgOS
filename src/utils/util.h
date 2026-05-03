#ifndef Util_h
#define Util_h

#define KERNEL_LONG 256
#define KERNEL_POINTER 0x10000
#define KERNEL_START_LBA 2
#define SECTOR_SIZE 512
#define STACK_PTR 0x900000

#define PACKED  __attribute__((packed))
#define ALIGNED(x)  __attribute__((aligned(x)))

#define get_bit(in,bit) (in&(1<<bit))>0?1:0
#define switche_byte16(x) ((x&0xff)<<8)|((x>>8)&0xff)
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

#define NULL (void*)0 ;
#define BOOT_SEGN 0x8000
#define BOOT_SEG_LBA 2
#define BOOT_SEG_LONG 6 

#define SYSTEM_ARCHITACTER 32 //32 bit, my be i will do 64 bit vertion ?


struct V2f
{
    float x;
    float y;
};


typedef struct{
    u32 Extend1 = 0;
    u64 Extend2 = 0;
}MEM_INFO;



extern "C" u32 VGAWidth;
extern "C" u32 VGAHeithg;
extern "C" u32 TTLWidth;
extern "C" u32 TTLHigth;




void print_char(u8 val , u8 Cl);
void printc(char* str, int Clor,...);
void TTLSW(void* buf,u32 x, u32 y);
void print_to_buf(char* str,void* buf,u32& x_, u32&y_, int Clor);
void movecrus(u8 x, u8 y);
V2f get_print_pos();
void printc(char* str , u32 size , u8 Cl );
void set_print_pos(u8 x , u8 y);
void sleep_print(char* str, u8 cl);
void TTLclear();
void laze_printc(char* str, u8 cl);
void sleep_print(char* str, u8 cl , u32 ms);
void sleep_print(char* str, u8 cl , u32 size, u32 ms );
void print_char_at(u8 val ,u8 x , u8 y, u8 Cl);
void printc_at(char* val ,u8 x , u8 y, u8 Cl);
u32 getcin(char *ret,u32 lng,u8 cl);
u32 getint(u8 cl);
u32 gethex(u8 cl);


bool memcmp(void* ptr1 , void* ptr2 , u32 lon);
 void* memfind(void* start/*first pointer of find area*/ ,
                     void* find /*find word*/,
                     u32 findlon/*long of find word*/ ,
                     u32 reserch_regen_lon/*the longth of find area*/);
void memcp(void* tocp , void* incp , u32 lng);
void memset(u32 toset , void* loc , u32 lng);
void memset(u16 toset , void* loc , u32 lng);
void memset(u8 toset , void* loc , u32 lng);
void memappend(void* buf, u32 buflng, void* append, u32 appendlng);/*the lasts are as you respe ct*/

bool readbm(volatile u8* bm, u32 bit);
void writebm(volatile u8* bm, u32 bit , bool val);/*bitmap memorys bufer, usely*/

void start_read_sector8(u32 lba,u32 count, u8* buffer);/*Ahh, wath?*/


void DMem_init();
MEM_INFO get_mem_info();
u32 usingmem();/*return using dynamic memort*/

char* Int_To_STR(int val, int base);
char * Hex(u64 num);
char* DHex(u32 num);
char* SHex(u16 num);
char* BHex(u8  num);

char* Bin(u32 num);
u32 strlen(char* str);

u32 strint(char* in, u32 lng);
u32 Hex2int(char* hex,u32 lng);

extern "C" void* malloc(u32 size);
void* malloc_aligned(u32 size, u32 aligned);
void* malloc(u32 size , u32 start_adress , u32 end_adress);
void* unloked_malloc(u32 size , u32 start_adress , u32 end_adress);
extern "C" void  mfree(void* adress,u32 size);
void* unloked_malloc_aligned(u32 size, u32 aligned , u32 stadrr , u32 endadrr);
void  prtcmem(void* adrr , u32 size);
void* realloc(void* buf, u32 busize, u32 newsize);


u8 inb(u32 ad);
void outb(u32 ad, u8 v);

u16 inw(u32 ad);
void outw(u32 ad, u16 v);

u32 inl(u32 ad);
void outl(u32 ad, u32 v);


void idt_init();
void systemcall();

void cpuid(u32& eax, u32& ebx, u32& ecx , u32& edx);
u64 rdmsr(u32 msr);
void wrmsr(u32 msr, u64 val);


void printerr(char* errname);
void printwarning(char* errname);
void printBerr(char* errmsg );

void paink(char* msg );

//#include "stack.h"



struct HARD_DISK_INFO {
    u16 General_Config;            // تكوين الجهاز العام
    u16 Cylinders;                 // عدد الأسطوانات (CHS)
    u16 Reserved1;
    u16 Heads;                     // عدد الرؤوس (CHS)
    u16 Track_Sectors;             // عدد القطاعات لكل مسار (CHS)
    u16 Sector_Bytes;              // عدد البايتات لكل قطاع (غالبًا 512)
    u16 Vendor_Specific1[3];
    u8  Serial_Number[20];         // الرقم التسلسلي (ASCII)
    u16 Buffer_Type;
    u16 Buffer_Size;               // حجم المخزن المؤقت (Cache)
    u16 ECC_Bytes;
    u8  Firmware_Revision[8];      // إصدار الـ Firmware (ASCII)
    u8  Model_Name[40];            // اسم الموديل (ASCII)
    u16 Max_Sectors_Per_Interrupt;
    u16 Capabilities;              // القدرات (LBA، DMA)
    u16 Reserved2;
    u16 PIO_Mode;
    u16 DMA_Mode;
    u16 Validity_Flags;            // صلاحية الحقول
    u32 LBA_28bit_Count;           // عدد القطاعات في وضع LBA-28
    u16 Reserved3[2];
    u64 LBA_48bit_Count;           // عدد القطاعات في وضع LBA-48
    u16 Reserved4[22];
    u16 Security_Status;           // حالة الأمان
    u16 Reserved5[31];
    u16 ATA_Version_Major;         // إصدار ATA المدعوم
    u16 ATA_Version_Minor;
    u16 Supported_Features[2];     // الميزات المدعومة
    u16 Reserved6[48];
} __attribute__((packed));



int disk_info(HARD_DISK_INFO* hhd_info);/*wath?*/
#endif
