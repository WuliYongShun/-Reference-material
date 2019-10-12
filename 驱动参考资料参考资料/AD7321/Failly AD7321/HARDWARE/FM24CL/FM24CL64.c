#include "FM24CL64.h"
#include "IIC.h"

#define WRITE_FRAM_CMD	0xA0	//写FRAM指令
#define READ_FRAM_CMD	0xA1	//读FRAM指令

//功能：读取FRAM地址src的数据
//参数：src，FRAM地址
//返回：src中的数据
unsigned char read_B_FRAM(unsigned int src)
{
	unsigned char ret;

	IIC_Start();
	IIC_Send_Byte(WRITE_FRAM_CMD);
	IIC_Send_Byte((unsigned char)(src >> 8));
	IIC_Send_Byte((unsigned char)src);
	IIC_Start();
	IIC_Send_Byte(READ_FRAM_CMD);
	ret = read_I2C();
	send_mNAK();
	stop_I2C();

	return ret;
}

/***********************************************************************************************
 ??:write multi-bytes,WriteAddr+=Addnum
 ??: @Param *pBuffer ????????????
       @Param WriteAddr ???
       @Param NumByteToWrite ????????
       @Param Addnum ????????????
 ??:0
 ??:??????NumByteToWrite?????????,???Addnum????
************************************************************************************************/
void FM25CL64_WriteMulByte(uint8_t *pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite, uint8_t Addnum)
{
	uint16_t i = 0;
	for(i=0;i<NumByteToWrite;i++)
{ 
	IIC_Start();  //Start
  IIC_Send_Byte(WRITE_FRAM_CMD); //地址
  IIC_Wait_Ack()；    
  
  IIC_Send_Byte(FM25CL64_I2C_BASE_PTR,((WriteAddr>>8)&0xFF)); //??8???,???8???R/W
  I2C_WaitAck(FM25CL64_I2C_BASE_PTR);  //  ?????? 
  
  I2C_SendData(FM25CL64_I2C_BASE_PTR,(WriteAddr & 0xFF)); //??8???
  I2C_WaitAck(FM25CL64_I2C_BASE_PTR);    //  ?????? 

  I2C_SendData(FM25CL64_I2C_BASE_PTR, *pBuffer);  //????
  I2C_WaitAck(FM25CL64_I2C_BASE_PTR);   //  ?????? 
	
  I2C_GenerateSTOP(FM25CL64_I2C_BASE_PTR);   //????
	DelayUs(5);
	pBuffer++;
	WriteAddr=WriteAddr+Addnum;  //???Addnum
}

/***********************************************************************************************
 ??:???????
 ??:  @Param *pBuffer ????????????
       @Param ReadAddr ????
       @Param NumByteToRead ????????
 ??:0
 ??:?FM24CL64?????ReadAddr?????????,??????????????pBuffer
************************************************************************************************/
uint8_t FM25CL64_Read(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
	
	uint16_t i = 0;
  I2C_GenerateSTART(FM25CL64_I2C_BASE_PTR);   //??
  
  I2C_Send7bitAddress(FM25CL64_I2C_BASE_PTR,0XA0,I2C_MASTER_WRITE);  //?????,?
  I2C_WaitAck(FM25CL64_I2C_BASE_PTR);  //????
  
  I2C_SendData(FM25CL64_I2C_BASE_PTR,((ReadAddr>>8)&0xFF));  //MSB
  I2C_WaitAck(FM25CL64_I2C_BASE_PTR);
  
  I2C_SendData(FM25CL64_I2C_BASE_PTR,(ReadAddr & 0xFF));   //LSB
  I2C_WaitAck(FM25CL64_I2C_BASE_PTR);
  
  I2C_GenerateRESTART(FM25CL64_I2C_BASE_PTR);    //????????
  
  I2C_Send7bitAddress(FM25CL64_I2C_BASE_PTR,0XA0,I2C_MASTER_READ);//?????,?
  I2C_WaitAck(FM25CL64_I2C_BASE_PTR);
  I2C_SetMasterMode(FM25CL64_I2C_BASE_PTR,I2C_MASTER_READ); //???
  
	I2C_ReadData(FM25CL64_I2C_BASE_PTR); //dummy read
	I2C_GenerateAck(FM25CL64_I2C_BASE_PTR);  
	I2C_WaitAck(FM25CL64_I2C_BASE_PTR);
	for(i=0;i<NumByteToRead-1;i++)    //?NumByteToRead???
	{
		*pBuffer = I2C_ReadData(FM25CL64_I2C_BASE_PTR);
		I2C_GenerateAck(FM25CL64_I2C_BASE_PTR);
    I2C_WaitAck(FM25CL64_I2C_BASE_PTR);
		pBuffer++;
	}
	*pBuffer = I2C_ReadData(FM25CL64_I2C_BASE_PTR);
	I2C_GenerateNAck(FM25CL64_I2C_BASE_PTR);  //???????????NACK
	I2C_WaitAck(FM25CL64_I2C_BASE_PTR);
  I2C_GenerateSTOP(FM25CL64_I2C_BASE_PTR);

  return 0;
}