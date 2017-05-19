#include "other.h"

extern u8   TIM3CH1_CAPTURE_STA;  //输入捕获状态             
extern u16  TIM3CH1_CAPTURE_VAL_A,TIM3CH1_CAPTURE_VAL_B;  //输入捕获值
extern u8   TIM3CH2_CAPTURE_STA;
extern u16  TIM3CH2_CAPTURE_VAL_A,TIM3CH2_CAPTURE_VAL_B;


//处理捕获数据，当捕获的高电平数值（时长）处于范围中间时，返回1，认为烟感动作
u8 DealCAP(u16 LowLevel,u16 HighLevel)
{
  u32 temp=0;  
  if(TIM3CH1_CAPTURE_STA&0X80)      //成功捕平获到了一个周期
  {
//			printf("TIM3CH1_CAPTURE_STA值：%#x\r\n",TIM3CH1_CAPTURE_STA);
    temp=TIM3CH1_CAPTURE_STA&0X3F;					 //这个获得了溢出次数
    temp*=65536;              //溢出时间总和		 ，一次溢出代表一个十六位的计数器计满
    temp+=TIM3CH1_CAPTURE_VAL_B;    //得到总的高电平时间
    temp-=TIM3CH1_CAPTURE_VAL_A;
//			printf("TIM3CH1_CAPTURE_VAL值：%#x  %#x\r\n",TIM3CH1_CAPTURE_VAL_A,TIM3CH1_CAPTURE_VAL_B);
//			printf("两 次 下 降 沿 时 间 : %d us\r\n---------------------------\r\n",temp);      //打印总的高点平时间
    TIM3CH1_CAPTURE_STA=0;      //开启下一次捕获
  }
  
  if(temp>LowLevel && temp<HighLevel) 
    return 0x2;//因为协议是BIT1表示烟雾报警
  else 
    return RESET;
}

extern u8 DetectorID;
extern u8 CAP_Detected;
extern u8 Temp_Detected;
extern u8 Temperature;
void UploadData(void)
{
  u8 i=0,temp=0;
  u8 cache[8];
  u8 cnt=0;
  cache[cnt++]=0xDD;//Head
  cache[cnt++]=0xDD;
  cache[cnt++]=0x01;//Func
  cache[cnt++]=DetectorID;//ID
  cache[cnt++]=CAP_Detected+Temp_Detected;//STATE,cap=1是0x20即bit1
  cache[cnt++]=Temperature;
  cache[cnt++]=0;  
  
  for(i=0;i<7;i++)
    temp=cache[i]+temp;
  
  cache[cnt++]=temp;
  
  USART_SendString(USART2,cache,8);
  
  cnt=temp=0;
  cache[cnt++]=0xF3;
  cache[cnt++]=0;
  cache[cnt++]=CAP_Detected+Temp_Detected;
  cache[cnt++]=Temperature;
  cache[cnt++]=0xD0;
  cache[cnt++]=0;
  cache[cnt++]=0;
  
  for(i=0;i<7;i++)
    temp=cache[i]+temp;
    
  cache[cnt++]=temp;
  
  CAN_Send(cache,DetectorID);
}

u8 PwrRxBuffer[8];
extern u8 Token;
//电力载波
void PwrCarrier_Deal(u8 data)
{
  static u8 RxState=0,data_cnt=0;
  
  if(RxState==0&&data==0xDD)//帧头
  {
    RxState=1;
    PwrRxBuffer[0]=data;
  }
  else if(RxState==1&&data==0xDD)
  {
    RxState=2;
    PwrRxBuffer[1]=data;
    data_cnt=0;
  }
  else if(RxState==2)//开始接收
  {
    PwrRxBuffer[2+data_cnt++]=data;
  }
  else
			RxState = 0;
  
  if((2+data_cnt)==8)//数据接收完成，处理数据并重置状态指针
  {
    data_cnt=RxState=0;
    if(PwrRxBuffer[2]==0x10&&PwrRxBuffer[3]==DetectorID)
    {
      Token=1;
    }
  }
  
}


void BeepInit(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 
												   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(GPIOB, &GPIO_InitStructure);	
    
    GPIO_ResetBits(GPIOB,GPIO_Pin_9);
}

