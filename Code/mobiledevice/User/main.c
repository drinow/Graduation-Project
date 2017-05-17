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
#include "other.h"
#include "HMC5883L.h"
#include "stdio.h"
#include "bsp_i2c_gpio.h"
#include "rc522.h"

#define ID1 0xA5
#define ID2 0x85

u8 DetectorID=0xD0;

extern u32 __IO tick;
extern u8  __IO Trig;
extern u32 __IO sec;

extern float angle;   
extern int Magn_x,Magn_y,Magn_z;

extern u8 CardID;

void NVIC_Configuration(void);

void ClearPoint(void)
{
  //BluePoint
  delay_ms(5);
  printf("PIC(380,150,7)\r\n");delay_ms(5);
  printf("PIC(80,255,7)\r\n");delay_ms(5);
  printf("PIC(390,255,7)\r\n");delay_ms(5);
  
  //RedPoint
  printf("PIC(325,95,7)\r\n");delay_ms(5);
  printf("PIC(30,155,7)\r\n");delay_ms(5);
  printf("PIC(325,305,7)\r\n");delay_ms(5);
}

void InitID(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 									   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);	
}

void ReadID(void)
{
  u8 temp=0;
  temp=(GPIO_ReadInputData(GPIOB)&0x0F);
  DetectorID=0xD0+temp;
  
  switch(DetectorID)
  {
    case 0xD0:ClearPoint();break;
    case 0xD1:ClearPoint();printf("PIC(325,95,1)\r\n");break;
    case 0xD2:ClearPoint();printf("PIC(30,155,1)\r\n");break;
    case 0xD3:ClearPoint();printf("PIC(325,305,1)\r\n");break;
  }
  delay_ms(5);
}

void Key_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;//
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void Orientation(void)//方向判断
{
  char str1[32]={0};
  char str2[32]={0};
  char str3[32]={0};
    if((angle < 22.5) || (angle > 337.5 ))
        printf("DS48(170,650,'正南',1,0);\r\n");
    if((angle > 22.5) && (angle < 67.5 ))
        printf("DS48(170,650,'西南',1,0);\r\n");
    if((angle > 67.5) && (angle < 112.5 ))
        printf("DS48(170,650,'正西',1,0);\r\n");
    if((angle > 112.5) && (angle < 157.5 ))
        printf("DS48(170,650,'西北',1,0);\r\n");
    if((angle > 157.5) && (angle < 202.5 ))
        printf("DS48(170,650,'正北',1,0);\r\n");
    if((angle > 202.5) && (angle < 247.5 ))
        printf("DS48(170,650,'东北',1,0);\r\n");
    if((angle > 247.5) && (angle < 292.5 ))
        printf("DS48(170,650,'正东',1,0);\r\n");
    if((angle > 292.5) && (angle < 337.5 ))
        printf("DS48(170,650,'东南',1,0);\r\n");
    
    switch(CardID)
    {
      case ID1:printf("DS48(170,750,'0xA5',1,0)\r\n");ClearPoint();printf("PIC(380,150,8)\r\n");break;
      case ID2:printf("DS48(170,750,'0x85',1,0)\r\n");ClearPoint();printf("PIC(80,255,8)\r\n");break;
      default:break;
    }
    delay_ms(5);
    sprintf(str1,"METS(0,%d)\r\n",(int)angle);
    printf("%s",str1);
    delay_ms(5);
    sprintf(str2,"DS32(20,550,'%3d度',1,0);\r\n",(int)angle);
    printf("%s",str2);
    delay_ms(5);
//    sprintf(str3,"DS32(370,550,'0x%2x',1,0);\r\n",DetectorID);
//    printf("%s",str3);
//    delay_ms(30);
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
	SysTick_Init();
	LED_GPIO_Config();
  Key_Config();
  InitID();
  InitRC522();	
  LED1_ON;
  tick=0;
  printf("START\r\n");
  
  SPG0();
  Init_HMC5883L();
  HMC5883L_Carlibrate();
  SPG1();
	for(;;)
	{
    if(Trig)
    {
      ReadID();
      RD_Card();
      Read_HMC5883L();
//      printf("x= %4d,y= %4d,z= %4d angle= %5.2f ",Magn_x,Magn_y,Magn_z,angle);
      Orientation();
      Trig=0;
    }
    
    if(tick%1000<500)
    { 
      LED1_ON; 
    }
    else
    { 
      LED1_OFF;
    }
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
