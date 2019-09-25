
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
#define Configuration_1         0x1000  //��ʪ��14λ���ȣ���ʪ��һ������ͬʱ�����������¶ȣ������ʪ�ȣ��ر��ڲ����ȣ��Զ���λ
#define Configuration_2         0x3000  //��ʪ��14λ���ȣ���ʪ��һ������ͬʱ�����������¶ȣ������ʪ�ȣ������ڲ����ȣ��Զ���λ


#define HDC_I2C_SCL_H GPIO_SetBits(GPIOB,GPIO_Pin_10)
#define HDC_I2C_SCL_L GPIO_ResetBits(GPIOB,GPIO_Pin_10)

#define HDC_I2C_SDA_H GPIO_SetBits(GPIOB,GPIO_Pin_11)
#define HDC_I2C_SDA_L GPIO_ResetBits(GPIOB,GPIO_Pin_11)


float HDC_TEMP,HDC_RH;  //HDC1080���¶Ⱥ�ʪ��


/*******************************************************************************
* Function Name  : HDC1080_I2C_Init
* Description    : ��ʼ��HDC1080��ģ��I2C�ӿ�
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HDC1080_I2C_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  //PB ʱ��ʹ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;  
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;  //����GPIOģʽ  ���ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //�������Ƶ��
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  //����
	GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ�� GPIO

	GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11); //PB10,PB11 �����   SDA��SCL��ʼ���ߵ�ƽ
}

void I2C_SDA_OUT(void)  //����SDAΪ���
{
	GPIO_InitTypeDef GPIO_InitStructure;	

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  //����
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void I2C_SDA_IN(void)  //����SDAΪ����
{
	GPIO_InitTypeDef GPIO_InitStructure;	

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : I2C_Start
* Description    : I2C��ʼ���������ź�
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
	HDC_I2C_SDA_L; //������ʼ��������
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
* Description    : I2Cֹͣ���������ź�
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
* Description    : I2C���͵ȴ��ź�
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
* Description    : I2C����һ���ֽ�
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

			if(bData & BitMask) //0x80 -> 10000000 ��λ���ȴ���
			{
				HDC_I2C_SDA_H;
			}
			else
			{
				HDC_I2C_SDA_L;   
			}
			
			BitMask >>= 1; //����һλ��������һλ
			Delay_ms(1);
			HDC_I2C_SCL_H;
			Delay_ms(1);
		}
}


/*******************************************************************************
* Function Name  : I2C_ReceiveByte
* Description    : I2C����һ���ֽ�
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
		
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)) //��λ�������ȴ���
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
* Return         : 0�����Ӧ���ź�ʧ�� 1�����Ӧ���źųɹ�
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
* Description    : I2C��HDC1080д������
* Input          : �豸��ַ���Ĵ�����ַ��ֵ
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_Write_HDC1080(unsigned char dev_addr,unsigned char register_addr,unsigned int value) 
{
	unsigned char i;
	unsigned char datax[2];

	datax[0] = (uint8_t)((value & 0xFF00) >> 8);
	datax[1] = (uint8_t)(value & 0x00FF);

	for(i=20;i>0;i--)  //дʧ�����ظ�20��
	{
		I2C_Start();  //��ʼͨ��

		I2C_SendByte(dev_addr);	  // HDC1080 Addr + W
		if(I2C_CheckAck())  //�ȴ�Ӧ���ź�
		{
			I2C_Stop();
			continue;
		}

		I2C_SendByte(register_addr);  //���ͼĴ�����ַ
		if(I2C_CheckAck())  //�ȴ�Ӧ���ź�
		{
			I2C_Stop();
			continue;
		}

		I2C_SendByte(datax[0]);  //д���8λ������Ϣ
		if(I2C_CheckAck())  //�ȴ�Ӧ���ź�
		{
			I2C_Stop();
			continue;
		}	 

		I2C_SendByte(datax[1]);  //д���8λ������Ϣ
		if(I2C_CheckAck())  //�ȴ�Ӧ���ź�
		{
			I2C_Stop();
			continue;
		}
		I2C_Stop();  //����ͨ��
		return;  //����ͨ��
	}
}

/*******************************************************************************
* Function Name  : I2C_Read_HDC1080
* Description    : ��ȡHDC1080��Ϣ
* Input          : �豸��ַ���Ĵ�����ַ����������
* Output         : None
* Return         : 0����ʼ��ʧ�� 1����ʼ���ɹ�
*******************************************************************************/
void I2C_Read_HDC1080(unsigned char dev_addr,unsigned char register_addr,unsigned char *datax) 
{
	unsigned char i;

	datax[0]=0;
	datax[1]=0;

	for(i=20;i>0;i--) //�ӻ�û����Ӧ���ظ�ͨ�ţ����20��
	{
		I2C_Start();

		I2C_SendByte(dev_addr);	  // HDC1080_I2C_ADDR 0x40+ W
		if(I2C_CheckAck())  //�ȴ��ӻ���Ӧ
		{
			I2C_Stop();  
			continue;  //û����Ӧ������������һ��ѭ��
		}

		I2C_SendByte(register_addr);  //д��Ĵ�����ַ
		if(I2C_CheckAck())  //�ȴ��ӻ���Ӧ
		{
			I2C_Stop();
			continue;  //û����Ӧ������������һ��ѭ��
		}

		I2C_ReStart();

		I2C_SendByte(dev_addr|0x01);    // HDC1080_I2C_ADDR 0x40 + R
		if(I2C_CheckAck())
		{
			I2C_Stop();
			continue;  //û����Ӧ������������һ��ѭ��
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
* Description    : ��ȡHDC1080��ʪ��ֵ
* Input          : �豸��ַ���Ĵ�����ַ
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

	Delay_ms(30);//����30ms

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
* Description    : ��ʼ��HDC1080,���üĴ���ֵ
* Input          : None
* Output         : None
* Return         : 0����ʼ��ʧ�� 1����ʼ���ɹ�
*******************************************************************************/
unsigned char HDC1080_Init(void)
{
	unsigned int data;
	unsigned char datax[2];
	
	I2C_Read_HDC1080(HDC1080_I2C_ADDR,Manufacturer_ID,datax);  //��ȡ��������ID��
	data = (unsigned int)(datax[0]<<8);
	data += datax[1];
	
	if(data != Manufacturer_ID_value)
		return 0;  //�豸ͨ���쳣����ʼ��ʧ��

	I2C_Read_HDC1080(HDC1080_I2C_ADDR,Device_ID,datax);  //��ȡ�豸ID��
	data = (unsigned int)(datax[0]<<8);
	data += datax[1];
	
	if(data != Device_ID_value)
		return 0;  //�豸ͨ���쳣����ʼ��ʧ��

	I2C_Write_HDC1080(HDC1080_I2C_ADDR,Configuration,Configuration_1);  //��HDC1080����ΪConfiguration_1������

	I2C_Read_HDC1080(HDC1080_I2C_ADDR,Configuration,datax); //��ȡHDC1080������Ϣ
	data=(unsigned int)(datax[0]<<8);
	data+=datax[1];

	return 1;  //�豸��ʼ���ɹ�
}




