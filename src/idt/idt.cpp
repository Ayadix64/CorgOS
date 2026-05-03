#include "idt.h"
#include "../mem/paging.h"
/*hello , if you indrestand this , now , dont move , as you are , go to nasa ;)*/

#include "system_call.h"
regs Kerenlregs;

extern "C" void irq_handler(regs* regesters);
void div0_exc(regs* reg);


idt_entry IDT[256];
idt_ptr IDT_ptr;
void* irqs_handlers[256];
void* isrs_handlers[32];

extern "C" void isr0();
extern "C" void isr1();
extern "C" void isr2();
extern "C" void isr3();
extern "C" void isr4();
extern "C" void isr5();
extern "C" void isr6();
extern "C" void isr7();
extern "C" void isr8();
extern "C" void isr9();
extern "C" void isr10();
extern "C" void isr11();
extern "C" void isr12();
extern "C" void isr13();
extern "C" void isr14();
extern "C" void isr15();
extern "C" void isr16();
extern "C" void isr17();
extern "C" void isr18();
extern "C" void isr19();
extern "C" void isr20();
extern "C" void isr21();
extern "C" void isr22();
extern "C" void isr23();
extern "C" void isr24();
extern "C" void isr25();
extern "C" void isr26();
extern "C" void isr27();
extern "C" void isr28();
extern "C" void isr29();
extern "C" void isr30();
extern "C" void isr31();
//extern "C" void isr32();


extern "C" void isr128();
extern "C" void isr177();


//CPP file ... auto creat!

extern "C" void irq0();
extern "C" void irq1();
extern "C" void irq2();
extern "C" void irq3();
extern "C" void irq4();
extern "C" void irq5();
extern "C" void irq6();
extern "C" void irq7();
extern "C" void irq8();
extern "C" void irq9();
extern "C" void irq10();
extern "C" void irq11();
extern "C" void irq12();
extern "C" void irq13();
extern "C" void irq14();
extern "C" void irq15();
extern "C" void irq16();
extern "C" void irq17();
extern "C" void irq18();
extern "C" void irq19();
extern "C" void irq20();
extern "C" void irq21();
extern "C" void irq22();
extern "C" void irq23();
extern "C" void irq24();
extern "C" void irq25();
extern "C" void irq26();
extern "C" void irq27();
extern "C" void irq28();
extern "C" void irq29();
extern "C" void irq30();
extern "C" void irq31();
extern "C" void irq32();
extern "C" void irq33();
extern "C" void irq34();
extern "C" void irq35();
extern "C" void irq36();
extern "C" void irq37();
extern "C" void irq38();
extern "C" void irq39();
extern "C" void irq40();
extern "C" void irq41();
extern "C" void irq42();
extern "C" void irq43();
extern "C" void irq44();
extern "C" void irq45();
extern "C" void irq46();
extern "C" void irq47();
extern "C" void irq48();
extern "C" void irq49();
extern "C" void irq50();
extern "C" void irq51();
extern "C" void irq52();
extern "C" void irq53();
extern "C" void irq54();
extern "C" void irq55();
extern "C" void irq56();
extern "C" void irq57();
extern "C" void irq58();
extern "C" void irq59();
extern "C" void irq60();
extern "C" void irq61();
extern "C" void irq62();
extern "C" void irq63();
extern "C" void irq64();
extern "C" void irq65();
extern "C" void irq66();
extern "C" void irq67();
extern "C" void irq68();
extern "C" void irq69();
extern "C" void irq70();
extern "C" void irq71();
extern "C" void irq72();
extern "C" void irq73();
extern "C" void irq74();
extern "C" void irq75();
extern "C" void irq76();
extern "C" void irq77();
extern "C" void irq78();
extern "C" void irq79();
extern "C" void irq80();
extern "C" void irq81();
extern "C" void irq82();
extern "C" void irq83();
extern "C" void irq84();
extern "C" void irq85();
extern "C" void irq86();
extern "C" void irq87();
extern "C" void irq88();
extern "C" void irq89();
extern "C" void irq90();
extern "C" void irq91();
extern "C" void irq92();
extern "C" void irq93();
extern "C" void irq94();
extern "C" void irq95();
extern "C" void irq97();
extern "C" void irq98();
extern "C" void irq99();
extern "C" void irq100();
extern "C" void irq101();
extern "C" void irq102();
extern "C" void irq103();
extern "C" void irq104();
extern "C" void irq105();
extern "C" void irq106();
extern "C" void irq107();
extern "C" void irq108();
extern "C" void irq109();
extern "C" void irq110();
extern "C" void irq111();
extern "C" void irq112();
extern "C" void irq113();
extern "C" void irq114();
extern "C" void irq115();
extern "C" void irq116();
extern "C" void irq117();
extern "C" void irq118();
extern "C" void irq119();
extern "C" void irq120();
extern "C" void irq121();
extern "C" void irq122();
extern "C" void irq123();
extern "C" void irq124();
extern "C" void irq125();
extern "C" void irq126();
extern "C" void irq127();
extern "C" void irq128();
extern "C" void irq129();
extern "C" void irq130();
extern "C" void irq131();
extern "C" void irq132();
extern "C" void irq133();
extern "C" void irq134();
extern "C" void irq135();
extern "C" void irq136();
extern "C" void irq137();
extern "C" void irq138();
extern "C" void irq139();
extern "C" void irq140();
extern "C" void irq141();
extern "C" void irq142();
extern "C" void irq143();
extern "C" void irq144();
extern "C" void irq146();
extern "C" void irq147();
extern "C" void irq148();
extern "C" void irq149();
extern "C" void irq150();
extern "C" void irq151();
extern "C" void irq152();
extern "C" void irq153();
extern "C" void irq154();
extern "C" void irq155();
extern "C" void irq156();
extern "C" void irq157();
extern "C" void irq158();
extern "C" void irq159();
extern "C" void irq160();
extern "C" void irq161();
extern "C" void irq162();
extern "C" void irq163();
extern "C" void irq164();
extern "C" void irq165();
extern "C" void irq166();
extern "C" void irq167();
extern "C" void irq168();
extern "C" void irq169();
extern "C" void irq170();
extern "C" void irq171();
extern "C" void irq172();
extern "C" void irq173();
extern "C" void irq174();
extern "C" void irq175();
extern "C" void irq176();
extern "C" void irq177();
extern "C" void irq178();
extern "C" void irq179();
extern "C" void irq180();
extern "C" void irq181();
extern "C" void irq182();
extern "C" void irq183();
extern "C" void irq184();
extern "C" void irq185();
extern "C" void irq186();
extern "C" void irq187();
extern "C" void irq188();
extern "C" void irq189();
extern "C" void irq190();
extern "C" void irq191();
extern "C" void irq192();
extern "C" void irq193();
extern "C" void irq194();
extern "C" void irq195();
extern "C" void irq196();
extern "C" void irq197();
extern "C" void irq198();
extern "C" void irq199();
extern "C" void irq200();
extern "C" void irq201();
extern "C" void irq202();
extern "C" void irq203();
extern "C" void irq204();
extern "C" void irq205();
extern "C" void irq206();
extern "C" void irq207();
extern "C" void irq208();
extern "C" void irq209();
extern "C" void irq210();
extern "C" void irq211();
extern "C" void irq212();
extern "C" void irq213();
extern "C" void irq214();
extern "C" void irq215();
extern "C" void irq216();
extern "C" void irq217();
extern "C" void irq218();
extern "C" void irq219();
extern "C" void irq220();
extern "C" void irq221();
extern "C" void irq222();
extern "C" void irq223();

void systemcall(){
    isr128();
}
bool APIC_enable = false;
bool SSE3_enable = false;

u32 APIC_Local = 0xFEE00000;
volatile u32* APICIO = (volatile u32*) 0xFEC00000;





void out_apic(u32 offset, u32 data){
   // *(volatile u32*)((APIC_Local+offset))/*hey , i dont know but mybe this most be reg*4*/=data;
    *(volatile u32*)((offset+APIC_Local))=(volatile int)data;
    //APIC_Local[offset]=data;
}

u32 in_apic(u32 offset){
    return *(volatile u32*)(APIC_Local + offset );//APIC_Local[offset];//  
}


void out_apic_io(u8 reg, u32 val){
    APICIO[0]=reg;
    *(volatile u32*)((u8*)APICIO+0x10)=val;
}

u32 in_apic_io(u8 reg){
    APICIO[0]=reg;
    return *(volatile u32*)((u8*)APICIO+0x10);    
}








void PIC_disable(){
    // إرسال أوامر لتعطيل PIC
    outb(0xA1, 0xFF);
    outb(0x21, 0xFF);

    outb(0x20, 0x11);       // Initialize both PICs
    	outb(0xA0, 0x11);       
    	outb(0x21, 0x20);       // Set vector offsets
    	outb(0xA1, 0x28);
    	outb(0x21, 0x04);       // tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    	outb(0xA1, 0x02);       // tell Slave PIC its cascade identity (0000 0010)
    	outb(0x21, 0x01);       // have the PICs use 8086 mode (and not 8080 mode)
    	outb(0xA1, 0x01);
    	outb(0x21, 0xff);        // set masks
    	outb(0xA1, 0xff);	
}




void EOI(regs* reg){
    if(APIC_enable){
        out_apic(0xb0,0);
        //*(volatile char*)(APIC_Local+0xb0)=-1;
        return;
    }
    else{
        if(reg->int_no >=40){
            outb(0xA0,0x20);
        }
        outb(0x20,0x20);
    }
    return;
}

/*********************************/





void irq_capabiltis(){
    u32 eax=1,ebx=0,ecx=0,edx=0;
    cpuid(eax,ebx,ecx,edx);
    
    if(edx&(1<<9)){
        PIC_disable();	
        APIC_enable=true;
        u64 readmsr = rdmsr(0x1B);
        /*if(!(readmsr&(1<<11))){
            //readmsr|=(1<<11);
            wrmsr(0x1B,readmsr|(1<<11));/*hi , my be wrmsr fonction have a problam , how are you doing ?*

        }*/
        readmsr&= ~(1<<11);
        readmsr|= (1<<11);
        out_apic(0xF0, in_apic(0xF0) | 0x100);
        wrmsr(0x1b,readmsr);
        
        readmsr = rdmsr(0x1B);

        APIC_Local = (volatile u32)((readmsr & 0xFFFFF000));
       	/**********Protect APIC MEMORY,it size is 4K**********/
	prtcmem((void*)APIC_Local, 0x1000);
	/*********************/
	
	//APIC_Local[0x3f0]=0x1ff;
        out_apic(0x3f0, 0x1ff);
        out_apic(0xF0, in_apic(0xF0) | 0x100);
        /*if(readmsr>0){printc("rdmsr!=0",12);} 
        printc("\nrdmsr : ",10);
        printc(Hex((u64)(readmsr)),14);
        printc("\nAPIC Enabel ! at adress : ",10);
        printc(Hex((u32)APIC_Local),14);
        */
        printc("\n",10);

    }else{
        APIC_enable=false;
    }
    if(ecx&1){
        SSE3_enable=true;
    }else{
        SSE3_enable=false;
    }

}
void PIC_APIC_INIT(){
    if(APIC_enable){
            
        outb(0x64, 0xA8);
        outb(0x64, 0x20);
        u8 status = inb(0x60) | 2;
        outb(0x64, 0x60);
        outb(0x60, status);
        
        outb(0x64,0xD4);
        outb(0x60,0xf6);
        //mouse_write(0xF6);
        
        //mouse_read();
        inb(0x60);

        outb(0x64,0xD4);
        outb(0x60,0xf4);
        //mouse_write(0xF4);
        inb(0x60);

        // تهيئة IOAPIC لمقاطعة لوحة المفاتيح (IRQ1 -> Vector 33)
//-new-      out_apic_io(0x12, 0x21);  // Vector 33 لـ IRQ1
//-new-      out_apic_io(0x13, 0x0);  // Destination: CPU0
        //out_apic_io(0x10+2*12, 0x20+12);  // Vector 33 لـ IRQ1
        //out_apic_io(0x10+2*12+1, 0x0);  // Destination: CPU0

    }else{
        // تهيئة PIC التقليدي
        outb(0x20, 0x11);       // Initialize both PICs
    	outb(0xA0, 0x11);       
    	outb(0x21, 0x20);       // Set vector offsets
    	outb(0xA1, 0x28);
    	outb(0x21, 0x04);       // tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    	outb(0xA1, 0x02);       // tell Slave PIC its cascade identity (0000 0010)
    	outb(0x21, 0x01);       // have the PICs use 8086 mode (and not 8080 mode)
    	outb(0xA1, 0x01);
    	outb(0x21, 0x0);        // set masks
    	outb(0xA1, 0x0);	
    }
}
/*void PIC_APIC_INIT(){
    if(APIC_enable){
        //outb(0x22,0x70);
        //outb(0x23,1);
        //out_apic_io(0x10+2 ,0x21/*|(1<<15)*);//i dont kwox xay , but this enable keybeard irss , but just one intrupt 
        //out_apic_io(0x13,(in_apic(0x20)>>24)<<24);
        //out_apic(0xE0, 0xFFFFFFFF); // ضبط DFR إلى Flat Mode
       //out_apic(0x300, 0x40 | (1 << 14));  // إرسال IPI إلى نفس المعالج
        //while(in_apic(0x200));

    }else{
	    outb(0x20, 0x11);       // Initialize both PICs
    	outb(0xA0, 0x11);       
    	outb(0x21, 0x20);       // Set vector offsets
    	outb(0xA1, 0x28);
    	outb(0x21, 0x04);       // tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    	outb(0xA1, 0x02);       // tell Slave PIC its cascade identity (0000 0010)
    	outb(0x21, 0x01);       // have the PICs use 8086 mode (and not 8080 mode)
    	outb(0xA1, 0x01);
    	outb(0x21, 0x0);        // set masks
    	outb(0xA1, 0x0);	
    }
}


*/




/*************************************************************/
void set_hadler(u32 index, u32 addr){
    IDT[index].offset_low=addr&0xffff;
    IDT[index].offset_high=(addr>>16)&0xffff;
    IDT[index].zero=0;
    IDT[index].selector=0x8;
    IDT[index].type_attr=0x8e;
}
void set_system_call(u32 index, u32 addr){
    IDT[index].offset_low=addr&0xffff;
    IDT[index].offset_high=(addr>>16)&0xffff;
    IDT[index].zero=0;
    IDT[index].selector=0x8;
    IDT[index].type_attr=0xee;//acss user interupt
}
/************************************************************/



void idt_init(){
    irq_capabiltis();
     //APIC_enable=false;
    PIC_APIC_INIT();
    set_hadler(0,(u32)isr0);
    set_hadler(1,(u32)isr1);
    set_hadler(2,(u32)isr2);
    set_hadler(3,(u32)isr3);
    set_hadler(4,(u32)isr4);
    set_hadler(5,(u32)isr5);
    set_hadler(6,(u32)isr6);
    set_hadler(7,(u32)isr7);
    set_hadler(8,(u32)isr8);
    set_hadler(9,(u32)isr9);
    set_hadler(10,(u32)isr10);
    set_hadler(11,(u32)isr11);
    set_hadler(12,(u32)isr12);
    set_hadler(13,(u32)isr13);
    set_hadler(14,(u32)isr14);
    set_hadler(15,(u32)isr15);
    set_hadler(16,(u32)isr16);
    set_hadler(17,(u32)isr17);
    set_hadler(18,(u32)isr18);
    set_hadler(19,(u32)isr19);
    set_hadler(20,(u32)isr20);
    set_hadler(21,(u32)isr21);
    set_hadler(22,(u32)isr22);
    set_hadler(23,(u32)isr23);
    set_hadler(24,(u32)isr24);
    set_hadler(25,(u32)isr25);
    set_hadler(26,(u32)isr26);
    set_hadler(27,(u32)isr27);
    set_hadler(28,(u32)isr28);
    set_hadler(29,(u32)isr29);
    set_hadler(30,(u32)isr30);
    set_hadler(31,(u32)isr31);
    //set_hadler(32,(u32)isr32);

    set_system_call(128,(u32)isr128);
    set_system_call(177,(u32)isr177);
   	
	
   //CPP file , install handler... auto creat!

    set_hadler(32,(u32)irq0);
    set_hadler(33,(u32)irq1);
    set_hadler(34,(u32)irq2);
    set_hadler(35,(u32)irq3);
    set_hadler(36,(u32)irq4);
    set_hadler(37,(u32)irq5);
    set_hadler(38,(u32)irq6);
    set_hadler(39,(u32)irq7);
    set_hadler(40,(u32)irq8);
    set_hadler(41,(u32)irq9);
    set_hadler(42,(u32)irq10);
    set_hadler(43,(u32)irq11);
    set_hadler(44,(u32)irq12);
    set_hadler(45,(u32)irq13);
    set_hadler(46,(u32)irq14);
    set_hadler(47,(u32)irq15);
    set_hadler(48,(u32)irq16);
    set_hadler(49,(u32)irq17);
    set_hadler(50,(u32)irq18);
    set_hadler(51,(u32)irq19);
    set_hadler(52,(u32)irq20);
    set_hadler(53,(u32)irq21);
    set_hadler(54,(u32)irq22);
    set_hadler(55,(u32)irq23);
    set_hadler(56,(u32)irq24);
    set_hadler(57,(u32)irq25);
    set_hadler(58,(u32)irq26);
    set_hadler(59,(u32)irq27);
    set_hadler(60,(u32)irq28);
    set_hadler(61,(u32)irq29);
    set_hadler(62,(u32)irq30);
    set_hadler(63,(u32)irq31);
    set_hadler(64,(u32)irq32);
    set_hadler(65,(u32)irq33);
    set_hadler(66,(u32)irq34);
    set_hadler(67,(u32)irq35);
    set_hadler(68,(u32)irq36);
    set_hadler(69,(u32)irq37);
    set_hadler(70,(u32)irq38);
    set_hadler(71,(u32)irq39);
    set_hadler(72,(u32)irq40);
    set_hadler(73,(u32)irq41);
    set_hadler(74,(u32)irq42);
    set_hadler(75,(u32)irq43);
    set_hadler(76,(u32)irq44);
    set_hadler(77,(u32)irq45);
    set_hadler(78,(u32)irq46);
    set_hadler(79,(u32)irq47);
    set_hadler(80,(u32)irq48);
    set_hadler(81,(u32)irq49);
    set_hadler(82,(u32)irq50);
    set_hadler(83,(u32)irq51);
    set_hadler(84,(u32)irq52);
    set_hadler(85,(u32)irq53);
    set_hadler(86,(u32)irq54);
    set_hadler(87,(u32)irq55);
    set_hadler(88,(u32)irq56);
    set_hadler(89,(u32)irq57);
    set_hadler(90,(u32)irq58);
    set_hadler(91,(u32)irq59);
    set_hadler(92,(u32)irq60);
    set_hadler(93,(u32)irq61);
    set_hadler(94,(u32)irq62);
    set_hadler(95,(u32)irq63);
    set_hadler(96,(u32)irq64);
    set_hadler(97,(u32)irq65);
    set_hadler(98,(u32)irq66);
    set_hadler(99,(u32)irq67);
    set_hadler(100,(u32)irq68);
    set_hadler(101,(u32)irq69);
    set_hadler(102,(u32)irq70);
    set_hadler(103,(u32)irq71);
    set_hadler(104,(u32)irq72);
    set_hadler(105,(u32)irq73);
    set_hadler(106,(u32)irq74);
    set_hadler(107,(u32)irq75);
    set_hadler(108,(u32)irq76);
    set_hadler(109,(u32)irq77);
    set_hadler(110,(u32)irq78);
    set_hadler(111,(u32)irq79);
    set_hadler(112,(u32)irq80);
    set_hadler(113,(u32)irq81);
    set_hadler(114,(u32)irq82);
    set_hadler(115,(u32)irq83);
    set_hadler(116,(u32)irq84);
    set_hadler(117,(u32)irq85);
    set_hadler(118,(u32)irq86);
    set_hadler(119,(u32)irq87);
    set_hadler(120,(u32)irq88);
    set_hadler(121,(u32)irq89);
    set_hadler(122,(u32)irq90);
    set_hadler(123,(u32)irq91);
    set_hadler(124,(u32)irq92);
    set_hadler(125,(u32)irq93);
    set_hadler(126,(u32)irq94);
    set_hadler(127,(u32)irq95);
    set_hadler(129,(u32)irq97);
    set_hadler(130,(u32)irq98);
    set_hadler(131,(u32)irq99);
    set_hadler(132,(u32)irq100);
    set_hadler(133,(u32)irq101);
    set_hadler(134,(u32)irq102);
    set_hadler(135,(u32)irq103);
    set_hadler(136,(u32)irq104);
    set_hadler(137,(u32)irq105);
    set_hadler(138,(u32)irq106);
    set_hadler(139,(u32)irq107);
    set_hadler(140,(u32)irq108);
    set_hadler(141,(u32)irq109);
    set_hadler(142,(u32)irq110);
    set_hadler(143,(u32)irq111);
    set_hadler(144,(u32)irq112);
    set_hadler(145,(u32)irq113);
    set_hadler(146,(u32)irq114);
    set_hadler(147,(u32)irq115);
    set_hadler(148,(u32)irq116);
    set_hadler(149,(u32)irq117);
    set_hadler(150,(u32)irq118);
    set_hadler(151,(u32)irq119);
    set_hadler(152,(u32)irq120);
    set_hadler(153,(u32)irq121);
    set_hadler(154,(u32)irq122);
    set_hadler(155,(u32)irq123);
    set_hadler(156,(u32)irq124);
    set_hadler(157,(u32)irq125);
    set_hadler(158,(u32)irq126);
    set_hadler(159,(u32)irq127);
    set_hadler(160,(u32)irq128);
    set_hadler(161,(u32)irq129);
    set_hadler(162,(u32)irq130);
    set_hadler(163,(u32)irq131);
    set_hadler(164,(u32)irq132);
    set_hadler(165,(u32)irq133);
    set_hadler(166,(u32)irq134);
    set_hadler(167,(u32)irq135);
    set_hadler(168,(u32)irq136);
    set_hadler(169,(u32)irq137);
    set_hadler(170,(u32)irq138);
    set_hadler(171,(u32)irq139);
    set_hadler(172,(u32)irq140);
    set_hadler(173,(u32)irq141);
    set_hadler(174,(u32)irq142);
    set_hadler(175,(u32)irq143);
    set_hadler(176,(u32)irq144);
    set_hadler(178,(u32)irq146);
    set_hadler(179,(u32)irq147);
    set_hadler(180,(u32)irq148);
    set_hadler(181,(u32)irq149);
    set_hadler(182,(u32)irq150);
    set_hadler(183,(u32)irq151);
    set_hadler(184,(u32)irq152);
    set_hadler(185,(u32)irq153);
    set_hadler(186,(u32)irq154);
    set_hadler(187,(u32)irq155);
    set_hadler(188,(u32)irq156);
    set_hadler(189,(u32)irq157);
    set_hadler(190,(u32)irq158);
    set_hadler(191,(u32)irq159);
    set_hadler(192,(u32)irq160);
    set_hadler(193,(u32)irq161);
    set_hadler(194,(u32)irq162);
    set_hadler(195,(u32)irq163);
    set_hadler(196,(u32)irq164);
    set_hadler(197,(u32)irq165);
    set_hadler(198,(u32)irq166);
    set_hadler(199,(u32)irq167);
    set_hadler(200,(u32)irq168);
    set_hadler(201,(u32)irq169);
    set_hadler(202,(u32)irq170);
    set_hadler(203,(u32)irq171);
    set_hadler(204,(u32)irq172);
    set_hadler(205,(u32)irq173);
    set_hadler(206,(u32)irq174);
    set_hadler(207,(u32)irq175);
    set_hadler(208,(u32)irq176);
    set_hadler(209,(u32)irq177);
    set_hadler(210,(u32)irq178);
    set_hadler(211,(u32)irq179);
    set_hadler(212,(u32)irq180);
    set_hadler(213,(u32)irq181);
    set_hadler(214,(u32)irq182);
    set_hadler(215,(u32)irq183);
    set_hadler(216,(u32)irq184);
    set_hadler(217,(u32)irq185);
    set_hadler(218,(u32)irq186);
    set_hadler(219,(u32)irq187);
    set_hadler(220,(u32)irq188);
    set_hadler(221,(u32)irq189);
    set_hadler(222,(u32)irq190);
    set_hadler(223,(u32)irq191);
    set_hadler(224,(u32)irq192);
    set_hadler(225,(u32)irq193);
    set_hadler(226,(u32)irq194);
    set_hadler(227,(u32)irq195);
    set_hadler(228,(u32)irq196);
    set_hadler(229,(u32)irq197);
    set_hadler(230,(u32)irq198);
    set_hadler(231,(u32)irq199);
    set_hadler(232,(u32)irq200);
    set_hadler(233,(u32)irq201);
    set_hadler(234,(u32)irq202);
    set_hadler(235,(u32)irq203);
    set_hadler(236,(u32)irq204);
    set_hadler(237,(u32)irq205);
    set_hadler(238,(u32)irq206);
    set_hadler(239,(u32)irq207);
    set_hadler(240,(u32)irq208);
    set_hadler(241,(u32)irq209);
    set_hadler(242,(u32)irq210);
    set_hadler(243,(u32)irq211);
    set_hadler(244,(u32)irq212);
    set_hadler(245,(u32)irq213);
    set_hadler(246,(u32)irq214);
    set_hadler(247,(u32)irq215);
    set_hadler(248,(u32)irq216);
    set_hadler(249,(u32)irq217);
    set_hadler(250,(u32)irq218);
    set_hadler(251,(u32)irq219);
    set_hadler(252,(u32)irq220);
    set_hadler(253,(u32)irq221);
    set_hadler(254,(u32)irq222);
    set_hadler(255,(u32)irq223);

    /*frome memory : i never forgat the day who evry thing not work corectly and the OS stop doing intrupt and frezz
    finaly , i chek out bootleoadr after 120.7 year frome debuging and i discover that my os dont reding the os complitly
    dont do this at home kids*/
        


    IDT_ptr.limit=sizeof(IDT)-1;
    IDT_ptr.base=(u32)(&IDT);
    asm("lidt %0" :: "m"(IDT_ptr));

    for(int i = 0 ; i < 256 ; i++){
        irqs_handlers[i]=(void*)0;
    }
     for(int i = 0 ; i < 32 ; i++){
        isrs_handlers[i]=(void*)0;
    }
    install_isr(0,div0_exc);
    
    //syscall1();
    //irq0();
    
    

    
    
    //out_apic(0x320,32|0x20000);
  //  out_apic(0x3e0,3);
  //  out_apic(0x380,10000000);
  //  out_apic_io(0x10,32);

    // asm("int $33");
    //while(!in_apic(0x200)){}
    //printc("INTRUPT",10);
    //printc(Hex(in_apic(0x200)),10);
   
    //u32 id = in_apic_io(0x20);
    //out_apic_io(0x22,1);
    //printc((id>0)?(char*)"APIC ID: ":(char*)"N", 10);
    //printc(Hex(id>0), 14);
    //printc("\n", 10);
    

    return;
}
extern "C" void isr_handler(regs* regesters){
    //install_mainpd(); 
    if(regesters->int_no<32){
        printc("\nsystem crach!!!:exception 0x",12);
        printc(Hex(regesters->int_no),12);
        printc("\n",12);
        if(isrs_handlers[regesters->int_no]!=(void*)0){
            void (*handler)(regs* reg) = (void(*)(regs*))isrs_handlers[regesters->int_no];
            handler(regesters);
        }else{
	    //printc("EAX: ",15);
	    //printc(Hex(regesters->eax),14);
	        printc("\nDEBUG INFO\n",12);
		printc("EAX : ",15);
		printc(Hex(regesters->eax),14);
		printc("            EBX : ",15);
		printc(Hex(regesters->ebx),14);
	
		printc("\nECX : ",15);
		printc(Hex(regesters->ecx),14);
		printc("            EDX : ",15);
		printc(Hex(regesters->edx),14);

		printc("\nCR0 : ",15);
		printc(Hex(regesters->cr0),14);
		printc("            CR2 : ",15);
		printc(Hex(regesters->cr2),14);
	
		printc("\nCR3 : ",15);
		printc(Hex(regesters->cr3),14);
		printc("            CR4 : ",15);
		printc(Hex(regesters->cr4),14);
	
		printc("\nESP : ",15);
		printc(Hex(regesters->esp),14);
		printc("            EBP : ",15);
		printc(Hex(regesters->ebp),14);

	    
            	asm("hlt");
        }
    } if(regesters->int_no == 128 || regesters->int_no == 177 ){
	//printc("\n SYSTEM CALL",12);

	system_call(regesters);
	
	
	
	//regesters->eip++;
	
    }
    EOI(regesters);
    return;
}




void install_irq(u32 irq , void (*irq_hand)(regs* reg),u32 CPU){
    if(irq==96/*128-32 system call*/ || irq==145/*177-32*/){return;}
    out_apic_io(0x10+irq*2, 0x20+irq);  // Vector 33 لـ IRQ1
    out_apic_io(0x13, CPU);  // Destination: CPU0

    irqs_handlers[irq]=(void*)(irq_hand);
    return;
}


void uninstall_irq(u32 irq){
    out_apic_io(0x10+irq*2, 0);  // Vector 33 لـ IRQ1
    out_apic_io(0x13, 0);  // Destination: CPU0
    irqs_handlers[irq]=(void*)0;
    return;
}

void install_isr(u32 isr , void (*isr_hand)(regs* reg)){
    if(isr >=32 ){return;}
    isrs_handlers[isr]=(void*)(isr_hand);
    return;
}


void uninstall_isr(u32 isr){
    isrs_handlers[isr]=(void*)0;
}

void div0_exc(regs* reg){
    reg->eip+=1;
    return;
}

extern "C" void irq_handler(regs* regesters){
    //install_mainpd();
    if(irqs_handlers[regesters->int_no-0x20]!=(void*)0){
        void (*handler)(regs* reg) = (void(*)(regs*))irqs_handlers[regesters->int_no-0x20];
        handler(regesters);
    }
    //int cr0 ;
    //asm("mov %%cr0 , %0":"=r"(cr0));
    //if(cr0!=regesters->cr0)printc("somthing wrong!",15);
    EOI(regesters);
    //wrmsr(0x80b,0);
    //out_apic(0xb0,(u32)0);
    //*(volatile u32*)0xFEE000B0=0;  
    return;
}




/*othor thing also*/





regs loadreg(){
	regs r;
	asm volatile ("mov %%cr0 , %0":"=r"(r.cr0));
	asm volatile ("mov %%cr2 , %0":"=r"(r.cr2));
	asm volatile ("mov %%cr3 , %0":"=r"(r.cr3));
	asm volatile ("mov %%cr4 , %0":"=r"(r.cr4));
	
	asm volatile ("mov %%eax , %0":"=r"(r.eax));
	asm volatile ("mov %%ebx , %0":"=r"(r.ebx));
	asm volatile ("mov %%ecx , %0":"=r"(r.ecx));
	asm volatile ("mov %%edx , %0":"=r"(r.edx));

	asm volatile ("mov %%edi , %0":"=r"(r.edi));
	asm volatile ("mov %%esi , %0":"=r"(r.esi));
	asm volatile ("mov %%ebp , %0":"=r"(r.ebp));
	asm volatile ("mov %%esp , %0":"=r"(r.esp));

	asm volatile ("mov %%gs , %0":"=r"(r.gs));
	asm volatile ("mov %%fs , %0":"=r"(r.fs));
	asm volatile ("mov %%es , %0":"=r"(r.es));
	asm volatile ("mov %%ds , %0":"=r"(r.ds));
	return r;
}

void setreg(regs& r){
	asm volatile ("mov %%cr0 , %0"::"r"(r.cr0));
	asm volatile ("mov %%cr2 , %0"::"r"(r.cr2));
	asm volatile ("mov %%cr3 , %0"::"r"(r.cr3));
	asm volatile ("mov %%cr4 , %0"::"r"(r.cr4));
	
	asm volatile ("mov %%eax , %0"::"r"(r.eax));
	asm volatile ("mov %%ebx , %0"::"r"(r.ebx));
	asm volatile ("mov %%ecx , %0"::"r"(r.ecx));
	asm volatile ("mov %%edx , %0"::"r"(r.edx));

	asm volatile ("mov %%edi , %0"::"r"(r.edi));
	asm volatile ("mov %%esi , %0"::"r"(r.esi));
	asm volatile ("mov %%ebp , %0"::"r"(r.ebp));
	asm volatile ("mov %%esp , %0"::"r"(r.esp));

	asm volatile ("mov %%gs , %0"::"r"(r.gs));
	asm volatile ("mov %%fs , %0"::"r"(r.fs));
	asm volatile ("mov %%es , %0"::"r"(r.es));
	asm volatile ("mov %%ds , %0"::"r"(r.ds));
	return ;
}
