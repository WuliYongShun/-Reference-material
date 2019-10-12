#ifndef __JDQ_H
#define __JDQ_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//继电器控制端口配置
////////////////////////////////////////////////////////////////////////////////// 
//#define FM1K PAout(2)	  // PA2
//#define FM1G PCout(4)	  // PC4	
//#define FM2K PBout(0)	  // PB0
//#define FM2G PBout(11)	// PB11

#define BEEP PAout(15)	// PA15


//继电器控制
#define FM1K_ON()         GPIO_SetBits(GPIOA,GPIO_Pin_2)
#define FM1K_OFF()        GPIO_ResetBits(GPIOA,GPIO_Pin_2)

#define FM1G_ON()         GPIO_SetBits(GPIOC,GPIO_Pin_4)
#define FM1G_OFF()        GPIO_ResetBits(GPIOC,GPIO_Pin_4)

#define FM2K_ON()         GPIO_SetBits(GPIOB,GPIO_Pin_0)
#define FM2K_OFF()        GPIO_ResetBits(GPIOB,GPIO_Pin_0)

#define FM2G_ON()         GPIO_SetBits(GPIOB,GPIO_Pin_11)
#define FM2G_OFF()        GPIO_ResetBits(GPIOB,GPIO_Pin_11)


//拨码开关控制
#define BUTTON1_READ()      GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)
#define BUTTON2_READ()      GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)
#define BUTTON3_READ()      GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)
#define BUTTON4_READ()      GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)
#define BUTTON5_READ()      GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6)
#define BUTTON6_READ()      GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)


//光耦控制
#define FM1KD_READ()      GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)
#define FM1GD_READ()      GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)
#define FM2KD_READ()      GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)
#define FM2GD_READ()      GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)


void JDQ_Init(void);//继电器初始化
void Button_Config(void);//拨码开关初始化	 
void Optical_Init(void);//光耦初始化
#endif
