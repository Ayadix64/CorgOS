#include "SystemC.h"
typedef unsigned char u8 ;
typedef unsigned short u16 ;
typedef unsigned int u32 ;
typedef unsigned long long u64 ;

void printf(char* s, unsigned int cl){
	u32 w = 0;
	for(u32 i = 0 ; s [i] != 0 ; i ++,w++);;
	print(s,w, cl);
}
extern "C" void main(){
	clear();
	//printf("Ground Control to Major Tom\n", 14);
	char* msg = "Ground Control to Major Tom\n";
	

	printf(msg,14);
	
	for(int i = 0 ; i < 0x10000 ; i++);;
	printf(msg, 14);

	printf("???", 15);
	for(int i = 0 ; i < 0x10000 ; i++);;
	printf("\ncan you lisent to us? this is earth, be ready fo your metion ", 15);
	return ;

}
