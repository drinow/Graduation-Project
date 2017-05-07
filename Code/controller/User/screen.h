#ifndef __SCREEN_H
#define __SCREEN_H

#include "including.h"

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

void Screen_Config(void);
void SC_SendTime(void);
void SC_SendIPAddr(void);
void SC_SendPort(void);
void SC_SendDHCP(void);
void SC_SendID(void);

#endif
