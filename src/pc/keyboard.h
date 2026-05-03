#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "../utils/util.h"
#include "../idt/idt.h"
extern "C" u8 pkey;
extern "C" u8 key;
extern "C" bool up;
extern "C" bool down;
extern "C" bool left;
extern "C" bool reight;
extern "C" bool lshift;
extern "C" bool rshift;
extern "C" bool vrrshift;
extern "C" bool vrrnum;
extern "C" bool ESC;
extern "C" bool LCtrl;
extern "C" bool RCtrl;
extern "C" bool Alt;
extern "C" bool AltGr;
extern "C" bool GUIkey;
extern "C" bool RGUIkey;
extern "C" bool Inser;
extern "C" bool List;
extern "C" bool begine;
extern "C" bool nextpage;
extern "C" bool lastpage;
extern "C" bool End;
extern "C" bool Delet;
extern "C" bool F1;
extern "C" bool F2;
extern "C" bool F3;
extern "C" bool F4;
extern "C" bool F5;
extern "C" bool F6;
extern "C" bool F7;
extern "C" bool F8;
extern "C" bool F9;
extern "C" bool F10;
extern "C" bool F11;
extern "C" bool F12;

extern "C" bool KeyPress;
void keyboard_handler(regs* reg);
void keybrard_init();
#endif
