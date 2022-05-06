#ifndef MyLightState_H
#define MyLightState_H
#include <Arduino.h>
//#include <WString.h>
#include "main.h"
#include "stdio.h"
#include "MyTime.h"

#define ONLight  1
#define OFFLight 0 

#define APPSwitchON   0
#define APPSwitchOFF  1
#define APPButtonON   2
#define APPButtonOFF  3
#define XIAOMIAION    4
#define XIAOMIAIOFF   5
#define WallSwitchOFF 6
#define WallSwitchON  7

// 储存用于显示开关状态改变原因的文字
struct MenuChange
{
    uint8_t id;
    char* DefineId;
    char* name; 
};

//灯状态对象
class MyLightState
{
public:
    // 默认构造
    MyLightState();

    // 开关灯函数
    void OpenCloseLight(bool ON_OFF, uint8_t ChangeReason);
    
    // 墙壁开关的控制函数
    void WallSwitchControl();
    
    //构造函数中赋初值
    // 存储墙壁开关状态的变量
    bool WallSwitchState; 
    // 存储灯状态的变量
    bool LedState;   
    // 信号强度
    float SignalStrength; 
    // 系统运行时间 单位s
    time_t RunTime; 
    // 上次获取数据时间 单位ms
    time_t LastGetDataTime;
    //储存灯状态发生改变的原因，只存4次
    std::vector<int> LightStateChange;  
    //储存用于显示 灯状态发生改变原因的 文字，只存4次
    std::vector<String> LightStateChangeWord;  
}; 


#endif