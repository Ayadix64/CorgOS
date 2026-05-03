#include "util.h"
#include "../pc/keyboard.h"
#include "../pc/timer.h"


u32 getcin(char *ret,u32 lng,u8 cl){
    int reti=0;
    reti=0;
    memset((u8)0,ret,lng);
    u8 pkey = 0;
    while (key=='\n') ;
    while(1){
        
        if(key!=pkey){
            if(key!='\b'|| reti){
                if(key!='\n')
                    printc((char*)(char[2]){key,0},cl);
            }
            switch (key)
            {
            case '\b':
                if(reti>0)reti--;
                ret[reti]=0;
                break;
            case '\n':
                return reti;
            default:
                if(key!=0){
                    ret[reti]=key;
                    reti++;
		    if(reti>=(lng-1)){return reti;}
                    
                }
                break;
            }
            
            pkey=key;
        }
        
    }
    return reti;
}



u32 getint(u8 cl){
    char ret[10];
    u32 lng = 10;
    int reti=0;
    reti=0;

    memset((u8)0,ret,lng);
    u8 pkey = 0;
    while (key=='\n') ;
    bool exit = false;
    while(!exit){
        
        if(key!=pkey){
            if(key!='\b'|| reti){
            if(key!='\n')
                    printc((char*)(char[2]){key,0},cl);
            }
            switch (key)
            {
            case '\b':
                if(reti>0)reti--;
                ret[reti]=0;
                break;
            case '\n':
                exit=true;
		break;
            default:
                bool allaw = false;
                
		//if(key!=0){if(key<='9'&&key>='0'){allaw=true;}}

                if(key!=0){
			for(int i = 0 ; i < 10 ; i++){
				if(key=="0123456789"[i]){
					allaw=true;
				}                    
       		        }

			if(allaw){	
                	    if(reti>=(lng-1)){return reti;}
        	            ret[reti]=key;
                            reti++;
			}
			if(!allaw){
				printc("\nNot a number",12);
				return 0;
			}
                }
		
                break;
            }
            
            pkey=key;
        }
        
    }
    //get it, evry thing swet, know we most conver it to a number
    u32 ret_=strint(ret, reti);


    return ret_;
}


u32 gethex(u8 cl){
    char ret[10];
    u32 lng = 10;
    int reti=0;
    reti=0;

    memset((u8)0,ret,lng);
    u8 pkey = 0;
    bool exit = false;
    while (key=='\n') ;
    while(!exit){
        
        if(key!=pkey){
            if(key!='\b'|| reti){
            if(key!='\n')
                    printc((char*)(char[2]){key,0},cl);
            }
            switch (key)
            {
            case '\b':
                if(reti>0)reti--;
                ret[reti]=0;
                break;
            case '\n':
                exit=true;
		break;
            default:
                bool allaw = false;
                
		//if(key!=0){if(key<='9'&&key>='0'){allaw=true;}}

                if(key!=0){
			for(int i = 0 ; i < 16 ; i++){
				if(key=="0123456789abcdef"[i]){
					allaw=true;
				}                    
       		        }

			if(allaw){	
                	    if(reti>=(lng-1)){return reti;}
        	            ret[reti]=key;
                            reti++;
			}
			if(!allaw){
				printc("\nNot a number",12);
				return 0;
			}
                }
		
                break;
            }
            
            pkey=key;
        }
        
    }
    
    //get it, evry thing swet, know we most conver it to a number
    
    u32 ret_=Hex2int(ret,reti);


    return ret_;
}
