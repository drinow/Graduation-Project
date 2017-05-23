#ifndef __SCREEN_H
#define __SCREEN_H

#include "including.h"

#define SC_ESCAPE 1

//typedef enum {SC_TIME = 0,SC_IPADDRESS,SC_PORT,SC_DHCP,SC_TCP } SC_Type;
//typedef enum {SC_WREG = 0X80,SC_RREG,SC_WDAT,SC_RDAT,SC_WDRA } SC_Mode;

#define SC_WREG 0x80
#define SC_RREG 0x81
#define SC_WDAT 0x82
#define SC_RDAT 0x83

#define SC_TIME     0x0000
#define SC_FIRE     0x0018
#define SC_ID       0x0080
#define SC_ADDRESS  0x0100
#define SC_PORT     0x0110
#define SC_DHCP     0x0120
#define SC_TCP      0x0130
#define SC_CONTROLLER      0x0140

#define SC_DETECTOR11 0x0210
#define SC_DETECTOR12 0x0212
#define SC_DETECTOR13 0x0214
#define SC_DETECTOR21 0x0220
#define SC_DETECTOR22 0x0222
#define SC_DETECTOR23 0x0224
#define SC_DETECTOR31 0x0230
#define SC_DETECTOR32 0x0232
#define SC_DETECTOR33 0x0234

#define SC_1FWAY 0x0240
#define SC_2FWAY 0x0242
#define SC_3FWAY 0x0244

#define ICON_RED 1
#define ICON_GREEN 2
#define ICON_RUN_CLR 3
#define ICON_RUN11 0
#define ICON_RUN12 1
#define ICON_RUN13 2
#define ICON_RUN21 0
#define ICON_RUN22 1
#define ICON_RUN23 2
#define ICON_RUN31 0
#define ICON_RUN32 1
#define ICON_RUN33 2

void Screen_Config(void);
void SC_SendTime(void);
void SC_SendIPAddr(void);
void SC_SendPort(void);
void SC_SendDHCP(void);
void SC_SendID(void);
void SC_SendFirePoint(void);
void SC_SendRUNICON(u8 num,u8 value);
void SC_SendFirelog(void);
void SC_SendClrFireLog(void);
#endif
