
/* Define to prevent recursive inclusion --------------------------------------*/
#ifndef __SPIAD_H
#define __SPIAD_H

#ifdef __cplusplus
 extern "C" {
#endif  /* __cplusplus */

/* INCLUDES ------------------------------------------------------------------- */
#include "stm32f10x.h"
#include "sys.h"
	 

#define DIN_H         GPIO_SetBits(GPIOC,GPIO_Pin_7)
#define DIN_L         GPIO_ResetBits(GPIOC,GPIO_Pin_7)
	 
#define CS_H         	GPIO_SetBits(GPIOC,GPIO_Pin_8)
#define CS_L         	GPIO_ResetBits(GPIOC,GPIO_Pin_8)

#define SCLK1_H         GPIO_SetBits(GPIOC,GPIO_Pin_9)
#define SCLK1_L         GPIO_ResetBits(GPIOC,GPIO_Pin_9)

#define DOUT_Read()      GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)
	 

//#define	CONTROL_REGISTER	0x8000  //���ƼĴ���
//#define	RANGE_REGISTER		0xA000  //��Χ�Ĵ���

//#define	CONVENTION		0x0000      //ת��

///*-----------------reg0(CONTROL_REGISTER)----------------*/
//#define	ADD0_VIN0		0x0            //ͨ��0
//#define	ADD0_VIN1     	0x400			 //ͨ��1
//#define	FULL_DIFFERENTIAL	0x200		 //����
//#define	PSEUDO_DIFFERENTIAL	0x100	 //α���
//#define	SINGLE_ENDED		0x0 			 //����ģ��
//#define	NORMAL_MODE			0x0        //����ģʽ
//#define	AUTOSTANDBY_MODE	0x40     //�Զ�����ģʽ
//#define	AUTOSTANDDOWN_MODE	0x80   //�Զ��ϵ�ģʽ
//#define	FULL_SHUTDOWN_MODE	0xB0	 //��ȫ�ϵ�ģʽ
//#define	SEQUENCER_USED		0x08		 //��������ת��
//#define	SEQUENCER_NOT_USED		0x0  //ͨ����������ʹ��
//#define	TWOS_COMPLEMENT_CODING	0x0//�����Ʋ���
//#define	STRAIGHT_BINARY			0x20   //����������
//#define	INTERNAL_REF			0x10     //�ڲ��ο�
//#define	EXTERNAL_REF			0x0			 //�ⲿ�ο�

///*-----------------reg1(RANGE_REGISTER)----------------*/
//#define	VIN0_10V			0x0
//#define	VIN0_5V				0x800
//#define	VIN0_2_5V			0x1000
//#define	VIN0_0_10V		0x1800
//#define	VIN1_10V			0x0
//#define	VIN1_5V				0x80
//#define	VIN1_2_5V			0x100
//#define	VIN1_0_10V		0x180

//unsigned int AD7321_read_write(unsigned int);

void AD7321_Init(void);
void AD7321WriteData(unsigned int dat);
unsigned int AD7321ReadData(void);
unsigned int AD7321GetData(void);
unsigned int AD7321WriteRead(unsigned int wrdat);

//void AD7321Congfig(void);
void AD7321Congfig(unsigned int cho);
void AD7321Congfig1(void);
void AD7321Delay(unsigned int t);
uint16_t AD7321_Config(uint8_t CHN_Num);

/* LOCAL VARIABLES ------------------------------------------------------------ */

/* LOCAL FUNCTIONS ------------------------------------------------------------ */


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif 

/***************************** END OF FILE *************************************/


