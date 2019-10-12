#ifndef __IIC_H
#define __IIC_H
  
#include "stm32f10x.h"
#include "sys.h"   	   

#define IIC_SCL    PBout(6)	// PB6
#define IIC_SDA    PBout(7)	// PB7
#define READ_SDA   PBin(7)  //输入SDA 

#define WP         PBout(5)  //输入SDA 

#define SDA_IN()   {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;} 
#define SDA_OUT()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;} 

//IIC所有操作函数

void IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
u8 IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_Send_Byte(u8 txd);
u8 IIC_Read_Byte(unsigned char ack);

u8 FM24CL64_Check(void);
u8 FM24CL64_ReadOneByte(u16 ReadAddr);							//指定地址读取一个字节
void FM24CL64_WriteOneByte(u16 WriteAddr,u8 DataToWrite);		//指定地址写入一个字节
void FM24CL64_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len);
u32 FM24CL64_ReadLenByte(u16 ReadAddr,u8 Len);
void FM24CL64_Write(u16 WriteAddr,u32 *pBuffer,u16 NumToWrite);	//从指定地址开始写入指定长度的数据
void FM24CL64_Read(u16 ReadAddr,u32 *pBuffer,u16 NumToRead);   	//从指定地址开始读出指定长度的数据
#endif
