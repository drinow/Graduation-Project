#ifndef _HMC5883L_H_
#define _HMC5883L_H_
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "bsp_usart1.h"
#include "bsp_SysTick.h"
#include "stdio.h"
#include  <math.h> 

#include "bsp_i2c_gpio.h"


/*宏定义------------------------------------------------------------------*/
#define ABS(x) ((x)>=0?(x):(-(x)))

#define	HMC5883L_Addr   				 0x3C	//磁场传感器器件地址   
#define HMC5883L_ConfigurationRegisterA  0x00
#define HMC5883L_ConfigurationRegisterB  0x01
#define HMC5883L_ModeRegister            0x02
#define HMC5883L_Output_X_MSB            0x03
#define HMC5883L_Output_X_LSB 			 0x04
#define HMC5883L_Output_Z_MSB            0x05
#define HMC5883L_Output_Z_LSB 			 0x06
#define HMC5883L_Output_Y_MSB            0x07
#define HMC5883L_Output_Y_LSB 			 0x08
#define HMC5883L_StatusRegister			 0x09
#define HMC5883L_ID_A					 0x0A
#define HMC5883L_ID_B 					 0x0B
#define HMC5883L_ID_C 					 0x0C

#define HMC5883L_OFFSET_X   (9)
#define HMC5883L_OFFSET_Y   (149)

#define CalThreshold 0 

void Init_HMC5883L(void);//读取
void Identify_HMC5883L(void);
void Read_HMC5883L(void);//读取
void HMC5883L_Carlibrate(void);//读取

#endif
