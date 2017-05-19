#ifndef __CAN_H
#define __CAN_H

#include "stm32f10x.h"
#include "stdio.h"

void CAN_Config(void);
void CAN_Send(u8 *data,u8 can_id);
void CAN_Deal(u8 array[],u8 id);
#endif
