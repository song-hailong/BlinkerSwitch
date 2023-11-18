#include "MyTime.h"

//构造
mytime::mytime()
{
    this->My_sec = 0;
    this->My_min = 0;
    this->My_hour = 0;
    this->My_wday = 0;
    this->My_mday = 0;
    this->My_yday = 0;
    this->My_month = 0;
    this->My_year = 0;
    this->is_ok = 0;
}

//赋值运算符重载
mytime& mytime::operator=(mytime &time)
{
    this->My_sec = time.My_sec;
    this->My_min = time.My_min;
    this->My_hour = time.My_hour;
    this->My_wday = time.My_wday;
    this->My_mday = time.My_mday;
    this->My_yday = time.My_yday;
    this->My_month = time.My_month;
    this->My_year = time.My_year;
    this->is_ok = time.is_ok;

    //返回自身
	return *this;
}

