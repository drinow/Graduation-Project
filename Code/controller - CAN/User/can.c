#include "can.h"

/*******************************************************************************
* ������  : CAN_Configuration
* ����    : ����CAN�����������ڱ�׼֡ģʽ����ʹ�ܽ����ж�
* ����    : None
* ���    : None
* ����    : None
* ע��		: None
*******************************************************************************/
void CAN_Config(void)
{
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	GPIO_InitTypeDef 			 GPIO_InitStructure;
//  NVIC_InitTypeDef 			 NVIC_InitStructure;	
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

  /* ����CAN.RX->PA11���� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);   
  /* ����CAN.TX->PA12���� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);	    

	
  /* CAN �Ĵ�����ʼ�� */
  CAN_DeInit(CAN1);//��CAN1���мĴ�������Ϊȱʡֵ
  CAN_StructInit(&CAN_InitStructure);//���ṹ�������ȱʡֵ����

  /* CAN �Ĵ�����ʼ�� ����CANͨ�Ų����ʵļ�����Բ鿴STM32���Ĳο��ֲ�*/
  CAN_InitStructure.CAN_TTCM = DISABLE;//ʧ�ܴ���ͨ��ģʽ
  CAN_InitStructure.CAN_ABOM = ENABLE;//ʹ���Զ����߹��������������Զ�������
  CAN_InitStructure.CAN_AWUM = DISABLE;//ʧ���Զ�����ģʽ
  CAN_InitStructure.CAN_NART = DISABLE;//ʧ�ܲ��Զ�ת��ģʽ
  CAN_InitStructure.CAN_RFLM = DISABLE;//ʧ�ܽ��ջ�������ģʽ
  CAN_InitStructure.CAN_TXFP = ENABLE;//ʹ�ܷ��ͻ�������Ȩ
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;//����ģʽ
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
  CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
  CAN_InitStructure.CAN_Prescaler = 6;

  if (CAN_Init(CAN1,&CAN_InitStructure) == CANINITFAILED)//��ʼ��CAN 		
  {
     //���������ӳ�ʼ��ʧ�ܴ���
  }	
	
  /* CAN ���������ʼ�� */
  CAN_FilterInitStructure.CAN_FilterNumber=0;
  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;//�����ڱ�ʾ������λģʽ
  CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;//������λ��Ϊ32λ
  CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;//Ҫ���˵ĸ�IDλ
  CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;//Ҫ���˵ĵ�IDλ
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//��������16λ����Ҫ��ȫƥ��(λ��)
  CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;//��������16λ����Ҫ��ȫƥ��(λ��)
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//��������������0
  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;//���������
  CAN_ITConfig(CAN1,CAN_IT_FMP0, ENABLE);//ʹ��CAN�����ж�
  CAN_FilterInit(&CAN_FilterInitStructure);
	
}

/*******************************************************************************
* ������  : USB_LP_CAN1_RX0_IRQHandler
* ����    : CAN�����ж�
* ����    : None
* ���    : None
* ����    : None
* ע��		: None
*******************************************************************************/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  CanRxMsg RxMessage;
  CAN_Receive(CAN1,CAN_FIFO0, &RxMessage);//��ȡ����0����
//  CAN_ID=RxMessage.StdId;      //��ȡ��׼֡��ʶ��
//		
//	GetCAN[0]=RxMessage.Data[0]; //��ȡ����
//	GetCAN[1]=RxMessage.Data[1];
//	GetCAN[2]=RxMessage.Data[2];
//	GetCAN[3]=RxMessage.Data[3];
//	GetCAN[4]=RxMessage.Data[4];
//	GetCAN[5]=RxMessage.Data[5];
//	GetCAN[6]=RxMessage.Data[6];
//	GetCAN[7]=RxMessage.Data[7];
  DealCAN(&RxMessage);
  CANFlag=1;
  
  CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);//������жϱ�־
  
}

void CAN_Send(u8 *data,u8 can_id)
{
  CanTxMsg TxMessage;
  
  TxMessage.StdId=can_id;
  TxMessage.RTR = CAN_RTR_DATA;//����֡(֡����)
  TxMessage.IDE = CAN_ID_STD ; //��׼֡ģʽ
  TxMessage.DLC = 8;           //���ݳ���
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
