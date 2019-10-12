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
//����Ĵ�����ʼ��ַ
#define REG_INPUT_START       0x0000
//����Ĵ�������
#define REG_INPUT_NREGS       8
//���ּĴ�����ʼ��ַ
#define REG_HOLDING_START     0x0000
//���ּĴ�������
#define REG_HOLDING_NREGS     8

//��Ȧ��ʼ��ַ
#define REG_COILS_START       0x0000
//��Ȧ����
#define REG_COILS_SIZE        16

//���ؼĴ�����ʼ��ַ
#define REG_DISCRETE_START    0x0000
//���ؼĴ�������
#define REG_DISCRETE_SIZE     16

/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
//����Ĵ�������
uint16_t usRegInputBuf[REG_INPUT_NREGS] ;
//�Ĵ�����ʼ��ַ
uint16_t usRegInputStart = REG_INPUT_START;

//���ּĴ�������
uint32_t usRegHoldingBuf[REG_HOLDING_NREGS] ;
//���ּĴ�����ʼ��ַ
uint16_t usRegHoldingStart = REG_HOLDING_START;

//��Ȧ״̬
uint8_t ucRegCoilsBuf[REG_COILS_SIZE / 8] = {0x00,0x00};
//���뿪������״̬
uint8_t ucRegDiscreteBuf[REG_DISCRETE_SIZE / 8] = {0x00,0x00};


uint16_t voltageDA1; //DA1���maֵ
uint16_t voltageDA2; //DA2���maֵ



//��ʱ����
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
  * @brief  ��ѭ��
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
  //��ʼ�� RTUģʽ �ӻ���ַΪ1 USART1 4800 ��У��
	Button_Poll();
  eMBInit(MB_RTU, ucRegDiscreteBuf[0], 0x01, 4800, MB_PAR_NONE);
  //����FreeModbus 
  eMBEnable(); 
	
	
	BEEP = 0;
	delayms(1000);
	BEEP = 1;
	delayms(1000);	
	
 	while(FM24CL64_Check())//��ⲻ��FM24CL64
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

    //����ѭ��
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
  * @brief  ����Ĵ���������������Ĵ����ɶ���������д��
  * @param  pucRegBuffer  ��������ָ��
  *         usAddress     �Ĵ�����ʼ��ַ
  *         usNRegs       �Ĵ�������
  * @retval eStatus       �Ĵ���״̬
  */
eMBErrorCode 
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
  eMBErrorCode    eStatus = MB_ENOERR;
  int16_t         iRegIndex;
  
  //��ѯ�Ƿ��ڼĴ�����Χ��
  //Ϊ�˱��⾯�棬�޸�Ϊ�з�������
  if( ( (int16_t)usAddress >= REG_INPUT_START ) \
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
  {
    //��ò���ƫ���������β�����ʼ��ַ-����Ĵ����ĳ�ʼ��ַ
    iRegIndex = ( int16_t )( usAddress - usRegInputStart );
    //�����ֵ
    while( usNRegs > 0 )
    {
      //��ֵ���ֽ�
      *pucRegBuffer++ = ( uint8_t )( usRegInputBuf[iRegIndex] >> 8 );
      //��ֵ���ֽ�
      *pucRegBuffer++ = ( uint8_t )( usRegInputBuf[iRegIndex] & 0xFF );
      //ƫ��������
      iRegIndex++;
      //�������Ĵ��������ݼ�
      usNRegs--;
    }
  }
  else
  {
    //���ش���״̬���޼Ĵ���  
    eStatus = MB_ENOREG;
  }

  return eStatus;
}

/**
  * @brief  ���ּĴ��������������ּĴ����ɶ����ɶ���д
  * @param  pucRegBuffer  ������ʱ--��������ָ�룬д����ʱ--��������ָ��
  *         usAddress     �Ĵ�����ʼ��ַ
  *         usNRegs       �Ĵ�������
  *         eMode         ������ʽ��������д
  * @retval eStatus       �Ĵ���״̬
  */
eMBErrorCode 
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                 eMBRegisterMode eMode )
{
  //����״̬
  eMBErrorCode    eStatus = MB_ENOERR;
  //ƫ����
  int16_t         iRegIndex;
  
  //�жϼĴ����ǲ����ڷ�Χ��
  if( ( (int16_t)usAddress >= REG_HOLDING_START ) \
     && ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
  {
    //����ƫ����
    iRegIndex = ( int16_t )( usAddress - usRegHoldingStart );
    
    switch ( eMode )
    {
      //��������  
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

      //д������ //06
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
    //���ش���״̬
    eStatus = MB_ENOREG;
  }
  
  return eStatus;
}


/**
  * @brief  ��Ȧ�Ĵ�������������Ȧ�Ĵ����ɶ����ɶ���д
  * @param  pucRegBuffer  ������---��������ָ�룬д����--��������ָ��
  *         usAddress     �Ĵ�����ʼ��ַ
  *         usNRegs       �Ĵ�������
  *         eMode         ������ʽ��������д
  * @retval eStatus       �Ĵ���״̬
  */
eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
               eMBRegisterMode eMode )
{
  //����״̬
  eMBErrorCode    eStatus = MB_ENOERR;
  //�Ĵ�������
  int16_t         iNCoils = ( int16_t )usNCoils;
  //�Ĵ���ƫ����
  int16_t         usBitOffset;

  //���Ĵ����Ƿ���ָ����Χ��
  if( ( (int16_t)usAddress >= REG_COILS_START ) &&
        ( usAddress + usNCoils <= REG_COILS_START + REG_COILS_SIZE ) )
  {
    //����Ĵ���ƫ����
    usBitOffset = ( int16_t )( usAddress - REG_COILS_START );
    switch ( eMode )
    {
      //������
      case MB_REG_READ:
        while( iNCoils > 0 )
        {
          *pucRegBuffer++ = xMBUtilGetBits( ucRegCoilsBuf, usBitOffset,
                                          ( uint8_t )( iNCoils > 8 ? 8 : iNCoils ) );
          iNCoils -= 8;
          usBitOffset += 8;
        }
        break;

      //д����
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
//��������״̬
eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
  //����״̬
  eMBErrorCode    eStatus = MB_ENOERR;
  //�����Ĵ�������
  int16_t         iNDiscrete = ( int16_t )usNDiscrete;
  //ƫ����
  uint16_t        usBitOffset;

  //�жϼĴ���ʱ�����ƶ���Χ��
  if( ( (int16_t)usAddress >= REG_DISCRETE_START ) &&
        ( usAddress + usNDiscrete <= REG_DISCRETE_START + REG_DISCRETE_SIZE ) )
  {
    //���ƫ����
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
  * @brief  ���뿪�ؼ��ѭ��
  * @param  None
  * @retval None
  */
void Button_Poll(void)
{
	uint16_t Button_Status = 0x00;
  
	//����ONΪ0
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
