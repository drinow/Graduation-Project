/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   systick系统定时器
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 ISO-MINI STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f10x.h"
#include "bsp_SysTick.h"
#include "bsp_led.h"
#include "bsp_usart1.h"
#include "bsp_ds18b20.h"
#include "bsp_adc.h"
#include "bsp_can.h"
#include "tim.h"
#include "other.h"
#include "bsp_pwrcarrier.h"

u8 DetectorID=0xD0;
u8 Temperature=0;
u8 CAP_Detected=0;//烟雾报警
u8 Temp_Detected=0;//温度报警
u8 Token=0;

extern __IO uint16_t ADC_ConvertedValue;
extern u32 __IO tick;
extern u8  __IO Trig;
extern u32 __IO msec;
extern u8  __IO HalfSecWave;
extern u8  __IO QuarterWave;
void NVIC_Configuration(void);

void ReadID(void)
{
  u8 temp;
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 									   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);	

  temp=(GPIO_ReadInputData(GPIOB)&0xF0)>>4;
  DetectorID=DetectorID+temp;
}

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{	
  USART1_Config();	
  NVIC_Configuration();
	/* 配置SysTick 为10us中断一次 */
	SysTick_Init();
	LED_GPIO_Config();

  PWRCarrier_USART2_Config();
	ADC1_Init();
  DS18B20_Init();
  CAN_Config();
  TIM3_Cap_Init(0XFFFF,72-1);
  BeepInit();
  ReadID();
  
  LED1_ON;
  tick=0;
  printf("START\r\n");
	for(;;)
	{
    if(Trig)//100ms
    {
      Temperature  = (u8)DS18B20_Get_Temp();
      CAP_Detected = DealCAP(450,550);
      if(Temperature>=40)Temp_Detected=1; else Temp_Detected=0;//温度检查
      if(Token==DetectorID)
      {
        UploadData();
        Token=0;
      }
        
      Trig=0;
      
      #ifdef DEBUG
      printf("Temp:%2d CAP:%2d \r\n",Temperature,CAP_Detected);
      #endif
    }
    
    if(Temp_Detected+CAP_Detected)
    {
      if(QuarterWave)
      { 
        BeepOn();
        LED1_ON; 
      }
      else
      { 
        LED1_OFF;
        BeepOff();
      }
    }
    else
    {
      if(HalfSecWave)
      { 
        LED1_ON; 
      }
      else
      { 
        LED1_OFF;
        BeepOff();
      }
    }
    
//		Delay_ms(500);		/* 1s 读取一次温度值 */
	}    
}

/*******************************************************************************
* 函数名  : NVIC_Configuration
* 描述    : 配置中断向量表
* 输入    : None
* 输出    : None
* 返回    : None
* 注意		: None
*******************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* Enable CAN1 RX0 interrupt IRQ channel */
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
  
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*********************************************END OF FILE**********************/
