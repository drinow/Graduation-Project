#include "led.h"

void LED_STATE_Config(void)
{
	GPIO_InitTypeDef 			 GPIO_InitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);      
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);      
  
  LED8_OFF;
  LED9_OFF;
}

void LED_Flash(void)
{
  if(LocalFire)
  {
    if(wave&0x04)
    {  LED8_ON;}
    else 
    {  LED8_OFF;}
  }
  else
  {
    if(wave&0x08)
    {  LED8_ON;}
    else 
    {  LED8_OFF;}
  }
}
