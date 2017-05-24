#ifndef __INCLUDING_H
#define __INCLUDING_H

#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"
#include "other.h"
#include "bsp_actuator.h"
#include "bsp_usart1.h"
#include "can.h"
#include "ds3231.h"
#include "bsp_SysTick.h"
#include "led.h"
#include "screen.h"
#include "w5500.h"
#include "W5500_conf.h"
#include "socket.h"
#include "utility.h"
#include "tcp_demo.h"
#include "dhcp.h"
#include "queue.h"
#include "bsp_74hc595.h"

typedef struct
{
//  u8 ID[4];//探测器编号
  u8 type;//bit1=烟感，bit0=温感，type！=0代表起火
  u8 temp;//温度
//  u8 Anum;//探测器数量;
}DetectorMsg;

typedef struct
{
  u8 ID;//BCD,
  u8 State;
}ActuatorMsg;//驱动器

typedef struct
{
  u8 Detector;
  u8 Controller;
}FiredMsg;//驱动器

extern volatile u32 tick;
extern TIME_TypeDef GetTime,SetTime;
extern volatile u8 wave;
extern volatile u8 pulse;
extern u8 CANFlag;
extern u8 GetCAN[8];
extern uint32_t CAN_ID;//标识符
extern u8 SecAlarm;
extern u8 Ctrl_ID;
extern u8 tempflag;
extern u8 Token;
extern DetectorMsg Detector_1F[4],Detector_2F[4],Detector_3F[4];
extern ActuatorMsg Fan;
extern ActuatorMsg Door;
extern ActuatorMsg Pump;
extern ActuatorMsg Alarm;
extern u32 FiredNum;
extern u8 FiredSended;
extern u8 Fired[9];
extern u8 RestFire;
extern u8 LocalFire;
extern u8 Reset;
extern u8 motorEnd;
extern u8 Monitor;
extern u8 OnService;
#endif

