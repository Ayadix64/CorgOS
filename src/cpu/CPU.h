#ifndef CPU_h
#define CPU_h

#include "../utils/util.h"

void cpuid(u32& eax, u32& ebx, u32& ecx , u32& edx);
u64 rdmsr(u32 msr);
void wrmsr(u32 msr, u64 val);
u64 rdtsc();
namespace CPU_info
{
   

    u64 get_cpu_freqence();
    char* get_cpu_name();
} // namespace CPU_info
#endif