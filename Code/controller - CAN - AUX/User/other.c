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

void TCP_SendFire(u8 detector,u8 controller)
{
  u8 TCPCache[15]={0};
  u8 cnt=0,i=0;

  TCPCache[cnt++]=0xAA;//Head
  TCPCache[cnt++]=0xAA;
  TCPCache[cnt++]=0x0A;//Len
  TCPCache[cnt++]=0x11;//Func
  TCPCache[cnt++]=0;//������ �������ˣ������
  TCPCache[cnt++]=controller;//floor
  TCPCache[cnt++]=detector;//detector num
  
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


void TCP_SendDetector1F(void)
{
  u8 TCPCache[15]={0};
  u8 cnt=0,i=0;

  TCPCache[cnt++]=0xAA;//Head
  TCPCache[cnt++]=0xAA;
  TCPCache[cnt++]=0x0;//Len
  TCPCache[cnt++]=0x15;//���ܴ���
  TCPCache[cnt++]=0xC1;//����������
  TCPCache[cnt++]=0xD1;//̽�������
  TCPCache[cnt++]=Detector_1F[1].type;
  TCPCache[cnt++]=Detector_1F[1].temp;
  TCPCache[cnt++]=0xD2;//̽�������
  TCPCache[cnt++]=Detector_1F[2].type;
  TCPCache[cnt++]=Detector_1F[2].temp;
  TCPCache[cnt++]=0xD3;//̽�������
  TCPCache[cnt++]=Detector_1F[3].type;
  TCPCache[cnt++]=Detector_1F[3].temp;
  
  TCPCache[2]=cnt-3;
  for(i=cnt;i<15;)
    TCPCache[i++]=0;
  send(SOCK_TCPS,TCPCache,15);
}

void TCP_SendDetector2F(void)
{
  u8 TCPCache[15]={0};
  u8 cnt=0,i=0;

  TCPCache[cnt++]=0xAA;//Head
  TCPCache[cnt++]=0xAA;
  TCPCache[cnt++]=0x0;//Len
  TCPCache[cnt++]=0x15;//���ܴ���
  TCPCache[cnt++]=0xC2;//����������
  TCPCache[cnt++]=0xD1;//̽�������
  TCPCache[cnt++]=Detector_2F[1].type;
  TCPCache[cnt++]=Detector_2F[1].temp;
  TCPCache[cnt++]=0xD2;//̽�������
  TCPCache[cnt++]=Detector_2F[2].type;
  TCPCache[cnt++]=Detector_2F[2].temp;
  TCPCache[cnt++]=0xD3;//̽�������
  TCPCache[cnt++]=Detector_2F[3].type;
  TCPCache[cnt++]=Detector_2F[3].temp;
  
  TCPCache[2]=cnt-3;
  for(i=cnt;i<15;)
    TCPCache[i++]=0;
  send(SOCK_TCPS,TCPCache,15);
}

void TCP_SendDetector3F(void)
{
  u8 TCPCache[15]={0};
  u8 cnt=0,i=0;

  TCPCache[cnt++]=0xAA;//Head
  TCPCache[cnt++]=0xAA;
  TCPCache[cnt++]=0x0;//Len
  TCPCache[cnt++]=0x15;//���ܴ���
  TCPCache[cnt++]=0xC3;//����������
  TCPCache[cnt++]=0xD1;//̽�������
  TCPCache[cnt++]=Detector_3F[1].type;
  TCPCache[cnt++]=Detector_3F[1].temp;
  TCPCache[cnt++]=0xD2;//̽�������
  TCPCache[cnt++]=Detector_3F[2].type;
  TCPCache[cnt++]=Detector_3F[2].temp;
  TCPCache[cnt++]=0xD3;//̽�������
  TCPCache[cnt++]=Detector_3F[3].type;
  TCPCache[cnt++]=Detector_3F[3].temp;
  
  TCPCache[2]=cnt-3;
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


void DealCAN(CanRxMsg* RxMessage)
{
  if( (RxMessage->StdId&0xF0)==0xC0 )//��������Ϣ
  {
    if(RxMessage->Data[0]==0xF0)//���̷����Ϣ
    {
      Fan.ID=RxMessage->Data[1];
      Fan.State=RxMessage->Data[2];
    }
    if(RxMessage->Data[0]==0xF1)//��������Ϣ
    {
      Door.ID=RxMessage->Data[1];
      Door.State=RxMessage->Data[2];
    }
    if(RxMessage->Data[0]==0xF2)//ˮ����Ϣ
    {
      Pump.ID=RxMessage->Data[1];
      Pump.State=RxMessage->Data[2];
    }  
    if(RxMessage->Data[0]==0xF4)//������Ϣ
    {
      Alarm.ID=RxMessage->Data[1];
      Alarm.State=RxMessage->Data[2];
    } 
    if(RxMessage->Data[0]==0xF5)//��λ��Ϣ
    {
      if(RxMessage->Data[2]==1) Reset=1;
      else Reset=0;
    } 
  }
  if((RxMessage->StdId&0xF0)==0xD0)//̽������Ϣ
  {
    if(RxMessage->Data[0]==0xF3)//̽������Ϣ
    {
      if((RxMessage->StdId&0x0F)<4)//����1���̽������Ϣ
      {
        if(Detector_1F[(RxMessage->StdId&0x0F)].type==0)//��1������
          Detector_1F[(RxMessage->StdId&0x0F)].type=RxMessage->Data[2];
        Detector_1F[(RxMessage->StdId&0x0F)].temp=RxMessage->Data[3];
        if(RxMessage->Data[2]!=0) Fired[(RxMessage->StdId&0x0F)-1]=1;
        else Fired[(RxMessage->Data[1]&0x0F)-1]=0;
      }
      else if((RxMessage->StdId&0x0F)<7)//����2���̽������Ϣ
      {
        if(Detector_2F[(RxMessage->StdId&0x0F)-3].type==0)
          Detector_2F[(RxMessage->StdId&0x0F)-3].type=RxMessage->Data[2];
        Detector_2F[(RxMessage->StdId&0x0F)-3].temp=RxMessage->Data[3];
        if(RxMessage->Data[2]!=0) Fired[3+(RxMessage->StdId&0x0F)-1-3]=1;
        else Fired[(RxMessage->Data[1]&0x0F)-1-3]=0;
      }
      else if((RxMessage->StdId&0x0F)<10)//����3���̽������Ϣ
      {
        if(Detector_3F[(RxMessage->StdId&0x0F)-6].type==0)
          Detector_3F[(RxMessage->StdId&0x0F)-6].type=RxMessage->Data[2];
        Detector_3F[(RxMessage->StdId&0x0F)-6].temp=RxMessage->Data[3];
        if(RxMessage->Data[2]!=0) Fired[6+(RxMessage->StdId&0x0F)-1-6]=1;
        else Fired[(RxMessage->Data[1]&0x0F)-1-6]=0;
      }
    }
  }
}

void CheckFire(void)
{
  u8 i=0;
  u8 sort=0;//�Ż��λ��������
  for(i=0;i<9;i++)
  {
    if(Fired[i])
    {
      if(i<3)
      {
        sort=sort|0x01;
        if(Ctrl_ID==0xC1)
        {
          Door.ID=1;Door.State=1;
          Alarm.ID=1;Alarm.State=1;
          Fan.ID=1;Fan.State=1;
          Pump.ID=1;Pump.State=1;
        }
        else
        {
          Alarm.ID=1;Alarm.State=1;
          Fan.ID=1;Fan.State=1;
        }
      }
      else if(i<6)
      {
        sort=sort|0x02;
        if(Ctrl_ID==0xC2)
        {
          Door.ID=1;Door.State=1;
          Alarm.ID=1;Alarm.State=1;
          Fan.ID=1;Fan.State=1;
          Pump.ID=1;Pump.State=1;
        }
        else
        {
          Alarm.ID=1;Alarm.State=1;
          Fan.ID=1;Fan.State=1;
        }
      }
      else if(i<9)
      {
        sort=sort|0x04;
        if(Ctrl_ID==0xC3)
        {
          Door.ID=1;Door.State=1;
          Alarm.ID=1;Alarm.State=1;
          Fan.ID=1;Fan.State=1;
          Pump.ID=1;Pump.State=1;
        }
        else
        {
          Alarm.ID=1;Alarm.State=1;
          Fan.ID=1;Fan.State=1;
        }
      }
    }
  }
  switch(Ctrl_ID)
  {
    case 0xC1:
      if(sort&0x01)LocalFire=1;
      if(sort&0x06)RestFire=1;
      if(sort==0)LocalFire=RestFire=0;
      break;
    case 0xC2:
      if(sort&0x02)LocalFire=1;
      if(sort&0x05)RestFire=1;
      if(sort==0)LocalFire=RestFire=0;
      break;
    case 0xC3:
      if(sort&0x04)LocalFire=1;
      if(sort&0x03)RestFire=1;
      if(sort==0)LocalFire=RestFire=0;
      break;
    default:break;
  }
}

void DealFan(void)
{
  if(Fan.ID==1)
  {
    if(Fan.State==0)
    {FanCtrl(STOP);}
    if(Fan.State==1)
    {FanCtrl(RUN);}
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
    if(Door.State==0)//ȫ��
    {StepMotorCtrl(NUM1,FRD,1400);}
    if(Door.State==1)//ȫ��
    {StepMotorCtrl(NUM1,REV,1400);}
    if(Door.State==2)//�뿪
    {}
  }
  if(Door.ID==2)
  {
    if(Door.State==0)
    {StepMotorCtrl(NUM2,FRD,1400);}
    if(Door.State==1)
    {StepMotorCtrl(NUM2,REV,1400);}
    if(Door.State==2)
    {}
  }
}

void DealPump(void)
{
  if(Pump.ID==1)
  {
    if(Pump.State==0)
    {WaterPumpCtrl(STOP);}
    if(Pump.State==1)
    {WaterPumpCtrl(RUN);}
  }
  if(Pump.ID==2)
  {
    if(Pump.State==0)
    {}
    if(Pump.State==1)
    {}
  }
}

void DealAlarm(void)
{
  if(wave&0x08)
  {
    AlarmCtrl(RUN);
  }
  else
  {
    AlarmCtrl(STOP);
  }
}

//��ɢָʾ
void Evacuate(void)
{
  
}

void DealActuator(void)
{
  if(LocalFire)
  {
    DealDoor();
    DealPump();
  }
  DealFan();
  DealAlarm();
  Evacuate();
}

//�ַ�����
void CAN_TokenCtl(void)
{
  static u8 Token=0xD0;
  u8 temp=0;
  u8 cache[8]={0};
  cache[0]=0xF6;
  cache[4]=Token;
  temp=0xF6+Token;
  cache[7]=temp;
  
  Token++;
  if(Token==0xDA)Token=0xD0;
  CAN_Send(cache,Ctrl_ID);
}

//CAN���߹㲥����
void CAN_Broadcast(void)
{
  u8 cache[8]={0};
  u8 broadcastID=0xC0;
  static u8 Detect_Num=0xD1;//ÿ�ι㲥һ��̽�������ݣ������ι㲥���

  //�㲥̽����
  switch(Ctrl_ID)//����IDѰ�ұ���̽�������������ڴ�
  {
    case 0xC1:cache[0]=0xF3;       cache[1]=Detect_Num;
              cache[2]=Detector_1F[(Detect_Num&0x0F)].type;
              cache[3]=Detector_1F[(Detect_Num&0x0F)].temp;
              cache[4]=Ctrl_ID;    break;
    
    case 0xC2:cache[0]=0xF3;       cache[1]=Detect_Num;
              cache[2]=Detector_2F[(Detect_Num&0x0F)].type;
              cache[3]=Detector_2F[(Detect_Num&0x0F)].temp;
              cache[4]=Ctrl_ID;    break;
    
    case 0xC3:cache[0]=0xF3;       cache[1]=Detect_Num;
              cache[2]=Detector_3F[(Detect_Num&0x0F)].type;
              cache[3]=Detector_3F[(Detect_Num&0x0F)].temp;
              cache[4]=Ctrl_ID;    break;
    default:break;
  }
  
  Detect_Num++;
  if(Detect_Num==0xD4)Detect_Num=0xD1;
  
  CAN_Send(&cache[0],broadcastID);
  
  //�㲥��λ�ź�
  if(Reset)
  {
    memset(cache,0,sizeof(cache));
    cache[0]=0xF5;
    cache[1]=0;
    cache[2]=1;
    
    memset(Detector_1F,0,sizeof(Detector_1F));
    memset(Detector_1F,0,sizeof(Detector_2F));
    memset(Detector_1F,0,sizeof(Detector_2F));
    RestFire=LocalFire=0;
    Reset=0;
  }
  CAN_Send(&cache[0],broadcastID);
}

u8 PwrRxBuffer[8]={0};
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
  
  if((2+data_cnt)==8)//���ݶ�λ������λ������������
  {
    data_cnt=RxState=0;
    switch(Ctrl_ID)
    {
      case 0xC1:Detector_1F[(PwrRxBuffer[3]&0x0F)].type=PwrRxBuffer[4];
                Detector_1F[(PwrRxBuffer[3]&0x0F)].temp=PwrRxBuffer[5];break;
      case 0xC2:Detector_2F[(PwrRxBuffer[3]&0x0F)].type=PwrRxBuffer[4];
                Detector_2F[(PwrRxBuffer[3]&0x0F)].temp=PwrRxBuffer[5];break;
      case 0xC3:Detector_3F[(PwrRxBuffer[3]&0x0F)].type=PwrRxBuffer[4];
                Detector_3F[(PwrRxBuffer[3]&0x0F)].temp=PwrRxBuffer[5];break;
      default:break;
    }
    if(PwrRxBuffer[4]==1)LocalFire=1;//�������
  }
  
}

void PwrTokenCtrl(void)
{
  u8 cache[8]={0};
  u8 cnt=0;
  cache[cnt++]=0xDD;
  cache[cnt++]=0xDD;
  cache[cnt++]=0x10;
  cache[cnt++]=Token;
  cache[cnt++]=0;
  cache[cnt++]=0;
  cache[cnt++]=0;
  cache[cnt++]=0;

  USART_SendString(USART1,cache,8);
  
  Token++;
  if(Token==0xD4)Token=0xD1;
}
