#ifndef __OTHER_H
#define __OTHER_H

#include "including.h"

void TCPS_Config(void);

void TCP_KeepAlive(void);
void TCP_DealData(u8 *str,u16 len);
void TCP_SendFire(u8 detector,u8 controller);
void TCP_SendDetector1F(void);
void TCP_SendDetector2F(void);
void TCP_SendDetector3F(void);

void DealCAN(CanRxMsg* RxMessage);
void DealActuator(void);


void CAN_Broadcast(void);

void PwrCarrier_Deal(u8 data);
void PwrTokenCtrl(void);
#endif
