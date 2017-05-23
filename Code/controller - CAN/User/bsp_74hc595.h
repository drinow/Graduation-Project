#ifndef __BSP_74HC595_H
#define __BSP_74HC595_H

#include "stm32f10x.h"

// 74HC595
#define HC595_PORT  GPIOB
#define HC595_RCC   RCC_APB2Periph_GPIOB
#define HC595_SCLK  GPIO_Pin_7    // shift register clock input, pin11
#define HC595_RCLK  GPIO_Pin_8     // storage register clock input, pin12
#define HC595_OE    GPIO_Pin_9     // output enable,active low,pin13
#define HC595_DATA  GPIO_Pin_10    // serial data input, pin14

//其他引脚连接
//Reset(PIN10)=VCC
//SQh(PIN9)(级联输出端)=下一个595的输入端

void hc595_init(void);
void hc595_input(uint8_t byte);
void hc595_latch(void);
void hc595_WriteByte(u8 data,u8 data2);
#endif
