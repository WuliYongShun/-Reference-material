/**
  ******************************************************************************
  *               Copyright(C) 2014-2024 GDKY  All Rights Reserved
  *
  * @file    : ad7321.c
  * @author  : ChenXY
  * @version : V1.01
  * @date    : 14-Mar-2018
  * @brief   : 2channels 12-Bit ADC.
  ******************************************************************************
  */

  
/* INCLUDES ------------------------------------------------------------------- */
#include "spiad.h"
#include "delay.h"
/* TYPEDEFS ------------------------------------------------------------------- */

void AD7321_Init(void)
{

	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PA,PC�˿�ʱ��
	
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				 //SCLK-->PC.9 �˿�����
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOC.9
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //CS-->PC.8 �˿�����
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOC.8
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 //DIN-->PC.7 �˿�����
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOC.7
	 
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 //DOUT-->PA.7 �˿�����
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //��������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOC.9

}



/****************************************************************************
 * ��  ����void AD7321WriteData(Uint16 dat);
 * ��  �ܣ���AD7321д��16λ����dat
 * ��  ����Uint16 dat:��AD7321��д���16λ����
 * ����ֵ����
 * ��  �£���
 * ��  ע����
****************************************************************************/
void AD7321WriteData(unsigned int dat)
{
	unsigned char i = 0;
	CS_L;
	SCLK1_H;
	for(i=0;i<16;i++)
	{
		SCLK1_H;                           //ʱ��׼��;
//		delay_us(1);
		if(0x8000 & dat)
		DIN_H;
		else
		DIN_L;
		SCLK1_L;                         //ʱ���½�����Ч
//		delay_us(1);
		dat <<= 1;
	}
	CS_H;
}
///****************************************************************************
// * ��  ����Uint16 AD7321ReadData(void);
// * ��  �ܣ���AD7321����16λ����dat
// * ��  ������
// * ����ֵ��Uint16 dat����AD7321����������
// * ��  �£���
// * ��  ע����
//****************************************************************************/
unsigned int AD7321ReadData(void)
{
	unsigned int dat = 0;
	unsigned char i = 0;
	CS_L;
	for(i=0;i<16;i++)
	{
		SCLK1_H;						//ʱ��׼��
	  delay_us(1);
		SCLK1_L;						//ʱ���½��ض���
		delay_us(1);
//		if(DOUT_Read())
//			dat |= 1;
//		dat <<= 1;
		dat <<= 1;
    dat |= DOUT_Read();
	}
	CS_H;
	SCLK1_H;
	delay_us(1);
	return (dat);
}

/****************************************************************************
 * ��  ����Uint16 AD7321GetData(void);
 * ��  �ܣ���AD7321����16λ����dat
 * ��  ������
 * ����ֵ��Uint16 dat����AD7321����16λ����
 * ��  �£���
 * ��  ע����
****************************************************************************/
unsigned int AD7321GetData(void)
{
	unsigned char i = 0;
	unsigned int dat = 0, ibit = 0;
	CS_L;
	for(i=0;i<16;i++) //����ʱ��ͼд
	{
		SCLK1_L;
		delay_us(3);
//		ibit = DOUT_Read();
//		dat <<= 1;
//		if(ibit & 1)
//			dat |= 1;
		dat <<= 1;
    dat |= DOUT_Read();
		SCLK1_H;
		delay_us(3);
	}
	SCLK1_H;
	CS_H;
	return (dat);
} 
/****************************************************************************
 * ��  ����Uint16 AD7321WriteRead(Uint16 wrdat);
 * ��  �ܣ���AD7321д��16λ����wrdat ��AD7321����16λ����rddat
 * ��  ����wrdat
 * ����ֵ��Uint16 rddat����AD7321����16λ����
 * ��  �£���
 * ��  ע����
****************************************************************************/
unsigned int AD7321WriteRead(unsigned int wrdat)
{
	unsigned char i = 0;
	unsigned int rddat = 0;
	CS_L;
	for(i=0;i<16;i++)
	{
		SCLK1_H;							//ʱ��׼��;
		if(0x8000 & wrdat)
			DIN_H;
		else
			DIN_H;
		SCLK1_L;							//ʱ���½�����Ч
		wrdat <<= 1;
		if(DOUT_Read() & 1)
		{
			rddat |= 1;
		}
		rddat <<= 1;
	}
	CS_H;
	SCLK1_H;
	delay_us(2);
	return (rddat);
}  
/****************************************************************************
 * ��  ����void AD7321Congfig(void)
 * ��  �ܣ�����AD7321
 * ��  ������
 * ����ֵ����
 * ��  �£���
 * ��  ע����
****************************************************************************/
void AD7321Congfig(unsigned int cho)
{
//	AD7321WriteData(0xB980);			//ͨ��0��1�����뷶Χ��Ϊ0-10V
	AD7321WriteData(0xA000);			  //ͨ��0��1�����뷶Χ��Ϊ+-10V
	delay_us(1000);
	if(cho==1)
	{
//		AD7321WriteData(0x8038);//AD7321����ģʽ��2������ͨ����ʹ��0ͨ�����ڲ��ο�
		AD7321WriteData(0x8010);
		delay_us(1000);
	}
	if(cho==2)
	{
//		AD7321WriteData(0x8438);//AD7321����ģʽ��2������ͨ����ʹ��1ͨ�����ڲ��ο�
		AD7321WriteData(0x8410);
		delay_us(1000);
	}
	CS_H;
}

void delay(u32 cnt)
{
	u32 i = 0;
	for(i=cnt; i>0; i--){}	
}

uint16_t AD7321_Config(uint8_t CHN_Num)
{
	
	uint16_t Data_Deal;
	uint8_t  flag;
//	CS_L;
//	delay(5);
	CS_H;
//	AD7321WriteData(0xB980);			//����ѹ0��12V����ͨ��0��1�����뷶Χ��Ϊ-+10V
	AD7321WriteData(0xA000);			  //˫��ѹ-12��+12V����ͨ��0��1�����뷶Χ��Ϊ+-10V
	delay(100);
//  Data_Deal	=AD7321ReadData();
//	CS_L;
//	delay(5);
	CS_H;
	AD7321WriteData(0x8030|(CHN_Num<<10));//1000 0000 0001 0000 :дcontrol�Ĵ�������ʼͨ��ΪCHN0����������ģʽ���޵͹���ģʽ�����������������ʹ���ڲ�REF
												//������������ȫ���������������ת�����ݹ����д���һ���о�
 	delay(500);//�������㹻������ʱ�����������ʱ���̵Ļ�������ֵ�һ�����Ե��������ADC���ڲ���׼û�б�׼ȷʹ��
	//��ʼֻdelay��200��ָ�����ȫ�����е�ʱ��AD7324��REF����û��2.5V�Ļ�׼�������������ʱ�������Ӵ���ʱ��REF��2.5V���
//  delay(500);
//	CS_L;
//	delay(5);
//	CS_H;
	AD7321WriteData(0);//д������ֺ��ȡ����������һ��ѡ��ͨ����ת�����ݣ�Ϊ�˷��������ǰ���ݣ����Կ�дһ��0
//	delay(100);

	Data_Deal	= AD7321GetData();
//�����Ʋ��벻��ȷ
//	if((Data_Deal&0x1000)==0x1000)//�ж�ת�����ݵķ��ţ���ת����������ĸ�ʽ�������ء�
//	{
//		Data_Deal = Data_Deal&0x0FFF;
//	}
//	else
//	{
//		
//		Data_Deal	=	Data_Deal&0x0FFF;
//		Data_Deal = 0 - Data_Deal;
//		Data_Deal	=	Data_Deal&0x1FFF;
//	}
	
	Data_Deal	=	Data_Deal&0x1FFF;//ֱ�Ӷ�����
	delay(100);
	return Data_Deal;

}


