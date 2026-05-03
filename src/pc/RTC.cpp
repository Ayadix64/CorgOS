#include "../utils/util.h"

#define CMOS_COMMAND_PORT 0x70
#define CMOS_DATA_PORT 0x71

#define BCD(x) (x & 0x0f)+(x>>4)*10 //Binary coded decimal  to  decimal , becaus cmos giv us BCD
#define toBCD(x) ((x/10)<<4)|(x%10)

u8 rtc_updating(){
    outb(CMOS_COMMAND_PORT,0xA);//rtc situationn command
    return inb(CMOS_DATA_PORT)&0x80;//secend bit talls if rtc is updating it time 
}

u8 cmos_io(u8 in){
    outb(CMOS_COMMAND_PORT,in);
    return inb(CMOS_DATA_PORT);
}
void setcmos(u8 in,u8 data){
    outb(CMOS_COMMAND_PORT,in);
    outb(CMOS_DATA_PORT,data);
}

static u32 h = 0 , m = 0 , s = 0 , da = 0 , mo = 0 , y = 0 ;
void get_time(u32& hour , u32& min  , u32& sec   ){
    if(rtc_updating()){//we don't wont break the time updating
        hour=h;
        min=m;
        sec=s;
        return;
    }
    sec = BCD(cmos_io(0x0));
    min = BCD(cmos_io(0x2));
    hour= BCD(cmos_io(0x4));
    s=sec;
    m=min;
    h=hour;
    return;
}

void set_time(u32 hour , u32 min  , u32 sec   ){
    while(rtc_updating()){}

    setcmos(0x0,toBCD(sec));
    setcmos(0x2,toBCD(min));
    setcmos(0x4,toBCD(hour));
    s=sec;
    m=min;
    h=hour;
    return;
}

void set_date(u32 day , u32 month  , u32 year  ){
    while(rtc_updating()){}

    setcmos(0x7,toBCD(day));
    setcmos(0x8,toBCD(month));
    setcmos(0x9,toBCD(year));

    da=day;
    mo=month;
    y=year;
    return;
}



void get_date(u32& day  , u32& month, u32& year){
    if(rtc_updating()){
        day=da;
        month=mo;
        year=y;
        return;
    }
    day   = BCD(cmos_io(0x7));
    month = BCD(cmos_io(0x8));
    year  = BCD(cmos_io(0x9));
    //0x6 --> day (sunday,...)
    
    da=day;
    mo=month;
    y=year;
    
    return;
}