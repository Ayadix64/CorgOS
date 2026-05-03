#include "../shell.h"
using namespace FAT12;
void edit(int argc, char* arg){
	Stack<char>buf;
	bool newfl=true;
	char filename[12]="NEW FILE   ";
	if(argc){
		memset((u8)' ',filename,11);
		for(int i = 0 ; i < argc && i < 11 ; i++){
			filename[i]=arg[i];
		}
		FATFILE *fils=(FATFILE *)malloc(0x1000*sizeof(FATFILE));
	        mfree(fils, 0x1000*sizeof(FATFILE)) ;
	 
	
	        memset((u8)0,(void*)fils,1000*sizeof(FATFILE));
	        fdir(ldir,(FATFILE*)fils,1000,MainDisk);
	    
	
	        for(int i = 0 ; i < 1000; i++){
	           u32 cmps = 11;
	        
	            if(fils[i].Name[0]|fils[i].Name[1]){
	                for(int ii = 10 ; ii>=0 ; ii--){
	                    if(fils[i].Name[ii]==' '){cmps=ii;}
	                }
	            }
	    
	           if(memcmp((void*)arg , (void*)fils[i].Name, cmps)){
	               char *data=(char*)malloc(fils[i].size);
	               read_file(fils[i],data,fils[i].size,MainDisk);
	               //printc("\n",10);
	                 newfl=false;
			 for(int ii = 0 ; ii < fils[i].size ; ii++){
				buf.push(data[ii]);
			 }
			 mfree(data,fils[i].size);
		       //sleep_print(data,15,fils[i].size,10);
	               
        	       //return;
	           } 
         	}
	}
	u32 width = 80;
	u32 higth = 24;
	u8 cl = 15;
	u8 Barcl = 15;
	TTLclear();
	
	u32 linein = 0;	
	bool medf = false;

	u32 rcrsp = 0;
	u32 bufcrs = 0;
	u32 cln = 0;
	u32 crx = 0;
	u32 cx = 0;
	u32 cy = 0;


	while(1){
		for(int i = 0 ;  i < 80 ; i ++)printc(" ",15<<4);
		set_print_pos(0, 0);
		printc("TEdit 0.000",Barcl<<4);
		set_print_pos(35, 0);
		printc(filename,(u32)11,Barcl<<4);
		printc("\n",15);
		
		
        	
		u32 line = 0;
		u32 printed = 0;
		u32 crsbufmax = buf.size();

		u32 plinew = 0;
		u32 linew = 0;
		u32 nlinew = 0;
		for(int i = 0, ii=0 ; i < buf.size()   ; i++,ii++){
			if(ii == 80|| buf[i]=='\n'){
				line++;
				if (buf[i] == '\n' && line == cln-1) {
					plinew = ii;
				}else if(line == cln-1){
					plinew=width;
				}
				
				if (buf[i] == '\n' && line == cln) {
					linew = ii;
				}else if(line == cln){
					linew=width;
				}

				if (buf[i] == '\n' && line == cln+1) {
					nlinew = ii;
				}else if(line == cln+1){
					nlinew=width;
				}
				//this is going more and more complecated ....
				ii=0;
			}
			if(line==cln && crx == ii){
					cx=get_print_pos().x;
					cy=get_print_pos().y;
					bufcrs = i; 
			}
			if(line >= linein && line < linein+higth){
				
				if(i==0)printc("*",9);
				print_char(buf[i],cl);
				if(buf[i]=='\n')printc("*",9);
			
			}
		}//printc(Int_To_STR(line,10),15);
		movecrus(cx, cy);
		while (!key&& !up && !down && !left  && !reight);
		if(key){
			if(key=='\b'){
				buf.popat(bufcrs);
				/*if(crx){
					crx--;
				}else if(cln){
					crx= plinew;
					cln--;
				}*/
			}else{
				buf.pushat(bufcrs, key);
				crx ++;
			}
		}
		if(up ){

			//linein--;
			if(cln-linein>0)cln--;
			else if((cln-linein)==0 && linein>0)linein--;
			//crx=1;
			/*if(crx > plinew){
				crx = plinew;
			}*/
		}
		if(down){
			cln++;
			if(cln > 25)
				linein++;
			//crx=1;
			/*if(crx > nlinew){
				nlinew++;

			}*/
		}
		if (left) {
			//if(crx){
				crx--;
			/*}/*else if(cln){
				cln--;
			}*/
		}
		if(reight ){
			//if(crx<=linew){
				crx++;
			/*}/*else{
				cln++;
				crx=0;
			}*/
		}
		
		//if(cln<linein && linein)linein--;
		//else if(cln>linein+higth)linein++;

		if((LCtrl||RCtrl)&&(key=='C'||key=='c')){break;;}
		u32 ll = 0x100000;
		u32 msleept=2000;
		u32 ssleept=20;
		static u32 tosleep = 1000;
		if ((key|| up || down || left  || reight) )
		{
			
			tosleep=ssleept;
			sleep(tosleep);
		}
		if(!KeyPress){
			tosleep = msleept;
		}
		TTLclear();
	}

		

	
	buf.end();
         
       return;



}
