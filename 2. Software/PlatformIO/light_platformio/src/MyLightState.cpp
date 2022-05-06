#include "MyLightState.h"

//当前时间 
mytime NowTime;  

struct MenuChange Menu[] = {
    {0 , "APPSwitchON"  , "通过APP首页按键打开",},
    {1 , "APPSwitchOFF" , "通过APP首页按键关闭"},
    {2 , "APPButtonON"  , "通过APP开关按键打开",},
    {3 , "APPButtonOFF" , "通过APP开关按键关闭",},
    {4 , "XIAOMIAION"   , "通过小爱同学打开",},
    {5 , "XIAOMIAIOFF"  , "通过小爱同学关闭",},
    {6 , "WallSwitchOFF" , "通过墙壁开关关闭",},
    {7 , "WallSwitchON", "通过墙壁开关打开",},
};

MyLightState::MyLightState()
{
    this->WallSwitchState = digitalRead(KEY);  // 获取墙壁开关此时的状态
    this->LedState = false;     // 灯默认为关闭
    this->SignalStrength = 0.0; // 信号强度
    this->RunTime = 0;   // 系统运行时间 单位s
    this->LastGetDataTime = 0; // 上次获取数据时间 单位ms
    //储存灯状态发生改变的原因，指定大小为4
    this->LightStateChange.resize(4, -1);
    //储存用于显示 灯状态发生改变原因的 文字，只存4次
    this->LightStateChangeWord.resize(4, "**********");
}

// 开/关灯函数
// 参数：
//       ON_OFF   ：  开 ||  关灯
//       ChangeReason  ：改变的原因
void MyLightState::OpenCloseLight(bool ON_OFF, uint8_t ChangeReason)
{
    if(ON_OFF != this->LedState) // 如果灯当前的状态与需要切换的状态不一致
    {
        digitalWrite(relay, ON_OFF);  // 开/关灯
        this->LedState = ON_OFF;      // 切换灯的状态

        NowTime.GetNowTime();

        String ChangeArray;
        char SprintfChar[30];
        sprintf(SprintfChar,"%d年%0d月%0d日 %02d:%02d:%02d ",
                NowTime.My_year, NowTime.My_month, NowTime.My_mday, 
                NowTime.My_hour, NowTime.My_min, NowTime.My_sec);
        ChangeArray = SprintfChar;
        ChangeArray += Menu[ChangeReason].name;
        // 储存灯状态发生改变的原因
        //尾删
        this->LightStateChange.pop_back();
        this->LightStateChangeWord.pop_back();
        //在头部插入当前状态发生改变的原因
        this->LightStateChange.insert(LightStateChange.begin(), ChangeReason);
        this->LightStateChangeWord.insert(LightStateChangeWord.begin(), ChangeArray);
        heartbeat();                // 更新app显示
    }
}


// 墙壁开关的控制函数
void MyLightState::WallSwitchControl()
{
    if (digitalRead(KEY) != this->WallSwitchState) //如果墙壁开关当前状态发生改变
    {
        delay(100); //延时消抖
        if (digitalRead(KEY) != this->WallSwitchState)
        {
            this->WallSwitchState = digitalRead(KEY); //记录墙壁开关当前的状态

            this->OpenCloseLight(!(this->LedState), (!(this->LedState)) + 6);
        }
    }
}

