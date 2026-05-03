#ifndef KERNEL_SHELL
#define KERNEL_SHELL
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
#include "../multitask/multitask.h"
#include "../pc/ps2mouse.h"
extern "C" char *ldir;
extern "C" u32 ldirlng;
extern "C" DISK MainDisk;
void dir      (int argc, char* arg);
void toch     (int argc, char* arg);
void echo     (int argc, char* arg);
void read     (int argc, char* agr);
void clear    (int argc, char* arg);
void blabla   (int argc, char* arg);
void fat      (int argc, char* arg);
void time     (int argc, char* arg);
void mkdir    (int argc, char* arg);
void write    (int argc, char* arg);
void cd       (int argc, char* arg);
void memtest  (int argc, char* arg);
void bitmap   (int agrc, char* arg);
void help     (int agrc, char* arg);
void sysinfo  (int argc, char* arg);
void rm       (int argc, char* arg);
void tree     (int argc, char* arg);
void Dtest    (int argc, char* arg);
void memdump  (int argc, char* arg);
void calc     (int argc, char* arg);
void lsdev    (int argc, char* arg);
void mount    (int argc, char* arg);
void regsvw   (int argc, char* arg);
void exec     (int argc , char* arg);
void installer(int argc , char* arg);

using namespace FAT12;

#endif
