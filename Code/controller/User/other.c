#include "other.h"

void TCPS_Config(void)
{
  u32 timingOLD=0,timingNEW=0;
	gpio_for_w5500_config();	         	/*初始化MCU相关引脚*/
	reset_w5500();					            /*硬复位W5500*/
	set_w5500_mac();                    /*配置MAC地址*/
  socket_buf_init(txsize, rxsize);    /*初始化8个Socket的发送接收缓存大小*/

  timingOLD=tick;
  while(dhcp_ok==0)
  {
    do_dhcp();                        /*DHCP测试程序*/
    timingNEW=tick;
    if((timingNEW-timingOLD)>2000)break;//超时退出
  }
  
	if(dhcp_ok==1)
  {
    printf("W5500作为TCP Server，建立侦听，等待TCP Client连接 \r\n");
    printf("W5500监听端口为： %d \r\n",local_port);
  }
  else
    printf("=====DHCP initialization failed=====\r\n");
  
  SC_SendID();//要发2次，可能是第一次发送又漏第一个数据
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

//寻找探测器是否记录
//数组是否you匹配的数
//若记录则返回所在数组下标
//否则返回0
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
  if(  (RxMessage->StdId==0xC0) || (RxMessage->StdId==Ctrl_ID) )//广播的消息或者发给本控制器的消息
  {
    if(RxMessage->Data[0]==0xF0)//排烟风机消息
    {
      Fan.ID=RxMessage->Data[1];
      Fan.State=RxMessage->Data[2];
    }
    if(RxMessage->Data[0]==0xF1)//卷帘门消息
    {
      Door.ID=RxMessage->Data[1];
      Door.State=RxMessage->Data[2];
    }
    if(RxMessage->Data[0]==0xF2)//水泵消息
    {
      Pump.ID=RxMessage->Data[1];
      Pump.State=RxMessage->Data[2];
    }  
    if(RxMessage->Data[0]==0xF3)//探测器消息
    {
      if((RxMessage->Data[4])==0xC1)//属于1层的探测器消息
      {
        Detector_1F[(RxMessage->Data[1]&0x0F)].type=RxMessage->Data[2];
        Detector_1F[(RxMessage->Data[1]&0x0F)].temp=RxMessage->Data[3];
      }
      if((RxMessage->Data[4])==0xC2)//属于2层的探测器消息
      {
        Detector_2F[(RxMessage->Data[1]&0x0F)].type=RxMessage->Data[2];
        Detector_2F[(RxMessage->Data[1]&0x0F)].temp=RxMessage->Data[3];
      }
      if((RxMessage->Data[4])==0xC3)//属于3层的探测器消息
      {
        Detector_3F[(RxMessage->Data[1]&0x0F)].type=RxMessage->Data[2];
        Detector_3F[(RxMessage->Data[1]&0x0F)].temp=RxMessage->Data[3];
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

//CAN总线广播数据
void CAN_Broadcast(void)
{
  u8 cache[8]={0};
  u8 broadcastID=0xC0;
  static u8 Detect_Num=0xD1;//每次广播一个探测器数据，分三次广播完毕

  switch(Ctrl_ID)//根据ID寻找本层探测器数据所在内存
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
}

u8 PwrRxBuffer[6];
//电力载波
void PwrCarrier_Deal(u8 data)
{
  static u8 RxState=0,data_cnt=0;
  
  if(RxState==0&&data==0xDD)//帧头
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
  else if(RxState==2)//开始接收
  {
    PwrRxBuffer[2+data_cnt++]=data;
  }
  else
			RxState = 0;
  
  if((2+data_cnt)==6)//数据定位变量复位，并处理数据
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
    }
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
  
  USART_SendString(USART1,cache,cnt);
  
  Token++;
  if(Token==0xC4)Token=0xC1;
}
