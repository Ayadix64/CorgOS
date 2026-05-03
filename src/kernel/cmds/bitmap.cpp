#include "../shell.h"

void bitmap(int argc, char* arg){
	TTLclear();
	printc("Bit map test program\n"
		"do Ctrl-C to exit\n",15);
	while(1){
		static u64 bitm = 0;
		printc("\nbit map : ",15);
		printc(Bin(bitm),15);
		printc("\n\nEnter bit # you wont : ",15);
		u32 bit = getint(14);
		
		while (key=='\n') ;
		if(bit>=64){printc("\nvery big",12);continue;}
		
		printc("\nNow Enter valu : ",15);
		u32 val = getint(14);
		
		while (key=='\n') ;
		
		writebm((u8*)&bitm, bit,  val);
		if((LCtrl|RCtrl )&&(key=='c'|key=='C'))break;;
		printc("\n\n",15);

	}
	while (key) {}
	printc("\npush test",14);
	Stack<u64> val;
	while (1) {
		
		printc("\n***valu in the stack ***\n",15);
		for(int i = 0 ; i < val .size() ; i++) {
			printc(Hex(val[i]),14);
			printc("\n",15);
		}
		printc("************************",15);
		printc("\n\npush(1) or pop (2) or set(3): ",15);
		u32 bit = getint(14);
		
		while (key=='\n') ;
		if(bit==1){
			printc("\nNow Enter valu to push : ",15);
			u64 valu = gethex(14);
			val.push(valu);
		}
		else if (bit == 2) {
			u64 pop = val.pop();
			printc("\npop: ",15);
			printc(Hex(pop),14);
		}else if(bit==3){
			printc("\nEnter Element # : ",15);
			u64 element = getint(14);
			while (key) ;
			printc("\nEnter # to set : ",15);
			u64 valu = getint(14);
			val [element] = valu;
		}
		
		while (key=='\n') ;
		
		if((LCtrl|RCtrl )&&(key=='c'|key=='C')){
			val.end();
			break;
		};
		printc("\n\n",15);
	}
}

