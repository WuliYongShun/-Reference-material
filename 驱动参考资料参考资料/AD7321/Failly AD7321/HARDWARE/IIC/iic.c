#include "iic.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//IIC���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//��ʼ��IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//ʹ��GPIOBʱ��
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7); 	//PB6,PB7 �����
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_5); 	//PB5 ����͵�ƽ
}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SCL=1;
	IIC_SDA=1;	
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low//	delay_us(60);//����4us
	delay_us(4);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL = 0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
	delay_us(4);
	IIC_SCL=1;
	IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);

}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ���� 
	IIC_SDA=1;
	delay_us(1);
	IIC_SCL=1;	 
	delay_us(1);
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//ʱ�����0 	   
//	SDA_OUT();
	return 0;  
} 
//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
		SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              

//			IIC_SDA=(txd&0x80)>>7;
			if((txd&0x80)>>7)
			 IIC_SDA=1;
		  else
			  IIC_SDA=0;
				txd<<=1; 	  	//��TEA5767��������ʱ���Ǳ����
				delay_us(2);
		  	IIC_SCL=1;
				delay_us(2);
			  IIC_SCL=0;	
				delay_us(2);
    }	
//		IIC_SCL=0;
//		delay_us(20);
//		IIC_SDA = 1;
//		delay_us(20);
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	  {
				IIC_SCL=0;
				delay_us(2);
        IIC_SCL=1; 
				delay_us(1);
        receive<<=1;
        if(READ_SDA)receive++; 
				delay_us(1);
    }		
    	if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}

//��FM24CL64ָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
u8 FM24CL64_ReadOneByte(u16 ReadAddr)
{				  
		u8 temp=0;		  	    																 
    IIC_Start();  
		IIC_Send_Byte(0XA0);	   //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//���͸ߵ�ַ	 5   
		IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   //���͵͵�ַ
		IIC_Wait_Ack();	    
		IIC_Start();  	 	   
		IIC_Send_Byte(0XA1);         //�������ģʽ 
		IIC_Wait_Ack();	 
		temp=IIC_Read_Byte(0);		
		IIC_Wait_Ack();		
		IIC_Stop();//����һ��ֹͣ����	    
	return temp;
}
//��FM24CL64ָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void FM24CL64_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC_Start();  
		IIC_Send_Byte(0XA0);	    //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte((WriteAddr>>8));//���͸ߵ�ַ	
		IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr%256);   //���͵͵�ַ
		IIC_Wait_Ack(); 	 										  		   
		IIC_Send_Byte(DataToWrite);     //�����ֽ�		
		IIC_Wait_Ack();  		    	   
    IIC_Stop();//����һ��ֹͣ���� 
	  delay_ms(10);
}


//��AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������
//�ú�������д��16bit����32bit������.
//WriteAddr  :��ʼд��ĵ�ַ  
//DataToWrite:���������׵�ַ
//Len        :Ҫд�����ݵĳ���2,4
void FM24CL64_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		FM24CL64_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}

//��AT24CXX�����ָ����ַ��ʼ��������ΪLen������
//�ú������ڶ���16bit����32bit������.
//ReadAddr   :��ʼ�����ĵ�ַ 
//����ֵ     :����
//Len        :Ҫ�������ݵĳ���2,4
u32 FM24CL64_ReadLenByte(u16 ReadAddr,u8 Len)
{  	
	u8 t;
	u32 temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=FM24CL64_ReadOneByte(ReadAddr+Len-t-1); 	 				   
	}
	return temp;												    
}
//��FM24CL64�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ 
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
void FM24CL64_Read(u16 ReadAddr,u32 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=FM24CL64_ReadOneByte(ReadAddr++);	
//		*pBuffer++=FM24CL64_ReadLenByte(ReadAddr++,4);
		NumToRead--;
	}
}  
//��FM24CL64�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ 
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
void FM24CL64_Write(u16 WriteAddr,u32 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		FM24CL64_WriteOneByte(WriteAddr,*pBuffer);
//		FM24CL64_WriteLenByte(WriteAddr,*pBuffer,4);
		WriteAddr++;
		pBuffer++;
	}
}

u8 FM24CL64_Check(void)
{
	u8 temp;
	temp=FM24CL64_ReadOneByte(100);//����ÿ�ο�����д		   
	if(temp==0X55)return 0;		   
	else//�ų���һ�γ�ʼ�������
	{
		  FM24CL64_WriteOneByte(100,0X55);
	    temp=FM24CL64_ReadOneByte(100);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}
