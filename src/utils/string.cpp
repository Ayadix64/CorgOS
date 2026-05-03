#include "util.h"
char* Int_To_STR(int val, int base){

    static char buf[32]{0};
    //for(int i = 0 ; i
    int i = 30;
    if(!val){
    	buf[0]='0';
	buf[1]= 0 ;
	return buf;
    }
    for(; val && i ; --i, val /= base){

        buf[i] = "0123456789abcdef"[val % base];
        /*if base =1 will return binary string 010101001 ,
          if base =10 will return int 1234567890
          if base = 16 will return hex nember 0x123456789abcdef*/
        //buf[i+1]='\0';
	}
    	buf[31]=0;
                                                
    return &buf[i+1];

}

u32 strint(char* in, u32 lng){
	u32 ret = 0;
	for(int i = 0; i < lng && i < 10 /*it cant be int wthe more than...you idrestand me, do you?*/
	   ;i++){
		bool allaw = true;
		if(in[i]<'0'|in[i]>'9')allaw=false;
		if(allaw){
			ret*=10;
			ret+=(in[i]-'0');
		}else{
			printc("\nErr: ",12);
			print_char(in[i],12);
			printc(" is not a number.",12);
			return 0xffffffff;
		}

	}
	return ret;
}
u32 Hex2int(char* hex , u32 lng){
	u32 ret = 0;
	for(int i = 0 ; i < 8 && hex[i] && i < lng ; i++){
		u32 n = 0;
		switch (hex[i]) {
			case '0':
				n=0;
				break;
			case '1':
				n=1;
				break;
			case '2':
				n=2;
				break;
			case '3':
				n=3;
				break;
			case '4':
				n=4;
				break;
			case '5':
				n=5;
				break;
			case '6':
				n=6;
				break;
			case '7':
				n=7;
				break;
			case '8':
				n=8;
				break;
			case '9':
				n=9;
				break;
			case 'a':
				n=0xa;
				break;
			case 'A':
				n=0xa;
				break;

			case 'b':
				n=0xb;
				break;
			case 'B':
				n=0xb;
				break;

			case 'c':
				n=0xc;
				break;
			case 'C':
				n=0xc;
				break;

			case 'd':
				n=0xd;
				break;
			case 'D':
				n=0xd;
				break;

			case 'e':
				n=0xe;
				break;

			case 'E':
				n=0xe;
				break;
			
			case 'f':
				n=0xf;
				break;
			case 'F':
				n=0xf;
				break;

			default:
				printc("\n",15);
				print_char(hex[i],14);
				printc(" is not a Hex number.\n",15);
				return 0;
		}
		ret=(ret<<4)|n;
	}
	return ret;
}


char * Hex(u64 num){
	static char ret[18]{0};
	for(int i = 0 ; i < 16 ; i ++){
		ret[i]=' ';
	}
	int ii = 0;
	for(int i = 15 ; i>=0; i--){
		char r= (num>>(i*4))&0xf;
		ret[ii]="0123456789abcdef"[(int)r];	
		ii++;
	}
	ret[16]=0;
	return (char*)(ret); 

}

char* DHex(u32 num){
	static char ret[10]{0};
	for(int i = 0 ; i < 8 ; i ++){
		ret[i]=' ';
	}
	int ii = 0;
	for(int i = 7 ; i>=0; i--){
		char r= (num>>(i*4))&0xf;
		ret[ii]="0123456789abcdef"[(int)r];	
		ii++;
	}
	ret[9]=0;
	return (char*)(ret);
}

char* SHex(u16 num){
	static char ret[6]{0};
	for(int i = 0 ; i < 6 ; i ++){
		ret[i]=' ';
	}
	int ii = 0;
	for(int i = 4 ; i>=0; i--){
		char r= (num>>(i*4))&0xf;
		ret[ii]="0123456789abcdef"[(int)r];	
		ii++;
	}
	ret[5]=0;
	return (char*)(ret);
}

char* BHex(u8 num){
	static char ret[6]{0};
	for(int i = 0 ; i < 6 ; i ++){
		ret[i]=' ';
	}
	int ii = 0;
	for(int i = 1 ; i>=0; i--){
		char r= (num>>(i*4))&0xf;
		ret[ii]="0123456789abcdef"[(int)r];	
		ii++;
	}
	ret[2]=0;
	return (char*)(ret);
}


char* Bin(u32 num){
	static char ret[34]{0};
	for(int i = 0 ; i < 16 ; i ++){
		ret[i]=0;
	}
	int ii = 0 ; 
	for(int i = 32 ; i>=0; i--){
		char r= ((num>>i)&0x1);
		ret[ii]="01"[(int)r];	
		ii++;
	}
	ret[32]=0;
	return ret;
}





u32 strlen(char* str){
	u32 i = 0;
	for(; str [i] ; i++);
	return i;
}
