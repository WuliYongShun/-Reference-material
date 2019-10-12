/****************************************Copyright (c)**************************************************
**                                     FZ
**--------------File Info-------------------------------------------------------------------------------
** File Name: 			main.c
** Last modified Date: 	2012-08-20
** Last Version: 		1.0
** Descriptions: 		AD5412测试程序
**------------------------------------------------------------------------------------------------------
** Created 	 By: 		FZ
** Created date: 		2012-08-20
** Version: 1.0
********************************************************************************************************/

#include "stm32f10x.h"
//#include "GLCD.h"
//#include "USART.h"
#include "AD5422_IO.h"

unsigned	char	buf[4] = {0,0,0,0};

void GPIO_Configuration(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
/*管脚配置说明：
 	PC8->CLEAR
  	PC3->LATCH
  	PC2->SCLK
  	PC1->SDIN
    PC0->SDO
	更改管脚时注意在ad5422_IO.c文件中更改相关宏定义
*/


  /*设置SPI输出*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_8 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
 
  /*设置SPI输入*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

/*DA电源控制脚PA8*/
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//系统中断管理
void NVIC_Configuration(void)
{ 
  	/* Configure the NVIC Preemption Priority Bits */  
  	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	#ifdef  VECT_TAB_RAM  
	  /* Set the Vector Table base location at 0x20000000 */ 
	  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
	#else  /* VECT_TAB_FLASH  */
	  /* Set the Vector Table base location at 0x08000000 */ 
	  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
	#endif
}

//配置系统时钟,使能各外设时钟
void RCC_Configuration(void)
{
	SystemInit();	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC 
         | RCC_APB2Periph_GPIOD /*| RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG| RCC_APB2Periph_AFIO*/ 
         , ENABLE);
}
 
//配置所有外设
void Init_All_Periph(void)
{
	RCC_Configuration();	
	GPIO_Configuration();
	NVIC_Configuration();

}

/*void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}*/

int main(void)
{  
    static uint16_t data;
	data = CurrentToData(IOUT_4_20,17);
	Init_All_Periph();
 	GPIOA->BSRR  = GPIO_Pin_8;//打开AD5422电源
    ad54x2_delay (1000);
 	buf[2] = 0x56;			   //reset
	buf[1] = 0x00;              
	buf[0] = 0x01;						 
	WriteToAD5422(3,buf);		
 	ad54x2_delay(100);
 
 	buf[2] = 0x55;
//	buf[1] = 0x1f;              //Enable Slew Rate and the Slew Rate Time is 20s while selecting the current mode
//	buf[0] = 0x15;
//	buf[1] = 0x1d;              //Enable Slew Rate and the Slew Rate Time is 12s while selecting the current mode
//	buf[0] = 0x15;
	buf[1] = 0x10;              //Disable Slew Rate	while selecting the current mode
//	buf[0] = 0x05;				//4~20mA
    buf[0] = 0x00;				//0~5V

						 
	WriteToAD5422(3,buf);		//Write 551005 to SHIFT REGISTER  to write 1005 to control register

	buf[2] = 0x02;
	buf[1] = 0x00;
	buf[0] = 0x02;
	WriteToAD5422(3,buf);

	ReadFromAD5422(3,buf);		//Read CONTROL REGISTER
/*	putchar(buf[2]);
	putchar(buf[1]);
	putchar(buf[0]);*/
	ad54x2_delay(100);
	buf[2] = 0x01;
	/*
	#ifdef AD5412
	buf[1] = (uint8_t)(data>>4);
	buf[0] = (uint8_t)(data<<4);
    #endif
	*/
//  #ifdef AD5422
//	buf[1] = (uint8_t)(data>>8);
//	buf[0] = (uint8_t)(data);
//  #endif
    buf[1] = 0xac;              //15mA (4~20mA)
	buf[0] = 0x00;
  //buf[1] = 0x80;
  //buf[0] = 0x00;
	WriteToAD5422(3,buf);		//Write 019966H to SHIFT REGISTER  to write 9966H to DATA REGISTER

//	delay(10000000);			//Dalay some time before reading the STATUS REGISTER and DATA REGISTER
	
	buf[2] = 0x02;
	buf[1] = 0x00;
	buf[0] = 0x00;
	WriteToAD5422(3,buf);
	
	ReadFromAD5422(3,buf);		//Read STATUS REGISTER
/*	
	putchar(buf[2]);
	putchar(buf[1]);
	putchar(buf[0]);*/
	ad54x2_delay(100);
	buf[2] = 0x02;
	buf[1] = 0x00;
	buf[0] = 0x01;
	WriteToAD5422(3,buf);

	ReadFromAD5422(3,buf);		//Read data REGISTER
/*	putchar(buf[2]);
	putchar(buf[1]);
	putchar(buf[0]); */
	ad54x2_delay(100);
	while(1);
  	
}


