#include "tim.h"
#include "stdio.h"



/**************以下注释以及配置以捕获相邻2次下降沿为例子**************/
/**************以下注释以及配置以捕获相邻2次下降沿为例子**************/
/**************以下注释以及配置以捕获相邻2次下降沿为例子**************/

/**************中断函数理解步骤按1，2，3标好，第3步出现支路a,b*********/


//定时器 5 通道 1 输入捕获配置
//arr：自动重装值
//psc：时钟预分频数
void TIM3_Cap_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//   	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_ICInitTypeDef  TIM3_ICInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能TIM3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);  //使能GPIOA时钟
	
	//GPIO配置好才会稳定不易被干扰
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7|GPIO_Pin_6;  //PA0 清除之前设置  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //PA0 输入  			改为上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1;  //PA0 清除之前设置  
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //PA0 输入  			改为上拉
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_6);						 //PA0 上拉	 改为置0
	GPIO_SetBits(GPIOA,GPIO_Pin_7);
//	GPIO_SetBits(GPIOB,GPIO_Pin_0);
//	GPIO_SetBits(GPIOB,GPIO_Pin_1);	
	
	//初始化定时器4 TIM3	 
	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
  
	//初始化TIM3输入捕获参数
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	选择输入端 IC1映射到TI1上
  TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;	//上升沿捕获  改为下降沿
  TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  TIM3_ICInitStructure.TIM_ICFilter = 0x08;//IC1F=0000 配置输入滤波器 不滤波		//改为过滤8个周期以下的
  TIM_ICInit(TIM3, &TIM3_ICInitStructure);
	
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	选择输入端 IC1映射到TI1上
  TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;	//上升沿捕获  改为下降沿
  TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  TIM3_ICInitStructure.TIM_ICFilter = 0x08;//IC1F=0000 配置输入滤波器 不滤波		//改为过滤8个周期以下的
  TIM_ICInit(TIM3, &TIM3_ICInitStructure);
	
//	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_3; //CC1S=01 	选择输入端 IC1映射到TI1上
//  TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;	//上升沿捕获  改为下降沿
//  TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
//  TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
//  TIM3_ICInitStructure.TIM_ICFilter = 0x08;//IC1F=0000 配置输入滤波器 不滤波		//改为过滤8个周期以下的
//  TIM_ICInit(TIM3, &TIM3_ICInitStructure);
//	
//	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_4; //CC1S=01 	选择输入端 IC1映射到TI1上
//  TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;	//上升沿捕获  改为下降沿
//  TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
//  TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
//  TIM3_ICInitStructure.TIM_ICFilter = 0x08;//IC1F=0000 配置输入滤波器 不滤波		//改为过滤8个周期以下的
//  TIM_ICInit(TIM3, &TIM3_ICInitStructure);
	
	//中断分组初始化
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级2级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
//	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	这个两个中断开关一个都不能少！！第一个少了你怎么检测溢出中断呢！！
																						/*注意这里不要漏掉TIM_IT_CC2*/
   	TIM_Cmd(TIM3,ENABLE ); 	//使能定时器3
}

//捕获状态 											/*************************这个解释很重要*************************/
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到高电平;1,已经捕获到高电平了.
//[5:0]:捕获高电平后溢出的次数
u8   TIM3CH1_CAPTURE_STA=0;  //输入捕获状态             
u16  TIM3CH1_CAPTURE_VAL_A=0  ;  //输入捕获值
u16  TIM3CH1_CAPTURE_VAL_B=0  ;

u8   TIM3CH2_CAPTURE_STA=0;  //输入捕获状态             
u16  TIM3CH2_CAPTURE_VAL_A=0  ;  //输入捕获值
u16  TIM3CH2_CAPTURE_VAL_B=0  ;

//u8   TIM3CH3_CAPTURE_STA=0;  //输入捕获状态             
//u16  TIM3CH3_CAPTURE_VAL_A  ;  //输入捕获值
//u16  TIM3CH3_CAPTURE_VAL_B  ;

//u8   TIM3CH4_CAPTURE_STA=0;  //输入捕获状态             
//u16  TIM3CH4_CAPTURE_VAL_A  ;  //输入捕获值
//u16  TIM3CH4_CAPTURE_VAL_B  ;


//定时器 4 中断服务程序   
void TIM3_IRQHandler(void)
{     
	u16 tsr;
	tsr=TIM3->SR;
//	printf("TIM3_IRQ\r\n");
/*1*/	if((TIM3CH1_CAPTURE_STA&0X80)==0)//还未成功捕获
	{
/*3(b)*/if(tsr&0X01)//溢出	  因为最低位为溢出标志位
		{
			if(TIM3CH1_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM3CH1_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM3CH1_CAPTURE_STA|=0X80;//标记成功捕获了一次
					TIM3CH1_CAPTURE_VAL_B=0XFFFF;
				} else TIM3CH1_CAPTURE_STA++;
				
			}	
		}
/*3(a)*/if(tsr&0x02)//捕获 1 发生捕获事件
		{
			if(TIM3CH1_CAPTURE_STA&0X40)    //捕获到下一个下降沿
			{ 
				TIM3CH1_CAPTURE_STA|=0X80;  //标记成功捕获到一次高电平脉宽
				TIM3CH1_CAPTURE_VAL_B=TIM3->CCR1;//获取当前的捕获值.
//				TIM3->CCER&=~(1<<1);      //CC1P=0  设置为上升沿捕获		1<<1 是第一通道，若其他通道要改变，改成什么看参考手册即可
			}
/*2*/		else                   //还未开始,第一次捕获下降沿
			{
				TIM3CH1_CAPTURE_STA=0;    //清空
				TIM3CH1_CAPTURE_VAL_B=0;
				TIM3CH1_CAPTURE_STA|=0X40;  //标记捕获到了下降沿
				//TIM3->CNT=0;          //计数器清空
				TIM3CH1_CAPTURE_VAL_A=TIM3->CCR1;
//				TIM3->CCER|=1<<1;          //CC1P=1  设置为下降沿捕获
			} 
		}                   
	}
	/////////////////////////////////
	if((TIM3CH2_CAPTURE_STA&0X80)==0)
	{
		if(tsr&0X01)//溢出	  
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
		if(tsr&0x04)//捕获				 //====不同之处:0x02->0x04
		{
			if(TIM3CH2_CAPTURE_STA&0X40)   
			{ 
				TIM3CH2_CAPTURE_STA|=0X80; 
				TIM3CH2_CAPTURE_VAL_B=TIM3->CCR2;//====不同之处:CCR1->CCR2
//				TIM3->CCER&=~(1<<1);      
			}
		else                   
			{
				TIM3CH2_CAPTURE_STA=0;    
				TIM3CH2_CAPTURE_VAL_B=0;
				TIM3CH2_CAPTURE_STA|=0X40;  
				TIM3CH2_CAPTURE_VAL_A=TIM3->CCR2;	//====不同之处:CCR1->CCR2
//				TIM3->CCER|=1<<1;          
			} 
		}                   
	}
	/////////////////////////////////
//	if((TIM3CH3_CAPTURE_STA&0X80)==0)
//	{
//		if(tsr&0X01)//溢出	  
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
//		if(tsr&0x08)//捕获				 //====不同之处:
//		{
//			if(TIM3CH3_CAPTURE_STA&0X40)   
//			{ 
//				TIM3CH3_CAPTURE_STA|=0X80; 
//				TIM3CH3_CAPTURE_VAL_B=TIM3->CCR3;//====不同之处:CCR1->CCR2     
//			}
//		else                   
//			{
//				TIM3CH3_CAPTURE_STA=0;    
//				TIM3CH3_CAPTURE_VAL_B=0;
//				TIM3CH3_CAPTURE_STA|=0X40;  
//				TIM3CH3_CAPTURE_VAL_A=TIM3->CCR3;	//====不同之处:CCR1->CCR2        
//			} 
//		}                   
//	}
	/////////////////////////////////
//	if((TIM3CH4_CAPTURE_STA&0X80)==0)
//	{
//		if(tsr&0X01)//溢出	  
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
//		if(tsr&0x10)//捕获				 //====不同之处
//		{
//			if(TIM3CH4_CAPTURE_STA&0X40)   
//			{ 
//				TIM3CH4_CAPTURE_STA|=0X80; 
//				TIM3CH4_CAPTURE_VAL_B=TIM3->CCR4;//====不同之处
//			}
//		else                   
//			{
//				TIM3CH4_CAPTURE_STA=0;    
//				TIM3CH4_CAPTURE_VAL_B=0;
//				TIM3CH4_CAPTURE_STA|=0X40;  
//				TIM3CH4_CAPTURE_VAL_A=TIM3->CCR4;	//====不同之处     
//			} 
//		}                   
//	}
	/////////////////////////////////
	TIM3->SR=0;//清除中断标志位     
}
