#ifndef main_H
#define main_H
#include "MyLightState.h"
#include "MyTime.h"
#include <vector>

// 设置各引脚别名
#define relay 10 // 定义继电器输入引脚为GPIO/10
#define KEY   5  // 定义按键引脚


void heartbeat(); //用户自定义状态返回的心跳包回调函数

#endif