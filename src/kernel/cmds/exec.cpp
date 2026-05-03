#include "../shell.h"

void exec(int argc , char* arg){
    //asm("int $0x80");
    
    //u8 statcmem[1024];
    //asm("jmpl %%eax"::"eax"((u32)statcmem));
    //return;
     
     FATFILE *fils=(FATFILE *)malloc(0x1000*sizeof(FATFILE));
     mfree(fils, 0x1000*sizeof(FATFILE)) ;


    memset((u8)0,(void*)fils,1000*sizeof(FATFILE));
    fdir(ldir,(FATFILE*)fils,1000,MainDisk);
     //void (*fen)() = (void(*)())(u32)statcmem;
    

    for(int i = 0 ; i < 1000; i++){
        u32 cmps = 11;
        
        if(fils[i].Name[0]|fils[i].Name[1]){
            for(int ii = 7 ; ii>=0 ; ii--){
                if(fils[i].Name[ii]==' '){cmps=ii;}
            }
        }
    
        if(memcmp((void*)arg , (void*)fils[i].Name, cmps)){
            char *data=(char*)malloc(fils[i].size);
            for(u32 ii = 0 ; ii < (fils[i].size-1+0x1000)/0x1000 ; ii += 0x1000){
	    	link_page((u32)(data+ii),0x100000+ii);
	    }
	    read_file(fils[i],(char*)0x100000,fils[i].size,MainDisk);
            //printc("\n",10);

            	    //printc((char*)0x100000,(u32)488,15);
	    //
	    
	    void *app=(void*)0x100000;
	    void (*funt)()=(void (*)())app;;
	    funt();
	    //printc((char*)0x100000,(u32)150,15);
	    link_page((u32)0x100000,0x100000);;
	    //asm("jmpl $0x0");
	    //memcp((void*)data, (void *)statcmem, fils[i].size);
	    //sleep_print((char*)statcmem,15,fils[i].size,10);
	    //fen();
			//asm("push %%ip");
	    //asm("call %0"::"r"((u32)statcmem));
	    //printc(Hex((u32)statcmem),12);
	    //loadtask((void*)data,fils[i].size, 0); 
	    //sleep_print(data,15,fils[i].size,10);
            mfree(data,fils[i].size); 
	    return;
        }
    }
    printc("\nErr: file not fond .",12);
    return;

}
