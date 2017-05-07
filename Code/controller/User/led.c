#include "led.h"

void LED_STATE_Config(void)
{
	GPIO_InitTypeDef 			 GPIO_InitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);      
  
  LED8_OFF;
  LED9_OFF;
}

void LED_Flash(void)
{
  if(wave&0x08)
  {  LED8_ON;}
  else 
  {  LED8_OFF;}
}
