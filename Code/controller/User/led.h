#ifndef __LED_H
#define __LED_H

#include "including.h"

#define LED8_ON GPIO_ResetBits(GPIOB,GPIO_Pin_8);
#define LED8_OFF GPIO_SetBits(GPIOB,GPIO_Pin_8);
#define LED9_ON GPIO_ResetBits(GPIOB,GPIO_Pin_9);
#define LED9_OFF GPIO_SetBits(GPIOB,GPIO_Pin_9);

void LED_STATE_Config(void);
void LED_Flash(void);

#endif

