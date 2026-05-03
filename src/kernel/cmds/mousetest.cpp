#include "../shell.h"

void mousetest(int argc, char* arg){
	TTLclear();
	printc("Press any key to exit.",15);
	KeyPress=0;
	while(!KeyPress){
		movecrus(msx,msy);
	}
	TTLclear();

}
