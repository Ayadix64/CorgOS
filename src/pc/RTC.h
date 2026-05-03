#ifndef RTC_H
#define RTC_H
void get_time(u32& hour , u32& min  , u32& s   );
void get_date(u32& day  , u32& month, u32& year);

void set_time(u32 hour , u32 min  , u32 s   );
void set_date(u32 day  , u32 month, u32 year);
#endif