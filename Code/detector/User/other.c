#include "other.h"

extern u8   TIM3CH1_CAPTURE_STA;  //���벶��״̬             
extern u16  TIM3CH1_CAPTURE_VAL_A,TIM3CH1_CAPTURE_VAL_B;  //���벶��ֵ
extern u8   TIM3CH2_CAPTURE_STA;
extern u16  TIM3CH2_CAPTURE_VAL_A,TIM3CH2_CAPTURE_VAL_B;

u8 CAP_Detect=0;
void DealCAP(void)
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
  if(temp>450&&temp<550) CAP_Detect=1;//����Ū����Ҫ�˹���λ
		//CH2
//		if(TIM3CH2_CAPTURE_STA&0X80)
//		{
////			printf("TIM3CH2_CAPTURE_STAֵ��%#x\r\n",TIM3CH2_CAPTURE_STA);
//			temp=TIM3CH2_CAPTURE_STA&0X3F;		
//			temp*=65536;           
//			temp+=TIM3CH2_CAPTURE_VAL_B; 
//			temp-=TIM3CH2_CAPTURE_VAL_A;
////			printf("TIM3CH2_CAPTURE_VALֵ��%#x  %#x\r\n",TIM3CH2_CAPTURE_VAL_A,TIM3CH2_CAPTURE_VAL_B);
////			printf("�� �� �� �� �� ʱ �� : %d us\r\n---------------------------\r\n",temp);   
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

