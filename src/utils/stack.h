#ifndef UTILS_STACK_H
#define UTILS_STACK_H
#include "util.h"

template<typename T>
class Stack {
	T* buf;
	u32 ln=0;
	u32 msize = 0x1000;
public:
	Stack(){
		buf=(T*)malloc(0x1000);
		ln=0;
		return;
	}
	//~Stack(){}
	void end(){
		mfree(buf, msize);
		return;
	}
	T& operator[](u32 index) {
        	if (index >= ln)  {printc("\nStack: not in the stack.",12);return (buf[0]);}
        	return buf[index];
    	}
	void push(T in){
		if(ln*sizeof(T)>msize){
			realloc(buf, msize, msize+0x1000);
			msize+=0x1000;
		}
		buf[ln]=in;
		ln++;
		return;
	}
	T pop(){
		T ret ;
		if(ln){
			ret = buf[ln-1];
			ln--;
			//buf[ln]=0;
			memset((u8)0,&buf[ln],sizeof(T));
		}
		return ret;
	}
	T& at(u32 ind){	
		if (ind >= ln)  
		{printc("\nStack: not in the stack.",12);return 0;}
		return buf[ind];
	}
	void pushat(u32 _at, T val){
		if(_at <= ln){
			ln++;
			if(ln*sizeof(T)>msize){
				realloc(buf, msize, msize+0x1000);
				msize+=0x1000;
			}
			for(u32 i = ln-1 ;i > _at ; i--){
				buf[i]=buf[i-1];
			}
			buf[_at]=val;
		}
	}
	T popat(u32 _at){
		T ret;
		if(_at <= ln){
			ret= buf[_at];
			for(u32 i = _at ;i < ln ; i++){
				buf[i]=buf[i+1];
			}
			ln--;
		}
		return ret;
	}
	void copy(T* tocp){
		for(int i = 0 ; i < ln ; i ++){
			tocp[i]=buf[i];
		}
	}
	u32 size(){
		return  ln;
	}
};;
#endif
