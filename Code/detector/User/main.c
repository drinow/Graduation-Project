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

u8 DetectID=0;
u8 gittest=1;
extern __IO uint16_t ADC_ConvertedValue;

void NVIC_Configuration(void);

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{	
	/* 配置SysTick 为1us中断一次 */
	SysTick_Init();
	NVIC_Configuration();
	LED_GPIO_Config();
	USART1_Config();	
	ADC1_Init();
  DS18B20_Init();
	CAN_Config();
  TIM3_Cap_Init(0XFFFF,72-1);
  
  LED1_ON;
	for(;;)
	{	
//		printf("\r\nTEMP %.1f\r\n",DS18B20_Get_Temp());		
//    printf("ADC:%d\r\n",ADC_ConvertedValue);
    DealCAP();
    ExchangeData();
		Delay_ms(1000);		/* 1s 读取一次温度值 */
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
}

/*********************************************END OF FILE**********************/
