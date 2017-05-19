#include "bsp_can.h"

void CAN_Config(void)
{
	CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
  /*ʱ��ʹ��*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	
  //NVIC���ã���

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
	/*1M������*/
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
  CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
  CAN_InitStructure.CAN_Prescaler = 6;

  if (CAN_Init(CAN1,&CAN_InitStructure) == CANINITFAILED)//��ʼ��CAN 		
  {
     //���������ӳ�ʼ��ʧ�ܴ���
		printf("CANģ���ʼ��ʧ��!\r\n");
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

u8 GetCAN[8];
uint32_t CAN_ID;//��ʶ��
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  CanRxMsg RxMessage;
  CAN_Receive(CAN1,CAN_FIFO0, &RxMessage);//��ȡ����0����
  CAN_ID=RxMessage.StdId;      //��ȡ��׼֡��ʶ��
	//CAN_ID=RxMessage.ExtId;
	
  GetCAN[0]=RxMessage.Data[0]; //��ȡ����
  GetCAN[1]=RxMessage.Data[1];
  GetCAN[2]=RxMessage.Data[2];
  GetCAN[3]=RxMessage.Data[3];
  GetCAN[4]=RxMessage.Data[4];
  GetCAN[5]=RxMessage.Data[5];
  GetCAN[6]=RxMessage.Data[6];
  GetCAN[7]=RxMessage.Data[7];
	
  CAN_Deal(GetCAN,CAN_ID);
  
  CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);//������жϱ�־
  
}

extern u8 Ring;
extern u8 DetectorID;
extern u8 Token;
void CAN_Deal(u8 array[],u8 id)
{
  if((id&0xF0)==0xD0)//̽������Ϣ
    return;
  if((id&0xF0)==0xC0)//��������Ϣ
  {
    if(GetCAN[0]==0xF6)//������Ϣ
      if(GetCAN[4]==DetectorID)
        Token=1;
  }
}

void CAN_Send(u8 *data,u8 can_id)
{
  CanTxMsg TxMessage;
  
  TxMessage.StdId=can_id;
  TxMessage.RTR = CAN_RTR_DATA;//��׼����֡
  TxMessage.IDE = CAN_ID_STD ; //ID
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
