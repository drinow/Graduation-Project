#include "other.h"

void TCPS_Config(void)
{
  u32 timingOLD=0,timingNEW=0;
	gpio_for_w5500_config();	         	/*��ʼ��MCU�������*/
	reset_w5500();					            /*Ӳ��λW5500*/
	set_w5500_mac();                    /*����MAC��ַ*/
  socket_buf_init(txsize, rxsize);    /*��ʼ��8��Socket�ķ��ͽ��ջ����С*/

  timingOLD=tick;
  while(dhcp_ok==0)
  {
    do_dhcp();                        /*DHCP���Գ���*/
    timingNEW=tick;
    if((timingNEW-timingOLD)>2000)break;//��ʱ�˳�
  }
  
	if(dhcp_ok==1)
  {
    printf("W5500��ΪTCP Server�������������ȴ�TCP Client���� \r\n");
    printf("W5500�����˿�Ϊ�� %d \r\n",local_port);
  }
  else
    printf("=====DHCP initialization failed=====\r\n");
  
  SC_SendID();//Ҫ��2�Σ������ǵ�һ�η�����©��һ������
  SC_SendID();
  SC_SendIPAddr();
  SC_SendPort();
  SC_SendDHCP();
  
}

void TCP_DealData(u8 *str,u16 len)
{
  if(str[0]==0xAA&&str[1]==0xAA)
  {
    if(str[3]==0x01)//Write Time
    {
      SetTime.year  =str[4];
      SetTime.month =str[5];
      SetTime.date  =str[6];
      SetTime.hour  =str[7];
      SetTime.min   =str[8];
      SetTime.sec   =str[9];
      DS3231_WriteTime(&SetTime);
//      printf("min:%x sec:%x\r\n",SetTime.min,SetTime.sec);
    }
  }
}

void TCP_KeepAlive(void)
{
  u8 TCPCache[15];
  u8 cnt=0;
  TCPCache[cnt++]=0x31;
  TCPCache[cnt++]=0x32;
  TCPCache[cnt++]=0x33;
  TCPCache[cnt++]=0x31;
  TCPCache[cnt++]=0x32;
  TCPCache[cnt++]=0x33;
  TCPCache[cnt++]=0x31;
  TCPCache[cnt++]=0x32;
  TCPCache[cnt++]=0x33;
  TCPCache[cnt++]=0x31;
  TCPCache[cnt++]=0x32;
  TCPCache[cnt++]=0x33;
  TCPCache[cnt++]=0x31;
  TCPCache[cnt++]=0x32;
  TCPCache[cnt++]=0x33;
  send(SOCK_TCPS,TCPCache,15);
}

void TCP_SendFire(void)
{
  u8 TCPCache[15];
  u8 cnt=0,i=0;
  static u8 j=0;
  TCPCache[cnt++]=0xAA;//Head
  TCPCache[cnt++]=0xAA;
  TCPCache[cnt++]=0x0A;//Len
  TCPCache[cnt++]=0x11;//Func
  TCPCache[cnt++]=j++;//Serial Num
  TCPCache[cnt++]=0x02;//floor
  TCPCache[cnt++]=0x12;//detector num
  
  TCPCache[cnt++]=GetTime.year;
  TCPCache[cnt++]=GetTime.month;
  TCPCache[cnt++]=GetTime.date ;
  TCPCache[cnt++]=GetTime.hour ;
  TCPCache[cnt++]=GetTime.min;
  TCPCache[cnt++]=GetTime.sec;
  for(i=cnt;i<15;)
    TCPCache[i++]=0;
  send(SOCK_TCPS,TCPCache,15);
}

//Ѱ��̽�����Ƿ��¼
//�����Ƿ�youƥ�����
//����¼�򷵻����������±�
//���򷵻�0
u8 search(u8* array,u8 len,u8 data)
{
  u8 i=0;
  u8 index=0;
  for(i=0;i<len;i++)
  {
    if(array[i]==data) index=i;
  }
  return index;
}

//


void DealCAN(CanRxMsg* RxMessage)
{
  u8 ReceiveID=(RxMessage->StdId&0x0F)*16+RxMessage->Data[1];//ID=¥����Ϣ+�豸���
//  u8 index=0;
  if( ( (RxMessage->StdId) &0xF0 ) ==0xC0)//���Կ���������Ϣ
  {
    if(RxMessage->Data[0]==0xF0)//���̷����Ϣ
    {
      Fan.ID=ReceiveID;
      Fan.State=RxMessage->Data[2];
    }
    if(RxMessage->Data[0]==0xF1)//��������Ϣ
    {
      Door.ID=ReceiveID;
      Door.State=RxMessage->Data[2];
    }
    if(RxMessage->Data[0]==0xF2)//ˮ����Ϣ
    {
      Pump.ID=ReceiveID;
      Pump.State=RxMessage->Data[2];
    }  
    if(RxMessage->Data[0]==0xF3)//������Ϣ
    {
      {
//      index=search(&Detector_1F.ID[0],12,ReceiveID);
//      if(index==0)//̽����δ�Ǽ�
//      {
//        Detector_1F.ID[Detector_1F.Anum]=ReceiveID;
//        Detector_1F.type[Detector_1F.Anum]=RxMessage->Data[2];
//        Detector_1F.temp[Detector_1F.Anum]=RxMessage->Data[3];
//        if(Detector_1F.type[Detector_1F.Anum]!=0)FireAlarm=1;
//        Detector_1F.Anum++;
//      }
//      else//�Ѿ��Ǽǣ���ˢ������
//      {
//        Detector_1F.type[index]=RxMessage->Data[2];
//        Detector_1F.temp[index]=RxMessage->Data[3];
//      }
      }
      if((RxMessage->StdId&0x0F)==0x01)//1��
      {
        Detector_1F[RxMessage->Data[1]].type=RxMessage->Data[2];
        Detector_1F[RxMessage->Data[1]].temp=RxMessage->Data[3];
      }
      if((RxMessage->StdId&0x0F)==0x02)//2��
      {
        Detector_2F[RxMessage->Data[1]].type=RxMessage->Data[2];
        Detector_2F[RxMessage->Data[1]].temp=RxMessage->Data[3];
      }
      if((RxMessage->StdId&0x0F)==0x03)//3��
      {
        Detector_3F[RxMessage->Data[1]].type=RxMessage->Data[2];
        Detector_3F[RxMessage->Data[1]].temp=RxMessage->Data[3];
      }
      if(RxMessage->Data[2]!=0)FireAlarmFlag=1;
    }
  }
}


void DealFan(void)
{
  if(Fan.ID==1)
  {
    if(Fan.State==0)
    {}
    if(Fan.State==1)
    {}
  }
  if(Fan.ID==2)
  {
    if(Fan.State==0)
    {}
    if(Fan.State==1)
    {}
  }
}

void DealDoor(void)
{
  if(Door.ID==1)
  {
    if(Door.State==0)
    {}
    if(Door.State==1)
    {}
    if(Door.State==2)
    {}
  }
  if(Door.ID==2)
  {
    if(Door.State==0)
    {}
    if(Door.State==1)
    {}
    if(Door.State==2)
    {}
  }
}

void DealPump(void)
{
  if(Pump.ID==1)
  {
    if(Pump.State==0)
    {}
    if(Pump.State==1)
    {}
  }
  if(Pump.ID==2)
  {
    if(Pump.State==0)
    {}
    if(Pump.State==1)
    {}
  }
}

void DealActuator(void)
{
  DealFan();
  DealDoor();
  DealPump();
}
