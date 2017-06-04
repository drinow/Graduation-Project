/*包含头------------------------------------------------------------------*/
#include "HMC5883L.h"



/*变量声明----------------------------------------------------------------*/
int Magn_x,Magn_y,Magn_z,
	Magn_x_Max,Magn_y_Max,Magn_z_Max,
	Magn_x_Min,Magn_y_Min,Magn_z_Min;    //5883用到
	
float angle;   //5883 的指南针
float X_Offset,Y_Offset,Z_Offset,X_scale,Y_scale,Z_scale;
int Xmax=0, Xmin=0, Ymax=0, Ymin=0, Zmax=0, Zmin=0;

int Mag_UserCal_Offset_X,Mag_UserCal_Offset_Y,Mag_UserCal_Offset_Z;
/*函数声明----------------------------------------------------------------*/

/************************************************************   
* 函数名:Identify_HMC5883L  
* 描述 : 设备识别 
* 输入  :无   
* 输出  :无    
*/
void Identify_HMC5883L(void)
{
	u8 ID_A,ID_B,ID_C;
//	ID_A=I2C_ReadByte(HMC5883L_Addr,HMC5883L_ID_A);
//	ID_B=I2C_ReadByte(HMC5883L_Addr,HMC5883L_ID_B);
//	ID_C=I2C_ReadByte(HMC5883L_Addr,HMC5883L_ID_C);
  IIC_ReadBytes(&ID_A,HMC5883L_ID_A,sizeof(ID_A));
  IIC_ReadBytes(&ID_B,HMC5883L_ID_A,sizeof(ID_B));
  IIC_ReadBytes(&ID_C,HMC5883L_ID_A,sizeof(ID_C));
	if(ID_A=='H'&&ID_B=='4'&&ID_C=='3')
	{
		printf("\r设备HMC5773L设别成功!\r\n\r");
	}else
	{
		printf("\r错误!无法识别设备HMC5773L!\r\n\r");
	}
}
/************************************************************   
* 函数名:Read_HMC5883L   
* 描述 : 读取磁场强度 
* 输入  :无   
* 输出  :无    
*/
void Init_HMC5883L(void)//读取
{
//	I2C_WriteByte(HMC5883L_Addr,HMC5883L_ConfigurationRegisterA,0x14);   //配置寄存器A：采样平均数1 输出速率30Hz 正常测量
//	I2C_WriteByte(HMC5883L_Addr,HMC5883L_ConfigurationRegisterB,0x20);   //配置寄存器B：增益控制
//	I2C_WriteByte(HMC5883L_Addr,HMC5883L_ModeRegister,0x00);   //模式寄存器：连续测量模式
  
  u8 data=0;
  i2c_CheckDevice(HMC5883L_Addr);
  Identify_HMC5883L();
  data=0x14;
	IIC_WriteBytes(&data,HMC5883L_ConfigurationRegisterA,sizeof(data));   //配置寄存器A：采样平均数1 输出速率30Hz 正常测量
  data=0x20;
	IIC_WriteBytes(&data,HMC5883L_ConfigurationRegisterB,sizeof(data));   //配置寄存器B：增益控制
  data=0x00;
	IIC_WriteBytes(&data,HMC5883L_ModeRegister,sizeof(data));   //模式寄存器：连续测量模式
}
/************************************************************   
* 函数名:HMC58X3_Offset 
* 描述 : 计算X,Y,Z轴偏移,被HMC5883L_Self_Test（）调用
* 输入  :无   
* 输出  :无    
*/
void HMC58X3_Offset(void)					 //计算零偏
{
	X_Offset   = (Xmax+Xmin)/2;
	Y_Offset   = (Ymax+Ymin)/2;
	Z_Offset   = (Zmax+Zmin)/2;	
	printf("X_Offset:%.2f  ",X_Offset);
	printf("Y_Offset:%.2f  ",Y_Offset);
	printf("Z_Offset:%.2f\n",Z_Offset);
}
/************************************************************   
* 函数名:Calibrate 
* 描述 : 计算X,Y,Z轴最值,被HMC5883L_Self_Test（）调用
* 输入  :无   
* 输出  :无    
*/
void Initialize_Cal_Variables(int MagX, int MagY, int  MagZ)
{
// set Max and Min values of the mag output to the current values
	Xmax=MagX;
	Xmin=MagX;
	Ymax=MagY;
	Ymin=MagY;
	Zmax=MagZ;
	Zmin=MagZ;
}
void Calibrate(int MagX, int MagY, int  MagZ)
{
	//  this routine will capture the max and min values of the mag X, Y, and Z data while the
	//  compass is being rotated 360 degrees through the level plane and the upright plane.  
	//  i.e. horizontal and vertical circles.
	// This function should be invoked while making continuous measurements 
	//on the magnetometers
	int  MagXreading,  MagYreading,  MagZreading;

	MagXreading=MagX;  // just for clarification...  can remove these lines
	MagYreading=MagY;
	MagZreading=MagZ;

	if(MagXreading > Xmax ) Xmax = MagXreading;
	if(MagXreading < Xmin ) Xmin = MagXreading;
	if(MagYreading > Ymax ) Ymax = MagYreading;
	if(MagYreading < Ymin ) Ymin = MagYreading;
	if(MagZreading > Zmax ) Zmax = MagZreading;
	if(MagZreading < Zmin ) Zmin = MagZreading;
	
	printf("Xmax:%d Xmin:%d || ",Xmax,Xmin);
	printf("Ymax:%d Ymin:%d || ",Ymax,Ymin);
	printf("Zmax:%d Zmin:%d\n",Zmax,Zmin);

}
void Compute_and_Save(void)
{
	if(ABS(Xmax - Xmin) > CalThreshold )
	{
	Mag_UserCal_Offset_X = (Xmax + Xmin)/2;   
	// Save parameters in EE
	}
	if(ABS(Ymax - Ymin) > CalThreshold )
	{
	Mag_UserCal_Offset_Y=  (Ymax + Ymin)/2;
	//Save parameters in EE
	}
	if(ABS(Zmax - Zmin) > CalThreshold )
	{
	Mag_UserCal_Offset_Z =  (Zmax +Zmin)/2;   
	// Save parameters in EE
	}
	printf("OffsetX:%d  ",Mag_UserCal_Offset_X);
	printf("OffsetY:%d  ",Mag_UserCal_Offset_Y);
	printf("OffsetZ:%d\n",Mag_UserCal_Offset_Z);
}
void Hard_Iron_Correction( int MagX, int MagY, int  MagZ)   // call this function for correction
		{
		MagX -= Mag_UserCal_Offset_X;
		MagY -= Mag_UserCal_Offset_Y;
		MagZ -= Mag_UserCal_Offset_Z;
}
/************************************************************   
* 函数名:HMC5883L_Carlibrate   
* 描述 : 5883校准
* 输入  :无   
* 输出  :无 
* 校准方法：在空间沿8字乱转，再按下C1脚退出
* 校准误差：±10°
* 校准说明：精度仍然有待提高，可能是空间校准的增益都简化为1的原因（未进行椭圆校准）
* 校准原理：Step1：水平校准。Step2：空间校准。详见配套截图
*/
void HMC5883L_Carlibrate(void)
{
	u8 BUF1[7]={0};

	Init_HMC5883L();

	while(1)
	{
	delay_ms(100);

//	BUF1[1]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_X_MSB);//OUT_X_H
//	BUF1[2]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_X_LSB);//OUT_X_L

//	BUF1[3]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Y_MSB);//OUT_Y_L_A
//	BUF1[4]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Y_LSB);//OUT_Y_H_A
//	
//	BUF1[5]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Z_MSB);//OUT_Z_L_A
//	BUF1[6]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Z_LSB);//OUT_Z_H_A
    
  IIC_ReadBytes(&BUF1[1],HMC5883L_Output_X_MSB,sizeof(BUF1[1]));//OUT_X_H
	IIC_ReadBytes(&BUF1[2],HMC5883L_Output_X_LSB,sizeof(BUF1[2]));//OUT_X_H

	IIC_ReadBytes(&BUF1[3],HMC5883L_Output_Y_MSB,sizeof(BUF1[3]));//OUT_X_H
	IIC_ReadBytes(&BUF1[4],HMC5883L_Output_Y_LSB,sizeof(BUF1[4]));//OUT_X_H
	
	IIC_ReadBytes(&BUF1[5],HMC5883L_Output_Z_MSB,sizeof(BUF1[5]));//OUT_X_H
	IIC_ReadBytes(&BUF1[6],HMC5883L_Output_Z_LSB,sizeof(BUF1[6]));//OUT_X_H
    
	Magn_x=(int)((BUF1[1] << 8) | BUF1[2]); //Combine MSB and LSB of X Data output register
	Magn_y=(int)((BUF1[3] << 8) | BUF1[4]); //Combine MSB and LSB of Z Data output register
	Magn_z=(int)((BUF1[5] << 8) | BUF1[6]); //Combine MSB and LSB of Z Data output register

	if(Magn_x>0x7fff) Magn_x-=0xffff;	//负数显示 
	if(Magn_y>0x7fff) Magn_y-=0xffff;
	if(Magn_z>0x7fff) Magn_z-=0xffff;		

	Calibrate(Magn_x,Magn_y,Magn_z);

	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)==1)
		break;
	}	
	HMC58X3_Offset();//计算偏移
}

/************************************************************   
* 函数名:Read_HMC5883L   
* 描述 : 读取磁场强度 
* 输入  :无   
* 输出  :无    
*/
void Read_HMC5883L(void)//读取
{
	u8 BUF1[7]={0};
//	I2C_WriteByte(HMC5883L_Addr,HMC5883L_ConfigurationRegisterA,0x14);   //配置寄存器A：采样平均数1 输出速率30Hz 正常测量
//	I2C_WriteByte(HMC5883L_Addr,HMC5883L_ConfigurationRegisterB,0x20);   //配置寄存器B：增益控制
//	I2C_WriteByte(HMC5883L_Addr,HMC5883L_ModeRegister,0x00);   //模式寄存器：连续测量模式
//	delay_ms(5);

//	BUF1[1]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_X_MSB);//OUT_X_H
//	BUF1[2]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_X_LSB);//OUT_X_L

//	BUF1[3]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Y_MSB);//OUT_Y_L_A
//	BUF1[4]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Y_LSB);//OUT_Y_H_A
//	
//	BUF1[5]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Z_MSB);//OUT_Z_L_A
//	BUF1[6]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Z_LSB);//OUT_Z_H_A
	
  IIC_ReadBytes(&BUF1[1],HMC5883L_Output_X_MSB,sizeof(BUF1[1]));//OUT_X_H
	IIC_ReadBytes(&BUF1[2],HMC5883L_Output_X_LSB,sizeof(BUF1[2]));//OUT_X_H

	IIC_ReadBytes(&BUF1[3],HMC5883L_Output_Y_MSB,sizeof(BUF1[3]));//OUT_X_H
	IIC_ReadBytes(&BUF1[4],HMC5883L_Output_Y_LSB,sizeof(BUF1[4]));//OUT_X_H
	
	IIC_ReadBytes(&BUF1[5],HMC5883L_Output_Z_MSB,sizeof(BUF1[5]));//OUT_X_H
	IIC_ReadBytes(&BUF1[6],HMC5883L_Output_Z_LSB,sizeof(BUF1[6]));//OUT_X_H
  
	Magn_x=(int)((BUF1[1] << 8) | BUF1[2]); //Combine MSB and LSB of X Data output register
	Magn_y=(int)((BUF1[3] << 8) | BUF1[4]); //Combine MSB and LSB of Z Data output register
	Magn_z=(int)((BUF1[5] << 8) | BUF1[6]); //Combine MSB and LSB of Z Data output register

	Magn_x=Magn_x-X_Offset;
	Magn_y=Magn_y-Y_Offset;
	Magn_z=Magn_z-Z_Offset;

	if(Magn_x>0x7fff) Magn_x-=0xffff;	//负数显示 
	if(Magn_y>0x7fff) Magn_y-=0xffff;
	if(Magn_z>0x7fff) Magn_z-=0xffff;		
	angle= atan2((double)Magn_y,(double)Magn_x) * (180 / 3.14159265) + 180; // angle in degrees
	
	//湖南磁偏角 -3° 山东磁偏角 吉林磁偏角 -9°
	angle=angle-9;//效果不大，可能是因为校准算法不是很好，误差比较大

}
