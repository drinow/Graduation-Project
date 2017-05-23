#include "bsp_actuator.h"

void FanInit(void)
{
	GPIO_InitTypeDef 			 GPIO_InitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);      
}

/*
���ȿ���
*/
void FanCtrl(u8 state)
{
  if(state)
    GPIO_SetBits(GPIOB,GPIO_Pin_15);
  else
    GPIO_ResetBits(GPIOB,GPIO_Pin_15);
}


void StepMotorInit(void)
{
	GPIO_InitTypeDef 			 GPIO_InitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);
  
  //Motor1
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure); 

  //Motor2
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);      

}

/*
�����������
order:������
direction:��ת����
*/
void StepMotor_CW(u8 order,u8 direction)
{
  if(order==NUM1)
  {
    if(direction==FRD)
    {
      GPIO_Write(GPIOC,0xC0);//ѭ����λ������ת��ֻ��Ҫ�ı���λ����
      Delay_ms(3);//���������ٶȣ�̫����ת������̫���˵����Ӧ������
      GPIO_Write(GPIOC,0x180);
      Delay_ms(3);
      GPIO_Write(GPIOC,0x300);
      Delay_ms(3);
      GPIO_Write(GPIOC,0x240);
      Delay_ms(3);
    }
    else if(direction ==REV)
    {
      GPIO_Write(GPIOC,0x240);//ѭ����λ������ת��ֻ��Ҫ�ı���λ����
      Delay_ms(3);//���������ٶȣ�̫����ת������̫���˵����Ӧ������
      GPIO_Write(GPIOC,0x300);
      Delay_ms(3);
      GPIO_Write(GPIOC,0x180);
      Delay_ms(3);
      GPIO_Write(GPIOC,0xC0);
      Delay_ms(3);
    }
  }
//  if(order==NUM2)
//  {
//    if(direction==FRD)
//    {
//      GPIO_Write(GPIOB,0x3000);//ѭ����λ������ת��ֻ��Ҫ�ı���λ����
//      Delay_ms(3);//���������ٶȣ�̫����ת������̫���˵����Ӧ������
//      GPIO_Write(GPIOB,0x6000);
//      Delay_ms(3);
//      GPIO_Write(GPIOB,0xC000);
//      Delay_ms(3);
//      GPIO_Write(GPIOB,0x9000);
//      Delay_ms(3);
//    }
//    else if(direction==REV)
//    {
//      GPIO_Write(GPIOB,0x9000);//ѭ����λ������ת��ֻ��Ҫ�ı���λ����
//      Delay_ms(3);//���������ٶȣ�̫����ת������̫���˵����Ӧ������
//      GPIO_Write(GPIOB,0xC000);
//      Delay_ms(3);
//      GPIO_Write(GPIOB,0x6000);
//      Delay_ms(3);
//      GPIO_Write(GPIOB,0x3000);
//      Delay_ms(3);
//    }
//  }
}

/*
�����������
order:������
direction:��ת����
period:ִ�д���
*/
//�����������
u32 StepMotorCnt;
u8  LastDirection=0,StepMotorRuning=0;
u8 motorEnd=0;
void StepMotorCtrl(u8 order,u8 direction,u32 period)
{
  if(StepMotorRuning==0)//δ����״̬���ղ������ڣ�����������ִ�У�ִ����֮ǰ���ٽ���������
  {
    StepMotorCnt=period;
    StepMotorRuning++;
  }  
  if(StepMotorCnt>0)//���������У���ִ����
  {
    StepMotorCnt--;
    
    if(StepMotorCnt>(period/3)&&StepMotorCnt<(period/3*2))//���м�ͣ10*600��
    {
      Delay_ms(15);
      return;
    }
    if(LastDirection!=direction)
      StepMotor_CW(order,direction);
    
    if(StepMotorCnt==0)//ִ�������ձ�־λ������ס�ϴεķ���
    {
      motorEnd=1;
      GPIO_Write(GPIOC,0);
//      GPIO_Write(GPIOB,0);
      StepMotorRuning=0;
      LastDirection=direction;
    }
  }
}

void WaterPumpInit(void)
{
	GPIO_InitTypeDef 			 GPIO_InitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
}

void WaterPumpCtrl(u8 state)
{
  if(state)
    GPIO_SetBits(GPIOB,GPIO_Pin_0);
  else
    GPIO_ResetBits(GPIOB,GPIO_Pin_0);
}

void AlarmInit(void)
{
	GPIO_InitTypeDef 			 GPIO_InitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
}

void AlarmCtrl(u8 state)
{
  if(state)
    GPIO_SetBits(GPIOB,GPIO_Pin_1);
  else
    GPIO_ResetBits(GPIOB,GPIO_Pin_1);
}

void PwrCutInit(void)
{
	GPIO_InitTypeDef 			 GPIO_InitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
}

void PwrCutCtrl(u8 state)
{
  if(state)
    GPIO_SetBits(GPIOC,GPIO_Pin_12);
  else
    GPIO_ResetBits(GPIOC,GPIO_Pin_12);
}

void Escape595Init(void)
{
  hc595_init();
  
  hc595_WriteByte(0x00,0x00);//self-check
  Delay_ms(500);
//  hc595_WriteByte(0xFF,0xFF);
//  hc595_WriteByte(0xFF,0xFF);
}

void ActuatorInit(void)
{
  Escape595Init();//����ָʾ��
  PwrCutInit();//���з�
  AlarmInit();//����
  WaterPumpInit();//ˮ��
  StepMotorInit();//������
  FanInit();//���̷��
}
