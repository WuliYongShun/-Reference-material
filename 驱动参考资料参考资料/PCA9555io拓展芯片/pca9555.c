
#include "stm32f10x.h"
#include "pca9555.h"
#include "usart.h"
#include "timer2.h"
#include "wdg.h"

#define SCL           GPIO_Pin_6
#define SDA           GPIO_Pin_7
#define SCL_LED           GPIO_Pin_10
#define SDA_LED           GPIO_Pin_11
#define IIC_BIT_DELAY           5
#define DELAY			delay(100)

//  定义PCA9555（NXP半导体I/O扩展芯片，I2C接口）    
#define  PCA9555_SLA             (0x40 >> 1)                 //  定义PAC9555的器件地址    
#define  PCA9555_REG_IN0         0x00                        //  定义输入寄存器0地址    
#define  PCA9555_REG_IN1         0x01                        //  定义输入寄存器1地址    
#define  PCA9555_REG_OUT0        0x02                        //  定义输出寄存器0地址    
#define  PCA9555_REG_OUT1        0x03                        //  定义输出寄存器1地址    
#define  PCA9555_REG_POL0        0x04                        //  定义极性反转寄存器0地址    
#define  PCA9555_REG_POL1        0x05                        //  定义极性反转寄存器1地址 
#define  PCA9555_REG_CFG0        0x06                        //  定义方向配置寄存器0地址    
#define  PCA9555_REG_CFG1        0x07                        //  定义方向配置寄存器1地址      
#define  PCA9555_DEVICE_ADDR     0x40                        //  定义PCA9555地址 

#define  LOOP_COUNT     10000

u8 pro0=0xff,pro1=0xff,pro2=0xff,pro3=0xff,PCA9555_dengban_ADDR,PCA9555_zkb_ADDR;

u8 value_keep0_l,value_keep0_h,value_keep1_l,value_keep1_h,value_keep2_l,value_keep2_h;
u8 parameters;
u16 led_buf[5];
u8 value_feedback0[2],value_feedback1[2],value_feedback2[2];
u16 auto_off_time_count; 
u16 auto_open_off_time_count[16];
u16 auto_close_off_time_count[16]; 
u8 doorstate[16];
u8 powerstate[8];
u8 Refresh_Led=0;
u8 Refresh_Power=0;
/*//  定义全局变量    
static unsigned long I2CM_BASE = I2C0_MASTER_BASE;  //  定义I2C主机基址，并初始化    
static tI2CM_DEVICE gtDevice;                       //  器件数据接口    
static unsigned short gusStatus = STAT_IDLE;        //  工作状态    
//static tBoolean gbSendRecv;                         //  收发操作标志，false发送，true接收    
static char gcAddr[4];                              //  数据地址数组    
static unsigned short gusAddrIndex;                 //  数据地址数组索引变量    
static unsigned short gusDataIndex;                 //  数据缓冲区索引变量    
 */ 

void delay(u32 conut)
{
		while(conut -- > 0);
}


void IIC_SDA_DIR_SET(u8 io_dir) //SDA引脚输入输出设置
{
	GPIO_InitTypeDef GPIO_InitStructure;
	if(io_dir==0)
	{
		GPIO_InitStructure.GPIO_Pin = SDA;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;//output
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure); 
	}
	else if(io_dir==1)
	{
		GPIO_InitStructure.GPIO_Pin = SDA; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//input  pull_up
		GPIO_Init(GPIOB, &GPIO_InitStructure); 
	}
}

void IIC_SDA_DIR_SET_LED(u8 io_dir) //SDA_LED引脚输入输出设置
{
	GPIO_InitTypeDef GPIO_InitStructure;
	if(io_dir==0)
	{
		GPIO_InitStructure.GPIO_Pin = SDA_LED;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;//output
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure); 
	}
	else if(io_dir==1)
	{
		GPIO_InitStructure.GPIO_Pin = SDA_LED; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//input  pull_up
		GPIO_Init(GPIOB, &GPIO_InitStructure); 
	}
}

void IIC_Ready(void)
{
   
   	GPIO_SetBits(GPIOB,SCL);
  	DELAY;
   	GPIO_SetBits(GPIOB,SDA);
   	DELAY;
}
 
void IIC_PORT_INIT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); 

 	GPIO_InitStructure.GPIO_Pin = SCL|SDA;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	IIC_Ready(); 
} 

void IIC_Start(void)
{
	GPIO_SetBits(GPIOB,SDA);
	DELAY;
	GPIO_SetBits(GPIOB,SCL);
	DELAY;
	GPIO_ResetBits(GPIOB, SDA);
	DELAY;
}

void IIC_Stop(void)

{
	GPIO_ResetBits(GPIOB, SDA);
	DELAY;
	GPIO_SetBits(GPIOB,SCL);
	DELAY;
	GPIO_SetBits(GPIOB,SDA);
	DELAY;
}

void IIC_Ack(void)

{
	u16 i;
	GPIO_SetBits(GPIOB,SCL);
	DELAY;
	IIC_SDA_DIR_SET(1);
	while(GPIO_ReadInputDataBit(GPIOB,SDA)&&(i<0x2b0)) {i++;}
	IIC_SDA_DIR_SET(0);
	GPIO_ResetBits(GPIOB,SCL);
	DELAY;
}

void IIC_Send_Ack(void)

{
	GPIO_ResetBits(GPIOB,SDA);
	DELAY;
	GPIO_SetBits(GPIOB,SCL);
	DELAY;
	GPIO_ResetBits(GPIOB,SCL);
	DELAY;
}

void IIC_Send_NoAck(void)

{
	GPIO_SetBits(GPIOB,SDA);
	DELAY;
	GPIO_SetBits(GPIOB,SCL);
	DELAY;
	GPIO_ResetBits(GPIOB,SCL);
	DELAY;
}


void IIC_Write_Byte(u8 a)
{
	u16 i;
	GPIO_ResetBits(GPIOB,SCL); //scl=0;
	DELAY;
	for(i=0;i<8;i++)
	{
		if(a&0x80)
		GPIO_SetBits(GPIOB,SDA);//sda=1;
		else
		GPIO_ResetBits(GPIOB,SDA);

		a=a<<1;
		GPIO_SetBits(GPIOB,SCL);//scl=1;
		DELAY;
		GPIO_ResetBits(GPIOB,SCL);//scl=0;
		DELAY;
	}
	GPIO_SetBits(GPIOB,SDA);//sda=1;
	DELAY;
}

u8 IIC_Read_Byte(void)
{
	u8 i,temp;
	temp=0;
	GPIO_SetBits(GPIOB,SDA);//sda=1;
	DELAY;
	GPIO_ResetBits(GPIOB,SCL); //scl=0;
	DELAY;
	IIC_SDA_DIR_SET(1);
	for(i=0;i<8;i++)
	{
		GPIO_SetBits(GPIOB,SCL); //scl=1;
		DELAY;
		DELAY;
		temp=(temp<<1)|GPIO_ReadInputDataBit(GPIOB,SDA);
		DELAY;
		GPIO_ResetBits(GPIOB,SCL); //scl=0;
		DELAY;
	}
	IIC_SDA_DIR_SET(0);
	GPIO_SetBits(GPIOB,SDA);//sda=1;
	DELAY;
	//GPIO_ResetBits(GPIOB,SCL);//scl=0;
	//DELAY;
	return temp;
}
////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
void IIC_Ready_LED(void)
{
   
   	GPIO_SetBits(GPIOB,SCL_LED);
  	DELAY;
   	GPIO_SetBits(GPIOB,SDA_LED);
   	DELAY;
}
 
void IIC_PORT_INIT_LED(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); 

 	GPIO_InitStructure.GPIO_Pin = SCL_LED|SDA_LED;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	IIC_Ready_LED(); 
} 

void IIC_Start_LED(void)
{
	GPIO_SetBits(GPIOB,SDA_LED);
	DELAY;
	GPIO_SetBits(GPIOB,SCL_LED);
	DELAY;
	GPIO_ResetBits(GPIOB, SDA_LED);
	DELAY;
}

void IIC_Stop_LED(void)

{
	GPIO_ResetBits(GPIOB, SDA_LED);
	DELAY;
	GPIO_SetBits(GPIOB,SCL_LED);
	DELAY;
	GPIO_SetBits(GPIOB,SDA_LED);
	DELAY;
}

void IIC_Ack_LED(void)

{
	u16 i;
	GPIO_SetBits(GPIOB,SCL_LED);
	DELAY;
	IIC_SDA_DIR_SET_LED(1);
	while(GPIO_ReadInputDataBit(GPIOB,SDA_LED)&&(i<0x2b0)) {i++;}
	IIC_SDA_DIR_SET_LED(0);
	GPIO_ResetBits(GPIOB,SCL_LED);
	DELAY;
}

void IIC_Send_Ack_LED(void)

{
	GPIO_ResetBits(GPIOB,SDA_LED);
	DELAY;
	GPIO_SetBits(GPIOB,SCL_LED);
	DELAY;
	GPIO_ResetBits(GPIOB,SCL_LED);
	DELAY;
}

void IIC_Send_NoAck_LED(void)

{
	GPIO_SetBits(GPIOB,SDA_LED);
	DELAY;
	GPIO_SetBits(GPIOB,SCL_LED);
	DELAY;
	GPIO_ResetBits(GPIOB,SCL_LED);
	DELAY;
}

void IIC_Write_Byte_LED(u8 a)
{
	u16 i;
	GPIO_ResetBits(GPIOB,SCL_LED); //scl=0;
	DELAY;
	for(i=0;i<8;i++)
	{
		if(a&0x80)
		GPIO_SetBits(GPIOB,SDA_LED);//sda=1;
		else
		GPIO_ResetBits(GPIOB,SDA_LED);

		a=a<<1;
		GPIO_SetBits(GPIOB,SCL_LED);//scl=1;
		DELAY;
		GPIO_ResetBits(GPIOB,SCL_LED);//scl=0;
		DELAY;
	}
	GPIO_SetBits(GPIOB,SDA_LED);//sda=1;
	DELAY;
}

u8 IIC_Read_Byte_LED(void)
{
	u8 i,temp;
	temp=0;
	GPIO_SetBits(GPIOB,SDA_LED);//sda=1;
	DELAY;
	GPIO_ResetBits(GPIOB,SCL_LED); //scl=0;
	DELAY;
	IIC_SDA_DIR_SET_LED(1);
	for(i=0;i<8;i++)
	{
		GPIO_SetBits(GPIOB,SCL_LED); //scl=1;
		DELAY;
		DELAY;
		temp=(temp<<1)|GPIO_ReadInputDataBit(GPIOB,SDA_LED);
		DELAY;
		GPIO_ResetBits(GPIOB,SCL_LED); //scl=0;
		DELAY;
	}
	IIC_SDA_DIR_SET_LED(0);
	GPIO_SetBits(GPIOB,SDA_LED);//sda=1;
	DELAY;
	//GPIO_ResetBits(GPIOB,SCL_LED);//scl=0;
	//DELAY;
	return temp;
}

void IIC_Write_Pca9555(u8 addr,u8 reg_addr,u8 low_byte,u8 high_byte)
{
	  IIC_Start();
	  IIC_Write_Byte(addr & 0xfe);
	  IIC_Ack();
	  IIC_Write_Byte(reg_addr);
	  IIC_Ack();
	  IIC_Write_Byte(low_byte);
	  IIC_Ack();
	  IIC_Write_Byte(high_byte);
	  IIC_Ack();
	  IIC_Stop();
}

void IIC_Write_Pca9555_LED(u8 addr,u8 reg_addr,u8 low_byte,u8 high_byte)
{
	  IIC_Start_LED();
	  IIC_Write_Byte_LED(addr & 0xfe);
	  IIC_Ack_LED();
	  IIC_Write_Byte_LED(reg_addr);
	  IIC_Ack_LED();
	  IIC_Write_Byte_LED(low_byte);
	  IIC_Ack_LED();
	  IIC_Write_Byte_LED(high_byte);
	  IIC_Ack_LED();
	  IIC_Stop_LED();
}

void IIC_Read_Pca9555(u8 addr,u8 reg_addr,u8* pBuffer,u16 num)
{
	  IIC_Start();
	  IIC_Write_Byte(addr & 0xfe);
	  IIC_Ack();
	  IIC_Write_Byte(reg_addr);
	  IIC_Ack();

	  IIC_Start();
	  IIC_Write_Byte(addr | 0x01);
	  IIC_Ack();
	  while (num)
	  {
		    *pBuffer = IIC_Read_Byte();
			if (num == 1)
			{
				IIC_Send_NoAck();
			}
			else
			{ 
				IIC_Send_Ack();
			}	
		    pBuffer++;
			num--;
	  }
	  IIC_Stop();
}

void IIC_Read_Pca9555_LED(u8 addr,u8 reg_addr,u8* pBuffer,u16 num)
{
	  IIC_Start_LED();
	  IIC_Write_Byte_LED(addr & 0xfe);
	  IIC_Ack_LED();
	  IIC_Write_Byte_LED(reg_addr);
	  IIC_Ack_LED();

	  IIC_Start_LED();
	  IIC_Write_Byte_LED(addr | 0x01);
	  IIC_Ack_LED();
	  while (num)
	  {
		    *pBuffer = IIC_Read_Byte_LED();
			if (num == 1)
			{
				IIC_Send_NoAck_LED();
			}
			else
			{ 
				IIC_Send_Ack_LED();
			}	
		    pBuffer++;
			num--;
	  }
	  IIC_Stop_LED();
}

//  I2C主机初始化    
void I2CM_Init(void)   
{   
//    I2CM_DeviceInitSet(>Device, 0, 0, 0, (void *)0, 0);   
   
    	I2C_InitTypeDef  I2C_InitStructure;
	  	GPIO_InitTypeDef  GPIO_InitStructure; 

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2 | RCC_APB1Periph_I2C1,ENABLE);
    	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    

		/* PB6,7 SCL and SDA */
		GPIO_InitStructure.GPIO_Pin =GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_10 | GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; 	//设置管脚为复用功能开漏输出
		GPIO_Init(GPIOB, &GPIO_InitStructure);          	//
	

		I2C_DeInit(I2C2);
		I2C_DeInit(I2C1);
	    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;         	//
	    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;	//
	    I2C_InitStructure.I2C_OwnAddress1 = 0x30;          	//
	    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;        	//
	    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	    I2C_InitStructure.I2C_ClockSpeed = 400000;        	//
    
		I2C_Cmd(I2C2, ENABLE);
		I2C_Cmd(I2C1, ENABLE);                            	//
		I2C_Init(I2C2, &I2C_InitStructure);               	//
		I2C_Init(I2C1, &I2C_InitStructure);
		I2C_AcknowledgeConfig(I2C2, ENABLE); 
		I2C_AcknowledgeConfig(I2C1, ENABLE); 
}   


//写主控板上的pca9555
/*
  u8 PCA9555_dengban_ADDR  芯片地址，0x40表示第一个芯片地址
  u8 pro2  写入芯片的前八位数据
  u8 pro3   写入芯片的前后位数据

*/
void write1(u8 PCA9555_dengban_ADDR,u8 pro2,u8 pro3)

{                         
//__disable_irq ; 
	
	IIC_Write_Pca9555_LED(PCA9555_dengban_ADDR,0x02,pro2,pro3);
	//IIC_Write_Pca9555_LED(PCA9555_dengban_ADDR,0x06,0x00,0x00);

//__enable_irq ;                            


}


 /********************************主控板上的pca555控制程序*********************************************/
    
  //////////////////////////////////////////////////////////////////写命令 i2c1操作的是主控板上的pca555
void write2(u8 PCA9555_zkb_ADDR,u8 pro0,u8 pro1)
{                        
//__disable_irq ; 

	IIC_Write_Pca9555(PCA9555_zkb_ADDR,0x02,pro0,pro1);
	//IIC_Write_Pca9555(PCA9555_zkb_ADDR,0x06,0x00,0x00);

//__enable_irq ;                            

}




/*****************************读命令主控板*****************************************/

void I2C_ReadS_PCA9555(u8 PCA9555_zhukong_ADDR,u8 PCA9555_REG_IN,u8* pBuffer,u16 no)

{
/*
 *Read from register
 */    
//__disable_irq ;
	//IIC_Write_Pca9555(PCA9555_zhukong_ADDR,0x06,0xff,0xff); 	
	IIC_Read_Pca9555(PCA9555_zhukong_ADDR,PCA9555_REG_IN,pBuffer,no);
//__enable_irq ;   //开中断
}



  /*****************************读命令灯板*****************************************/

void I2C_ReadS_PCA95552(u8 PCA9555_dengban_ADDR,u8 PCA9555_REG_IN,u8* pBuffer,u16 no)

{
/*
 *Read from register

 */
                        
//__disable_irq ; 
 	IIC_Read_Pca9555_LED(PCA9555_dengban_ADDR,PCA9555_REG_IN,pBuffer,no);
//__enable_irq ;                            

}


void Set_Value_To_Output(u8 num)
{
	 if (num <17)
	 {
	 		if  (num <9)
			{
				   value_keep0_l |= 0x01<<(num - 1);
				   write2(0x44,value_keep0_l,value_keep0_h);
				   return;
			}
			else
			{
				   value_keep0_h |= 0x01<<(num - 9);
				   write2(0x44,value_keep0_l,value_keep0_h);
				   return;
			}
	 }
	 else if (num >16 && num <33)
	 {
	 		if  (num <25)
			{
				   value_keep1_l |= 0x01<<(num - 17);
				   write2(0x46,value_keep1_l,value_keep1_h);
				   return;
			}
			else
			{
				   value_keep1_h |= 0x01<<(num - 25);
				   write2(0x46,value_keep1_l,value_keep1_h);
				   return;
			}
	 }
	 else if (num <41)
	 {
	 		value_keep2_l |= 0x01<<(num - 33);
			write2(0x4a,value_keep2_l,value_keep2_h);
			return;
	 }
	 else
	 {
	 		//error
	 }
}

void Clear_Value_To_Output(u8 num)
{
	 if (num <17)
	 {
	 		if  (num <9)
			{
				   value_keep0_l &= ~(0x01<<(num - 1));
				   write2(0x44,value_keep0_l,value_keep0_h);
				   return;
			}
			else
			{
				   value_keep0_h &= ~(0x01<<(num - 9));
				   write2(0x44,value_keep0_l,value_keep0_h);
				   return;
			}
	 }
	 else if (num >16 && num <33)
	 {
	 		if  (num <25)
			{
				   value_keep1_l &= ~(0x01<<(num - 17));
				   write2(0x46,value_keep1_l,value_keep1_h);
				   return;
			}
			else
			{
				   value_keep1_h &= ~(0x01<<(num - 25));
				   write2(0x46,value_keep1_l,value_keep1_h);
				   return;
			}
	 }
	 else if (num <41)
	 {
	 		value_keep2_l &= ~(0x01<<(num - 33));
			write2(0x4a,value_keep2_l,value_keep2_h);
			return;
	 }
	 else
	 {
	 		//error
	 }
}

void Change_Value_To_Output(u8 num,u8 value)
{
		if (value == 0)
			 Clear_Value_To_Output(num);
		else
			 Set_Value_To_Output(num);
}

void All_Off__To_Output(void)
{
		write2(0x44,0,0);
		write2(0x46,0,0);
		write2(0x4a,0,0);
		value_keep0_l = 0;value_keep0_h = 0;
		value_keep1_l = 0;value_keep1_h = 0;
		value_keep2_l = 0;value_keep2_h = 0;
}	

void All_On__To_Output(void)
{
		write2(0x44,0xff,0xff);//delay(100000);
		write2(0x46,0xff,0xff);//delay(100000);
		write2(0x4a,0xff,0xff);//delay(100000);
		value_keep0_l = 0xff;value_keep0_h = 0xff;
		value_keep1_l = 0xff;value_keep1_h = 0xff;
		value_keep2_l = 0xff;value_keep2_h = 0xff;
}

void All_Off__To_OpenDoor_Output(void)
{		
		write2(0x46,0,0);
		write2(0x44,0,0);
		value_keep0_l = 0;value_keep0_h = 0;
		value_keep1_l = 0;value_keep1_h = 0;
}	

void All_On__To_OpenDoor_Output(void)
{
		write2(0x44,0xff,0xff);
		write2(0x46,0xff,0xff);
		value_keep0_l = 0xff;value_keep0_h = 0xff;
		value_keep1_l = 0xff;value_keep1_h = 0xff;

}
void All_Off__To_CloseDoor_Output(void)
{		
		write2(0x46,0,0);
		write2(0x44,0,0);
		value_keep0_l = 0;value_keep0_h = 0;
		value_keep1_l = 0;value_keep1_h = 0;
}	

void All_On__To_CloseDoor_Output(void)
{
		write2(0x44,0,0);
		write2(0x46,0xff,0xff);
		value_keep0_l = 0;value_keep0_h = 0;
		value_keep1_l = 0xff;value_keep1_h = 0xff;

}
void All_Off__To_Power_Output(void)
{
		write2(0x4a,0,0);
		value_keep2_l = 0;value_keep2_h = 0;
}	

void All_On__To_Power_Output(void)
{
		write2(0x4a,0xff,0xff);
		value_keep2_l = 0xff;value_keep2_h = 0xff;

}

u16 uart_deal_function(void)
{
	u8 i;
	u8 temp_data[BUFFER_LEN],temp_data_length;
	u8 state=0;
	if (uart_data_length > 0 && uart_timerout_count == 0)
	{
		//__disable_irq ;
		for (i=0;i<uart_data_length;i++)
		{
			temp_data[i] = uart_data485[i];
		}
		temp_data_length = 	uart_data_length;
	
		uart_data_length = 0;
		//__enable_irq ;
		for (i=0;i<temp_data_length;i++)
		{
			//Send_Byte(temp_data[i]);
			switch(state)
			{
				case 0:	
					if (temp_data[i] == 'o')
					{
						state = 10;
					}
					else if (temp_data[i] == 'c')
					{
						state = 20;
					}
					else if (temp_data[i] == 't')
					{
						state = 30;
					}
					else if (temp_data[i] == 'd')
					{
						if (temp_data_length - i > 8)
						{
							if (temp_data[i+1] == 'o')
								if (temp_data[i+2] == 'o')
									if (temp_data[i+3] == 'r')
										if (temp_data[i+4] == 's')
											if (temp_data[i+5] == 't')
												if (temp_data[i+6] == 'a')
													if (temp_data[i+7] == 't')
														if (temp_data[i+8] == 'e')
														{
															return 10;//doorstate
														}


							state = 0;
						}
					}
					else if (temp_data[i] == 'p')
					{
						if (temp_data_length - i > 9)
						{
							if (temp_data[i+1] == 'o')
								if (temp_data[i+2] == 'w')
									if (temp_data[i+3] == 'e')
										if (temp_data[i+4] == 'r')
											if (temp_data[i+5] == 's')
												if (temp_data[i+6] == 't')
													if (temp_data[i+7] == 'a')
														if (temp_data[i+8] == 't')
															if (temp_data[i+9] == 'e')
															{
																return 11;//powerstate
															}


							state = 0;
						}
					}
					else
					{
					}
					break;
				case 10:
					if (temp_data[i] == 'p')
					{
						state = 11;
					}
					else if (temp_data[i] == 'n')
					{
						if (i + 1 == temp_data_length)
						{
							return 3;//on
						}
						else
						{
							if (temp_data_length - i > 1)
							{
								   if (temp_data[i+1] > 0x2f && temp_data[i+1] < 0x3a )
								   {
								   		parameters = temp_data[i+1] - 0x30;
								   		return 4 + (parameters<<8);//onx
								   }
								   else
								   		state = 0;
							}
							else
								return 0;
						}
					}
					else if (temp_data[i] == 'f')
					{
						state = 13;
					}
					else
					{
						state = 0;
					}
					break;
				case 11:
					if (temp_data[i] == 'e')
					{
						state = 14;
					}
					else
					{
						state = 0;
					}
					break;
				case 12:
					break;
				case 13:
					if (temp_data[i] == 'f')
					{
						if (i + 1 == temp_data_length)
						{
							return 5;//off
						}
						else
						{
							if (temp_data_length - i > 1)
							{
								   if (temp_data[i+1] > 0x2f && temp_data[i+1] < 0x3a )
								   {
								   		parameters = temp_data[i+1] - 0x30;
								   		return 6 + (parameters<<8);//offx
								   }
								   else
								   		state = 0;
							}
							else
								return 0;
						}
					}
					else
					{
						state = 0;
					}
					break;
				case 14:
					if (temp_data[i] == 'n')
					{
						if (i + 1 == temp_data_length)
						{
							return 1;//open
						}
						else
						{
							if (temp_data_length - i > 2)
							{
								   if (temp_data[i+1] > 0x2f && temp_data[i+1] < 0x3a && temp_data[i+2] > 0x2f && temp_data[i+2] < 0x3a)
								   {
								   		parameters = (temp_data[i+1] - 0x30) * 10 + (temp_data[i+2] - 0x30);
								   		return 2 + (parameters<<8);//openxx
								   }
								   else
								   		state = 0;
							}
							else
								return 0;
						}
					}
					else
					{
						state = 0;
					}
					break;
				case 20:
					if (temp_data[i] == 'l')
					{
						state = 21;
					}
					else
					{
						state = 0;
					}
					break;
				case 21:
					if (temp_data[i] == 'o')
					{
						state = 22;
					}
					else
					{
						state = 0;
					}
					break;
				case 22:
					if (temp_data[i] == 's')
					{
						state = 23;
					}
					else
					{
						state = 0;
					}
					break;
				case 23:
					if (temp_data[i] == 'e')
					{
						if (i + 1 == temp_data_length)
						{
							return 7;//close
						}
						else
						{
							if (temp_data_length - i > 2)
							{
								   if (temp_data[i+1] > 0x2f && temp_data[i+1] < 0x3a && temp_data[i+2] > 0x2f && temp_data[i+2] < 0x3a)
								   {
								   		parameters = (temp_data[i+1] - 0x30) * 10 + (temp_data[i+2] - 0x30);
								   		return 8 + (parameters<<8);//closexx
								   }
								   else
								   		state = 0;
							}
							else
								return 0;
						}
					}
					else
					{
						state = 0;
					}
					break;
				case 30:
					if (temp_data[i] == 'i')
					{
						state = 31;
					}
					else
					{
						state = 0;
					}
					break;
				case 31:
					if (temp_data[i] == 'm')
					{
						state = 32;
					}
					else
					{
						state = 0;
					}
					break;
				case 32:
					if (temp_data[i] == 'e')
					{
						
						if (temp_data_length - i > 2)
						{
							   if (temp_data[i+2] == 's')
							   {
							   		if (temp_data[i+1] > 0x30 && temp_data[i+1] < 0x3a)
									{
								   		parameters = temp_data[i+1] - 0x30;
								   		return 9 + (parameters<<8);//timexs
									}
									else if (temp_data[i+1] == 0x41)
									{
										parameters = 0x0a;
								   		return 9 + (parameters<<8);//timexs
									}
									else
										state = 0;
							   }
							   else
							   		state = 0;
						}
						else
							return 0;
					}
					else
					{
						state = 0;
					}
					break;
			}
		}
		return 0;
	}
	else
		return 0;

}

void Led_On_RGB(u8 num,u16 value)
{
		if (num < 6)
		{
			led_buf[0] &= (~(value<<((num-1)*3)));
			write1(0x40,led_buf[0],led_buf[0] >> 8);
		}
		else if(num > 6 && num <11)
		{
			led_buf[1] &= (~(value<<((num-7)*3+2)));
			write1(0x42,led_buf[1],led_buf[1] >> 8);
		}
		else if(num > 11 && num <17)
		{
			led_buf[2] &= (~(value<<((num-12)*3+1)));
			write1(0x44,led_buf[2],led_buf[2] >> 8);
		}
		else if(num > 16 && num <22)
		{
			led_buf[3] &=( ~(value<<((num-17)*3)));
			write1(0x46,led_buf[3],led_buf[3] >> 8);
		}
		else if(num > 22 && num <25)
		{
			led_buf[4] &= (~(value<<((num-23)*3+2)));
			write1(0x48,led_buf[4],led_buf[4] >> 8);
		}
		else if (num == 6)
		{
			led_buf[0] &= (~(value<<15));
			led_buf[1] &= (0xfffc | (~(value>>1)));
			write1(0x40,led_buf[0],led_buf[0] >> 8);
			write1(0x42,led_buf[1],led_buf[1] >> 8);
		}
		else if (num == 11)
		{
			led_buf[1] &= (~(value<<14));
			led_buf[2] &= (0xfffe | (~(value>>2)));
			write1(0x42,led_buf[1],led_buf[1] >> 8);
			write1(0x44,led_buf[2],led_buf[2] >> 8);
		}
		else if (num == 22)
		{
			led_buf[3] &= (~(value<<15));
			led_buf[4] &= (0xfffc | (~(value>>1)));
			write1(0x46,led_buf[3],led_buf[3] >> 8);
			write1(0x48,led_buf[4],led_buf[4] >> 8);
		}	
}

void Led_Off_RGB(u8 num,u16 value)
{
		if (num < 6)
		{
			led_buf[0] |= (value<<((num-1)*3));
			write1(0x40,led_buf[0],led_buf[0] >> 8);
		}
		else if(num > 6 && num <11)
		{
			led_buf[1] |= (value<<((num-7)*3+2));
			write1(0x42,led_buf[1],led_buf[1] >> 8);
		}
		else if(num > 11 && num <17)
		{
			led_buf[2] |= (value<<((num-12)*3+1));
			write1(0x44,led_buf[2],led_buf[2] >> 8);
		}
		else if(num > 16 && num <22)
		{
			led_buf[3] |= (value<<((num-17)*3));
			write1(0x46,led_buf[3],led_buf[3] >> 8);
		}
		else if(num > 22 && num <25)
		{
			led_buf[4] |= (value<<((num-23)*3+2));
			write1(0x48,led_buf[4],led_buf[4] >> 8);
		}
		else if (num == 6)
		{
			led_buf[0] |= ((~0x7fff) & (value<<15));
			led_buf[1] |= ((~0xfffc) & ((value>>1)& 0x7fff));
			write1(0x40,led_buf[0],led_buf[0] >> 8);
			write1(0x42,led_buf[1],led_buf[1] >> 8);
		}
		else if (num == 11)
		{
			led_buf[1] |= ((~0x3fff) & (value<<14));
			led_buf[2] |= ((~0xfffe) & ((value>>2)&0x3fff));
			write1(0x42,led_buf[1],led_buf[1] >> 8);
			write1(0x44,led_buf[2],led_buf[2] >> 8);
		}
		else if (num == 22)
		{
			led_buf[3] |= ((~0x7fff) & (value<<15));
			led_buf[4] |= ((~0xfffc) & ((value>>1)& 0x7fff));
			write1(0x46,led_buf[3],led_buf[3] >> 8);
			write1(0x48,led_buf[4],led_buf[4] >> 8);
		}	
}

void Led_Off_R(u8 num)
{
	Led_Off_RGB(num,0x0004);
}
void Led_On_R(u8 num)
{
	Led_On_RGB(num,0x0004);
}
void Led_Off_G(u8 num)
{
	Led_Off_RGB(num,0x0002);
}
void Led_On_G(u8 num)
{
	Led_On_RGB(num,0x0002);
}
void Led_Off_B(u8 num)
{
	Led_Off_RGB(num,0x0001);
}
void Led_On_B(u8 num)
{
	Led_On_RGB(num,0x0001);
}
void All_Led_Off(void)
{
		write1(0x40,0xff,0xff);
		write1(0x42,0xff,0xff);
		write1(0x44,0xff,0xff);
		write1(0x46,0xff,0xff);
		write1(0x48,0xff,0xff);
		led_buf[0] = ~0;
		led_buf[1] = ~0;
		led_buf[2] = ~0;
		led_buf[3] = ~0;
		led_buf[4] = ~0;
}
void All_Led_On(void)
{
		write1(0x40,0x00,0x00);
		write1(0x42,0x00,0x00);
		write1(0x44,0x00,0x00);
		write1(0x46,0x00,0x00);
		write1(0x48,0x00,0x00);
		led_buf[0] = 0x00;
		led_buf[1] = 0x00;
		led_buf[2] = 0x00;
		led_buf[3] = 0x00;
		led_buf[4] = 0x00;
}

void Refresh_Power_Condition(void)
{
	I2C_ReadS_PCA9555(0x4d,0,value_feedback2,2);
	Refresh_Power = 1;
}

void Refresh_Power_Condition_Led(void)
{
	u8 i;
	for (i=0;i<8;i++)
	{
		if (value_feedback2[0] & (0x01<<i))
		{
			Led_Off_G(i+1);Led_On_R(i+1);
			powerstate[i] = 0x30;
		}
		else
		{
			Led_Off_R(i+1);Led_On_G(i+1);
			powerstate[i] = 0x31;
		}
	}
}

void Refresh_Door_Condition(void)
{
	u8 i;	 
	I2C_ReadS_PCA9555(0x40,0,value_feedback0,2);
	I2C_ReadS_PCA9555(0x42,0,value_feedback1,2);
	
	for (i=0;i<8;i++)
	{
		if ((value_feedback0[0] & (0x01<<i)) == (value_feedback1[0] & (0x01<<i)))
		{
			//Led_Off_G(i+9);Led_Off_R(i+9);Led_On_B(i+9);
			//doorstate[i] = 0x32;
		}
		else if ((value_feedback0[0] & (0x01<<i)) == 0 )
		{
			if (auto_open_off_time_count[i]!= 0xffff)
			{
				//auto_open_off_time_count[i] = 0;
				Clear_Value_To_Output(17+i);
				Clear_Value_To_Output(1+i);							   
				auto_open_off_time_count[i] = 0xffff;
			}
			//Led_Off_B(i+9);Led_Off_R(i+9);Led_On_G(i+9);
			//doorstate[i] = 0x31;
		}
		else
		{
			if (auto_close_off_time_count[i]!= 0xffff)
			{
				//auto_close_off_time_count[i] = 0;
				Clear_Value_To_Output(17+i);
				Clear_Value_To_Output(1+i);
				auto_close_off_time_count[i] = 0xffff;
			}
			//Led_Off_G(i+9);Led_Off_B(i+9);Led_On_R(i+9);
			//doorstate[i] = 0x30;
		}
	}
	for (i=0;i<8;i++)
	{
		if ((value_feedback0[1] & (0x01<<i)) == (value_feedback1[1] & (0x01<<i)))
		{
			//Led_Off_G(i+17);Led_Off_R(i+17);Led_On_B(i+17);
			//doorstate[i+8] = 0x32;
		}
		else if ((value_feedback0[1] & (0x01<<i)) == 0 )
		{
			if (auto_open_off_time_count[i+8]!= 0xffff)
			{
				//auto_open_off_time_count[i+8] = 0;
				Clear_Value_To_Output(25+i);
				Clear_Value_To_Output(9+i);
				auto_open_off_time_count[i+8] = 0xffff;
			}
			//Led_Off_B(i+17);Led_Off_R(i+17);Led_On_G(i+17);
			//doorstate[i+8] = 0x31;
		}
		else
		{
			if (auto_close_off_time_count[i+8] != 0xffff)
			{
				//auto_close_off_time_count[i+8] = 0;
				Clear_Value_To_Output(25+i);
				Clear_Value_To_Output(9+i);
				auto_close_off_time_count[i+8] = 0xffff;
			}
			//Led_Off_G(i+17);Led_Off_B(i+17);Led_On_R(i+17);
			//doorstate[i+8] = 0x30;
		}
	}
	Refresh_Led = 1;
}

void Refresh_Door_Condition_Led(void)
{
	u8 i;	 	
	for (i=0;i<8;i++)
	{
		if ((value_feedback0[0] & (0x01<<i)) == (value_feedback1[0] & (0x01<<i)))	 //value_feedback0[0]反馈数据  16路
		{
			Led_Off_G(i+9);Led_Off_R(i+9);Led_On_B(i+9);
			doorstate[i] = 0x32;	//ascii码的字符为2 表示异常
		}
		else if ((value_feedback0[0] & (0x01<<i)) == 0 )
		{
			Led_Off_B(i+9);Led_Off_R(i+9);Led_On_G(i+9);
			doorstate[i] = 0x31;
		}
		else
		{
			Led_Off_G(i+9);Led_Off_B(i+9);Led_On_R(i+9);
			doorstate[i] = 0x30;
		}
	}
	for (i=0;i<8;i++)
	{
		if ((value_feedback0[1] & (0x01<<i)) == (value_feedback1[1] & (0x01<<i)))
		{
			Led_Off_G(i+17);Led_Off_R(i+17);Led_On_B(i+17);
			doorstate[i+8] = 0x32;
		}
		else if ((value_feedback0[1] & (0x01<<i)) == 0 )
		{
			Led_Off_B(i+17);Led_Off_R(i+17);Led_On_G(i+17);
			doorstate[i+8] = 0x31;
		}
		else
		{
			Led_Off_G(i+17);Led_Off_B(i+17);Led_On_R(i+17);
			doorstate[i+8] = 0x30;
		}
	}
}

void PowerOn_Auto_CloseOff(void)
{
	u8 i;
	for (i=0;i<16;i++)
	{
		if (doorstate[i] != 0x30)
		{
			Clear_Value_To_Output(1+i);
			Set_Value_To_Output(17+i);
			auto_close_off_time_count[i] = auto_off_time_count;
		}
	}
}
