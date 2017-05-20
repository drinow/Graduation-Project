#include "tim.h"
#include "stdio.h"



/**************����ע���Լ������Բ�������2���½���Ϊ����**************/
/**************����ע���Լ������Բ�������2���½���Ϊ����**************/
/**************����ע���Լ������Բ�������2���½���Ϊ����**************/

/**************�жϺ�����ⲽ�谴1��2��3��ã���3������֧·a,b*********/


//��ʱ�� 5 ͨ�� 1 ���벶������
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM3_Cap_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//   	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_ICInitTypeDef  TIM3_ICInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ��TIM3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);  //ʹ��GPIOAʱ��
	
	//GPIO���úòŻ��ȶ����ױ�����
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7|GPIO_Pin_6;  //PA0 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //PA0 ����  			��Ϊ����
	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1;  //PA0 ���֮ǰ����  
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //PA0 ����  			��Ϊ����
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_6);						 //PA0 ����	 ��Ϊ��0
	GPIO_SetBits(GPIOA,GPIO_Pin_7);
//	GPIO_SetBits(GPIOB,GPIO_Pin_0);
//	GPIO_SetBits(GPIOB,GPIO_Pin_1);	
	
	//��ʼ����ʱ��4 TIM3	 
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  
	//��ʼ��TIM3���벶�����
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;	//�����ز���  ��Ϊ�½���
  TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  TIM3_ICInitStructure.TIM_ICFilter = 0x08;//IC1F=0000 ���������˲��� ���˲�		//��Ϊ����8���������µ�
  TIM_ICInit(TIM3, &TIM3_ICInitStructure);
	
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;	//�����ز���  ��Ϊ�½���
  TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  TIM3_ICInitStructure.TIM_ICFilter = 0x08;//IC1F=0000 ���������˲��� ���˲�		//��Ϊ����8���������µ�
  TIM_ICInit(TIM3, &TIM3_ICInitStructure);
	
//	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_3; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
//  TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;	//�����ز���  ��Ϊ�½���
//  TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
//  TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
//  TIM3_ICInitStructure.TIM_ICFilter = 0x08;//IC1F=0000 ���������˲��� ���˲�		//��Ϊ����8���������µ�
//  TIM_ICInit(TIM3, &TIM3_ICInitStructure);
//	
//	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_4; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
//  TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;	//�����ز���  ��Ϊ�½���
//  TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
//  TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
//  TIM3_ICInitStructure.TIM_ICFilter = 0x08;//IC1F=0000 ���������˲��� ���˲�		//��Ϊ����8���������µ�
//  TIM_ICInit(TIM3, &TIM3_ICInitStructure);
	
	//�жϷ����ʼ��
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
//	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2,ENABLE);//��������ж� ,����CC1IE�����ж�	��������жϿ���һ���������٣�����һ����������ô�������ж��أ���
																						/*ע�����ﲻҪ©��TIM_IT_CC2*/
   	TIM_Cmd(TIM3,ENABLE ); 	//ʹ�ܶ�ʱ��3
}

//����״̬ 											/*************************������ͺ���Ҫ*************************/
//[7]:0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
//[6]:0,��û���񵽸ߵ�ƽ;1,�Ѿ����񵽸ߵ�ƽ��.
//[5:0]:����ߵ�ƽ������Ĵ���
u8   TIM3CH1_CAPTURE_STA=0;  //���벶��״̬             
u16  TIM3CH1_CAPTURE_VAL_A=0  ;  //���벶��ֵ
u16  TIM3CH1_CAPTURE_VAL_B=0  ;

u8   TIM3CH2_CAPTURE_STA=0;  //���벶��״̬             
u16  TIM3CH2_CAPTURE_VAL_A=0  ;  //���벶��ֵ
u16  TIM3CH2_CAPTURE_VAL_B=0  ;

//u8   TIM3CH3_CAPTURE_STA=0;  //���벶��״̬             
//u16  TIM3CH3_CAPTURE_VAL_A  ;  //���벶��ֵ
//u16  TIM3CH3_CAPTURE_VAL_B  ;

//u8   TIM3CH4_CAPTURE_STA=0;  //���벶��״̬             
//u16  TIM3CH4_CAPTURE_VAL_A  ;  //���벶��ֵ
//u16  TIM3CH4_CAPTURE_VAL_B  ;


//��ʱ�� 4 �жϷ������   
void TIM3_IRQHandler(void)
{     
	u16 tsr;
	tsr=TIM3->SR;
//	printf("TIM3_IRQ\r\n");
/*1*/	if((TIM3CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
	{
/*3(b)*/if(tsr&0X01)//���	  ��Ϊ���λΪ�����־λ
		{
			if(TIM3CH1_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM3CH1_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM3CH1_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
					TIM3CH1_CAPTURE_VAL_B=0XFFFF;
				} else TIM3CH1_CAPTURE_STA++;
				
			}	
		}
/*3(a)*/if(tsr&0x02)//���� 1 ���������¼�
		{
			if(TIM3CH1_CAPTURE_STA&0X40)    //������һ���½���
			{ 
				TIM3CH1_CAPTURE_STA|=0X80;  //��ǳɹ�����һ�θߵ�ƽ����
				TIM3CH1_CAPTURE_VAL_B=TIM3->CCR1;//��ȡ��ǰ�Ĳ���ֵ.
//				TIM3->CCER&=~(1<<1);      //CC1P=0  ����Ϊ�����ز���		1<<1 �ǵ�һͨ����������ͨ��Ҫ�ı䣬�ĳ�ʲô���ο��ֲἴ��
			}
/*2*/		else                   //��δ��ʼ,��һ�β����½���
			{
				TIM3CH1_CAPTURE_STA=0;    //���
				TIM3CH1_CAPTURE_VAL_B=0;
				TIM3CH1_CAPTURE_STA|=0X40;  //��ǲ������½���
				//TIM3->CNT=0;          //���������
				TIM3CH1_CAPTURE_VAL_A=TIM3->CCR1;
//				TIM3->CCER|=1<<1;          //CC1P=1  ����Ϊ�½��ز���
			} 
		}                   
	}
	/////////////////////////////////
	if((TIM3CH2_CAPTURE_STA&0X80)==0)
	{
		if(tsr&0X01)//���	  
		{
			if(TIM3CH2_CAPTURE_STA&0X40)
			{
				if((TIM3CH2_CAPTURE_STA&0X3F)==0X3F)
				{
					TIM3CH2_CAPTURE_STA|=0X80;
					TIM3CH2_CAPTURE_VAL_B=0XFFFF;
				} else TIM3CH2_CAPTURE_STA++;
				
			}	
		}
		if(tsr&0x04)//����				 //====��֮ͬ��:0x02->0x04
		{
			if(TIM3CH2_CAPTURE_STA&0X40)   
			{ 
				TIM3CH2_CAPTURE_STA|=0X80; 
				TIM3CH2_CAPTURE_VAL_B=TIM3->CCR2;//====��֮ͬ��:CCR1->CCR2
//				TIM3->CCER&=~(1<<1);      
			}
		else                   
			{
				TIM3CH2_CAPTURE_STA=0;    
				TIM3CH2_CAPTURE_VAL_B=0;
				TIM3CH2_CAPTURE_STA|=0X40;  
				TIM3CH2_CAPTURE_VAL_A=TIM3->CCR2;	//====��֮ͬ��:CCR1->CCR2
//				TIM3->CCER|=1<<1;          
			} 
		}                   
	}
	/////////////////////////////////
//	if((TIM3CH3_CAPTURE_STA&0X80)==0)
//	{
//		if(tsr&0X01)//���	  
//		{
//			if(TIM3CH3_CAPTURE_STA&0X40)
//			{
//				if((TIM3CH3_CAPTURE_STA&0X3F)==0X3F)
//				{
//					TIM3CH3_CAPTURE_STA|=0X80;
//					TIM3CH3_CAPTURE_VAL_B=0XFFFF;
//				} else TIM3CH3_CAPTURE_STA++;
//				
//			}	
//		}
//		if(tsr&0x08)//����				 //====��֮ͬ��:
//		{
//			if(TIM3CH3_CAPTURE_STA&0X40)   
//			{ 
//				TIM3CH3_CAPTURE_STA|=0X80; 
//				TIM3CH3_CAPTURE_VAL_B=TIM3->CCR3;//====��֮ͬ��:CCR1->CCR2     
//			}
//		else                   
//			{
//				TIM3CH3_CAPTURE_STA=0;    
//				TIM3CH3_CAPTURE_VAL_B=0;
//				TIM3CH3_CAPTURE_STA|=0X40;  
//				TIM3CH3_CAPTURE_VAL_A=TIM3->CCR3;	//====��֮ͬ��:CCR1->CCR2        
//			} 
//		}                   
//	}
	/////////////////////////////////
//	if((TIM3CH4_CAPTURE_STA&0X80)==0)
//	{
//		if(tsr&0X01)//���	  
//		{
//			if(TIM3CH4_CAPTURE_STA&0X40)
//			{
//				if((TIM3CH4_CAPTURE_STA&0X3F)==0X3F)
//				{
//					TIM3CH4_CAPTURE_STA|=0X80;
//					TIM3CH4_CAPTURE_VAL_B=0XFFFF;
//				} else TIM3CH4_CAPTURE_STA++;
//				
//			}	
//		}
//		if(tsr&0x10)//����				 //====��֮ͬ��
//		{
//			if(TIM3CH4_CAPTURE_STA&0X40)   
//			{ 
//				TIM3CH4_CAPTURE_STA|=0X80; 
//				TIM3CH4_CAPTURE_VAL_B=TIM3->CCR4;//====��֮ͬ��
//			}
//		else                   
//			{
//				TIM3CH4_CAPTURE_STA=0;    
//				TIM3CH4_CAPTURE_VAL_B=0;
//				TIM3CH4_CAPTURE_STA|=0X40;  
//				TIM3CH4_CAPTURE_VAL_A=TIM3->CCR4;	//====��֮ͬ��     
//			} 
//		}                   
//	}
	/////////////////////////////////
	TIM3->SR=0;//����жϱ�־λ     
}
