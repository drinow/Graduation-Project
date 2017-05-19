#include "other.h"

extern u8   TIM3CH1_CAPTURE_STA;  //���벶��״̬             
extern u16  TIM3CH1_CAPTURE_VAL_A,TIM3CH1_CAPTURE_VAL_B;  //���벶��ֵ
extern u8   TIM3CH2_CAPTURE_STA;
extern u16  TIM3CH2_CAPTURE_VAL_A,TIM3CH2_CAPTURE_VAL_B;


//���������ݣ�������ĸߵ�ƽ��ֵ��ʱ�������ڷ�Χ�м�ʱ������1����Ϊ�̸ж���
u8 DealCAP(u16 LowLevel,u16 HighLevel)
{
  u32 temp=0;  
  if(TIM3CH1_CAPTURE_STA&0X80)      //�ɹ���ƽ����һ������
  {
//			printf("TIM3CH1_CAPTURE_STAֵ��%#x\r\n",TIM3CH1_CAPTURE_STA);
    temp=TIM3CH1_CAPTURE_STA&0X3F;					 //���������������
    temp*=65536;              //���ʱ���ܺ�		 ��һ���������һ��ʮ��λ�ļ���������
    temp+=TIM3CH1_CAPTURE_VAL_B;    //�õ��ܵĸߵ�ƽʱ��
    temp-=TIM3CH1_CAPTURE_VAL_A;
//			printf("TIM3CH1_CAPTURE_VALֵ��%#x  %#x\r\n",TIM3CH1_CAPTURE_VAL_A,TIM3CH1_CAPTURE_VAL_B);
//			printf("�� �� �� �� �� ʱ �� : %d us\r\n---------------------------\r\n",temp);      //��ӡ�ܵĸߵ�ƽʱ��
    TIM3CH1_CAPTURE_STA=0;      //������һ�β���
  }
  
  if(temp>LowLevel && temp<HighLevel) 
    return 0x2;//��ΪЭ����BIT1��ʾ������
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
  cache[cnt++]=CAP_Detected+Temp_Detected;//STATE,cap=1��0x20��bit1
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
//�����ز�
void PwrCarrier_Deal(u8 data)
{
  static u8 RxState=0,data_cnt=0;
  
  if(RxState==0&&data==0xDD)//֡ͷ
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
  else if(RxState==2)//��ʼ����
  {
    PwrRxBuffer[2+data_cnt++]=data;
  }
  else
			RxState = 0;
  
  if((2+data_cnt)==8)//���ݽ�����ɣ��������ݲ�����״ָ̬��
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

