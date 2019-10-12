#include "stm32f10x.h"
#include <stdio.h>
#include "mb.h"
#include "mbutils.h"
#include "jdq.h"
#include "spi538.h"
#include "spiad.h"
#include "iic.h"
#include "delay.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//输入寄存器起始地址
#define REG_INPUT_START       0x0000
//输入寄存器数量
#define REG_INPUT_NREGS       8
//保持寄存器起始地址
#define REG_HOLDING_START     0x0000
//保持寄存器数量
#define REG_HOLDING_NREGS     8

//线圈起始地址
#define REG_COILS_START       0x0000
//线圈数量
#define REG_COILS_SIZE        16

//开关寄存器起始地址
#define REG_DISCRETE_START    0x0000
//开关寄存器数量
#define REG_DISCRETE_SIZE     16

/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
//输入寄存器内容
uint16_t usRegInputBuf[REG_INPUT_NREGS] ;
//寄存器起始地址
uint16_t usRegInputStart = REG_INPUT_START;

//保持寄存器内容
uint32_t usRegHoldingBuf[REG_HOLDING_NREGS] ;
//保持寄存器起始地址
uint16_t usRegHoldingStart = REG_HOLDING_START;

//线圈状态
uint8_t ucRegCoilsBuf[REG_COILS_SIZE / 8] = {0x00,0x00};
//拨码开关输入状态
uint8_t ucRegDiscreteBuf[REG_DISCRETE_SIZE / 8] = {0x00,0x00};


uint16_t voltageDA1; //DA1输出ma值
uint16_t voltageDA2; //DA2输出ma值



//延时函数
void delayms(u32 m)
{
  u32 j,k;
  for(j=m;j>0;j--)
   for(k=2667;k>0;k--);
}

/* Private function prototypes -----------------------------------------------*/
void RCC_Config(void);

void Button_Config(void);
void Button_Poll(void);

void JDQ_Poll(void);

void Optical_Poll(void);
	
void MAX538_Poll1(void);
void MAX538_Poll2(void);
	
void AD7810_Poll1(void);
void AD7810_Poll2(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  主循环
  * @param  None
  * @retval None
  */

//uint16_t datatemp[REG_HOLDING_NREGS];
u32 datatemp[6] ={0x0000,0x0000,0x0000,0x0000,0x0000,0x0000};
u32 readdata[6] ={0x0000,0x0000,0x0000,0x0000,0x0000,0x0000};

int main(void)
{

	delay_init(72);
	JDQ_Init();
	Button_Config();
	Optical_Init();
	MAX538_Init();
	AD7810_Init();
	IIC_Init();
  //初始化 RTU模式 从机地址为1 USART1 4800 无校验
	Button_Poll();
  eMBInit(MB_RTU, ucRegDiscreteBuf[0], 0x01, 4800, MB_PAR_NONE);
  //启动FreeModbus 
  eMBEnable(); 
	
	
	BEEP = 0;
	delayms(1000);
	BEEP = 1;
	delayms(1000);	
	
 	while(FM24CL64_Check())//检测不到FM24CL64
	{
		BEEP = 0;
		delayms(1000);
		BEEP = 1;
		delayms(1000);	
		
		
	}

	usRegHoldingBuf[0] = 0x0000;
	usRegHoldingBuf[1] = 0x0000;
	usRegHoldingBuf[2] = 0x0000;
	usRegHoldingBuf[3] = FM24CL64_ReadLenByte(0,2);
	usRegHoldingBuf[4] = FM24CL64_ReadLenByte(2,2);
	usRegHoldingBuf[5] = FM24CL64_ReadLenByte(4,2);
  

	JDQ_Poll();
	MAX538_Poll1();
	MAX538_Poll2();
  while (1)
  {

    //不断循环
    eMBPoll(); 
		
		datatemp[0] = usRegHoldingBuf[3];
		datatemp[1] = usRegHoldingBuf[4];
		datatemp[2] = usRegHoldingBuf[5];
		
		FM24CL64_WriteLenByte(0,datatemp[0],2);
		FM24CL64_WriteLenByte(2,datatemp[1],2);
		FM24CL64_WriteLenByte(4,datatemp[2],2);
  }
}


/**
  * @brief  输入寄存器处理函数，输入寄存器可读，但不可写。
  * @param  pucRegBuffer  返回数据指针
  *         usAddress     寄存器起始地址
  *         usNRegs       寄存器长度
  * @retval eStatus       寄存器状态
  */
eMBErrorCode 
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
  eMBErrorCode    eStatus = MB_ENOERR;
  int16_t         iRegIndex;
  
  //查询是否在寄存器范围内
  //为了避免警告，修改为有符号整数
  if( ( (int16_t)usAddress >= REG_INPUT_START ) \
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
  {
    //获得操作偏移量，本次操作起始地址-输入寄存器的初始地址
    iRegIndex = ( int16_t )( usAddress - usRegInputStart );
    //逐个赋值
    while( usNRegs > 0 )
    {
      //赋值高字节
      *pucRegBuffer++ = ( uint8_t )( usRegInputBuf[iRegIndex] >> 8 );
      //赋值低字节
      *pucRegBuffer++ = ( uint8_t )( usRegInputBuf[iRegIndex] & 0xFF );
      //偏移量增加
      iRegIndex++;
      //被操作寄存器数量递减
      usNRegs--;
    }
  }
  else
  {
    //返回错误状态，无寄存器  
    eStatus = MB_ENOREG;
  }

  return eStatus;
}

/**
  * @brief  保持寄存器处理函数，保持寄存器可读，可读可写
  * @param  pucRegBuffer  读操作时--返回数据指针，写操作时--输入数据指针
  *         usAddress     寄存器起始地址
  *         usNRegs       寄存器长度
  *         eMode         操作方式，读或者写
  * @retval eStatus       寄存器状态
  */
eMBErrorCode 
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                 eMBRegisterMode eMode )
{
  //错误状态
  eMBErrorCode    eStatus = MB_ENOERR;
  //偏移量
  int16_t         iRegIndex;
  
  //判断寄存器是不是在范围内
  if( ( (int16_t)usAddress >= REG_HOLDING_START ) \
     && ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
  {
    //计算偏移量
    iRegIndex = ( int16_t )( usAddress - usRegHoldingStart );
    
    switch ( eMode )
    {
      //读处理函数  
      case MB_REG_READ:
        while( usNRegs > 0 )
 				{
					if(iRegIndex==0)  Optical_Poll();
					if(iRegIndex==1)  AD7810_Poll1();
					if(iRegIndex==2)  AD7810_Poll2();
					if(iRegIndex==3)  JDQ_Poll();
					if(iRegIndex==4)  MAX538_Poll1();
					if(iRegIndex==5)  MAX538_Poll2();
	        *pucRegBuffer++ = ( UCHAR ) ( usRegHoldingBuf[iRegIndex] >> 8 );
					*pucRegBuffer++ = ( UCHAR ) ( usRegHoldingBuf[iRegIndex] & 0xFF );
					iRegIndex++;
					usNRegs--;				
				}
        break;

      //写处理函数 //06
      case MB_REG_WRITE:
        while( usNRegs > 0 )
        {
          usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
          usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;

					if(iRegIndex==3)  JDQ_Poll();
					if(iRegIndex==4)  MAX538_Poll1();
					if(iRegIndex==5)  MAX538_Poll2();
					
					iRegIndex++;
          usNRegs--;
        }
        break;

    }
  }
  else
  {
    //返回错误状态
    eStatus = MB_ENOREG;
  }
  
  return eStatus;
}


/**
  * @brief  线圈寄存器处理函数，线圈寄存器可读，可读可写
  * @param  pucRegBuffer  读操作---返回数据指针，写操作--返回数据指针
  *         usAddress     寄存器起始地址
  *         usNRegs       寄存器长度
  *         eMode         操作方式，读或者写
  * @retval eStatus       寄存器状态
  */
eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
               eMBRegisterMode eMode )
{
  //错误状态
  eMBErrorCode    eStatus = MB_ENOERR;
  //寄存器个数
  int16_t         iNCoils = ( int16_t )usNCoils;
  //寄存器偏移量
  int16_t         usBitOffset;

  //检查寄存器是否在指定范围内
  if( ( (int16_t)usAddress >= REG_COILS_START ) &&
        ( usAddress + usNCoils <= REG_COILS_START + REG_COILS_SIZE ) )
  {
    //计算寄存器偏移量
    usBitOffset = ( int16_t )( usAddress - REG_COILS_START );
    switch ( eMode )
    {
      //读操作
      case MB_REG_READ:
        while( iNCoils > 0 )
        {
          *pucRegBuffer++ = xMBUtilGetBits( ucRegCoilsBuf, usBitOffset,
                                          ( uint8_t )( iNCoils > 8 ? 8 : iNCoils ) );
          iNCoils -= 8;
          usBitOffset += 8;
        }
        break;

      //写操作
      case MB_REG_WRITE:
        while( iNCoils > 0 )
        {
          xMBUtilSetBits( ucRegCoilsBuf, usBitOffset,
                        ( uint8_t )( iNCoils > 8 ? 8 : iNCoils ),
                        *pucRegBuffer++ );
          iNCoils -= 8;
        }
        break;
    }

  }
  else
  {
    eStatus = MB_ENOREG;
  }
  return eStatus;
}
//开关输入状态
eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
  //错误状态
  eMBErrorCode    eStatus = MB_ENOERR;
  //操作寄存器个数
  int16_t         iNDiscrete = ( int16_t )usNDiscrete;
  //偏移量
  uint16_t        usBitOffset;

  //判断寄存器时候再制定范围内
  if( ( (int16_t)usAddress >= REG_DISCRETE_START ) &&
        ( usAddress + usNDiscrete <= REG_DISCRETE_START + REG_DISCRETE_SIZE ) )
  {
    //获得偏移量
    usBitOffset = ( uint16_t )( usAddress - REG_DISCRETE_START );
    
    while( iNDiscrete > 0 )
    {
      *pucRegBuffer++ = xMBUtilGetBits( ucRegDiscreteBuf, usBitOffset,
                                      ( uint8_t)( iNDiscrete > 8 ? 8 : iNDiscrete ) );
      iNDiscrete -= 8;
      usBitOffset += 8;
    }

  }
  else
  {
    eStatus = MB_ENOREG;
  }
  return eStatus;
}

/**
  * @brief  拨码开关检测循环
  * @param  None
  * @retval None
  */
void Button_Poll(void)
{
	uint16_t Button_Status = 0x00;
  
	//拨到ON为0
  BUTTON1_READ()?(Button_Status |= 0x01):(Button_Status &=~ 0x01);
  BUTTON2_READ()?(Button_Status |= 0x02):(Button_Status &=~ 0x02); 
  BUTTON3_READ()?(Button_Status |= 0x04):(Button_Status &=~ 0x04);
  BUTTON4_READ()?(Button_Status |= 0x08):(Button_Status &=~ 0x08); 
  BUTTON5_READ()?(Button_Status |= 0x10):(Button_Status &=~ 0x10);
	BUTTON6_READ()?(Button_Status |= 0x20):(Button_Status &=~ 0x20);
	
  ucRegDiscreteBuf[0] = Button_Status;
}

void JDQ_Poll(void)
{
  
  uint16_t Relay_Status = usRegHoldingBuf[3];
	
  if(Relay_Status & 0x01) {FM1K_ON();} else {FM1K_OFF();}
  if(Relay_Status & 0x02) {FM1G_ON();} else {FM1G_OFF();}
  if(Relay_Status & 0x04) {FM2K_ON();} else {FM2K_OFF();}
  if(Relay_Status & 0x08) {FM2G_ON();} else {FM2G_OFF();}	

}

void Optical_Poll(void)
{

  uint16_t Optical_Status = 0x00;
		
  FM1KD_READ()?(Optical_Status &=~ 0x01):(Optical_Status |= 0x01);
  FM1GD_READ()?(Optical_Status &=~ 0x02):(Optical_Status |= 0x02); 
  FM2KD_READ()?(Optical_Status &=~ 0x04):(Optical_Status |= 0x04);
  FM2GD_READ()?(Optical_Status &=~ 0x08):(Optical_Status |= 0x08); 
	
	usRegHoldingBuf[0] = Optical_Status;
}

void MAX538_Poll1(void)
{
		 voltageDA1 = usRegHoldingBuf[4];//0xfff=25ma 4095 ==163.8
		 MAX538_WriteValue(1,voltageDA1);
	
}
void MAX538_Poll2(void)
{
		 voltageDA2 = usRegHoldingBuf[5];//0xfff=25ma 4095 ==163.8
		 MAX538_WriteValue(2,voltageDA2);
	
}
    
void AD7810_Poll1(void)
{

  filter();
	aftervalue[0]=(float)aftervalue[0]*4.88;
	usRegHoldingBuf[1]=aftervalue[0];

}
void AD7810_Poll2(void)
{

  Wave_filtering();
	aftervalue[1]=(float)aftervalue[1]*4.88;
	usRegHoldingBuf[2]=aftervalue[1];
}


/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
