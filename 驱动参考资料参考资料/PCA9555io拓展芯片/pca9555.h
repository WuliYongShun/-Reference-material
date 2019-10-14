#ifndef __PCA9555_H
#define __PCA9555_H	 

extern u8 value_keep0_l,value_keep0_h,value_keep1_l,value_keep1_h,value_keep2_l,value_keep2_h;
extern u8 parameters;
extern u16 led_buf[5];
extern u8 value_feedback0[2],value_feedback1[2],value_feedback2[2]; 
extern u16 auto_off_time_count;
extern u16 auto_open_off_time_count[16];
extern u16 auto_close_off_time_count[16];
extern u8 doorstate[16];
extern u8 powerstate[8];
extern u8 Refresh_Led;
extern u8 Refresh_Power;

void delay(u32 conut);

void IIC_SDA_DIR_SET(u8 io_dir);
void IIC_Ready(void); 
void IIC_PORT_INIT(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Ack(void);
void IIC_Send_Ack(void);
void IIC_Send_NoAck(void);
void IIC_Write_Byte(u8 a);
u8 IIC_Read_Byte(void);
void IIC_Write_Pca9555(u8 addr,u8 reg_addr,u8 low_byte,u8 high_byte);
void IIC_Read_Pca9555(u8 addr,u8 reg_addr,u8* pBuffer,u16 num);

void IIC_SDA_DIR_SET_LED(u8 io_dir);
void IIC_Ready_LED(void); 
void IIC_PORT_INIT_LED(void);
void IIC_Start_LED(void);
void IIC_Stop_LED(void);
void IIC_Ack_LED(void);
void IIC_Send_Ack_LED(void);
void IIC_Send_NoAck_LED(void);
void IIC_Write_Byte_LED(u8 a);
u8 IIC_Read_Byte_LED(void);
void IIC_Write_Pca9555_LED(u8 addr,u8 reg_addr,u8 low_byte,u8 high_byte);
void IIC_Read_Pca9555_LED(u8 addr,u8 reg_addr,u8* pBuffer,u16 num);

void write1(u8 PCA9555_dengban_ADDR,u8 pro2,u8 pro3);	 //控制灯板上的pca芯片
void write2(u8 PCA9555_zkb_ADDR,u8 pro0,u8 pro1);		 //控制主控板的上pca芯片


void I2C_ReadS_PCA9555(u8 PCA9555_zhukong_ADDR,u8 PCA9555_REG_IN,u8* pBuffer,u16 no);
void I2C_ReadS_PCA95552(u8 PCA9555_dengban_ADDR,u8 PCA9555_REG_IN,u8* pBuffer,u16 no);
void PCA9555_Init(void);
void I2CM_Init(void) ;	

void Set_Value_To_Output(u8 num);
void Clear_Value_To_Output(u8 num);
void Change_Value_To_Output(u8 num,u8 value);
void All_Off__To_Output(void);
void All_On__To_Output(void);
void All_Off__To_OpenDoor_Output(void);
void All_On__To_OpenDoor_Output(void);
void All_Off__To_Power_Output(void);
void All_On__To_Power_Output(void);
void All_Off__To_CloseDoor_Output(void);
void All_On__To_CloseDoor_Output(void);
u16 uart_deal_function(void);

void Led_Off_RGB(u8 num,u16 value);
void Led_On_RGB(u8 num,u16 value);
void Led_Off_R(u8 num);
void Led_On_R(u8 num);
void Led_Off_G(u8 num);
void Led_On_G(u8 num);
void Led_Off_B(u8 num);
void Led_On_B(u8 num);	 
void All_Led_On(void);
void All_Led_Off(void);

void Refresh_Door_Condition(void);
void Refresh_Power_Condition(void);
void Refresh_Door_Condition_Led(void);
void Refresh_Power_Condition_Led(void);
void PowerOn_Auto_CloseOff(void);				    
#endif
