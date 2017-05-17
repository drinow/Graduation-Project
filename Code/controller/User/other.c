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
  u8 TCPCache[15];
  u8 cnt=0,i=0;

  TCPCache[cnt++]=0xAA;//Head
  TCPCache[cnt++]=0xAA;
  TCPCache[cnt++]=0x0A;//Len
  TCPCache[cnt++]=0x11;//Func
  TCPCache[cnt++]=0;//起火序号 ，不用了，随便填
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
  u8 TCPCache[15];
  u8 cnt=0,i=0;

  TCPCache[cnt++]=0xAA;//Head
  TCPCache[cnt++]=0xAA;
  TCPCache[cnt++]=0x0;//Len
  TCPCache[cnt++]=0x15;//功能代号
  TCPCache[cnt++]=0xC1;//所属控制器
  TCPCache[cnt++]=0xD1;//探测器编号
  TCPCache[cnt++]=Detector_1F[1].type;
  TCPCache[cnt++]=Detector_1F[1].temp;
  TCPCache[cnt++]=0xD2;//探测器编号
  TCPCache[cnt++]=Detector_1F[2].type;
  TCPCache[cnt++]=Detector_1F[2].temp;
  TCPCache[cnt++]=0xD3;//探测器编号
  TCPCache[cnt++]=Detector_1F[3].type;
  TCPCache[cnt++]=Detector_1F[3].temp;
  
  TCPCache[2]=cnt-3;
  for(i=cnt;i<15;)
    TCPCache[i++]=0;
  send(SOCK_TCPS,TCPCache,15);
}

void TCP_SendDetector2F(void)
{
  u8 TCPCache[15];
  u8 cnt=0,i=0;

  TCPCache[cnt++]=0xAA;//Head
  TCPCache[cnt++]=0xAA;
  TCPCache[cnt++]=0x0;//Len
  TCPCache[cnt++]=0x15;//功能代号
  TCPCache[cnt++]=0xC2;//所属控制器
  TCPCache[cnt++]=0xD1;//探测器编号
  TCPCache[cnt++]=Detector_2F[1].type;
  TCPCache[cnt++]=Detector_2F[1].temp;
  TCPCache[cnt++]=0xD2;//探测器编号
  TCPCache[cnt++]=Detector_2F[2].type;
  TCPCache[cnt++]=Detector_2F[2].temp;
  TCPCache[cnt++]=0xD3;//探测器编号
  TCPCache[cnt++]=Detector_2F[3].type;
  TCPCache[cnt++]=Detector_2F[3].temp;
  
  TCPCache[2]=cnt-3;
  for(i=cnt;i<15;)
    TCPCache[i++]=0;
  send(SOCK_TCPS,TCPCache,15);
}

void TCP_SendDetector3F(void)
{
  u8 TCPCache[15];
  u8 cnt=0,i=0;

  TCPCache[cnt++]=0xAA;//Head
  TCPCache[cnt++]=0xAA;
  TCPCache[cnt++]=0x0;//Len
  TCPCache[cnt++]=0x15;//功能代号
  TCPCache[cnt++]=0xC3;//所属控制器
  TCPCache[cnt++]=0xD1;//探测器编号
  TCPCache[cnt++]=Detector_3F[1].type;
  TCPCache[cnt++]=Detector_3F[1].temp;
  TCPCache[cnt++]=0xD2;//探测器编号
  TCPCache[cnt++]=Detector_3F[2].type;
  TCPCache[cnt++]=Detector_3F[2].temp;
  TCPCache[cnt++]=0xD3;//探测器编号
  TCPCache[cnt++]=Detector_3F[3].type;
  TCPCache[cnt++]=Detector_3F[3].temp;
  
  TCPCache[2]=cnt-3;
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
        if(RxMessage->Data[2]!=0) Fired[(RxMessage->Data[1]&0x0F)-1]=1;
        else Fired[(RxMessage->Data[1]&0x0F)-1]=0;
      }
      if((RxMessage->Data[4])==0xC2)//属于2层的探测器消息
      {
        Detector_2F[(RxMessage->Data[1]&0x0F)].type=RxMessage->Data[2];
        Detector_2F[(RxMessage->Data[1]&0x0F)].temp=RxMessage->Data[3];
        if(RxMessage->Data[2]!=0) Fired[3+(RxMessage->Data[1]&0x0F)-1]=1;
        else Fired[(RxMessage->Data[1]&0x0F)-1]=0;
      }
      if((RxMessage->Data[4])==0xC3)//属于3层的探测器消息
      {
        Detector_3F[(RxMessage->Data[1]&0x0F)].type=RxMessage->Data[2];
        Detector_3F[(RxMessage->Data[1]&0x0F)].temp=RxMessage->Data[3];
        if(RxMessage->Data[2]!=0) Fired[6+(RxMessage->Data[1]&0x0F)-1]=1;
        else Fired[(RxMessage->Data[1]&0x0F)-1]=0;
      }
      if(RxMessage->Data[2]!=0)
      { 
        RestFire=1;//其它层起火
        FiredNum=RxMessage->Data[4]*256+RxMessage->Data[1];
      }
    }
    if(RxMessage->Data[0]==0xF4)//警铃消息
    {
      Alarm.ID=RxMessage->Data[1];
      Alarm.State=RxMessage->Data[2];
    } 
    if(RxMessage->Data[0]==0xF5)//复位消息
    {
      if(RxMessage->Data[2]==1) Reset=1;
    } 
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
    if(Door.State==0)//全开
    {StepMotorCtrl(NUM1,FRD,500);}
    if(Door.State==1)//全关
    {StepMotorCtrl(NUM1,REV,500);}
    if(Door.State==2)//半开
    {}
  }
  if(Door.ID==2)
  {
    if(Door.State==0)
    {StepMotorCtrl(NUM2,FRD,500);}
    if(Door.State==1)
    {StepMotorCtrl(NUM2,REV,500);}
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

//疏散指示
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

//CAN总线广播数据
void CAN_Broadcast(void)
{
  u8 cache[8]={0};
  u8 broadcastID=0xC0;
  static u8 Detect_Num=0xD1;//每次广播一个探测器数据，分三次广播完毕

  //广播探测器
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
  
  //广播复位信号
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

u8 PwrRxBuffer[8];
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
  
  if((2+data_cnt)==8)//数据定位变量复位，并处理数据
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
    if(PwrRxBuffer[4]!=0)LocalFire=1;//本地起火
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
