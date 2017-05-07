#ifndef __TIM_H
#define __TIM_H

#include "stm32f10x.h"

//#include "sys.h" 
//通过改变 TIM3->CCR2 的值来改变占空比，从而控制 LED0 的亮度

void TIM3_Cap_Init(u16 arr,u16 psc);

#endif
