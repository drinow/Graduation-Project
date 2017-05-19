#ifndef __OTHER_H
#define __OTHER_H

#include "stm32f10x.h"
#include "bsp_usart1.h"
#include "bsp_can.h"

#define BeepOn()  GPIO_SetBits(GPIOB, GPIO_Pin_9);
#define BeepOff() GPIO_ResetBits(GPIOB, GPIO_Pin_9);

void UploadData(void);
u8 DealCAP(u16 LowLevel,u16 HighLevel);
void PwrCarrier_Deal(u8 data);
void BeepInit(void);
#endif

