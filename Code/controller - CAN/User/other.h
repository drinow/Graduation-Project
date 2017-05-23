#ifndef __OTHER_H
#define __OTHER_H

#include "including.h"

#define RUN31G 0x9B
#define RUN31R 0x64
#define RUN32G 0xFB
#define RUN32R 0x04
#define RUN33G 0xDB
#define RUN33R 0x24

#define RUN21G 0x9B
#define RUN21R 0x64
#define RUN22G 0xFB
#define RUN22R 0x04
#define RUN23G 0xDB
#define RUN23R 0x24

#define RUN11G 0x02
#define RUN11R 0x80
#define RUN12G 0x02
#define RUN12R 0x80
#define RUN13G 0x02
#define RUN13R 0x80

void TCPS_Config(void);

void TCP_KeepAlive(void);
void TCP_DealData(u8 *str,u16 len);
void TCP_SendFire(u8 detector,u8 controller);
void TCP_SendDetector1F(void);
void TCP_SendDetector2F(void);
void TCP_SendDetector3F(void);

void DealCAN(CanRxMsg* RxMessage);
void DealActuator(void);
void DealAlarm(void);

void CAN_Broadcast(void);

void PwrCarrier_Deal(u8 data);
void PwrTokenCtrl(void);

void DealDoor(void);
void CheckFire(void);
void CAN_TokenCtl(void);
#endif
