#ifndef __LED_H
#define __LED_H

#include "including.h"

#define LED8_OFF GPIO_ResetBits(GPIOB,GPIO_Pin_11);
#define LED8_ON GPIO_SetBits(GPIOB,GPIO_Pin_11);
#define LED9_OFF GPIO_ResetBits(GPIOC,GPIO_Pin_11);
#define LED9_ON GPIO_SetBits(GPIOC,GPIO_Pin_11);

void LED_STATE_Config(void);
void LED_Flash(void);

#endif

