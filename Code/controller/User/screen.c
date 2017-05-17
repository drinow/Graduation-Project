#include "screen.h"

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

void Screen_Config(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		
		/* config USART2 clock */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
		/* USART2 GPIO config */
		/* Configure USART2 Tx (PA.02) as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		/* Configure USART2 Rx (PA.03) as input floating */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
			
		/* USART1 mode config */
		USART_InitStructure.USART_BaudRate = 115200;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No ;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART2, &USART_InitStructure); 
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
		USART_Cmd(USART2, ENABLE);
		
}




void SC_SendTime(void)
{
  u8 len=0;
  u8 SC_Cache[25];
  u16 temp=SC_TIME;
  SC_Cache[len++]=0xA5;//HEADER 
  SC_Cache[len++]=0x5A;
  SC_Cache[len++]=0x00;//LEN,filled later.

  SC_Cache[len++]=SC_WDAT;//MODE
  
  SC_Cache[len++]=BYTE1(temp);//ADDRESS is equal to type!
  SC_Cache[len++]=BYTE0(temp);
   
  SC_Cache[len++]='2';
  SC_Cache[len++]='0';
  SC_Cache[len++]='1';
  SC_Cache[len++]='7';
  SC_Cache[len++]='-';
  SC_Cache[len++]='0'+((GetTime.month&0xF0)>>4);
  SC_Cache[len++]='0'+(GetTime.month&0x0F);
  SC_Cache[len++]='-';
  SC_Cache[len++]='0'+((GetTime.date&0xF0)>>4);
  SC_Cache[len++]='0'+(GetTime.date&0x0F);
  SC_Cache[len++]=' ';
  SC_Cache[len++]='0'+((GetTime.hour&0xF0)>>4);
  SC_Cache[len++]='0'+(GetTime.hour&0x0F);
  SC_Cache[len++]=':';
  SC_Cache[len++]='0'+((GetTime.min&0xF0)>>4);
  SC_Cache[len++]='0'+(GetTime.min&0x0F);
  SC_Cache[len++]=':';
  SC_Cache[len++]='0'+((GetTime.sec&0xF0)>>4);
  SC_Cache[len++]='0'+(GetTime.sec&0x0F);
  
  SC_Cache[2]=len-3;//fill the lenth
  
  len++;
  USART_SendString(USART2,&SC_Cache[0],len);
}

void SC_SendFireICON(u16 pointer,u8 value)
{
  u8 len=0;
  u8 SC_Cache[25];
  u16 temp=pointer;
  SC_Cache[len++]=0xA5;//HEADER 
  SC_Cache[len++]=0x5A;
  SC_Cache[len++]=0x00;//LEN,filled later.

  SC_Cache[len++]=SC_WDAT;//MODE
  
  SC_Cache[len++]=BYTE1(temp);//ADDRESS is equal to type!
  SC_Cache[len++]=BYTE0(temp);
   
  SC_Cache[len++]=0;
  
  if(value==1)
    SC_Cache[len++]=ICON_RED;
  else
    SC_Cache[len++]=ICON_GREEN;
  
  SC_Cache[2]=len-3;//fill the lenth
  
  len++;
  USART_SendString(USART2,&SC_Cache[0],len);
}


//发送逃跑路线
//num:第num个探测器 范围0-8
//value：探测器是否报警
void SC_SendRUNICON(u8 num,u8 value)
{
  u8 len=0;
  u8 SC_Cache[25];
  u16 temp=0;
  u8 runway=0;
  if(num<3)temp=SC_1FWAY;
  if(num<6)temp=SC_2FWAY;
  if(num<9)temp=SC_3FWAY;
  
  switch(num)
  {
    case 0:runway=ICON_RUN11;break;
    case 1:runway=ICON_RUN12;break;
    case 2:runway=ICON_RUN13;break;
    case 3:runway=ICON_RUN21;break;
    case 4:runway=ICON_RUN22;break;
    case 5:runway=ICON_RUN23;break;
    case 6:runway=ICON_RUN31;break;
    case 7:runway=ICON_RUN32;break;
    case 8:runway=ICON_RUN33;break;
    default :runway=ICON_CLR;break;
  }
  
  SC_Cache[len++]=0xA5;//HEADER 
  SC_Cache[len++]=0x5A;
  SC_Cache[len++]=0x00;//LEN,filled later.

  SC_Cache[len++]=SC_WDAT;//MODE
  
  SC_Cache[len++]=BYTE1(temp);//ADDRESS is equal to type!
  SC_Cache[len++]=BYTE0(temp);
   
  SC_Cache[len++]=0;
  
  if(value==1)
    SC_Cache[len++]=runway;
  else
    SC_Cache[len++]=ICON_CLR;
  
  SC_Cache[2]=len-3;//fill the lenth
  
  len++;
  USART_SendString(USART2,&SC_Cache[0],len);
}


void SC_SendFirePoint(void)
{
  u8 i=0;
  u16 j=SC_DETECTOR11;
  for(i=0;i<9;i++)
  {
    SC_SendFireICON(j,Fired[i]);
    j++;j++;
    if(Fired[i]==1)
      SC_SendRUNICON(i,SC_ESCAPE);
  }
}

void SC_SendIPAddr(void)
{
  u8 len=0;
  u8 SC_Cache[25];
  u16 temp=SC_ADDRESS;
  
  SC_Cache[len++]=0xA5;//HEADER 
  SC_Cache[len++]=0x5A;
  SC_Cache[len++]=0x00;//LEN,filled later.

  SC_Cache[len++]=SC_WDAT;//MODE
  
  SC_Cache[len++]=BYTE1(temp);//ADDRESS is equal to type!
  SC_Cache[len++]=BYTE0(temp);

  SC_Cache[len++]='0'+local_ip[0] / 100 % 10;
  SC_Cache[len++]='0'+local_ip[0] / 10 % 10;
  SC_Cache[len++]='0'+local_ip[0] / 1 % 10;
  SC_Cache[len++]='.';
  
  SC_Cache[len++]='0'+local_ip[1] / 100 % 10;
  SC_Cache[len++]='0'+local_ip[1] / 10 % 10;
  SC_Cache[len++]='0'+local_ip[1] / 1 % 10;
  SC_Cache[len++]='.';
  
  SC_Cache[len++]='0'+local_ip[2] / 100 % 10;
  SC_Cache[len++]='0'+local_ip[2] / 10 % 10;
  SC_Cache[len++]='0'+local_ip[2] / 1 % 10;
  SC_Cache[len++]='.';
  
  SC_Cache[len++]='0'+local_ip[3] / 100 % 10;
  SC_Cache[len++]='0'+local_ip[3] / 10 % 10;
  SC_Cache[len++]='0'+local_ip[3] / 1 % 10;
  
  SC_Cache[2]=len-3;//fill the lenth
  
  len++;
  USART_SendString(USART2,&SC_Cache[0],len);
}


void SC_SendPort(void)
{
  u8 len=0;
  u8 SC_Cache[25];

  u16 temp=SC_PORT;
  
  SC_Cache[len++]=0xA5;//HEADER 
  SC_Cache[len++]=0x5A;
  SC_Cache[len++]=0x00;//LEN,filled later.

  SC_Cache[len++]=SC_WDAT;//MODE
  
  SC_Cache[len++]=BYTE1(temp);//ADDRESS is equal to type!
  SC_Cache[len++]=BYTE0(temp);

  SC_Cache[len++]='0'+local_port / 1000 % 10;
  SC_Cache[len++]='0'+local_port / 100 % 10;
  SC_Cache[len++]='0'+local_port / 10 % 10;
  SC_Cache[len++]='0'+local_port / 1 % 10;
  
  SC_Cache[2]=len-3;//fill the lenth
  
  len++;
  USART_SendString(USART2,&SC_Cache[0],len);
}

void SC_SendDHCP(void)
{
  u8 len=0;
  u8 SC_Cache[25];

  u16 temp=SC_DHCP;
  
  SC_Cache[len++]=0xA5;//HEADER 
  SC_Cache[len++]=0x5A;
  SC_Cache[len++]=0x00;//LEN,filled later.

  SC_Cache[len++]=SC_WDAT;//MODE
  
  SC_Cache[len++]=BYTE1(temp);//ADDRESS is equal to type!
  SC_Cache[len++]=BYTE0(temp);

  if(dhcp_ok==0)
  {
    SC_Cache[len++]=0xce;
    SC_Cache[len++]=0xb4;
    SC_Cache[len++]=0xc1;
    SC_Cache[len++]=0xac;
    SC_Cache[len++]=0xbd;
    SC_Cache[len++]=0xd3;
  }
  else
  {
    SC_Cache[len++]=0xd2;
    SC_Cache[len++]=0xd1;
    SC_Cache[len++]=0xc1;
    SC_Cache[len++]=0xac;
    SC_Cache[len++]=0xbd;
    SC_Cache[len++]=0xd3;
  }
  
  SC_Cache[2]=len-3;//fill the lenth
  
  len++;
  USART_SendString(USART2,&SC_Cache[0],len);
}

void SC_SendID(void)
{
  u8 len=0;
  u8 SC_Cache[25];

  u16 temp=SC_ID;
  
  SC_Cache[len++]=0xA5;//HEADER 
  SC_Cache[len++]=0x5A;
  SC_Cache[len++]=0x00;//LEN,filled later.

  SC_Cache[len++]=SC_WDAT;//MODE
  
  SC_Cache[len++]=BYTE1(temp);//ADDRESS is equal to type!
  SC_Cache[len++]=BYTE0(temp);

  SC_Cache[len++]=0xb1;//本
  SC_Cache[len++]=0xbe;
  SC_Cache[len++]=0xbb;//机
  SC_Cache[len++]=0xfa;
  SC_Cache[len++]='I';
  SC_Cache[len++]='D';
  SC_Cache[len++]=':';
  SC_Cache[len++]='0';
  SC_Cache[len++]='x';
  SC_Cache[len++]='0'+(Ctrl_ID>>4);
  SC_Cache[len++]='0'+(Ctrl_ID&0x0F);
  SC_Cache[2]=len-3;//fill the lenth
  
  len++;
  USART_SendString(USART2,&SC_Cache[0],len);
}

void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{ 	
	    //ch = USART1->DR;
			USART_ReceiveData(USART2);
	} 
}

