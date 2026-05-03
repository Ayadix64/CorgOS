#include "util.h"
#include "../pc/keyboard.h"
#include "../pc/timer.h"
u8 xp=0,yp=0;

void movecrus(u8 x, u8 y){
	u16 pos=y*80+x;
	outb(0x3d4 , 0xf);
	outb(0x3d5,(char)(pos&0xff));
	outb(0x3d4,0xe);
	outb(0x3d5,(char)(pos>>8)&0xff);
	return;
}

V2f get_print_pos(){
	return {
		.x=(float)xp,
		.y=(float)yp
	};
}
void set_print_pos(u8 x , u8 y){
	xp=x;
	yp=y;
	return;
}


void rows_up(u32 ups){
	for(int i = 0 ; i < ups ; i ++){
		for(int ii = 0 ; ii<80*24*2;ii++){
			*(char*)(0xb8000+ii)=*(char*)(0xb8000+ii+80*2);
		}
		for(int ii = 0 ; ii < 80 ; ii++){
			((u16*)(0xb8000+80*24*2))[ii]=(15<<8)|' ';
		}
	}
	return;
}



void print_char_at(u8 val ,u8 x , u8 y, u8 Cl){
	*(u16*)(0x8b000+(y*80+x)*2)=(Cl<<8)|val;
	return;
}

/*void print_at(char* val ,u8 x , u8 y, u8 Cl){
	u8 x_ = xp , y_ = yp;
	set_print_pos(x, y);
	printc(val, Cl);
	set_print_pos(x_, y_);
	return;
}

*/



void print_char(u8 val , u8 Cl){
	
		if(xp>=80){
			yp++;
			xp=0;
		}
		int VM=0xb8000+(yp*80+xp)*2;
		if(val=='\n'){
            		yp++;
			xp=0;
			if(yp>=25){
				rows_up(1);
				yp=24;
				xp=0;
			}
			return;
		}
		else if(val=='\t'){
			xp+=5;
			val=' ';
            return;
		}
		else if(val=='\r'){
			xp=0;
            return;
		}
		else if(val=='\b'){
			*(u16*)(0x8b000+(yp*80+xp)*2)=(15<<8)|' ';
			if(xp==0 && yp>=0){
				xp=79;
				yp--;
			}else if(yp==0){	
			}else{
				xp--;
			}
			*(u16*)(0x8b000+(yp*80+xp))=(15<<8)|' ';
			//val=str[i++];
            		return;
		}
		else if (val==0) {
			return;
		}
       
		//Clear the screan==========================================
		if(yp>=25){

			rows_up(1);
            //i--;
			yp=24;
			xp=0;
		}else{
			xp++;
		}
		 *(short*)VM=(Cl<<8)|val;
		//*(short*)(0xb8000+(yp*80+xp)*2)=(15<<8)|' ';
	u16 pos=0;
    	pos=yp*80+xp;
		//pos<0?pos*=-1:1;
		//pos=(u16)(yp<<8+xp<<8);
		
		outb(0x3d4 , 0xf);
		outb(0x3d5,(char)(pos&0xff));
		outb(0x3d4,0xe);
		outb(0x3d5,(char)(pos>>8)&0xff);
}

void TTLSW(void* buf,u32 x, u32 y){
	memcp(buf, (void *)0xb8000, 80*25*2);
	movecrus(x, y);
}

void print_to_buf(char* str,void* buf,u32& x_, u32&y_, int Clor){
    //va_list gf;
	for(int i =0 ; str[i] != '\0' ; ++i){
		
		if(xp>=80){
			y_++;
			x_=0;
            		continue;
		}
		u32 VM=(u32)buf+(y_*80+x_)*2;
		if(str[i]=='\n'){
           		y_++;
			x_=0;
			//Clear the screan==========================================
			if(y_>=25){	
				rows_up(1);
				y_=24;
				x_=0;
			}
            	continue;
		}
		else if(str[i]=='\t'){
			x_+=5;
			//str[i]=' ';
            		//i++;
			continue;
		}
		else if(str[i]=='\r'){
			x_=0;
			//str[i]=str[i++];
            		i++;
		}
		else if(str[i]=='\b'){
			if(x_==0 && y_>=0){
				x_=79;
				y_--;
			}else if(y_==0){
				
			}else{
				x_--;
			}
			i++;
			*(u16*)((u32)buf+(y_*80+x_)*2)=(15<<8)|' ';

			//str[i]=str[i++];
            
		}
		if(str[i]==0){return;}
		
        
		//Clear the screan==========================================
		if(y_>=25){
			rows_up(1);
			y_=24;
			x_=0;
		}else{
			x_++;
		}
		*(u16*)VM=((Clor<<8)|str[i]);
		

	}
//	*(short*)(0xb8000+(yp*80+xp)*2)=(15<<8)|' ';
	u16 pos=0;
    	pos=yp*80+xp;
		//pos<0?pos*=-1:1;
		//pos=(u16)(yp<<8+xp<<8);
		
	}




void printc(char* str, int Clor,...){
    //va_list gf;
	for(int i =0 ; str[i] != '\0' ; ++i){
		
		if(xp>=79){
			yp++;
			xp=0;
            		continue;
		}
		u32 VM=0xb8000+(yp*80+xp)*2;
		if(str[i]=='\n'){
           		yp++;
			xp=0;
			//Clear the screan==========================================
			if(yp>=25){	
				rows_up(1);
				yp=24;
				xp=0;
			}
            	continue;
		}
		else if(str[i]=='\t'){
			xp+=5;
			//str[i]=' ';
            		//i++;
			continue;
		}
		else if(str[i]=='\r'){
			xp=0;
			//str[i]=str[i++];
            		i++;
		}
		else if(str[i]=='\b'){
			if(xp==0 && yp>=0){
				xp=79;
				yp--;
			}else if(yp==0){
				
			}else{
				xp--;
			}
			i++;
			*(u16*)(0xb8000+(yp*80+xp)*2)=(15<<8)|' ';

			//str[i]=str[i++];
            
		}
		if(str[i]==0){return;}
		
        
		//Clear the screan==========================================
		if(yp>=25){
			rows_up(1);
			yp=24;
			xp=0;
		}else{
			xp++;
		}
		*(u16*)VM=((Clor<<8)|str[i]);
		

	}
//	*(short*)(0xb8000+(yp*80+xp)*2)=(15<<8)|' ';
	u16 pos=0;
    	pos=yp*80+xp;
		//pos<0?pos*=-1:1;
		//pos=(u16)(yp<<8+xp<<8);
		
		outb(0x3d4 , 0xf);
		outb(0x3d5,(char)(pos&0xff));
		outb(0x3d4,0xe);
		outb(0x3d5,(char)(pos>>8)&0xff);
}


void printc(char* str , u32 size , u8 Cl ){
	for(int i = 0 ; i < size ; i++){
		print_char((u8)str[i],Cl);
	}
}

void TTLclear(){
	xp=0;
	yp=0;
	for(int i = 0 ; i < 25*80 ; i++){
		((u16*)(0xb8000))[i]=(15<<8)|' ';
	}
	
}
void laze_printc(char* str, u8 cl){
	for(int i = 0 ; str[i]!=0 ; i++){
		//printc((char*)(char[2]){str[i],0},cl);
		print_char((u8)str[i],cl);
		if(yp>=24){
			printc("\nPress Space to jump page or ESC to quit or any key to continu.\r",15);
			bool conti=false;
			while ( 1){
				if(key==' '){
					TTLclear();
					break;
				}else if(ESC){
					return;
				}else if(key!=0){
					break;
				}
			}
			while(key || ESC){}
			
			
			
			xp=0;
			yp>=24?yp=23:yp=yp;
			for(int ii = 0 ; ii < 80*2 ; ii++){
				*(short*)(0xb8000+ii+80*24*2)=0;
			}
		}
	}
}


void sleep_print(char* str, u8 cl){
	for(int i = 0 ; str[i]!=0 ; i++){
		printc((char*)(char[2]){str[i],0},cl);
		//print_char((u8)str[i],cl);
		sleep(5);
	}
}

void sleep_print(char* str, u8 cl , u32 ms){
	for(int i = 0 ; str[i]!=0 ; i++){
		printc((char*)(char[2]){str[i],0},cl);
		//print_char((u8)str[i],cl);
		sleep(ms);
	}
}
void sleep_print(char* str, u8 cl , u32 size, u32 ms ){
	for(int i = 0 ; i<size ; i++){
		printc((char*)(char[2]){str[i],0},cl);
		//print_char((u8)str[i],cl);
		sleep(ms);
	}
}
