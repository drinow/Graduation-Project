#include "other.h"

extern u8   TIM3CH1_CAPTURE_STA;  //输入捕获状态             
extern u16  TIM3CH1_CAPTURE_VAL_A,TIM3CH1_CAPTURE_VAL_B;  //输入捕获值
extern u8   TIM3CH2_CAPTURE_STA;
extern u16  TIM3CH2_CAPTURE_VAL_A,TIM3CH2_CAPTURE_VAL_B;

u8 CAP_Detect=0;
void DealCAP(void)
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
  if(temp>450&&temp<550) CAP_Detect=1;//故意弄成需要人工复位
		//CH2
//		if(TIM3CH2_CAPTURE_STA&0X80)
//		{
////			printf("TIM3CH2_CAPTURE_STA值：%#x\r\n",TIM3CH2_CAPTURE_STA);
//			temp=TIM3CH2_CAPTURE_STA&0X3F;		
//			temp*=65536;           
//			temp+=TIM3CH2_CAPTURE_VAL_B; 
//			temp-=TIM3CH2_CAPTURE_VAL_A;
////			printf("TIM3CH2_CAPTURE_VAL值：%#x  %#x\r\n",TIM3CH2_CAPTURE_VAL_A,TIM3CH2_CAPTURE_VAL_B);
////			printf("两 次 下 降 沿 时 间 : %d us\r\n---------------------------\r\n",temp);   
//			TIM3CH2_CAPTURE_STA=0;    
//		}
}

extern u8 DetectID;
void ExchangeData(void)
{
  u8 cache[8];
  u8 cnt=0;
  cache[cnt++]=0xDD;//Head
  cache[cnt++]=0xDD;
  cache[cnt++]=0x01;//Func
  cache[cnt++]=DetectID;//ID
  cache[cnt++]=CAP_Detect;//STATE
  cache[cnt++]=0;
  cache[cnt++]=0;
  cache[cnt++]=0;
  
  USART_SendString(USART1,cache,8);
}

