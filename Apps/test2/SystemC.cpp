typedef unsigned char u8 ;
typedef unsigned short u16 ;
typedef unsigned int u32 ;
typedef unsigned long long u64 ;
void print(char* s, unsigned int w , unsigned int cl){
	asm volatile ("movl %0 , %%ebx\n"::"r"((u32)s));
	asm volatile ("movl %0 , %%ecx\n"::"r"((u32)cl));
	asm volatile ("movl %0 , %%edx\n"::"r"((u32)w));
	asm volatile ("movl $0x1 , %eax\n");
	asm volatile ("int $0x80\n");
	return;
}
void clear(){
	asm volatile ("movl $0x2 , %eax\n");
	asm volatile ("int $0x80\n");
	return;
}
