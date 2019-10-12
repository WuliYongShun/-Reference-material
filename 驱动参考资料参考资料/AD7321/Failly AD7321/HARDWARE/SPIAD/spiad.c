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
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);	 //使能PA,PC端口时钟
	
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				 //SCLK-->PC.9 端口配置
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOC.9
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //CS-->PC.8 端口配置
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOC.8
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 //DIN-->PC.7 端口配置
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOC.7
	 
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 //DOUT-->PA.7 端口配置
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //浮空输入
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOC.9

}



/****************************************************************************
 * 函  数：void AD7321WriteData(Uint16 dat);
 * 功  能：向AD7321写入16位数据dat
 * 参  数：Uint16 dat:向AD7321所写入的16位数据
 * 返回值：无
 * 更  新：无
 * 备  注：无
****************************************************************************/
void AD7321WriteData(unsigned int dat)
{
	unsigned char i = 0;
	CS_L;
	SCLK1_H;
	for(i=0;i<16;i++)
	{
		SCLK1_H;                           //时钟准备;
//		delay_us(1);
		if(0x8000 & dat)
		DIN_H;
		else
		DIN_L;
		SCLK1_L;                         //时钟下降沿有效
//		delay_us(1);
		dat <<= 1;
	}
	CS_H;
}
///****************************************************************************
// * 函  数：Uint16 AD7321ReadData(void);
// * 功  能：从AD7321读出16位数据dat
// * 参  数：无
// * 返回值：Uint16 dat：从AD7321读出的数据
// * 更  新：无
// * 备  注：无
//****************************************************************************/
unsigned int AD7321ReadData(void)
{
	unsigned int dat = 0;
	unsigned char i = 0;
	CS_L;
	for(i=0;i<16;i++)
	{
		SCLK1_H;						//时钟准备
	  delay_us(1);
		SCLK1_L;						//时钟下降沿读数
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
 * 函  数：Uint16 AD7321GetData(void);
 * 功  能：从AD7321读出16位数据dat
 * 参  数：无
 * 返回值：Uint16 dat：从AD7321读出16位数据
 * 更  新：无
 * 备  注：无
****************************************************************************/
unsigned int AD7321GetData(void)
{
	unsigned char i = 0;
	unsigned int dat = 0, ibit = 0;
	CS_L;
	for(i=0;i<16;i++) //根据时序图写
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
 * 函  数：Uint16 AD7321WriteRead(Uint16 wrdat);
 * 功  能：向AD7321写入16位数据wrdat 从AD7321读出16位数据rddat
 * 参  数：wrdat
 * 返回值：Uint16 rddat：从AD7321读出16位数据
 * 更  新：无
 * 备  注：无
****************************************************************************/
unsigned int AD7321WriteRead(unsigned int wrdat)
{
	unsigned char i = 0;
	unsigned int rddat = 0;
	CS_L;
	for(i=0;i<16;i++)
	{
		SCLK1_H;							//时钟准备;
		if(0x8000 & wrdat)
			DIN_H;
		else
			DIN_H;
		SCLK1_L;							//时钟下降沿有效
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
 * 函  数：void AD7321Congfig(void)
 * 功  能：配置AD7321
 * 参  数：无
 * 返回值：无
 * 更  新：无
 * 备  注：无
****************************************************************************/
void AD7321Congfig(unsigned int cho)
{
//	AD7321WriteData(0xB980);			//通道0、1的输入范围均为0-10V
	AD7321WriteData(0xA000);			  //通道0、1的输入范围均为+-10V
	delay_us(1000);
	if(cho==1)
	{
//		AD7321WriteData(0x8038);//AD7321工作模式：2个独立通道。使用0通道，内部参考
		AD7321WriteData(0x8010);
		delay_us(1000);
	}
	if(cho==2)
	{
//		AD7321WriteData(0x8438);//AD7321工作模式：2个独立通道。使用1通道，内部参考
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
//	AD7321WriteData(0xB980);			//单电压0到12V供电通道0、1的输入范围均为-+10V
	AD7321WriteData(0xA000);			  //双电压-12到+12V供电通道0、1的输入范围均为+-10V
	delay(100);
//  Data_Deal	=AD7321ReadData();
//	CS_L;
//	delay(5);
	CS_H;
	AD7321WriteData(0x8030|(CHN_Num<<10));//1000 0000 0001 0000 :写control寄存器，初始通道为CHN0，单端输入模式，无低功耗模式，互补二进制输出，使能内部REF
												//在这里，如果采用全二进制输出，对其转换数据规律有待进一步研究
 	delay(500);//必须有足够长的延时。如果这里延时过短的话，则出现的一个明显的问题就是ADC的内部基准没有被准确使能
	//开始只delay了200个指令，发现全速运行的时候，AD7324的REF引脚没有2.5V的基准输出。单步运行时正常。加大延时后，REF有2.5V输出
//  delay(500);
//	CS_L;
//	delay(5);
//	CS_H;
	AD7321WriteData(0);//写完控制字后读取的数据是上一次选择通道的转换数据，为了方便读出当前数据，所以空写一次0
//	delay(100);

	Data_Deal	= AD7321GetData();
//二进制补码不正确
//	if((Data_Deal&0x1000)==0x1000)//判断转换数据的符号，其转换数据输出的格式与符号相关。
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
	
	Data_Deal	=	Data_Deal&0x1FFF;//直接二进制
	delay(100);
	return Data_Deal;

}


