/*一楼客厅的灯泡*/
#define BLINKER_WIFI
#define BLINKER_MIOT_LIGHT//支持小爱同学
#include <Blinker.h>

char auth[] = "e350489ea1cf";//app中获取到的Secret Key(密钥)
char ssid[] = "song_second";//WIFI账号
char pswd[] = "20110722";//WIFI密码

// 设置各引脚别名
const int GPIO = 10; // 定义继电器输入引脚为GPIO/10
const int KEY = 5; // 定义按键引脚
// 变量定义
bool State = 0; // 存储墙壁开关状态的变量
bool led_state = false;// 存储灯状态的变量

// 新建组件对象
BlinkerButton Button1("btn-abc");//注意：要和APP组件’数据键名’一致
BlinkerText Text1("tex-30b");


void switch_callback(const String & state)
{
  BLINKER_LOG("get switch state: ", state);

  if (state == BLINKER_CMD_ON)
  {
    led_state = true;
    digitalWrite(GPIO, HIGH);
    BUILTIN_SWITCH.print("on"); //发送开关当前的状态(多用于反馈开关状态)到APP
    Button1.print("on");
    Text1.color(led_state ? "Blue" : "black");
    Text1.print(led_state ? "灯状态: 开启" : "灯状态: 关闭");
  }
  else
  {
    led_state = false;
    digitalWrite(GPIO, LOW);
    BUILTIN_SWITCH.print("off");
    Button1.print("off");
    Text1.color(led_state ? "Blue" : "black");
    Text1.print(led_state ? "灯状态: 开启" : "灯状态: 关闭");
  }
}

// 按下BlinkerAPP按键即会执行该函数
void button1_callback(const String & state)
{
  BLINKER_LOG("get button state: ", state);

  if (state == BLINKER_CMD_ON)
  {
    led_state = true;
    digitalWrite(GPIO, HIGH);
    BUILTIN_SWITCH.print("on"); //发送开关当前的状态(多用于反馈开关状态)到APP
    Button1.print("on");

    Text1.color(led_state ? "Blue" : "black");
    Text1.print(led_state ? "灯状态: 开启" : "灯状态: 关闭");
  }
  else
  {
    led_state = false;
    digitalWrite(GPIO, LOW);
    BUILTIN_SWITCH.print("off");
    Button1.print("off");

    Text1.color(led_state ? "Blue" : "black");
    Text1.print(led_state ? "灯状态: 开启" : "灯状态: 关闭");
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
    digitalWrite(GPIO, HIGH);
    BlinkerMIOT.powerState("on");
    BlinkerMIOT.print();

    led_state = true;
    Button1.print("on");
    BUILTIN_SWITCH.print("on");
    Text1.color(led_state ? "Blue" : "black");
    Text1.print(led_state ? "灯状态: 开启" : "灯状态: 关闭");
  }
  else if (state == BLINKER_CMD_OFF)
  {
    digitalWrite(GPIO, LOW);
    BlinkerMIOT.powerState("off");
    BlinkerMIOT.print();

    led_state = false;
    Button1.print("off");
    BUILTIN_SWITCH.print("off");
    Text1.color(led_state ? "Blue" : "black");
    Text1.print(led_state ? "灯状态: 开启" : "灯状态: 关闭");
  }
}

//当有设备收到APP发来的数据时会调用该函数
void dataRead(const String & data)
{
  BLINKER_LOG("Blinker readString: ", data);

  Blinker.vibrate();

  uint32_t BlinkerTime = millis();

  Blinker.print("millis", BlinkerTime);
}


/*
   在此函数中添加您的心跳消息详细信息

   当get heartbeat命令{“get”:“state”}时，设备将调用此函数
   例如，您可以打印回消息

   Every 30s will get a heartbeat command from app
*/
void heartbeat()
{
  BUILTIN_SWITCH.print(led_state ? "on" : "off");
  Button1.print(led_state ? "on" : "off");
  Text1.color(led_state ? "Blue" : "black");
  Text1.print(led_state ? "灯状态: 开启" : "灯状态: 关闭");

  BLINKER_LOG("heartbeat!");
}


void setup()
{
  // 初始化串口，并开启调试信息，调试用可以删除
  Serial.begin(115200);
  BLINKER_DEBUG.stream(Serial);
  // 初始化IO
  pinMode(GPIO, OUTPUT);
  digitalWrite(GPIO, LOW);
  pinMode(KEY, INPUT);
  digitalWrite(KEY, HIGH);

  State = digitalRead(KEY);
  led_state = false;


  // 初始化blinker
  Blinker.begin(auth, ssid, pswd);
  Blinker.attachData(dataRead); //注册回调函数，当有设备收到APP发来的数据时会调用对应的回调函数
  Blinker.attachHeartbeat(heartbeat);//注册心跳包回调函数
  BUILTIN_SWITCH.attach(switch_callback); //设置开关的回调函数, 当收到指令时会调用该回调函数
  Button1.attach(button1_callback);//注册按键的回调函数, 当收到指令时会调用该回调函数

  //给APP发送当前的状态
  BUILTIN_SWITCH.print(led_state ? "on" : "off");
  Button1.print(led_state ? "on" : "off");
  Text1.color(led_state ? "Blue" : "black");
  Text1.print(led_state ? "灯状态: 开启" : "灯状态: 关闭");

  //小爱同学务必在回调函数中反馈该控制状态
  BlinkerMIOT.attachPowerState(miotPowerState);//注册回调函数
}

void loop()
{
  Blinker.run();

  //墙壁开关的控制指令
  if (digitalRead(KEY) != State)
  {
    delay(100);
    if (digitalRead(KEY) != State)
    {
      State = digitalRead(KEY);
      digitalWrite(GPIO, !digitalRead(GPIO));
      led_state = !led_state;//改变存储灯状态的变量

      BUILTIN_SWITCH.print(led_state ? "on" : "off");
      Button1.print(led_state ? "on" : "off");
      Text1.color(led_state ? "Blue" : "black");
      Text1.print(led_state ? "灯状态: 开启" : "灯状态: 关闭");

      delay(200);
    }
  }
}
