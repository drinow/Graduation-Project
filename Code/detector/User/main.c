/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   systickϵͳ��ʱ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� ISO-MINI STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
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
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{	
	/* ����SysTick Ϊ1us�ж�һ�� */
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
		Delay_ms(1000);		/* 1s ��ȡһ���¶�ֵ */
	}    
}

/*******************************************************************************
* ������  : NVIC_Configuration
* ����    : �����ж�������
* ����    : None
* ���    : None
* ����    : None
* ע��		: None
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
  
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
}

/*********************************************END OF FILE**********************/
