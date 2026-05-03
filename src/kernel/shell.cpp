#include "../utils/util.h"
#include "../gdt/gdt.h"
#include "../cpu/CPU.h"
#include "../idt/idt.h"
#include "../pc/timer.h"
#include "../pc/RTC.h"
#include "../pc/keyboard.h"
#include "../mem/paging.h"
#include "../cpu/pci.h"
#include "../pc/Disk.h"
#include "../pc/Speker.h"
#include "../filesys/1fat16.h"
#include "../utils/stack.h"
#include "../multitask/multitask.h"








using namespace FAT12;

template<typename T>
class stack {
	T* buf;
	u32 size=0x1000;
public:
	stack(){
		buf=(T*)malloc(size); }
	//~stack(){
		//mfree((void*)buf, size);
	//}
	void push(T val){}
};

DISK MainDisk{
	._ATA=true,
	.index1=0,
	.index2=0
};


class command{
    public:
    char* name;
    char* disc="\0";
    u32 namelong=0;
    void (*function)(int argc,char* arg);
    void install(char* na , u32 lng , void (*fun)(int argc,char* arg)){
        name=na;
        namelong=lng;
        function=fun;
        return;
    }
    void install(char* na , u32 lng ,char *dis, void (*fun)(int argc,char* arg)){
        name=na;
        namelong=lng;
        function=fun;
	disc=dis;
        return;
    }
};;

char *ldir;
u32 ldirlng = 1;
void dir      (int argc, char* arg);
void toch     (int argc, char* arg);
void echo     (int argc, char* arg);
void read     (int argc, char* agr);
void clear    (int argc, char* arg);
void blabla   (int argc, char* arg);
void fat      (int argc, char* arg);
void time     (int argc, char* arg);
void mkdir    (int argc, char* arg);
void write    (int argc, char* arg);
void cd       (int argc, char* arg);
void memtest  (int argc, char* arg);
void bitmap   (int agrc, char* arg);
void help     (int agrc, char* arg);
void sysinfo  (int argc, char* arg);
void rm       (int argc, char* arg);
void tree     (int argc, char* arg);
void Dtest    (int argc, char* arg);
void memdump  (int argc, char* arg);
void calc     (int argc, char* arg);
void lsdev    (int argc, char* arg);
void mount    (int argc, char* arg);
void regsvw   (int argc, char* arg);
void exec     (int argc, char* arg);
void installer(int argc, char* arg);
void edit     (int argc, char* arg);
void mousetest(int argc, char* arg);
command *commands;
u32 cmdnum=27;

void Shell(){
    sleep(1000);
    TTLclear();
    printc("\nAyadi OS (c) 2024 - 2025 . All rights reseved, typ help to show comands\n",15);
    
    
    if(isitfat12({0,0})){
        printc("\nmont file system of the first sata disk .\n",10);
    }
    
    //char random[1024]; for(int i = 0 ; i < 1024 ; i++){ random[i]="Who have a lot of fun not at all funy \n"[i%39];


    //creatfile("/", "Log            ", MainDisk);
    //save("/", "Log            ", (u8 *)random, 1024, MainDisk);
    ldir = (char*)malloc(0x1000);
    commands = (command*)malloc(cmdnum*sizeof(command));
    ldir[0]='/';
    if(bootfind){
	MainDisk=bootdisk;
    }else{
	printc("\n**Boot Disk is not detacted, mybe you boot frome unsuported disk drve, plog out the boot drive or other thing, it recomend to reboot but you can continu**",12);
	//printc("\ncontinu [y,N")
    }
    while (key=='\n'){}

    
	
    commands[0].install("dir",3,"Show all files in the file, this not work if OS not in the hard disk",dir);
    commands[1].install("toch\0",4,"Creat a file, work only if os in the disk",toch);
    commands[2].install("echo\0",4,"Print wath ever you print",echo);
    commands[3].install("read\0",4,"Read a filse, also have to be at disk",read);
    commands[4].install("clear\0",5,"Clear the scren",clear);
    commands[5].install("blabla\0",6,"Write whath ever you enter at a sctor and read from it",blabla);
    commands[6].install("fat",3,"Show the fat tibel at the disk",fat);
    commands[7].install("time",4,"Show the CMOS time",time);
    commands[8].install("mkdir",5,"Creat a folder in the folder",mkdir);
    commands[9].install("write",5,"Write to a file in the disk.",write);
    commands[10].install("cd",2,"Enter to a folder",cd);
    commands[11].install("memtest",7,"Test Ayadi OS Dynamic memory",memtest);
    commands[12].install("bitmap",6,"A bit map you can test it, it just a test to defrent things",bitmap);
    commands[13].install("sysinfo",7,"Show you Some basyc info about your PC.",sysinfo);
    commands[14].install("rm",2,"remove a file.",rm);
    commands[15].install("tree",4,"show all fols and folders in the diractry.",tree);
    commands[16].install("Dtest",5,"Test Disk Read",Dtest);
    commands[17].install("memdump",7,"A simpel memory dump",memdump);
    commands[18].install("calc",4,"A Very basic calcuter",calc);
    commands[19].install("mount",5,"mount a other disk in the system and braws it.",mount);
    commands[20].install("lsdev",5,"List all the devices in your pc",lsdev);
    commands[21].install("regvw",5,"view the contins of the regesters of your cpu",regsvw);
    commands[22].install("exec",4,"exute a app (BETA)",exec);
    commands[23].install("install",7,"Install the Ayadi OS on your machine, wich is no remanded.",installer);
    commands[24].install("edit",4,"simpel Editor, very simpel, very",edit);
    commands[25].install("mouse",5,"Test your mouse, if you have one",mousetest); 
    commands[26].install("help",4,"List of commands and a simpel decrpition of it.",help);
    u8 pkey = 0 ;
    char input[400] ;

    if(bootfind){
	    printc("BOOT DEVICE:",15);
	    printc(bootdisk._SATA?(char*)" SATA ":bootdisk._ATA?(char*)" ATA ":bootdisk._USB?(char*)" USB ":(char*)" ?UNFIND? ",15);
	    printc(" PORT # ",15);
	    printc(Int_To_STR(bootdisk._SATA?bootdisk.index2:bootdisk._ATA?bootdisk.index1:0,10),15);
    }


    while (1)
    {  
	//asm("int $0x80");
        printc("\n",15);
        printc(ldir,15);
        printc(" >> ",15);
        memset((u8)0,input,400);
        u32 cmlon = getcin((char*)input,400,15);
        if(key=='\n'){
            sleep(200);
            if(!cmlon)continue;
        }

        bool itisacmd=false;
        for(int i = 0 ; i < cmdnum ; i++){
            if(memcmp((void*)commands[i].name,(void*)input,commands[i].namelong)&& 
              (input[commands[i].namelong]==0   || 
               input[commands[i].namelong]==' ' || 
               input[commands[i].namelong]=='\t'))
            {
                
                commands[i].function((cmlon-commands[i].namelong),
                                    (char*)&input[commands[i].namelong+1]);
                itisacmd=true;
                break;
            }
        }
        if(!itisacmd){
            printc("\n",15);
            //int i = 0;
            //for( ; input [i] !=' '|| input [i] !='\t'|| input [i] !=0 ;i++);
            printc(input,14);
            printc(" it not a defined command.\n",15);
        }
	       
       //if(key!=pkey){
       //printc((char*)(char[2]){key,0},10);
       //pkey=key;
       // }
    }
    
}

void regsvw(int argc, char* arg){
	regs reg=loadreg();
	TTLclear();
	printc("EAX : ",15);
	printc(Hex(reg.eax),14);
	printc("            EBX : ",15);
	printc(Hex(reg.ebx),14);
	
	printc("\nECX : ",15);
	printc(Hex(reg.ecx),14);
	printc("            EDX : ",15);
	printc(Hex(reg.edx),14);

	printc("\nCR0 : ",15);
	printc(Hex(reg.cr0),14);
	printc("            CR2 : ",15);
	printc(Hex(reg.cr2),14);
	
	printc("\nCR3 : ",15);
	printc(Hex(reg.cr3),14);
	printc("            CR4 : ",15);
	printc(Hex(reg.cr4),14);

	printc("\nESP : ",15);
	printc(Hex(reg.esp),14);
	printc("            EBP : ",15);
	printc(Hex(reg.ebp),14);



}

void lsdev(int argc , char* arg){
	TTLclear();
	//print_divaces();
	KeyPress=0;
	for(int i = 0 ; i < devacenum ; i++){
		sleep_print("\nFind Device : ",15);

	        sleep_print(devices_names[Devaces[i].classcode>=0x14?0:Devaces[i].classcode][Devaces[i].subclass_code>=0x10?0:Devaces[i].subclass_code],11);
	        if(KeyPress)break;
		/*sleep_print(" (class code ",10);
	        sleep_print(Int_To_STR(class_code,10),10);*/
	        sleep_print("  Vendor ID ",15);
	        sleep_print(Int_To_STR(Devaces[i].VindorID,10),10);
	        if(KeyPress)break;
		sleep_print("  Device ID ",15);
	        sleep_print(Int_To_STR(Devaces[i].DeviceID,10),10);
	        //sleep_print("  Progif ",10);
	        //sleep_print(Int_To_STR(Devaces[i].prog_if,10),10);
		if(KeyPress)break;
	        sleep_print("\n",10);
	        sleep_print("  base ",10);
	        if(KeyPress)break;
		sleep_print(Hex(Devaces[i].base),10);
	        if(KeyPress)break;
		sleep_print(Devaces[i].adrasing64?(char*)" --64bit-- ": (char*)" --32bit-- ",14);
	        sleep_print(Devaces[i].suport_MMIO?(char*)" MMIO":(char*)" Port I/O",14);
	        if(KeyPress)break;
		sleep_print("  IRQ# ",10);
	        if(KeyPress)break;
		sleep_print(Hex(Devaces[i].IRQ),10);
	        sleep_print("\n",10);
		if(KeyPress)break;
    }

}


void dir(int argc, char* arg){
    FATFILE* fils=(FATFILE*)malloc(0x1000*sizeof(FATFILE));
    memset((u8)0,(void*)fils,0x1000*sizeof(FATFILE));
    //creatfile((char*)'helpme  TXT',{0,0});
    if(argc==1 || argc==0){
    //rootdir(fils,{0,0});
    fdir(ldir,(FATFILE*)fils,0x1000,MainDisk);    
    //u8 zero[11]{0};
    ;    
    }
    else{
        if(arg[0]!='/'){
            char dirdd[0x1000];
            memset((u8)0,dirdd,0x1000);
            memcp(ldir,dirdd,ldirlng);
            //memappend(dirdd,ldirlng,arg,argc);
	        memcp(arg,(void*)(dirdd+ldirlng),argc);
            printc((char*)dirdd,26);
            fdir(dirdd,(FATFILE*)fils,0xfff,MainDisk);
        }else{
            fdir(arg,(FATFILE*)fils,0xfff,MainDisk);
        }
    }
    for(int i = 0 ; i < 100; i++){
        if(fils[i].Name[0]|fils[i].Name[1] && (fils[i].Attributs==0x20||fils[i].Attributs==0x10)){
            printc("\n",10);
            printc((char*)fils[i].Name,(u32)11,15);
            printc("     ",10);
            printc((char*)file_attributs[fils[i].Attributs],15);
            printc("     ",10);
            printc(Int_To_STR(fils[i].size,10),15);
            printc(" byte",15);
            /*if(fils[i].Attributs==0x10){
                FATFILE fils2[1000];
                char path[13]{0};
                memset((u8)0,path,13);
                path[0]='/';
                memcp((void*)fils[i].Name,(void*)(path+1),11);
                fdir((char*)path,(FATFILE*)fils2,MainDisk);
                //read_file(fils[i],(char*)fils2,MainDisk);
                for(int ii = 0 ; ii < 100 ;ii++){
                    if(fils2[ii].Name[0]){
                        printc("\n",10);
                        printc((char*)fils2[ii].Name,(u32)11,15);;
                        printc("     ",10);
                        printc((char*)file_attributs[fils2[ii].Attributs],15);
                        printc("     ",10);
                        printc(Int_To_STR(fils2[ii].size,10),15);
                        printc(" byte",15);
                    }
                }
            }*/
        }
    }
   mfree(fils, 0x1000*sizeof(FATFILE));
}

//FATFILE fils[0x1000];

void cd(int argc, char* arg){
    FATFILE *fils=(FATFILE *)malloc(0x1000*sizeof(FATFILE));
    
    memset((u8)0,(void*)fils,0x1000*sizeof(FATFILE));
    fdir(ldir,(FATFILE*)fils,0xfff,MainDisk);
    mfree(fils, 0x1000*sizeof(FATFILE));

    for(int i = 0 ; i < 1000; i++){
        u32 cmps = 9;
        if(fils[i].Name[0]|fils[i].Name[1]){
            for(int ii = 8; ii>=0 ; ii--){
                if(fils[i].Name[ii]==0 || fils[i].Name[ii]==' '){cmps=ii;}
            }
        }

        if(memcmp((void*)arg , (void*)fils[i].Name, cmps) && fils[i].Attributs==0x10){
            if(arg[0]=='.' && arg[1]=='.' &&  (arg[2]==' ' || arg[2]==0)){
                if(ldir[ldirlng-1]=='/' && ldirlng>1){
                    ldir[ldirlng-1]=0;
                    ldirlng--;
                }
                for ( ; ((ldir[ldirlng-1]!='/') && ldirlng>1); ldirlng--)
                {
                    ldir[ldirlng-1]=0;
                    if(!ldirlng){printc("\nErr: folder in/out Err .\n",12);return;}
                }
		ldir[0]='/';

                printc("\n",10);
                printc(ldir,(u32)ldirlng,15);
                return;
            }else if(arg[0]=='.' &&   (arg[1]==' ' || arg[1]==0)){
                printc("\n",10);
                printc(ldir,(u32)ldirlng,15);
                return;//heh, how ckare
            }else{
                memcp(arg,(void*)(ldir+ldirlng),cmps);
                memcp((void*)"/",(void*)(ldir+ldirlng+cmps),1);
                ldirlng+=cmps+1;
                printc("\n",10);
                printc(ldir,(u32)ldirlng,15);
                return;
            }
        }
    }
    printc("\nSomthing wrong happyn, my be err name?\n",12);
    //creatfile(arg,MainDisk);
    return;
}

void toch(int argc, char* arg){
    creatfile(ldir,arg,MainDisk);
    return;
}

void mkdir(int argc, char* arg){
    /*char* name = 0;
    int i = argc;
    bool longp = false;
    char path[1000];
    for(int ii =  0 ; ii < argc ; ii ++){
        if(arg[ii]=='/'){
            longp=true;
        }
    }
    u32 names = 0;
    if(longp){
        for(; arg[i]=='/' ;i-- ){   }
        for(; arg[i]!='/' ;i-- ){names++;}
        name=&arg[i];
    }
    else{
        name=arg;
        names=argc;
    }   
    /*char path[1000];
    for(int ii = 0 ; ii < i ; ii++){
        path[ii]=arg[ii];
        path[ii+1]=0;
    }*
    
    if(arg[0]!='/'){
        memcp(ldir,path,ldirlng);
        if(longp){memcp(arg,path+ldirlng,i);}
    }
    char nam[11]{0};
    memcp(name,nam,names>11?11:names);
    
    mkdir(name,path,MainDisk);*/
    mkdir(arg,ldir,MainDisk);
    return;
}

void echo(int argc, char* arg){
    printc("\n",10);
    printc(arg,15);
    return; 
}

void write(int argc, char* arg){
    FAT12BOOT boot;
     
    //fat12readboot(MainDisk,boot);
    RDBOOT(boot, MainDisk);
    FATFILE *fils=( FATFILE *)malloc(0x1000*32);
    //return ;
    mfree((void*)fils, 0x1000*32);
    memset((u8)0,(void*)fils,0x1000*32);
    
    fdir(ldir,(FATFILE*)fils,getrootenris(boot),MainDisk);
   // return;
    //memset((u8)0,(void*)fils,0x1000);
    //fdir(ldir,(FATFILE*)fils,1000,MainDisk);
    //read_sata(getrootdir(boot),4,(u8*)fils,MainDisk.AHCIcontrolnum,MainDisk.portnum);
    //rootdir(fils,MainDisk);
    char* data = arg;
    char* filename;
    u32 datasize = 0;
    bool coorect=false;
    for(int i = 0 ; i < argc  ; i++){
        if(arg[i]=='>'){
            filename=&arg[i+1];
            coorect=1;
            break;
        }
        datasize++;
    }
    if(!coorect){
        printc("\nWrite formal Err.\n",12);
        return;
    }
    
    for(int i = 0 ; i < 1000; i++){
        u32 cmps = 9;
        if(fils[i].Name[0]|fils[i].Name[1]){
            for(int ii = 8 ; ii>=0 ; ii--){
                if(fils[i].Name[ii]==0){cmps=ii;}
            }
        }
        
        if(memcmp((void*)filename , (void*)fils[i].Name, cmps)){       
           //write_to_file(fils[i],data,datasize,MainDisk); 
           save(ldir, (char *)fils[i].Name, (u8 *)data, datasize, MainDisk);
	   return;
        }
    }
    printc("\nFile Not faind.",12);
}


void read(int argc, char* arg){
    //FATFILE fils[0x1000] ;
     KeyPress=0;
     FATFILE *fils=(FATFILE *)malloc(0x1000*sizeof(FATFILE));
     mfree(fils, 0x1000*sizeof(FATFILE)) ;


    memset((u8)0,(void*)fils,1000*sizeof(FATFILE));
    fdir(ldir,(FATFILE*)fils,1000,MainDisk);
    

    for(int i = 0 ; i < 1000; i++){
        u32 cmps = 11;
        
        if(fils[i].Name[0]|fils[i].Name[1]){
            for(int ii = 7 ; ii>=0 ; ii--){
                if(fils[i].Name[ii]==' '){cmps=ii;}
            }
        }
    
        if(memcmp((void*)arg , (void*)fils[i].Name, cmps)){
            char *data=(char*)malloc(fils[i].size);
            read_file(fils[i],data/*,fils[i].size+512*/,MainDisk);
            printc("\n",10);
            //sleep_print(data,15,fils[i].size,10);
            bool inslp = false;
	    if(memcmp((void*)((u32)arg+cmps+1),(void*)"sleep",5))inslp=true;
	    for(int ii = 0 ; ii < fils[i].size;ii++){
		print_char((u8)data[ii], 15);
		if(inslp)sleep(2);
		if(KeyPress)break;
	    }
	    mfree(data,fils[i].size);
            return;
        }
    }
    printc("\nErr: file not fond .",12);
    return;
}

void rm(int argc, char* arg){
     removefile(ldir, arg, MainDisk); 
}

void clear(int argc, char* arg){
    TTLclear();
}

void blabla(int argc , char* arg){
    u8 sec[512];
    for(int i = 0 ; i<512 ; i++){
        sec[i]=arg[i%argc];
    }
    //write_sata(2,1,(volatile u8*)sec,MainDisk.AHCIcontrolnum,MainDisk.portnum);
    writD(2, (u8 *)sec, 1, MainDisk);
    u8 seci[512];
    //read_sata(2,1,(volatile u8*)seci,MainDisk.AHCIcontrolnum,MainDisk.portnum);
    readD(2, (u8 *)seci, 1, MainDisk);
    sleep_print((char*)seci,15,512,5);
    return;
}


void fat(int argc, char* arg){
    u16 fat[12*512];
    FAT12BOOT boot;
    fat12readboot(MainDisk,boot);
    printc("\n",12);
    if(arg[0]=='1'){
        for(int i = 0 ; i < 128 ; i++){
            writecluster(i,-1,boot,MainDisk);
        }
    }else if(arg[0]=='0'){
        for(int i = 0 ; i < 128 ; i++){
            writecluster(i,0,boot,MainDisk);
        }
    }else if(arg[0]=='2'){
        for(int i = 0 ; i < 128 ; i++){
            writecluster(i,i+1,boot,MainDisk);
        }
    }
    memset((u16)0,fat,512);;
    readD(getFatAT(boot),(u8*)fat,24,MainDisk);
    for(int i = 0 ; i < 2024  ; i++){
        printc(Int_To_STR(i,16),15);
        printc(" : ",15);
        if(fat[i]==0xffff){
            printc(Hex(nextcluster(i,boot,MainDisk)),1);    
        }else printc(Hex(nextcluster(i,boot,MainDisk)),((u8 [2]){15,14})[i%2]);
        printc("--",15);
        if(fat[i]==0xffff){
            printc(Hex(fat[i]),1);    
        }else printc(Hex(fat[i]),((u8 [2]){15,14})[(i+1)%2]);
        printc("  ",12);
        printc(Hex(getempitycluster(boot,MainDisk)),12);
        printc("\n",12);
        
        sleep(50);
        if((LCtrl|RCtrl )&&(key=='c'|key=='C'))return;
        if(key==' '){
            printc("PASSED",15);
            while (key==' ');
            while(key!=' '){
                if((LCtrl|RCtrl )&&(key=='c'|key=='C'))return;
            }
            printc("\r",12);
            while (key==' ');
        }
    }
}


void time(int arc, char* arg){
    u32 h,mi,se;
    get_time(h,mi,se);
    printc("\n",12);
    printc(Int_To_STR(h,10),14);
    printc(":",15);
    printc(Int_To_STR(mi,10),14);
    printc(":",15);
    printc(Int_To_STR(se,10),14);
    printc("\n",12);
    u32 y,mo,da;
    get_date(da,mo,y);
    printc(Int_To_STR(da,10),14);
    printc("/",15);
    printc(Int_To_STR(mo,10),14);
    printc("/",15);
    printc(Int_To_STR(y,10),14);
    printc("\n",12);
    return;
}










void help(int agrc, char* arg){
	for (int i = 0; i<cmdnum; i++) {
		printc("\n",15);
		printc(commands[i].name,14);
		//printc("       ",15);
		set_print_pos((10), get_print_pos().y);
		printc(commands[i].disc,15);
	}


	return;
}


