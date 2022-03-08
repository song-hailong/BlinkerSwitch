#define BLINKER_WIFI
#define BLINKER_MIOT_LIGHT//支持小爱同学
#define BLINKER_OTA_VERSION_CODE "3.0.0" //版本号
#include <Arduino.h>
#include <Blinker.h>
#include "main.h"


char auth[] = "xxxxxxxxxxxxxx";//app中获取到的Secret Key(密钥)
char ssid[] = "xxxxxxxxxxxxxx";
char pswd[] = "xxxxxxxxxxxxxx";


MyLightState LightState; //灯的状态


// 新建组件对象
#define BUTTON_Light (char*)"btn-light"
BlinkerButton ButtonLight(BUTTON_Light);                 // 按键组件：开/关灯
BlinkerText TextLightState((char*)"tex-state");          // 文字组件：显示灯的状态
BlinkerText TextBlinkerTime((char*)"tex-BlinkerTime");   // 文字组件：显示系统运行时间
BlinkerNumber NumberSignal((char*)"num-signal");         // 数据组件：显示WiFi信号强度
BlinkerText TextStateChange((char*)"tex-change");        // 文字组件：显示灯状态改变原因
BlinkerText TextStateChange1((char*)"tex-change1");      // 文字组件：显示灯状态改变原因1
BlinkerText TextStateChange2((char*)"tex-change2");      // 文字组件：显示灯状态改变原因2
BlinkerText TextStateChange3((char*)"tex-change3");      // 文字组件：显示灯状态改变原因3
BlinkerText TextStateChange4((char*)"tex-change4");      // 文字组件：显示灯状态改变原因4

// 首页开关的回调函数, 当收到指令时会调用该回调函数
void switch_callback(const String & state)
{
  BLINKER_LOG("get switch state: ", state);

  if (state == BLINKER_CMD_ON)
  {
    LightState.OpenCloseLight(ONLight, APPSwitchON); // 开灯
  }
  else
  {
    LightState.OpenCloseLight(OFFLight, APPSwitchOFF); // 关灯
  }
}

// 开关灯按键回调函数，当收到指令时会调用该回调函数
void BUTTONLight_callback(const String & state)
{
  BLINKER_LOG("get button state: ", state);

  if (state == BLINKER_CMD_ON)
  {
    LightState.OpenCloseLight(ONLight, APPButtonON); // 开灯
  }
  else
  {
    LightState.OpenCloseLight(OFFLight, APPButtonOFF); // 关灯
  }
  Blinker.vibrate();//发送手机振动指令
}

//小爱电源类操作的回调函数:
//当小爱同学向设备发起控制, 设备端需要有对应控制处理函数
void miotPowerState(const String & state)
{
  BLINKER_LOG("need set power state: ", state);
  if (state == BLINKER_CMD_ON)
  {
    BlinkerMIOT.powerState("on"); //给小爱同学反馈控制状态：已打开
    BlinkerMIOT.print();

    LightState.OpenCloseLight(ONLight, XIAOMIAION); // 开灯
  }
  else if (state == BLINKER_CMD_OFF)
  {
    BlinkerMIOT.powerState("off"); //给小爱同学反馈控制状态：已关闭
    BlinkerMIOT.print();

    LightState.OpenCloseLight(OFFLight, XIAOMIAIOFF); // 关灯
  }
}
// 用户自定义设备查询的回调函数:
void miotQuery(int32_t queryCode)
{
    BLINKER_LOG("MIOT Query codes: ", queryCode);

    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_ALL_NUMBER :
            BLINKER_LOG("MIOT Query All");
            BlinkerMIOT.powerState(LightState.LedState ? "on" : "off");
            BlinkerMIOT.print();
            break;
        case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
            BLINKER_LOG("MIOT Query Power State");
            BlinkerMIOT.powerState(LightState.LedState ? "on" : "off");
            BlinkerMIOT.print();
            break;
        default :
            BlinkerMIOT.powerState(LightState.LedState ? "on" : "off");
            BlinkerMIOT.print();
            break;
    }
}

// 如果未绑定的组件被触发，则会执行其中内容
void dataRead(const String & data)
{
  BLINKER_LOG("Blinker readString: ", data);
  Blinker.vibrate();  //发送手机振动指令
}

// 用户自定义状态返回的心跳包回调函数
void heartbeat()
{
  BUILTIN_SWITCH.print(LightState.LedState ? "on" : "off"); //开/关组件
  //灯泡  <i class="far fa-lightbulb"></i>
  //灯泡亮<i class="far fa-lightbulb-on"></i>
  ButtonLight.icon(LightState.LedState ? "far fa-lightbulb-on" : "far fa-lightbulb"); //设置图标
  ButtonLight.color(LightState.LedState ? "Blue" : "Black");  //设置颜色
  ButtonLight.print(LightState.LedState ? "on" : "off"); // 按键组件
  //天花灯 <i class="fad fa-light-ceiling"></i>
  TextLightState.icon("fad fa-light-ceiling");
  TextLightState.color(LightState.LedState ? "Blue" : "Black");
  TextLightState.print(LightState.LedState ? "灯状态: 开启" : "灯状态: 关闭");

  char SprintfChar[25];
  sprintf(SprintfChar, "%lld天%lld时%lld分", 
          LightState.RunTime/3600/24,LightState.RunTime/3600%24,LightState.RunTime/60%60);
  //卡车时钟图标 <i class="far fa-shipping-timed"></i>
  TextBlinkerTime.icon("far fa-shipping-timed");       
  TextBlinkerTime.print(SprintfChar/*,"系统运行时间"*/);

  //无信号  <i class="fad fa-wifi-slash"></i>
  //1格信号 <i class="fad fa-wifi-1"></i>
  //2格信号 <i class="fad fa-wifi-2"></i>
  //3格信号 <i class="fad fa-wifi"></i>
  NumberSignal.icon("fad fa-wifi");
  NumberSignal.color("Green");  //LimeGreen
  //NumberSignal.unit("Dbm");
  NumberSignal.print(LightState.SignalStrength);//信号强度

  //问号 <i class="far fa-question"></i>
  TextStateChange.icon("far fa-question");
  TextStateChange.color("MediumPurple");
  TextStateChange.print("灯泡状态改变的原因");
  //重复箭头 <i class="fad fa-repeat-alt"></i>
  //语音 <i class="fad fa-microphone"></i>
  //手机 <i class="far fa-mobile-android"></i>
  //开关 <i class="far fa-light-switch"></i>
  if(LightState.LightStateChange[0] == APPSwitchON || LightState.LightStateChange[0] == APPSwitchOFF || 
     LightState.LightStateChange[0] == APPButtonON || LightState.LightStateChange[0] == APPButtonOFF)
    TextStateChange1.icon("far fa-mobile-android");//手机
  else if(LightState.LightStateChange[0] == XIAOMIAION || LightState.LightStateChange[0] == XIAOMIAIOFF)
    TextStateChange1.icon("fad fa-microphone");//语音
  else if (LightState.LightStateChange[0] == WallSwitchON || LightState.LightStateChange[0] == WallSwitchOFF)
    TextStateChange1.icon("far fa-light-switch");//开关
  else
    TextStateChange1.icon("fad fa-repeat-alt");//重复箭头
  //TextStateChange1.color("Purple");
  TextStateChange1.print(LightState.LightStateChangeWord[0]);


  if(LightState.LightStateChange[1] == APPSwitchON || LightState.LightStateChange[1] == APPSwitchOFF || 
     LightState.LightStateChange[1] == APPButtonON || LightState.LightStateChange[1] == APPButtonOFF)
    TextStateChange2.icon("far fa-mobile-android");//手机
  else if(LightState.LightStateChange[1] == XIAOMIAION || LightState.LightStateChange[1] == XIAOMIAIOFF)
    TextStateChange2.icon("fad fa-microphone");//语音
  else if (LightState.LightStateChange[1] == WallSwitchON || LightState.LightStateChange[1] == WallSwitchOFF)
    TextStateChange2.icon("far fa-light-switch");//开关
  else
    TextStateChange2.icon("fad fa-repeat-alt");//重复箭头
  //TextStateChange2.color("Purple");
  TextStateChange2.print(LightState.LightStateChangeWord[1]);


  if(LightState.LightStateChange[2] == APPSwitchON || LightState.LightStateChange[2] == APPSwitchOFF || 
     LightState.LightStateChange[2] == APPButtonON || LightState.LightStateChange[2] == APPButtonOFF)
    TextStateChange3.icon("far fa-mobile-android");//手机
  else if(LightState.LightStateChange[2] == XIAOMIAION || LightState.LightStateChange[2] == XIAOMIAIOFF)
    TextStateChange3.icon("fad fa-microphone");//语音
  else if (LightState.LightStateChange[2] == WallSwitchON || LightState.LightStateChange[2] == WallSwitchOFF)
    TextStateChange3.icon("far fa-light-switch");//开关
  else
    TextStateChange3.icon("fad fa-repeat-alt");//重复箭头
  //TextStateChange3.color("Purple");
  TextStateChange3.print(LightState.LightStateChangeWord[2]);


  if(LightState.LightStateChange[3] == APPSwitchON || LightState.LightStateChange[3] == APPSwitchOFF || 
     LightState.LightStateChange[3] == APPButtonON || LightState.LightStateChange[3] == APPButtonOFF)
    TextStateChange4.icon("far fa-mobile-android");//手机
  else if(LightState.LightStateChange[3] == XIAOMIAION || LightState.LightStateChange[3] == XIAOMIAIOFF)
    TextStateChange4.icon("fad fa-microphone");//语音
  else if (LightState.LightStateChange[3] == WallSwitchON || LightState.LightStateChange[3] == WallSwitchOFF)
    TextStateChange4.icon("far fa-light-switch");//开关
  else
    TextStateChange4.icon("fad fa-repeat-alt");//重复箭头
  //TextStateChange4.color("Purple");
  TextStateChange4.print(LightState.LightStateChangeWord[3]);

  BLINKER_LOG("heartbeat!");
}


void setup()
{
  // 初始化串口，并开启调试信息，调试用可以删除
  Serial.begin(115200);
  BLINKER_DEBUG.stream(Serial);
  // 初始化IO
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
  pinMode(KEY, INPUT);
  digitalWrite(KEY, HIGH);
  LightState.WallSwitchState = digitalRead(KEY);  // 获取墙壁开关此时的状态

  // 初始化blinker
  Blinker.begin(auth, ssid, pswd);
  Blinker.attachData(dataRead); //注册回调函数，当有设备收到APP发来的数据时会调用对应的回调函数
  Blinker.attachHeartbeat(heartbeat);//注册心跳包回调函数
  BUILTIN_SWITCH.attach(switch_callback); //设置首页开关的回调函数, 当收到指令时会调用该回调函数
  ButtonLight.attach(BUTTONLight_callback);//注册开关灯按键的回调函数, 当收到指令时会调用该回调函数

  //给APP发送当前的状态
  heartbeat();

  //小爱同学务必在回调函数中反馈该控制状态
  BlinkerMIOT.attachPowerState(miotPowerState);//注册小爱电源类的操作接口回调函数
  BlinkerMIOT.attachQuery(miotQuery);  //注册小爱设备查询接口回调函数
}

void loop()
{
  Blinker.run();

  //墙壁开关的控制
  LightState.WallSwitchControl();

  if(millis() - LightState.LastGetDataTime >= 500) //距上次获取数据已经过去0.5s
  {
    LightState.LastGetDataTime = millis();   // 更新获取数据时间，单位为毫秒（ms）
    LightState.RunTime = LightState.LastGetDataTime/1000;  // 获取设备运行时间, 单位为秒(s)
    LightState.SignalStrength = WiFi.RSSI(); // WIFI信号强度
  }
}


//获取当前时间
bool mytime::GetNowTime()
{
  int8_t  sec = Blinker.second();  //秒
  int8_t  min = Blinker.minute();  //分
  int8_t  hour = Blinker.hour();   //时
  int8_t  wday = Blinker.wday();   //周几 0-6(依次为周日/一/二/三/四/五/六)
  int8_t  mday = Blinker.mday();   //当月第几天  1-31
  int16_t yday = Blinker.yday();   //当年第几天  1-366
  int8_t  month = Blinker.month(); //月份
  int16_t year = Blinker.year();   //年份

  if( sec != -1 && min != -1 && hour != -1 && wday != -1 && mday != -1 && yday != -1 && month != -1 && year != -1)
  {
    this->My_sec = sec;
    this->My_min = min;
    this->My_hour = hour;
    this->My_wday = wday;
    this->My_mday = mday;
    this->My_yday = yday;
    this->My_month = month;
    this->My_year = year;

    this->is_ok = true;
  }
  else
    this->is_ok = false;

  return this->is_ok;
}