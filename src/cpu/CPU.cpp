#include "../utils/util.h"
#include "../cpu/CPU.h"
void cpuid(u32& eax, u32& ebx, u32& ecx , u32& edx){
    asm volatile("cpuid" : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx) : "a"(eax),"b"(ebx) ,"c"(ecx),"d"(edx) );
    return;
}

u64 rdmsr(u32 msr){
    u32 high, low;
       asm ("rdmsr": "=a"(low), "=d"(high) : "c"(msr));
        return (((u64)high) << 32) | low;
}
void wrmsr(u32 msr, u64 val){
    u32 hight=(val>>32)&0xffffffff,low=(val&0xffffffff);
    asm volatile("wrmsr"::"c"(msr),"a"(low),"d"(hight));
    return;
}


u64 rdtsc() {
    u32 lo, hi;
    asm volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((u64)hi << 32) | lo;
}

/*TODO: remmaber HEPT */

u64 CPU_info::get_cpu_freqence(){
        //u32 eax=0x15,ebx=0,ecx=0,edx=0;
       // cpuid(eax,ebx,ecx,edx);
        //return ecx;
        /*volatile HPET* find_hpet = (HPET*)memfind((u8*)0xE0000 , (u8*)"HEPT" , 4, 0x1FFFF);
        if(find_hpet!=(HPET*)0){
            printc("\nFind HEPT!\n",10);
            u64 ccp = find_hpet->CounterClockPeriod;
            return ccp;
          //  return (u64)(1000000000000000.0/ccp);
        }
        
        
        volatile RSDPDescriptor* find_rsdp = (RSDPDescriptor*)memfind((u8*)0xE0000 , (u8*)"RSD PTR " , 8, 0x1FFFF);
        if(find_rsdp==(RSDPDescriptor*)0){
            return 0;
        }else{
            //printc("\nFind RSDP!\n",10);
        }
        FADT* fadt = (FADT*)(find_rsdp->RsdtAddress);
        return fadt->PM_TMR_FREQ*1000;
        */
        /*if(memcmp((u8*)fadt->Signature,(u8*)"FACP",4)){
            return fadt->PM_TMR_FREQ*1000000;
        }*/

        //return 0;
        


 }

char* CPU_info::get_cpu_name(){
        u32 eax=0x0,ebx=0,ecx=0,edx=0;
        //cpuid(eax,ebx,ecx,edx);
        static char cpuname[50]={0};
	u32 Nptr = 0;
        //((int*)cpuname)[0]=ebx;
        //((int*)cpuname)[1]=ecx;
        //((int*)cpuname)[2]=edx;
	eax=0x80000002;
	for(int i = 0 ; i < 3 ; i ++ , eax ++){
		u32 peax = eax;
		ebx = ecx = edx = 0; //yap, you can do this and it will work corectly, change your life?
		cpuid(eax, ebx, ecx, edx);
		((int*)cpuname)[Nptr]   = eax;
		((int*)cpuname)[Nptr+1] = ebx;
		((int*)cpuname)[Nptr+2] = ecx;
		((int*)cpuname)[Nptr+3] = edx;
		Nptr+=4;

		//printc((char*)&eax,(u32)4,12);
		//printc((char*)&ebx,(u32)4,12);
		//printc((char*)&ecx,(u32)4,12);
		//printc((char*)&edx,(u32)4,12);
		eax=peax;
	}
	
	//eax = ebx = ecx = edx = 0;
	//eax = 0x5;
	//cpuid(eax, ebx, ecx, edx);
	//printc(Hex((eax|ebx|ecx|edx)),12);


        return (char*)cpuname;
 }
