#ifndef MyTime_H
#define MyTime_H

#include <stdint.h>

class mytime
{
public:
    //构造
    mytime();

    //获取当前时间
    bool GetNowTime();

    //负载运算符重载
    mytime& operator=(mytime &time);

    int8_t  My_sec;    //秒
    int8_t  My_min;    //分
    int8_t  My_hour;   //时
    int8_t  My_wday;   //周几 0-6(依次为周日/一/二/三/四/五/六)
    int8_t  My_mday;   //当月第几天  1-31
    int16_t My_yday;   //当年第几天  1-366
    int8_t  My_month;  //月份
    int16_t My_year;   //年份
    bool is_ok;        //获取时间是否成功
};


#endif