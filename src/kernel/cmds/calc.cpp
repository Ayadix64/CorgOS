#include "../shell.h"

void calc(int argc, char* arg){
	TTLclear();
	while (1) {
		printc("#1 : ",15);
		u32 n1=getint(14);
		while (key=='\n');
		printc("\nOppr(+,-,*,/,%): ",15);
		u8 opp;
		getcin((char*)&opp, 1, 14);
		while (key) ;
		printc("\n#2: ",15);
		u32 n2 = getint(14);
		while (key=='\n') ;
		u32 result;
		if((LCtrl|RCtrl)&&(key=='C'||key=='c'))return;

		switch (opp) {
			case '*':
				result=n1*n2;
				break;
			case '/':
				if(n2==0){
					break;
				}
				result=n1/n2;
				break;
			case '+':
				result=n1+n2;
				break;
			case '-':
				result=n1-n2;
				break;
			case '%':
				result=n1%n2;
				break;
			default:
				printc("\nOpperation non manched.",12);
				continue;
		}
		printc("\n\n\n             ",15);
		printc(Int_To_STR(n1,10),15);
		printc(" ",15);
		print_char(opp,14);
		printc(" ",15);
		printc(Int_To_STR(n2, 10),15);
		printc(" =  ",15);
		if((opp=='/' || opp=='%') && n2 == 0){
			printc("Yeah , you hate your pc are you",12<<4);
		}else if(result&0x80000000){
			printc("-",14);
			printc(Int_To_STR(0xffffffff-result+1, 10),14);//iam not crayey , that how your pc calcling things
		}else {
			printc(Int_To_STR(result, 10),14);
		}
		printc("\n            *********************************\n\n",15);
			
	}
}

