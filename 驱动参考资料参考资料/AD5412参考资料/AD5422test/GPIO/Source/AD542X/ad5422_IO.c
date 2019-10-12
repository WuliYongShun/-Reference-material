/***************************************************************************

 Author        : FangZheng - CAST

 Date          : Jun 12th, 2012

 File          : ad5422_IO.c

 Hardware      : STM32 and AD5422

 Description   : Use the GPIO to simulate the SPI communication of AD5422
	
***************************************************************************/
#include "stm32f10x_gpio.h"
#include "ad5422_io.h"
 

  #define SET_CLEAR()	GPIOC->BSRR  = GPIO_Pin_8	//PC8->CLEAR
  #define CLR_CLEAR()	GPIOC->BRR   = GPIO_Pin_8

  #define SET_LATCH()	GPIOC->BSRR  = GPIO_Pin_3	//PC3->LATCH
  #define CLR_LATCH()	GPIOC->BRR   = GPIO_Pin_3

  #define SET_SCL()		GPIOC->BSRR  = GPIO_Pin_2	//PC2->SCLK
  #define CLR_SCL()		GPIOC->BRR   = GPIO_Pin_2

  #define SET_SDO()		GPIOC->BSRR  = GPIO_Pin_1	//PC1->SDIN
  #define CLR_SDO()		GPIOC->BRR   = GPIO_Pin_1

  #define GET_SDI()     GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0) //PC0->SDO



void ad54x2_delay (int length)
{
	while (length >0)
    	length--;
}

//---------------------------------
//void WriteToAD5422(unsigned char count,unsigned char *buf);
//---------------------------------
//Function that writes to the AD5422 via the SPI port. 
//--------------------------------------------------------------------------------
void WriteToAD5422(unsigned char count,unsigned char *buf)
{

	unsigned	char	ValueToWrite = 0;
    unsigned	char	i = 0;
	unsigned	char	j = 0;
	

	CLR_LATCH();

	for ( i=count;i>0;i-- )
 	{
	 	ValueToWrite =	*(buf+i-1);
		for (j=0; j<8; j++)
		{
			CLR_SCL();
			if(0x80 == (ValueToWrite & 0x80))
			{
				SET_SDO();	    //Send one to SDIN pin of AD5422
			}
			else
			{
				CLR_SDO();	    //Send zero to SDIN pin of AD5422
			}
										 
			ad54x2_delay(1);
			SET_SCL();
			ad54x2_delay(1);
			ValueToWrite <<= 1;	//Rotate data
	
		}

	}
	CLR_SCL();
	ad54x2_delay(1);
	SET_LATCH();
	ad54x2_delay(20);
}


//---------------------------------
//ReadFromAD5422();
//---------------------------------
//Function that reads from the AD5422 via the SPI port. 
//--------------------------------------------------------------------------------
void ReadFromAD5422(unsigned char count,unsigned char *buf)
{
	unsigned	char	i = 0;
	unsigned	char	j = 0;
	unsigned	char  	iTemp = 0;
	unsigned	char  	RotateData = 0;

	CLR_LATCH();

	for(j=count; j>0; j--)
	{
		for(i=0; i<8; i++)
		{
		    CLR_SCL();
			RotateData <<= 1;		//Rotate data
			ad54x2_delay(1);
			CLR_SDO();				//Write a nop condition when read the data. 
			iTemp = GET_SDI();		//Read SDO of AD5422
			SET_SCL();	
			if(iTemp == 1)
			{
				RotateData |= 1;	
			}
			ad54x2_delay(1);
			
		}
		*(buf+j-1)= RotateData;
	}
	CLR_SCL();
	ad54x2_delay(1);	 
	SET_LATCH();
	ad54x2_delay(20);
} 
//---------------------------------
//ReadFromAD5422();
//---------------------------------
// 
//--------------------------------------------------------------------------------
uint16_t CurrentToData(unsigned char current_type, unsigned char output_current)
{
	  uint16_t data;
	  #ifdef AD5412
	  switch (current_type){
	  case IOUT_4_20: data = ((output_current-4)*4096)/16;break;
	  case IOUT_0_20: data = ((output_current)*4096)/20;break;
	  case IOUT_0_24: data = ((output_current)*4096)/24;break;
	  default: break;
	  }
	  #endif
	  #ifdef AD5422
	  switch (current_type){
	  case IOUT_4_20: data = ((output_current-4)*65536)/16;break;
	  case IOUT_0_20: data = ((output_current)*65536)/20;break;
	  case IOUT_0_24: data = ((output_current)*65536)/24;break;
	  default: break;
	  }
	  #endif
	  return data;
}
