# -blinker-ESP8266---
[点灯科技 blinker]通过ESP8266接入语音助手-智能墙壁开关

视频教程：https://www.bilibili.com/video/BV1dz4y1f7gt

打包好的软件包阿里云链接：https://www.aliyundrive.com/s/KLycuo3tpkM

前提：家里的墙壁开关里必须同时有零线和火线。

## 一：前期准备

1.  安装Arduino IDE 1.8.19或更新版本（目前最新版本为1.8.19）
    软件链接 https://www.arduino.cn/thread-5838-1-1.html （来自Arduino论坛）
    				 https://www.arduino.cc/en/software （来自官网）

2.  Arduino IDE安装esp8266扩展
    目前在线安装有点问题，推荐离线安装。下载链接：https://www.arduino.cn/thread-76029-1-1.html（来自Arduino论坛），目前最新版本为3.0.2。
    安装好之后，打开Arduino IDE，打开【工具】，开发板选择就可以选择ESP8266了。

3. 下载blinkerArduino库
下载链接：https://github.com/blinker-iot/blinker-library/archive/dev_3.0.zip

将下载好的blinker库解压到 我的电脑>文档>Arduino>libraries 文件夹中

4. 下载并安装blinker APP
    下载链接：https://github.com/blinker-iot/app-release/releases
    4.1 在app中添加设备，获取Secret Key
           进入App，点击右上角的“+”号，然后选择添加设备，点击选择Arduino -> WiFi接入-> 选择要接入的服务商 -> 复制申请到的Secret Key
    4.2 DIY界面
           在设备列表页，点击刚才新建的设备图标，进入设备控制面板。首次进入设备控制面板，会弹出向导页，在向导页点击载入示例，即可载入示例组件。点击右上角的修改，选择开关的按钮，样式选择中间那种，按键类型选择：开关按键，再点击有上角的保存。之后我们就可以通过这个按键来控制并查看灯。

5. 硬件

      ​       我是使用的自己绘制的电路板，学习阶段可以使用Node-MCU，真正接入到墙壁里还是需要自己绘制板子，不然板子太大放不进去。

## 二：程序下载

​      将程序烧入即可。

