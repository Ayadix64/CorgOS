#include "../shell.h"


u32 rnd(){
	static u32 la = 0;
	static u32 lo = 0;
	static u32 li = 0;
	u8 in = inb(0x60);
	static u32 r = 0;
	r |= TimerClock;
	r += (in<<16);
	//r -= ~(Seconds);
	r |= (~la-TimerClock);
	la|=in;
	//la<<=1;
	la-=TimerClock;
	la+=(r&0x152141);
	la<<=(r%5);

	lo = r - Seconds&TimerClock;
	li = ( r - (lo - la))|Seconds;
	la+=lo;
	r += (~lo-la)|li;
	//r |= usingmem();
	//r &= ~(get_mem_info().Extend2);

	return r;
}

void sysinfo(int argc , char* arg){
	KeyPress=0;
	u8* stars = (u8*)malloc(80*25);
	for(int i = 0 ; i < 80*25 ; i++){
		stars[i]=!(rnd()%50)?'*':!(rnd()%48)?'.':' ';
	}
	u8* ttlbb=(u8*)malloc(80*25*2);
	u32 x_=0,y_=0;
	while (1) {
		//TTLclear();
		x_=0,y_=0;
		set_print_pos(0, 0);
		/*stars efect*/
		for(u32 i = 0 ; i <80*25-1 ; i ++){
			stars[i] = stars[i+1];
		}
		for(int i = 0 ; i < 25 ; i++){
			stars[79+i*80]=!(rnd()%70)?'*':!(rnd()%35)?'.':' ';
		}
		for(u32 i = 0 ; i < 80*25 ; i++){
			print_to_buf((char*)(char[2]){(char)stars[i],0},(void*)ttlbb,x_,y_,15);
		}
		/**/
		x_=0,y_=0;
		print_to_buf("\t\t\t\t\t\t*** system info ***\n\n",ttlbb,x_,y_,15);
		print_to_buf("OS : ",ttlbb,x_,y_,15);
		print_to_buf("Ayadi OS (c) 0.02 Demo , All rigths reseved.\n",ttlbb,x_,y_,14);
		print_to_buf("CPU Model : ",ttlbb,x_,y_,15);
		for(int i = 0 ; i < 52 ; i++){
			print_to_buf((char*)(char[2]){(char)CPU_info::get_cpu_name()[i],0},(void*)ttlbb,x_,y_,14);
		}
		print_to_buf("\nboot seg : ",ttlbb,x_,y_,15);
		print_to_buf(Hex(*(u64*)(0x7e00)),ttlbb,x_,y_,14);
		print_to_buf("\nMomory installed : ",ttlbb,x_,y_,15);
		print_to_buf(Int_To_STR(get_mem_info().Extend2, 10),ttlbb,x_,y_,14);
		print_to_buf(" installsd .\n",ttlbb,x_,y_,14);
		print_to_buf("\tDynamic memory used : ",ttlbb,x_,y_,15);
		print_to_buf(Int_To_STR(usingmem(), 10),ttlbb,x_,y_,14);
		print_to_buf(" byte\n\tKernel Static Used Memory : ",ttlbb,x_,y_,15);
		print_to_buf(Int_To_STR(KERNEL_LONG*512, 10),ttlbb,x_,y_,14);
		print_to_buf(" byte\n\tFree Memory : ",ttlbb,x_,y_,15);
		print_to_buf(Int_To_STR((get_mem_info().Extend2-(usingmem()+(KERNEL_LONG*512))), 10),ttlbb,x_,y_,14);
		print_to_buf(" byte\n\n",ttlbb,x_,y_,15);
		print_to_buf("\t\t\t\t\t\t-The Best worste OS- ",ttlbb,x_,y_,15);
		TTLSW(ttlbb, x_,  y_);

		//sleep(30);
		sleep(30);
		if(KeyPress){
			TTLclear();
			mfree(ttlbb, 80*25*2);
			mfree((void*)stars, 80*25);
			return;
		}
		
	}
	return;
}


