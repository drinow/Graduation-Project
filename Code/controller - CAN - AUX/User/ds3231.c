#include "ds3231.h"

/*设置EXTI功能去检测INT脚*/
void DS3231_INT_EXTI_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/* config the extiline clock and AFIO clock */
	RCC_APB2PeriphClockCmd(DS3231_INT_RCC | RCC_APB2Periph_AFIO,ENABLE);
												
	/* config the NVIC */
//	NVIC_Configuration();//其他地方处理了

	/* EXTI line gpio config*/	
  GPIO_InitStructure.GPIO_Pin = DS3231_INT_PIN;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // 上拉输入
  GPIO_Init(DS3231_INT_PORT, &GPIO_InitStructure);

	/* EXTI line mode config */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource4); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line4;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
}

void DS3231_GPIO_Init(void)
{
	//初始化IIC引脚并检查IIC设备
	i2c_CheckDevice(DS3231_ADDRESS);
}

//设置秒中断，输出到INT脚上
void DS3231_SetSecondAlarm(void)
{
	u8 ctrldata=0;
	u8 data=0x80;//为什么是0x80见手册Table2
	
	//使能Alarm1中断
	ctrldata=0x01;
	IIC_WriteBytes(&ctrldata,DS3231_ControlReg,1);
	
	//配置Alarm1模式
//	if(IIC_WriteBytes(&data,DS3231_A1SecReg,4)==1) return SUCCESS;
//	else return ERROR;
	IIC_WriteBytes(&data,DS3231_A1SecReg,1);//为啥不能连续写？
	IIC_WriteBytes(&data,DS3231_A1MinReg,1);
	IIC_WriteBytes(&data,DS3231_A1HourReg,1);
	IIC_WriteBytes(&data,DS3231_A1WeekReg,1);
}

/*配置DS3231*/
void DS3231_Config(void)
{
  DS3231_GPIO_Init();//config the iic
  DS3231_INT_EXTI_Init();//config the Int pin
  DS3231_SetSecondAlarm();//set second alarm
}

void DS3231_WriteTime(TIME_TypeDef* time)
{
	u8 setTime[7]={0};
  u8 i=0;
	setTime[0]=time->sec;
	setTime[1]=time->min;
	setTime[2]=time->hour;
	setTime[3]=time->week;
	setTime[4]=time->date;
	setTime[5]=time->month;
	setTime[6]=time->year;
//	if(IIC_WriteBytes(&setTime[0],DS3231_SecReg,7)==1) printf("Write OK\r\n");
//	else printf("Write Failed!\r\n");
  while(i<5)
  {
    i++;
    if(IIC_WriteBytes(&setTime[0],DS3231_SecReg,7)==1) 
    {
      printf("Write OK\r\n");
      break;
    }
	  else printf("Write Failed!\r\n");
  }
}

void DS3231_ReadTime(TIME_TypeDef* time)
{
	  u8 getTime[7]={0};
		if(IIC_ReadBytes(&getTime[0],DS3231_SecReg,7)==1) 
		{
//			printf("time:%x-%x-%x %x:%x:%x\r\n",getTime[6],getTime[5],getTime[4],getTime[2],getTime[1],getTime[0]);
			time->sec =getTime[0];
			time->min =getTime[1];
			time->hour =getTime[2];
			time->week =getTime[3];
			time->date =getTime[4];
			time->month =getTime[5];
			time->year =getTime[6];
		}
    #ifdef DEBUG
		else 
      printf("Read failed!\r\n");
    #endif
}

//读取指定寄存器数据
u8 DS3231_ReadReg(u8 Reg)
{
	u8 data;
	IIC_ReadBytes(&data,Reg,1);
	return data;
}

//向指定寄存器写入数据
u8 DS3231_WriteReg(u8 Reg,u8 data)
{
	if(IIC_WriteBytes(&data,Reg,1)==1) return SUCCESS;
	else return ERROR;
}

//读取所有寄存器
void DS3231_ReadAll(void)
{
	  u8 array[19]={0},i=0;
		if(IIC_ReadBytes(&array[0],DS3231_SecReg,19)==1) 
		{
			for(i=0;i<19;i++)
			printf("Reg0x%x:0x%x\r\n",i,array[i]);
		}
		else printf("Read failed!\r\n");
}

//
//以下2个函数适用于任何IIC设备
//

/*
*********************************************************************************************************
*	函 数 名: IIC_WriteBytes
*	功能说明: 向指定地址写入若干数据，采用页写操作提高写入效率
*	形    参：_usAddress : 起始地址
*			 _usSize : 数据长度，单位为字节
*			 _pWriteBuf : 存放要写的数据的缓冲区指针
*	返 回 值: 0 表示失败，1表示成功
*********************************************************************************************************
*/
uint8_t IIC_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i,m;
	uint16_t usAddr;
	

	usAddr = _usAddress;	
	for (i = 0; i < _usSize; i++)
	{
		/* 当发送第1个字节或是页面首地址时，需要重新发起启动信号和地址 */
		if (i == 0)
		{
			/*　第０步：发停止信号，启动内部写操作　*/
			i2c_Stop();
			
			/* 通过检查器件应答的方式，判断内部写操作是否完成, 一般小于 10ms 			
				CLK频率为200KHz时，查询次数为30次左右
			*/
			for (m = 0; m < 100; m++)
			{				
				/* 第1步：发起I2C总线启动信号 */
				i2c_Start();
				
				/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
				i2c_SendByte(DS3231_ADDRESS | I2C_WR);	/* 此处是写指令 */
				
				/* 第3步：发送一个时钟，判断器件是否正确应答 */
				if (i2c_WaitAck() == 0)
				{
					break;
				}
			}
			if (m  == 1000)
			{
				goto cmd_fail;	/* 写超时 */
			}
		
			/* 第4步：发送字节地址*/
			i2c_SendByte((uint8_t)usAddr);
			
			/* 第5步：发送ACK */
			if (i2c_WaitAck() != 0)
			{
				goto cmd_fail;	/* EEPROM器件无应答 */
			}
		}
	
		/* 第6步：开始写入数据 */
		i2c_SendByte(_pWriteBuf[i]);
	
		/* 第7步：发送ACK */
		if (i2c_WaitAck() != 0)
		{
			goto cmd_fail;	/* EEPROM器件无应答 */
		}

		usAddr++;	/* 地址增1 */		
	}
	
	/* 命令执行成功，发送I2C总线停止信号 */
	i2c_Stop();
	return 1;

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_Stop();
	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: IIC_ReadBytes
*	功能说明: 从指定地址处开始读取若干数据
*	形    参：_usAddress : 起始地址
*			 _usSize : 数据长度，单位为字节
*			 _pReadBuf : 存放读到的数据的缓冲区指针
*	返 回 值: 0 表示失败，1表示成功
*********************************************************************************************************
*/
uint8_t IIC_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i;
	
	/* 第1步：发起I2C总线启动信号 */
	i2c_Start();
	
	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte(DS3231_ADDRESS | I2C_WR);	/* 此处是写指令 */
	
	/* 第3步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* 器件无应答 */
	}

	/* 第4步：发送字节地址*/
	i2c_SendByte((uint8_t)_usAddress);
	
	/* 第5步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/*器件无应答 */
	}
	
	/* 第6步：重新启动I2C总线。前面的代码的目的向EEPROM传送地址，下面开始读取数据 */
	i2c_Start();
	
	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte(DS3231_ADDRESS | I2C_RD);	/* 此处是读指令 */
	
	/* 第8步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* 器件无应答 */
	}	
	
	/* 第9步：循环读取数据 */
	for (i = 0; i < _usSize; i++)
	{
		_pReadBuf[i] = i2c_ReadByte();	/* 读1个字节 */
		
		/* 每读完1个字节后，需要发送Ack， 最后一个字节不需要Ack，发Nack */
		if (i != _usSize - 1)
		{
			i2c_Ack();	/* 中间字节读完后，CPU产生ACK信号(驱动SDA = 0) */
		}
		else
		{
			i2c_NAck();	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
		}
	}
	/* 发送I2C总线停止信号 */
	i2c_Stop();
	return 1;	/* 执行成功 */

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_Stop();
	return 0;
}
/************************************end of file 2014-08-08*********************************/
