/**
  ******************************************************************************
  * @�ļ�   
  * @���� 
  * @�汾
  * @����
  * @��Ҫ
  ******************************************************************************
  * @ע������
  *
  *
  *
  ******************************************************************************
  */  

/* ͷ�ļ�		--------------------------------------------------------------*/
#include "mb.h"
/* ˽���������� --------------------------------------------------------------*/
/* ˽�ж��� 	--------------------------------------------------------------*/
/* ˽�к궨�� 	--------------------------------------------------------------*/
/* ˽�б��� 	--------------------------------------------------------------*/
/* ˽�к������� --------------------------------------------------------------*/

/**
  * @����
  * @����
  * @����ֵ
  */
eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    return eStatus;
}

eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
	eMBErrorCode    eStatus = MB_ENOERR;

    return eStatus;
}

eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{

	return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
	return MB_ENOREG;
}

/**
  * @}
  */


/*******************�ļ���β**************************************************/
