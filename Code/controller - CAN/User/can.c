#include "can.h"

/*******************************************************************************
* 函数名  : CAN_Configuration
* 描述    : 配置CAN控制器工作在标准帧模式，并使能接收中断
* 输入    : None
* 输出    : None
* 返回    : None
* 注意		: None
*******************************************************************************/
void CAN_Config(void)
{
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	GPIO_InitTypeDef 			 GPIO_InitStructure;
//  NVIC_InitTypeDef 			 NVIC_InitStructure;	
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

  /* 配置CAN.RX->PA11引脚 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);   
  /* 配置CAN.TX->PA12引脚 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);	    

	
  /* CAN 寄存器初始化 */
  CAN_DeInit(CAN1);//将CAN1所有寄存器设置为缺省值
  CAN_StructInit(&CAN_InitStructure);//将结构体参数按缺省值填入

  /* CAN 寄存器初始化 关于CAN通信波特率的计算可以查看STM32中文参考手册*/
  CAN_InitStructure.CAN_TTCM = DISABLE;//失能触发通信模式
  CAN_InitStructure.CAN_ABOM = ENABLE;//使能自动离线管理，断线重连能自动连接上
  CAN_InitStructure.CAN_AWUM = DISABLE;//失能自动唤醒模式
  CAN_InitStructure.CAN_NART = DISABLE;//失能不自动转发模式
  CAN_InitStructure.CAN_RFLM = DISABLE;//失能接收缓存锁定模式
  CAN_InitStructure.CAN_TXFP = ENABLE;//使能发送缓存优先权
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;//正常模式
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
  CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
  CAN_InitStructure.CAN_Prescaler = 6;

  if (CAN_Init(CAN1,&CAN_InitStructure) == CANINITFAILED)//初始化CAN 		
  {
     //这里可以添加初始化失败处理
  }	
	
  /* CAN 过滤器组初始化 */
  CAN_FilterInitStructure.CAN_FilterNumber=0;
  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;//工作在标示符屏蔽位模式
  CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;//过滤器位宽为32位
  CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;//要过滤的高ID位
  CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;//要过滤的低ID位
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//过滤器高16位不需要完全匹配(位与)
  CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;//过滤器低16位不需要完全匹配(位与)
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器关联邮箱0
  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;//激活过滤器
  CAN_ITConfig(CAN1,CAN_IT_FMP0, ENABLE);//使能CAN接收中断
  CAN_FilterInit(&CAN_FilterInitStructure);
	
}

/*******************************************************************************
* 函数名  : USB_LP_CAN1_RX0_IRQHandler
* 描述    : CAN接收中断
* 输入    : None
* 输出    : None
* 返回    : None
* 注意		: None
*******************************************************************************/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  CanRxMsg RxMessage;
  CAN_Receive(CAN1,CAN_FIFO0, &RxMessage);//获取邮箱0数据
//  CAN_ID=RxMessage.StdId;      //获取标准帧标识符
//		
//	GetCAN[0]=RxMessage.Data[0]; //获取数据
//	GetCAN[1]=RxMessage.Data[1];
//	GetCAN[2]=RxMessage.Data[2];
//	GetCAN[3]=RxMessage.Data[3];
//	GetCAN[4]=RxMessage.Data[4];
//	GetCAN[5]=RxMessage.Data[5];
//	GetCAN[6]=RxMessage.Data[6];
//	GetCAN[7]=RxMessage.Data[7];
  DealCAN(&RxMessage);
  CANFlag=1;
  
  CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);//清接收中断标志
  
}

void CAN_Send(u8 *data,u8 can_id)
{
  CanTxMsg TxMessage;
  
  TxMessage.StdId=can_id;
  TxMessage.RTR = CAN_RTR_DATA;//数据帧(帧类型)
  TxMessage.IDE = CAN_ID_STD ; //标准帧模式
  TxMessage.DLC = 8;           //数据长度
  TxMessage.Data[0]=data[0];
  TxMessage.Data[1]=data[1];
  TxMessage.Data[2]=data[2];
  TxMessage.Data[3]=data[3];
  TxMessage.Data[4]=data[4];
  TxMessage.Data[5]=data[5];
  TxMessage.Data[6]=data[6];
  TxMessage.Data[7]=data[7];
  
  CAN_Transmit(CAN1,&TxMessage);
}
