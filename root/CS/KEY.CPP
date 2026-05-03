#include "../utils/util.h"
#include "../idt/idt.h"
//#include "keyboard.h"

extern "C" u8 pkey = 0;
extern "C" u8 key = 0;
extern "C" bool up = 0;
extern "C" bool down = 0;
extern "C" bool left = 0;
extern "C" bool reight = 0;
extern "C" bool ESC = 0;
extern "C" bool lshift = 0;
extern "C" bool rshift = 0;
extern "C" bool vrrshift = 0;
extern "C" bool vrrnum = 1;//hah, how intrest
extern "C" bool LCtrl = 0;
extern "C" bool RCtrl = 0;
extern "C" bool Alt = 0;
extern "C" bool AltGr = 0;
extern "C" bool GUIkey = 0;
extern "C" bool RGUIkey = 0;
extern "C" bool Inser = 0;
extern "C" bool List = 0;
extern "C" bool begine = 0;
extern "C" bool nextpage = 0;
extern "C" bool lastpage = 0;
extern "C" bool End = 0;
extern "C" bool Delet = 0;
extern "C" bool center = 0; //5 in number
extern "C" bool F1 = 0;
extern "C" bool F2 = 0;
extern "C" bool F3 = 0;
extern "C" bool F4 = 0;
extern "C" bool F5 = 0;
extern "C" bool F6 = 0;
extern "C" bool F7 = 0;
extern "C" bool F8 = 0;
extern "C" bool F9 = 0;
extern "C" bool F10 = 0;
extern "C" bool F11 = 0;
extern "C" bool F12 = 0;
bool KeyPress = 0;
const char kbdus[2][128] =
{                 /*Franch Key board (This what i have)*/
  {
    0,  27, '&', '~', '\"', '\'', '(', '-', '`', '_',	/* 9 */
  '^', '@', ')', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'a', 'z', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'q', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'w', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  ',', ';', ':', '!',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,  0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   '<',
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
  },
  {
       0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '+', '\b',	/* Backspace */
  '\t',			/* Tab */
  'A', 'Z', 'E', 'R',	/* 19 */
  'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'Q', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M',	/* 39 */
 '%', '\'',   0,		/* Left shift */
 '\\', 'W', 'X', 'C', 'V', 'B', 'N',			/* 49 */
  '?', '.', '/', '+',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,  0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   '>',
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
  }
};

void keyboard_handler(regs* reg){
    u8 scancode = inb(0x60);

    static bool heyper = 0;
    if(scancode&0x80){
	KeyPress = false;
        switch (scancode)
        {
        case 224://the extern number scan code like the distines key , 
                 //but the disteny code make scan code 224 befur the disteny(up for exampel make scan code 224 72 224 200 but 8 in extern number 
                 // make 72 - 200)
            heyper=true;
            break;
        case 200:
            if(heyper || !vrrnum){
              up=false;
            }
            key=0;
            break;
        case 203:
            if(heyper || !vrrnum){
              left=false;
            }
            key=0;
            break;
        case 205:
            if(heyper || !vrrnum){
              reight=false;
            }
            key=0;
            break;
        case 208:
            if(heyper || !vrrnum){
              down=false;
            }
            key=0;
            break;
        case 182:
            rshift=false;
            break;
        case 170:
            lshift=false;
            break;
        case 186:
            vrrshift=(!vrrshift);
            break;
        case 197:
            vrrnum=(!vrrnum);
        case 129:
            ESC=false;
            break;
        case 220:
              if(heyper)RGUIkey=false;
              else GUIkey=false;
              break;
 
        case 157:
            if(heyper)RCtrl=false;
            else LCtrl=false;
            break;

        case 184:
            if(heyper)AltGr=false;
            else Alt=false;
            break;
        case 221:
              if(heyper)List=false;
              break;
        
        case 210:
            if(heyper||!vrrnum)Inser=false;
            else key=0;
            break;

        case 211:
            if(heyper||!vrrnum)Delet=false;
            else key=0;
            break;
        case 199:
            if(heyper||!vrrnum)begine=false;
            else key=0;
            break;
        case 201:
            if(heyper||!vrrnum)nextpage=false;
            else key=0;
            break;
        
        case 209:
            if(heyper||!vrrnum)lastpage=false;
            else key=0;
            break;
        
        case 207:
            if(heyper||!vrrnum)End=false;
            else key=0;
            break;
        case 204:
              center=false;
              key=0;
              break;
        
        case 187:
            F1=false;
            break;
        case 188:
            F2=false;
            break;
        case 189:
            F3=false;
            break;
        case 190:
            F4=false;
            break;
        case 191:
            F5=false;
            break;
        case 192:
            F6=false;
            break;
        case 193:
            F7=false;
            break;
        case 194:
            F8=false;
            break;
        case 195:
            F9=false;
            break;
        case 196:
            F10=false;
            break;
        case 215:
            F11=false;
            break;
        case 216:
            F12=false;
            break;
        
        default:
            heyper=false;
            key=0;
            break;
        }
        
    }else{
	KeyPress=true;
        switch(scancode){
          case 72:
              if(heyper || !vrrnum){
                up=true;
              }else{
                key='8';
              }
              break;
          case 77:
              if(heyper || !vrrnum){
                reight=true;
              }else{
                key='6';
              }
            break;
          case 75:
              if(heyper || !vrrnum){
                left=true;
              }else{
                key='4';
              }     
              break;
          case 80:
              if(heyper || !vrrnum){
                down=true;
              }else{
                key='2';
              }
               break;
          case 54:
              rshift=true;
              break;
          case 42:
              lshift=true;
              break;

          case 92:
              if(heyper)RGUIkey=true;
              else GUIkey=true;
              break;
 
          case 29:
              if(heyper)RCtrl=true;
              else LCtrl=true;
              break;
          case 56:
              if(heyper)AltGr=true;
              else Alt=true;
              break;
          case 82:
              if(heyper||!vrrnum)Inser=true;
              else key='0';
              break;
          case 83:
              if(heyper||!vrrnum)Delet=true;
              else key='.';
              break;
          case 71:
              if(heyper||!vrrnum)begine=true;
              else key='7';
              break;
          case 73:
              if(heyper||!vrrnum)nextpage=true;
              else key='9';
              break;
          
          case 81:
              if(heyper||!vrrnum)lastpage=true;
              else key='3';
              break;
          
          case 79:
              if(heyper||!vrrnum)End=true;
              else key='1';
              break;
          case 76:
              if(!vrrnum)center=true;
              else key='5';
              break;
          case 93:
              if(heyper)List=true;
              break;
          case 59:
              F1=true;
              break;
          case 60:
              F2=true;
              break;
          case 61:
              F3=true;
              break;
          case 62:
              F4=true;
              break;
          case 63:
              F5=true;
              break;
          case 64:
              F6=true;
              break;
          case 65:
              F7=true;
              break;
          case 66:
              F8=true;
              break;
          case 67:
              F9=true;
              break;
          case 68:
              F10=true;
              break;
          case 87:
              F11=true;
              break;
          case 88:
              F12=true;
              break;
          
          case 1:
              ESC=true;
              break;
          default:
            pkey=key;
            key=kbdus[((lshift|rshift)|(vrrshift))&&!(vrrshift&(lshift|rshift))][scancode];
            if(heyper && key=='!')key='/';
            break;
        }
        heyper=false;
        //printc((char*)(char[2]){key,0},15);
        //printc("\n",10);
    }
    //printc("\n",10);
    //printc(Int_To_STR(scancode,10),14);

    /*black magic*/
}

void LED(bool Scroll, bool Num , bool Caps){
  
    /*while (inb(0x64)&0x2){}
    outb(0x60,0xED);
    u8 send;
    send=(Caps<<2)|(Num<<1)|(Scroll);
    while (inb(0x60)!=0xFA){
      
    }
    //while (inb(0x64)&0x2){}
    outb(0x60,);send
    while (inb(0x60)!=0xFA){inb(0x60);}
    /*
    while (inb(0x64)&0x2){}
    outb(0x60,0xAD);
    while (inb(0x64)&0x2){}
    outb(0x60,0xAE);*/
    while (inb(0x64)&0x2){}
    outb(0x60,0xED);
    inb(0x60);
    while (!inb(0x64)&0x1){}
    inb(0x60);
    outb(0x60,1);
    while (!inb(0x64)&0x1){}
};


void keyboard_handler__(regs* reg){

    u8 scancode = inb(0x60);

    printc("\n\n\nkey press ",10);
    printc(Int_To_STR(scancode,10),10);
    printc("\n\n\n",10);
    return;
}
void keybrard_init(){
    install_irq(1,keyboard_handler,0);
    u8 emp = inb(0x60);
    return;
}
 
