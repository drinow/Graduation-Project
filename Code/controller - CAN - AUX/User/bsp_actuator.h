#ifndef __BSP_ACTUATOR_H
#define __BSP_ACTUATOR_H

#include "including.h"

#define  RUN   1
#define  STOP  0

#define  NUM1   1
#define  NUM2   2

#define  FRD   1  //正转forward
#define  REV   2  //反转reverse

void ActuatorInit(void);

void PwrCutCtrl(u8 state);
void AlarmCtrl(u8 state);
void WaterPumpCtrl(u8 state);
void StepMotorCtrl(u8 order,u8 direction,u32 period);
void FanCtrl(u8 state);

#endif

