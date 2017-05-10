#ifndef __OTHER_H
#define __OTHER_H

#include "stm32f10x.h"
#include "bsp_usart1.h"


void UploadData(void);
u8 DealCAP(u16 LowLevel,u16 HighLevel);
void PwrCarrier_Deal(u8 data);
#endif

