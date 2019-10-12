/**
  ******************************************************************************
  * @文件   
  * @作者 
  * @版本
  * @日期
  * @概要
  ******************************************************************************
  * @注意事项
  *
  *
  *
  ******************************************************************************
  */  

/* 头文件		--------------------------------------------------------------*/
#include "mb.h"
/* 私有数据类型 --------------------------------------------------------------*/
/* 私有定义 	--------------------------------------------------------------*/
/* 私有宏定义 	--------------------------------------------------------------*/
/* 私有变量 	--------------------------------------------------------------*/
/* 私有函数声明 --------------------------------------------------------------*/

/**
  * @功能
  * @参数
  * @返回值
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


/*******************文件结尾**************************************************/
