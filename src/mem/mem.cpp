#include "../utils/util.h"
#define MEM_INFO_LOC 0x5000
#define BLOCK_SIZE 4096
#define START_KERNEL 0x100000

MEM_INFO mem_info;
u32 Using_Mem = 0;
u32 Start_Free_Mem = 0;
volatile u8* MEM_map;//a bit map , evry bit reprisent a block , if it 1 that mean is used , else is free
u32 Map_Long=0;
u32 Map_Pointer=0;


/*Welcom to AyadiOS dynamick Memory mangment, the worst plase in the infinty univers */



typedef struct {
    u64 base;
    u64 length;
    u32 type;
    u32 acpi; // ignore or accept
} __attribute__((packed)) boot_mem_info;





void DMem_init(){
    /*mem_info.Extend1= (*(u16*)(MEM_INFO_PTR))*1024;
mem_info.Extend2= (*(u16*)(MEM_INFO_PTR+2))<<16;*/
    mem_info.Extend1=0;
    mem_info.Extend2= 0;
    Map_Long=0;
    Map_Pointer=0;
    u16 entris = *(short*)MEM_INFO_LOC;
    //printc("\nentris : ",10);
    //printc(Int_To_STR(entris,10),10);
    u64 memlong = 0;
    for(int i = 0; i < entris ; i++){
        volatile boot_mem_info* bootmem = (volatile boot_mem_info*)(MEM_INFO_LOC+2+(i*sizeof(boot_mem_info)));
        if(bootmem->type==1){
            memlong+=bootmem->length;
        }
        printc("Entry ",10);
        printc(Int_To_STR(i,10),10);
        printc(" base: ",10);
        printc(Hex((u32)(bootmem->base)),10);
        printc(" length: ",10);
        printc(Hex((u32)(bootmem->length)),10);
        printc(" type: ",10);
        printc(Int_To_STR(bootmem->type,10),10);
        printc("\n",10);
    }
    mem_info.Extend2=memlong;


    MEM_map =(volatile u8*)(START_KERNEL+(KERNEL_LONG*SECTOR_SIZE));
    
    Map_Long=(((mem_info.Extend2-(START_KERNEL+(KERNEL_LONG*SECTOR_SIZE)))/BLOCK_SIZE)+7)/8;
    Map_Long-=(Map_Long/BLOCK_SIZE)/8;

    Using_Mem=(KERNEL_LONG*SECTOR_SIZE)+Map_Long;
    
    Start_Free_Mem=(START_KERNEL+(KERNEL_LONG*SECTOR_SIZE))+Map_Long;
    /*Exend2 have the full mem*/

    for(u32 i = 0 ; i < Map_Long/8 ; i++){
        MEM_map[i]=0;
    }
    prtcmem((void*)(STACK_PTR-0x4000),0x8000);//portact memory stack pointer
    prtcmem((void*)START_KERNEL, KERNEL_LONG*512);
    prtcmem((void*)MEM_map, Map_Long);
    for(int i = 0; i < entris ; i++){
        volatile boot_mem_info* bootmem = (volatile boot_mem_info*)(MEM_INFO_LOC+2+(i*sizeof(boot_mem_info)));
        if(bootmem->type!=1){
	    prtcmem((void*)bootmem->base, bootmem->length);
            //memlong+=bootmem->length;
        } 
    }
    //printc(Hex((u64)MEM_map),14);
    //printc("\n",15);
    //printc(Hex((u64)Map_Long),14);

    prtcmem((void*)0x401000, 0x2000);//whay you my ask? , i dont know, but in the mem test, it reset evry time when write into
    prtcmem((void*)0x1200000, 0x200000);//this too, how awsome headacke
}


MEM_INFO get_mem_info(){
    MEM_INFO returned = mem_info;/*some how  ,a think that this make protact , some how ....*/
    return returned;
}


u32 usingmem(){
	return Using_Mem;
}

void add_memp(){
    if(Map_Pointer>=(mem_info.Extend2-Start_Free_Mem)){
        Map_Pointer=0;
    }
    Map_Pointer++;
}

extern "C" void* malloc(u32 size){
    /*u32 Block_take=(int)((size/BLOCK_SIZE)+1.0);
    bool isit = true;
    bool no_mem = false;
    u32 tourns_cont=0;
    //printc(Int_To_STR(Map_Long,10),10);
    while(1){
        for(; get_bit(MEM_map[Map_Pointer/8],Map_Pointer%8);
            Map_Pointer>=(mem_info.Extend2-Start_Free_Mem)?Map_Pointer=0:Map_Pointer++
            ,tourns_cont++){}
        
        for(int i = 0 ; i<Block_take ; i++){
            if(get_bit(MEM_map[(Map_Pointer+i)/8],(Map_Pointer+i)%8)){
		        isit=false;
            }
	        if(Map_Pointer+i>=Map_Long/*(Map_Pointer+i)*BLOCK_SIZE>=(mem_info.Extend2-Start_Free_Mem)*){
                isit=false;
            }
            tourns_cont++;
        }
        if(isit){
            for(int i= 0 ; i<Block_take;i++){
                MEM_map[(Map_Pointer+i)/8]|=1<<((Map_Pointer+i)/8);
            }
	    void* array = (void*)(Map_Pointer*BLOCK_SIZE+Start_Free_Mem);
            Map_Pointer+=Block_take;
            return array;
        }
        if((tourns_cont>=Map_Long)){
            return (void*)0;
        }
        
        isit=true;

    }*/

    u32 tblock = (u32)((size+BLOCK_SIZE-1)/BLOCK_SIZE);//+(size%BLOCK_SIZE!=0?1:0));
    u32 count = Map_Pointer;
    bool ret = true;
    u32 xa= 0;
    while(1){
        if((count*BLOCK_SIZE)>=(mem_info.Extend2-Start_Free_Mem)){
	    count=0;
            //Map_Pointer=0;
        }
        ret = true;
	u32 stcount = count;
        while (readbm((u8*)MEM_map, count)){
           /* if(count==Map_Pointer-1){
                
            }*/
	    if ((count*BLOCK_SIZE)>=(mem_info.Extend2-Start_Free_Mem)) {
		count=0;
	    }
	    count++;
	    if(stcount==count){//we have a erorr hir bro, and it not great
		printc("\nMemory Err: Fail to Allocate Memory adress, it full :(\n",12);
                return (void*)0;	
	    }

        }
	
        
        for(u32 i = 0 ; i < tblock;i++/*,count++*/){   
            if(readbm((u8*)MEM_map, count+i)/*MEM_map[(count)/8]&(1<<((count)%8))*/){
                ret=false;
		count+=i;
                //continue;
		break;
            }else if ((count*BLOCK_SIZE)>=(mem_info.Extend2-Start_Free_Mem)) {
           	count=0;
		Map_Pointer=0;
                ret=false;
                //continue;
 		break;;
            } 
            
        }
        if(ret){
            Map_Pointer=count+tblock;
	    //printc(Bin(*(u64*)MEM_map+8),58);
	    prtcmem((void*)(Start_Free_Mem+count*BLOCK_SIZE),size);
            /*for(int ii = 0 ; ii < tblock ; ii++){
		writebm(MEM_map, count+ii, bool val)
	    }*/
	    //memset((u8)0,(void*)(Start_Free_Mem+count*BLOCK_SIZE),tblock*BLOCK_SIZE);//yeah, bare metal, ram is totaly random,REMOVE IT
	    void* retadrr = (void*)(Start_Free_Mem+count*BLOCK_SIZE);
	    memset((u8)0,(void*)retadrr,size); //not good for prfermence, but good for the software, and also baremetal
	    return retadrr;
        }
        if(count==Map_Pointer){
            printc("\nMemory Err: Fail to Allocate Memory adress, it full :(\n",12);
            return (void*)0;
        }
        xa++;   
        if((xa*BLOCK_SIZE)>mem_info.Extend2-Start_Free_Mem)break;
        //printc("*",14);
    }
    printc("\nMemory Err: Fail to Allocate Memory adress, it full :(\n",12);
    return (void*)0;
}

void* malloc(u32 size , u32 start_adress , u32 end_adress){//from N adress to 0xffffffff(the memory max) , end adress choldnt be less than start adress
    if(end_adress<start_adress){
        end_adress=0xffffffff;
    }
    u32 count = start_adress;
    
    u32 tblock = (u32)((size+BLOCK_SIZE-1)/BLOCK_SIZE);//+(size%BLOCK_SIZE!=0?1:0));
    bool ret = true;
    u32 xa= 0;
    
    while(1){
        if((count*BLOCK_SIZE)>=(mem_info.Extend2-Start_Free_Mem)){
            count=0;
            //Map_Pointer=0;
        }
        ret = true;
        for(;MEM_map[count/8]&(1<<(count%8));(count*BLOCK_SIZE)>=(mem_info.Extend2-Start_Free_Mem)?count=0:count++){
            if(count==Map_Pointer-1){
                printc("\nMemory Err: Fail to Allocate Memory adress, it full :(\n",12);
                return (void*)0;
            }
        }
        
        for(u32 i = 0 ; i < tblock;i++,count++){   
            if(MEM_map[(count)/8]&(1<<((count)%8))||(count*BLOCK_SIZE)>=(mem_info.Extend2-Start_Free_Mem)){
                count=0;
                ret=false;
                continue;
            } 
            
        }
        if(ret){
            //Map_Pointer=count;
            prtcmem((void*)(Start_Free_Mem+count*BLOCK_SIZE),size);
            return (void*)(Start_Free_Mem+count*BLOCK_SIZE);
        }
        if(count==Map_Pointer){
            printc("\nMemory Err: Fail to Allocate Memory adress, it full :(\n",12);
            return (void*)0;
        }
        xa++;   
        if((xa*BLOCK_SIZE)>mem_info.Extend2-Start_Free_Mem)break;
        //printc("*",14);
    }
    printc("\nMemory Err: Fail to Allocate Memory adress, it full :(\n",12);
    return (void*)0;

}

void* unloked_malloc(u32 size , u32 start_adress , u32 end_adress){//this type of malloc(hope is the end) aloccating with out care of phisicly memory size , useful with paging
    if(end_adress<start_adress){
        end_adress=0xffffffff;
    }
    u32 tblock = (u32)((size/BLOCK_SIZE)+(size%BLOCK_SIZE!=0?1:0));
    u32 count = start_adress;
    bool ret = true;
    while(1){
        if(count>=0xffffffff || count>=end_adress){
            return (void*)0;
        }
        ret = true;
        for(;MEM_map[count/8]&(1<<(count%8));){
            if((count>=0xffffffff||count>=end_adress)){
                return (void*)0;
                count=0;
            }
            count++;
        }
        
        for(u32 i = 0 ; i < tblock;i++,count++){
            
            if(MEM_map[(count)/8]&(1<<((count)%8))||count>=0xffffffff||count>=end_adress){
                ret=false;
                continue;
            } 
            
        }
        if(ret){
            prtcmem((void*)(Start_Free_Mem+count*BLOCK_SIZE),size);
            return (void*)(Start_Free_Mem+count*BLOCK_SIZE);
        }

    }

}


extern "C" void mfree(void* adress,u32 size){
    if(((u32)adress)>mem_info.Extend2)return;
    else if ((u32)adress<Start_Free_Mem)return;
     ;
    u32 Block_take=(size+BLOCK_SIZE-1)/BLOCK_SIZE;
    u32 freeptr=((u32)adress-Start_Free_Mem)/BLOCK_SIZE;
    for(int i = freeptr ; i < freeptr+Block_take; i++){
        //MEM_map[i/8]&=~(1<<(i%8));
	writebm((u8*)MEM_map, i,0);
    }
   // Map_Pointer-=size/BLOCK_SIZE;
    if(Using_Mem<Block_take*BLOCK_SIZE){
    	Using_Mem=0;
    }else{
	Using_Mem-=Block_take*BLOCK_SIZE;
    }
    return;
}

void* malloc_aligned(u32 size, u32 aligned){
    void* ptr = malloc(size+aligned-1);
    return (void*)((((u32)ptr+aligned-1)&(~(aligned-1))));
}
void* unloked_malloc_aligned(u32 size, u32 aligned , u32 stadrr , u32 endadrr){
    void* ptr = unloked_malloc(size +aligned-1,stadrr,endadrr);
    return (void*)((((u32)ptr+aligned-1)&(~(aligned-1))));
}







void prtcmem(void* adrr , u32 size){
    if(((u32)adrr)>mem_info.Extend2)return;
    else if ((u32)adrr<Start_Free_Mem)return;
    
    u32 tblock = (u32)((size+BLOCK_SIZE-1)/BLOCK_SIZE);
    u32 start = ((u32)adrr-Start_Free_Mem)/BLOCK_SIZE;
    for(int i = 0 ; i < tblock/*size*/ ; i++){
	writebm((u8*)MEM_map, i+start, 1);
	//printc(readbm(MEM_map, i+start)==1?(char*)"1":(char*)"0",15);
    }
    //printc(Hex(*(u64*)&MEM_map[(start)/8]),15);
    Using_Mem+=tblock*BLOCK_SIZE;
    return;
}


void* realloc(void* adrr , u32 bsize, u32 nsize){
    u8* buf = (u8*) malloc(nsize);
    for(int i = 0 ; i < nsize && i < bsize ; i ++){
        buf[i]=((u8*)adrr)[i];
    }
    mfree(adrr,bsize);
    return (void*)buf;
}

bool memcmp(void* ptr1 , void* ptr2 , u32 lon){
    bool trh = true;
    for(int i = 0 ; i<lon ; i++){
        if(((u8*)ptr1)[i]!=((u8*)ptr2)[i]){
            trh=false;
        }
    }
    return trh;
}



bool readbm(volatile u8* bm, u32 bit){
	return ((*(volatile u8*)(bm+(bit/8)))&(1<<(bit%8)))>0;//maths
}

void writebm(volatile u8* bm, u32 bit , bool val){
	(*(volatile u8*)(bm+(bit/8)))&=((u8)(~(1<<(bit%8)))&0xff);//make it zero first
	(*(volatile u8*)(bm+(bit/8)))|=((u8)((val<<(bit%8)))&0xff);//if val==0, it will do any thing(0<<4=0), else, write one
	return;
}



void* memfind(void* start/*first pointer of find area*/ ,
                     void* find /*find word*/,
                     u32 findlon/*long of find word*/ ,
                     u32 end/*the longth of find area*/)
{
    for(int i = 0 ; i< end ; i++){
        if(memcmp((void*)(start+i),find,findlon)){
            return (void*)(start+i);
        }
    }
    return (void*)0;
}

void memcp(void* tocp , void* incp , u32 lng){
    for(int i = 0 ; i < lng ; i++){
        ((u8*)incp)[i]=((u8*)tocp)[i];
    }
    return;
}

void memset(u32 toset , void* loc , u32 lng){
    for(int i = 0 ; i < lng ; i++){
        (((u32*)loc)[i])=toset;
    }
}
void memset(u16 toset , void* loc , u32 lng){
    for(int i = 0 ; i < lng ; i++){
        (((u16*)loc)[i])=toset;
    }
}
void memset(u8 toset , void* loc , u32 lng){
    for(int i = 0 ; i < lng ; i++){
        (((u8*)loc)[i])=toset;
    }
}

void memappend(void* buf, u32 buflng, void* append, u32 appendlng){
    for(int i = 0 ; i < appendlng ; i++){
        ((u8*)((u32)buf+buflng))[i]=((u8*)append)[i];
    }
    return;
}
