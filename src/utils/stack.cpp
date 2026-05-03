#include "util.h"
#include "stack.h"
#define tem template<typename T>

/*tem Stack<T>::Stack(){
	buf=(T*)malloc(0x1000);
	ln=0;
}

/*tem Stack<T>::~Stack(){
	mfree(buf, msize);
}

tem void Stack<T>::push(T& in){
	if(ln*sizeof(T)>msize){
		realloc(buf, msize, msize+0x1000);
		msize+=0x1000;
	}
	buf[ln]=in;
	ln++;
	return;
}
tem
void Stack<T>::pop(){
	if(ln){
		ln--;
		buf[ln]=0;
	}
	return;
}
tem
T& Stack<T>:: at(u32 ind){
	if (ind >= ln)  {printc("\nStack: not in the stack.",12);return 0;}
	return buf[ind];
}
tem
void Stack<T>::copy(T* tocp){
	for(int i = 0 ; i < ln ; i ++){
		tocp[i]=buf[i];
	}
}

tem u32 Stack<T>::size(){
	return ln;
}*/
