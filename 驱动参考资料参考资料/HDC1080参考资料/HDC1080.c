
/**************************************************************************//**
 * @file     HDC1080.c
 * @version  V3.00
 * $Revision: 2 $
 * $Date: 18/08/17 10:22a $
 * @brief
 *           Show a Master how to access Slave.
 *           This sample code needs to work with I2C_Slave.
 * @note
 * Copyright (C) 2018 CHN Spec Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#include "main.h"
/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
#define HDC1080_I2C_ADDR        0x80    //0x40 << 1

#define Temperature             0x00    //0x0000 Temperature measurement output
#define Humidity                0x01    //0x0000 Relative Humidity measurement output
#define Configuration           0x02    //0x1000 HDC1080 configuration and status
#define Manufacturer_ID         0xFE    //0x5449 ID of Texas Instruments
#define Device_ID               0xFF    //0x1050 ID of the device


#define Manufacturer_ID_value   0x5449
#define Device_ID_value         0x1050
#define Configuration_1         0x1000  //温湿度14位精度，温湿度一个命令同时输出，先输出温度，在输出湿度，关闭内部加热，自动复位
#define Configuration_2         0x3000  //温湿度14位精度，温湿度一个命令同时输出，先输出温度，在输出湿度，开启内部加热，自动复位


#define HDC_I2C_SCL_H GPIO_SetBits(GPIOB,GPIO_Pin_10)
#define HDC_I2C_SCL_L GPIO_ResetBits(GPIOB,GPIO_Pin_10)

#define HDC_I2C_SDA_H GPIO_SetBits(GPIOB,GPIO_Pin_11)
#define HDC_I2C_SDA_L GPIO_ResetBits(GPIOB,GPIO_Pin_11)


float HDC_TEMP,HDC_RH;  //HDC1080的温度和湿度


/*******************************************************************************
* Function Name  : HDC1080_I2C_Init
* Description    : 初始化HDC1080的模拟I2C接口
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HDC1080_I2C_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  //PB 时钟使能
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;  
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;  //设置GPIO模式  输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //设置输出频率
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  //上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化 GPIO

	GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11); //PB10,PB11 输出高   SDA，SCL初始化高电平
}

void I2C_SDA_OUT(void)  //设置SDA为输出
{
	GPIO_InitTypeDef GPIO_InitStructure;	

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void I2C_SDA_IN(void)  //设置SDA为输入
{
	GPIO_InitTypeDef GPIO_InitStructure;	

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : I2C_Start
* Description    : I2C开始传输数据信号
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_Start(void)
{
	I2C_SDA_OUT();                 //   SCL: 111-111-111
	HDC_I2C_SDA_H;                 //   SDA: 111-111-000
	Delay_ms(1); 
	HDC_I2C_SCL_H;
	Delay_ms(1);
	HDC_I2C_SDA_L; //主机开始传输数据
}

/*******************************************************************************
* Function Name  : I2C_ReStart
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_ReStart(void)
{
	HDC_I2C_SCL_L;          // SCL: 000-000-111-111
	Delay_ms(1);            // SDA: fff-111-111-000
	I2C_SDA_OUT();
	HDC_I2C_SDA_H;
	Delay_ms(1);
	HDC_I2C_SCL_H;
	Delay_ms(1);
	HDC_I2C_SDA_L;
}

/*******************************************************************************
* Function Name  : I2C_Stop
* Description    : I2C停止传输数据信号
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_Stop(void)
{
	HDC_I2C_SCL_L;                     // SCL: 000-000-111-111
	Delay_ms(1);                       // SDA: fff-000-000-111
	I2C_SDA_OUT();
	HDC_I2C_SDA_L;
	Delay_ms(1);
	HDC_I2C_SCL_H;
	Delay_ms(5);
	HDC_I2C_SDA_H;
}

/*******************************************************************************
* Function Name  : I2C_SendAck
* Description    : I2C发送等待信号
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_SendAck(void)
{
	HDC_I2C_SCL_L;       // SCL: 000-000-111
	Delay_ms(1);         // SDA: fff-000-000
	I2C_SDA_OUT();
	HDC_I2C_SDA_L;
	Delay_ms(1);
	HDC_I2C_SCL_H;
	Delay_ms(5);
}

/*******************************************************************************
* Function Name  : I2C_SendNAck
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_SendNAck(void)
{
	HDC_I2C_SCL_L;      // SCL: 000-000-111
	Delay_ms(1);        // SDA: fff-111-111
	I2C_SDA_OUT();
	HDC_I2C_SDA_H;
	Delay_ms(1);
	HDC_I2C_SCL_H;
	Delay_ms(5);
}

/*******************************************************************************
* Function Name  : I2C_SendByte
* Description    : I2C发送一个字节
* Input          : char
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_SendByte(unsigned char bData)
{
		uint8_t i;
		uint8_t BitMask = 0x80;

		for(i=0;i<8;i++)  		//1Byte = 8Bit                        //   SCL: 000-111-000-111-000-111-000-111-000-111-000-111-000-111-000-111
		{                                                           //   SDA: fff-xxx-fff-xxx-fff-xxx-fff-xxx-fff-xxx-fff-xxx-fff-xxx-fff-xxx
			Delay_ms(1);
			HDC_I2C_SCL_L;
			Delay_ms(1);
			I2C_SDA_OUT();

			if(bData & BitMask) //0x80 -> 10000000 高位优先传输
			{
				HDC_I2C_SDA_H;
			}
			else
			{
				HDC_I2C_SDA_L;   
			}
			
			BitMask >>= 1; //右移一位，传输下一位
			Delay_ms(1);
			HDC_I2C_SCL_H;
			Delay_ms(1);
		}
}


/*******************************************************************************
* Function Name  : I2C_ReceiveByte
* Description    : I2C接收一个字节
* Input          : None
* Output         : None
* Return         : Byte
*******************************************************************************/
unsigned char I2C_ReceiveByte(void)
{
	uint8_t i;
	uint8_t ByteData = 0;
	uint8_t BitMask = 0x80;

	HDC_I2C_SCL_L;
	I2C_SDA_IN();

	for(i=0;i<8;i++)  //1Byte = 8Bit                                      // SCL: 000-000-111-000-111-000-111-000-111-000-111-000-111-000-111-000-111
	{                                                                     // SDA: fff-fff-xxx-fff-xxx-fff-xxx-fff-xxx-fff-xxx-fff-xxx-fff-xxx-fff-xxx
		HDC_I2C_SCL_L;
		Delay_ms(5);
		HDC_I2C_SCL_H;
		Delay_ms(1);
		
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)) //高位数据优先传输
		{
			ByteData |= BitMask;
		}
		
		Delay_ms(1);
		BitMask >>= 1;
	}
	return ByteData;
}

/*******************************************************************************
* Function Name  : I2C_CheckAck
* Description    : 
* Input          : None
* Output         : None
* Return         : 0：检测应答信号失败 1：检测应答信号成功
*******************************************************************************/
unsigned char I2C_CheckAck(void)
{
	HDC_I2C_SCL_L;                                     // SCL: 000-111
	I2C_SDA_IN();                                      // SDA: fff-000
	Delay_ms(5);
	HDC_I2C_SCL_H;
	Delay_ms(1);

	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)!=0)
		return 1;
	else
		return 0;
}

/*******************************************************************************
* Function Name  : I2C_Write_HDC1080
* Description    : I2C向HDC1080写入数据
* Input          : 设备地址，寄存器地址，值
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_Write_HDC1080(unsigned char dev_addr,unsigned char register_addr,unsigned int value) 
{
	unsigned char i;
	unsigned char datax[2];

	datax[0] = (uint8_t)((value & 0xFF00) >> 8);
	datax[1] = (uint8_t)(value & 0x00FF);

	for(i=20;i>0;i--)  //写失败则重复20次
	{
		I2C_Start();  //开始通信

		I2C_SendByte(dev_addr);	  // HDC1080 Addr + W
		if(I2C_CheckAck())  //等待应答信号
		{
			I2C_Stop();
			continue;
		}

		I2C_SendByte(register_addr);  //发送寄存器地址
		if(I2C_CheckAck())  //等待应答信号
		{
			I2C_Stop();
			continue;
		}

		I2C_SendByte(datax[0]);  //写入高8位配置信息
		if(I2C_CheckAck())  //等待应答信号
		{
			I2C_Stop();
			continue;
		}	 

		I2C_SendByte(datax[1]);  //写入低8位配置信息
		if(I2C_CheckAck())  //等待应答信号
		{
			I2C_Stop();
			continue;
		}
		I2C_Stop();  //结束通信
		return;  //结束通信
	}
}

/*******************************************************************************
* Function Name  : I2C_Read_HDC1080
* Description    : 读取HDC1080信息
* Input          : 设备地址，寄存器地址，读出数据
* Output         : None
* Return         : 0：初始化失败 1：初始化成功
*******************************************************************************/
void I2C_Read_HDC1080(unsigned char dev_addr,unsigned char register_addr,unsigned char *datax) 
{
	unsigned char i;

	datax[0]=0;
	datax[1]=0;

	for(i=20;i>0;i--) //从机没有相应则重复通信，最多20次
	{
		I2C_Start();

		I2C_SendByte(dev_addr);	  // HDC1080_I2C_ADDR 0x40+ W
		if(I2C_CheckAck())  //等待从机响应
		{
			I2C_Stop();  
			continue;  //没有响应则跳出进行下一次循环
		}

		I2C_SendByte(register_addr);  //写入寄存器地址
		if(I2C_CheckAck())  //等待从机响应
		{
			I2C_Stop();
			continue;  //没有响应则跳出进行下一次循环
		}

		I2C_ReStart();

		I2C_SendByte(dev_addr|0x01);    // HDC1080_I2C_ADDR 0x40 + R
		if(I2C_CheckAck())
		{
			I2C_Stop();
			continue;  //没有响应则跳出进行下一次循环
		}

		datax[0] = I2C_ReceiveByte();
		I2C_SendAck();

		datax[1] = I2C_ReceiveByte();
		I2C_SendNAck();
		I2C_Stop();
		return;
	}
}

/*******************************************************************************
* Function Name  : Convert_HDC1080_TempHumidity
* Description    : 读取HDC1080温湿度值
* Input          : 设备地址，寄存器地址
* Output         : None
* Return         : None
*******************************************************************************/
void Convert_HDC1080_TempHumidity(unsigned char dev_addr,unsigned char register_addr) 
{
	unsigned long temp,humidity;
	unsigned char datax[4];
	
	datax[0]=0;
	datax[1]=0;
	datax[2]=0;
	datax[3]=0;

	I2C_Start();

	I2C_SendByte(dev_addr);	  // HDC1080 Addr + W
	if(I2C_CheckAck())
	{
		I2C_Stop();
		return;
	}

	I2C_SendByte(register_addr);
	if(I2C_CheckAck())
	{
		I2C_Stop();
		return;
	}

	Delay_ms(30);//最少30ms

	I2C_ReStart();

	I2C_SendByte(dev_addr|0x01);    // HDC1080 Addr + R
	if(I2C_CheckAck())
	{
		I2C_Stop();
		return;
	}

	datax[0] = I2C_ReceiveByte();
	I2C_SendAck();

	datax[1] = I2C_ReceiveByte();
	I2C_SendAck();

	datax[2] = I2C_ReceiveByte();
	I2C_SendAck();

	datax[3] = I2C_ReceiveByte();
	I2C_SendNAck();
	
	I2C_Stop();

	temp = (unsigned long)(datax[0]<<8);
	temp += datax[1];
	temp = (temp*16500)>>16;
	temp -= 4000;
	HDC_TEMP = (float)temp/100;

	humidity = (unsigned long)(datax[2]<<8);
	humidity += datax[3];
	humidity = (humidity*100)>>16;
	HDC_RH = humidity;
}


/*******************************************************************************
* Function Name  : HDC1080_Init
* Description    : 初始化HDC1080,设置寄存器值
* Input          : None
* Output         : None
* Return         : 0：初始化失败 1：初始化成功
*******************************************************************************/
unsigned char HDC1080_Init(void)
{
	unsigned int data;
	unsigned char datax[2];
	
	I2C_Read_HDC1080(HDC1080_I2C_ADDR,Manufacturer_ID,datax);  //读取生产厂家ID号
	data = (unsigned int)(datax[0]<<8);
	data += datax[1];
	
	if(data != Manufacturer_ID_value)
		return 0;  //设备通信异常，初始化失败

	I2C_Read_HDC1080(HDC1080_I2C_ADDR,Device_ID,datax);  //读取设备ID号
	data = (unsigned int)(datax[0]<<8);
	data += datax[1];
	
	if(data != Device_ID_value)
		return 0;  //设备通信异常，初始化失败

	I2C_Write_HDC1080(HDC1080_I2C_ADDR,Configuration,Configuration_1);  //将HDC1080配置为Configuration_1的配置

	I2C_Read_HDC1080(HDC1080_I2C_ADDR,Configuration,datax); //读取HDC1080配置信息
	data=(unsigned int)(datax[0]<<8);
	data+=datax[1];

	return 1;  //设备初始化成功
}




