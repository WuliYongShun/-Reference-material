//-----------------------------------------------------------------------------
//PROJECT CODE : 5115测试
//CREATE DATE  : 2010.12.29
//CREATED BY   : GaoWei
//FUNCTION     : 机械老化检测
//部分说明     :

//-------------------------------- ---------------------------------------------  
#include <c8051f000.h>                   
#include"intrins.h"  
#include <stdio.h> 

//定义PCA9555A内部八个寄存器地址
#define PCA9555A_InPutPort0   		0x00     //输入端口0  
#define PCA9555A_InPutPort1   		0x01     //输入端口1
#define PCA9555A_OutPutPort0   	  0x02     //输出端口0
#define PCA9555A_OutPutPort1   	  0x03     //输出端口1
#define PCA9555A_Inversionport0   0x04		 //极性反转端口0
#define PCA9555A_Inversionport1   0x05		 //极性反转端口1
#define PCA9555A_Configport0			0x06		 //配制端口0
#define PCA9555A_Configport1			0x07		 //配制端口1

//#define  SYSCLK         4000000L       // System clock frequency in Hz

//#define  SMB_FREQUENCY  10000L         // Target SCL clock rate
                                       // This example supports between 10kHz
                                       // and 100kHz

#define  WRITE       0x00                 // WRITE direction bit
#define  READ        0x01                 // READ direction bit

#define  CHIP_A   0x40                 // Device address for chip A PCA9555DeviceAddress=0x40
#define  CHIP_B   0x42                 // Device address for chip B PCA9555DeviceAddress=0x41
#define  CHIP_C   0x44                 // Device address for chip C PCA9555DeviceAddress=0x42
#define  CHIP_D   0x46                 // Device address for chip D PCA9555DeviceAddress=0x43
#define  CHIP_E   0x48                 // Device address for chip E PCA9555DeviceAddress=0x44
#define  CHIP_F   0x4A                 // Device address for chip F PCA9555DeviceAddress=0x45
#define  CHIP_G   0x4C                 // Device address for chip G PCA9555DeviceAddress=0x46
#define  CHIP_H   0x4E                 // Device address for chip H PCA9555DeviceAddress=0x47

#define  SMB_BUS_ERROR  0x00        // (all modes) BUS ERROR
#define  SMB_START      0x08        // (MT & MR) START transmitted 起始条件已发出
#define  SMB_RP_START   0x10        // (MT & MR) repeated START重复起始条件已发出
#define  SMB_MTADDACK   0x18        // (MT) Slave address + W transmitted;从地址+W已发出，且收到ACK
                                    //  ACK received
#define  SMB_MTADDNACK  0x20        // (MT) Slave address + W transmitted;从地址+W已发出，且收到NACK
                                    //  NACK received
#define  SMB_MTDBACK    0x28        // (MT) data byte transmitted; ACK rec'vd 数据字节已发出，收到ACK
#define  SMB_MTDBNACK   0x30        // (MT) data byte transmitted; NACK rec'vd数据字节已发出，收到NACK
#define  SMB_MTARBLOST  0x38        // (MT) arbitration lost 竞争失败，保存当前数据
#define  SMB_MRADDACK   0x40        // (MR) Slave address + R transmitted;从地址+R已发出，且收到ACK
                                    //  ACK received
#define  SMB_MRADDNACK  0x48        // (MR) Slave address + R transmitted;从地址+R已发出，且收到ACK
                                    //  NACK received
#define  SMB_MRDBACK    0x50        // (MR) data byte rec'vd; ACK transmitted数据字节收到，ACK已发出
#define  SMB_MRDBNACK   0x58        // (MR) data byte rec'vd; NACK transmitted数据字节收到，ACK已发出
unsigned char COMMAND;                       // Holds the slave address + R/W bit for
unsigned char WORDR;                          // Holds data to be transmitted by the SMBus,OR data that has just been received.
unsigned char BYTE_NUMBER;          // Used by ISR to check what data has just been, sent - High address byte, Low byte, or data byte
unsigned char RegAdd;
unsigned char HIGH_DAT,LOW_DAT;    // High & Low byte for EEPROM memory address

//----------------------------------------------------------------------------- 
sfr16 TMR3RL   = 0x92;                 // Timer3 reload value
sfr16 TMR3     = 0x94;                 // Timer3 counter
sfr16 ADC0     = 0xbe;                 // ADC0 data
sfr16 ADC0GT   = 0xc4;                 // ADC0 greater than window
sfr16 ADC0LT   = 0xc6;                 // ADC0 less than window 
//-----------------------------------------------------------------------------
bit SM_BUSY;
sbit Start = P3^3;
sbit Led = P3^2;
sbit Buzz = P3^1;

sbit Mos = P1^6;
sbit Led_red = P1^7;
sbit Led_yellow = P3^4;
sbit Led_green = P3^5;

 


unsigned char Flag_Short;             //短路标志位
unsigned char Junc_Short,Num_Short;  
unsigned char Channel_Select;   //AD通道
float data adtemp,aaa,xtemp=0;                       //ad临时采样值
unsigned int i=0,u=0; 
unsigned int xdata flag_a0_pass,flag_a1_pass,flag_b0_pass,flag_b1_pass,flag_c0_pass,flag_c1_pass,flag_d0_pass,flag_d1_pass,flag_e0_pass,flag_e1_pass,flag_f0_pass,flag_f1_pass,flag_g0_pass,flag_g1_pass;
unsigned char xdata flag_a0_1,flag_a0_2,flag_a0_3,flag_a0_4,flag_a0_5,flag_a0_6,flag_a0_7,flag_a0_8;
unsigned char xdata flag_a1_1,flag_a1_2,flag_a1_3,flag_a1_4,flag_a1_5,flag_a1_6,flag_a1_7,flag_a1_8;
unsigned char xdata flag_b0_1,flag_b0_2,flag_b0_3,flag_b0_4,flag_b0_5,flag_b0_6,flag_b0_7,flag_b0_8;
unsigned char xdata flag_b1_1,flag_b1_2,flag_b1_3,flag_b1_4,flag_b1_5,flag_b1_6,flag_b1_7,flag_b1_8;
unsigned char xdata flag_c0_1,flag_c0_2,flag_c0_3,flag_c0_4,flag_c0_5,flag_c0_6,flag_c0_7,flag_c0_8;
unsigned char xdata flag_c1_1,flag_c1_2,flag_c1_3,flag_c1_4,flag_c1_5,flag_c1_6,flag_c1_7,flag_c1_8;
unsigned char xdata flag_d0_1,flag_d0_2,flag_d0_3,flag_d0_4,flag_d0_5,flag_d0_6,flag_d0_7,flag_d0_8;
unsigned char xdata flag_d1_1,flag_d1_2,flag_d1_3,flag_d1_4,flag_d1_5,flag_d1_6,flag_d1_7,flag_d1_8;
unsigned char xdata flag_e0_1,flag_e0_2,flag_e0_3,flag_e0_4,flag_e0_5,flag_e0_6,flag_e0_7,flag_e0_8;
unsigned char xdata flag_e1_1,flag_e1_2,flag_e1_3,flag_e1_4,flag_e1_5,flag_e1_6,flag_e1_7,flag_e1_8;
unsigned char xdata flag_f0_1,flag_f0_2,flag_f0_3,flag_f0_4,flag_f0_5,flag_f0_6,flag_f0_7,flag_f0_8;
unsigned char xdata flag_f1_1,flag_f1_2,flag_f1_3,flag_f1_4,flag_f1_5,flag_f1_6,flag_f1_7,flag_f1_8;
unsigned char xdata flag_g0_1,flag_g0_2,flag_g0_3,flag_g0_4;
unsigned int xdata out_a,out_b,out_c,out_d,out_e,out_f,out_g;



//-----------------------------------------------------------------------------
void delay1s(unsigned char U);
void delaynms (unsigned int uu);
void delayn_ms (unsigned int uu);
void delay(unsigned char us);
void SYSCLK_Init(void);
void init_port (void);
void init_int(void);
void init_smbus();
//void init_adc();
void rst_smbus();
void SM_Send (unsigned char chip_select, unsigned char byte_address, unsigned int out_byte);
char SM_Receive (unsigned char chip_select, unsigned char byte_address);
void SMBus_ISR (void);
void Buzz100ms(unsigned char u);   //蜂鸣器短
void Buzz1s(unsigned char uuu);    //蜂鸣器长
void ADC (unsigned char Channel);
void Clear();
void dac0_12();
void Dac0Rst();
void DacInit();
void RefInit();
void tst();
void init_smbus_read();
void output_final();
//void Timer3_Init ();


//-----------------------------------------------------------------------------
main()  
{   
	float uu=1;
    
  WDTCN = 0xde;
  WDTCN = 0xad;
  SYSCLK_Init();              //初始化时钟
  init_int();
  //Timer3_Init (); 
  init_port (); 
  init_smbus();
  RefInit();
  DacInit();



  //init_adc();
  delaynms(10); 
 
  
   
 
   
  
while(1)
  {  
  	if(!(Start))
  	{
  	 Clear();
  	 Led_yellow = 0;
  	 dac0_12();
  	 init_smbus_read();
  	 delaynms(300);
  	 
  	 
  	 for(i=0;i<500;i++)
  	 {
  	 	Mos = 0;
  	 	delaynms(73);
  	 	tst();
  	 	Mos = 1;
  	 	delaynms(73);
  	 	}
  	 	
  	 Led_yellow = 1;
  delaynms(300);
  	 output_final();
  	 if((flag_a0_pass==0)&&(flag_a1_pass==0)&&(flag_b0_pass==0)&&(flag_b1_pass==0)&&(flag_c0_pass==0)&&(flag_c1_pass==0)&&(flag_d0_pass==0)&&(flag_d1_pass==0)&&(flag_e0_pass==0)&&(flag_e1_pass==0)&&(flag_f0_pass==0)&&(flag_f1_pass==0)&&(flag_g0_pass==0))
  	  {
  	  	Led_green = 0;
  	  	Buzz100ms(3);
  	   
  	    }
  	   else
  	   	{
  	   		Led_red = 0;
  	   		Buzz1s(2);
  	   		}
  	
  	 //delaynms(300);
 
}		
} 
}

//-----------------------------------------------------------------------------
void delay(unsigned char us)
{
	while (us)
	{  _nop_();
		  --us;
	}
}
//-----------------------------------------------------------------------------
void SYSCLK_Init(void)
{  int i;                              // delay counter
	 OSCXCN = 0x67;                      // start external oscillator with
                                       // 4MHz crystal
   for (i=0; i < 256; i++) ;           // Wait for osc. to start up
   while (!(OSCXCN & 0x80)) ;  
   OSCICN = 0x88;                      // select external oscillator as SYSCLK
                                       // source and enable missing clock
                                       // detector
                                       //	OSCICN = 0x07;   //interal 16MHZ
                            
}
//------------------------------------------------------------------------------
void init_port (void) 
{   XBR0  = 0x01;     //使能SMBUS
	  XBR2  = 0x40;
	 
	  PRT0CF = 0Xff;  
	  PRT1CF = 0Xff;
	  PRT2CF = 0Xff;
	  PRT3CF = 0X00; 
	}
//------------------------------------------------------------------------------------------
void init_smbus()    //SDA=P0.0  SCL=P0.1
{
    SMB0CN    = 0x40;	                      //允许ＳＭｂｕｓ
    SMB0CR    = 0x99;						  //系统时钟为100KHZ
}

//------------------------------------------------------------------------------------------
void rst_smbus()
{
    SMB0CN    = 0x00;	                      //禁止ＳＭｂｕｓ 
    SMB0CN    = 0x40;	                      //允许ＳＭｂｕｓ
    SMB0CR    = 0x99;						  //系统时钟为100KHZ

}

//-----------------------------------------------------------------------------
void init_int(void)
{   IE        = 0x80; 
    EIE1      = 0x02;
    //EIE2 |= 0x01;                       // Timer3 interrupt enable
     EIP2      = 0x02;    //AD优先 
    EA = 1; 
}
//-----------------------------------------------------------------------------
// SMBus random read function------------------------------------------------------
// Reads 1 byte from the specified memory location.
//
// byte_address = memory address of byte to read
// chip_select = device address of EEPROM to be read from
char SM_Receive (unsigned char chip_select, unsigned char byte_address)
{
   while (SM_BUSY);                          // Wait for bus to be free.
   SM_BUSY = 1;                              // Occupy SMBus (set to busy)
    //SFRPAGE   = SMB0_PAGE;
   SMB0CN = 0x44;                            // SMBus enabled, ACK on acknowledge cycle

   BYTE_NUMBER = 2;                          // 2 address bytes
   COMMAND = (chip_select | READ);           // Chip select + READ
   RegAdd = byte_address;
   
   
   STO = 0;
   STA = 1;                                  // Start transfer
   
   while (SM_BUSY);          
   //return WORD;
}
//------------------------------------------------------------------------------------------
void SM_Send (unsigned char chip_select, unsigned char byte_address, unsigned int out_byte)
//out_byte为16位，高8位写P0，低8位些P1
{
    while (SM_BUSY);                          // Wait for SMBus to be free.
    SM_BUSY = 1;                              // Occupy SMBus (set to busy)
    //SFRPAGE   = SMB0_PAGE;
    SMB0CN = 0x44;                            // SMBus enabled,
                                             // ACK on acknowledge cycle 
   RegAdd = byte_address;
   BYTE_NUMBER = 2;                          // 2 address bytes.
   COMMAND = (chip_select | WRITE);  
   HIGH_DAT = ((out_byte >> 8) & 0x00FF);// Upper 8 address bits
   LOW_DAT  = (out_byte & 0x00FF);        // Lower 8 address bits
   
   STO = 0;
   STA = 1;                                  // Start transfer
  
}
//------------------------------------------------------------------------------------
// Interrupt Service Routine
//------------------------------------------------------------------------------------
void SMBUS_ISR (void) interrupt 7
{
   switch (SMB0STA){                   // Status code for the SMBus (SMB0STA register)

      // Master Transmitter/Receiver: START condition transmitted.
      // The R/W bit of the COMMAND word sent after this state will
      // always be a zero (W) because for both read and write,
      // the memory address must be written first.
      case SMB_START:
          SMB0DAT = (COMMAND & 0xFE);   // Load address of the slave to be accessed.
          STA = 0;                      // Manually clear START bit
          break;

      // Master Transmitter/Receiver: Repeated START condition transmitted.
      // This state should only occur during a read, after the memory address has been
      // sent and acknowledged.
      case SMB_RP_START:
         SMB0DAT = COMMAND;            // COMMAND should hold slave address + R.
         STA = 0;
         break;

      // Master Transmitter: Slave address + WRITE transmitted.  ACK received.
      case SMB_MTADDACK:
           SMB0DAT = RegAdd;    
           //SMB0DAT = HIGH_ADD;           // Load high byte of memory address
           break;                                // to be written.
          
      // Master Transmitter: Slave address + WRITE transmitted.  NACK received.
      // The slave is not responding.  Send a STOP followed by a START to try again.
      case SMB_MTADDNACK:
          STO = 1;
          STA = 1;
          break;

      // Master Transmitter: Data byte transmitted.  ACK received.
      // This state is used in both READ and WRITE operations.  Check BYTE_NUMBER
      // for memory address status - if only HIGH_ADD has been sent, load LOW_ADD.
      // If LOW_ADD has been sent, check COMMAND for R/W value to determine 
      // next state.
      case SMB_MTDBACK:
         if((COMMAND & 0x01)==1)
            {                     // If R/W=READ, sent repeated START.
             //STO = 0;
             STA = 1;
             } 
         	 else
         	   {
         	    switch (BYTE_NUMBER)
                 {
         	        case 2:                    // If BYTE_NUMBER=2, only HIGH_ADD
                     SMB0DAT = HIGH_DAT;      // has been sent.
                     BYTE_NUMBER--;          // Decrement for next time around.
                     STA = 0;
                     break;
                  case 1:                    // If BYTE_NUMBER=1, LOW_ADD was just sent.
                     SMB0DAT = LOW_DAT;      // If R/W=WRITE, load byte to write.
                     BYTE_NUMBER--;
                     STA = 0;
                     break;
                  default:                   // If BYTE_NUMBER=0, transfer is finished.
                     STO = 1;
                     STA = 0;
                     SM_BUSY = 0;            // Free SMBus
                  }
         	    }
//         {
//            case 2:                    // If BYTE_NUMBER=2, only HIGH_ADD
//            	 if((RegAdd==0)||(RegAdd==2)||(RegAdd==4)||(RegAdd==6))
//            	 	{
//            	 	//WORDT = WORDT>>8;
//            	 	SMB0DAT = (char)(WORD>>8);      // has been sent.
//            	 	}else
////            	 if((RegAdd==1)||(RegAdd==3)||(RegAdd==5)||(RegAdd==7))
////            	 	{WORDT = WORD&0xFF;}
////            	 WORD = (char)WORDT;
//               SMB0DAT = (char)WORD;      // has been sent.
//               BYTE_NUMBER--;          // Decrement for next time around.
//               break;
//            case 1:                    // If BYTE_NUMBER=1, LOW_ADD was just sent.
//               if (COMMAND & 0x01){    // If R/W=READ, sent repeated START.
//                 //SFRPAGE   = SMB0_PAGE;
//    			        STO = 0;
//                  STA = 1;
//               } 
//               else { 
//	            if((RegAdd==0)||(RegAdd==2)||(RegAdd==4)||(RegAdd==6))
////            	 	{WORDT = WORD&0xFF;}
//            	 if((RegAdd==0)||(RegAdd==2)||(RegAdd==4)||(RegAdd==6))
//            	 	{
//            	 	//WORDT = WORDT>>8;
//            	 	 SMB0DAT = (char)(WORD);      // If R/W=WRITE, load byte to write.
//            	 	}else
////            	 	WORDT = (char)WORDT; 
//	              SMB0DAT = (char)(WORD>>8);      // If R/W=WRITE, load byte to write.
//                  BYTE_NUMBER--;
//               }
//         
//               break; 
//            case 0:                   // If BYTE_NUMBER=0, transfer is finished.
//             STO = 1;
//               SM_BUSY = 0;            // Free SMBus
//            }
         break;

      // Master Transmitter: Data byte transmitted.  NACK received.
      // Slave not responding.  Send STOP followed by START to try again.
      case SMB_MTDBNACK:
          STO = 1;
          STA = 1;
          break;

      // Master Transmitter: Arbitration lost.
      // Should not occur.  If so, restart transfer.
      case SMB_MTARBLOST:
        STO = 1;
        STA = 1;
        break;

      // Master Receiver: Slave address + READ transmitted.  ACK received.
      // Set to transmit NACK after next transfer since it will be the last (only)
      // byte.
      case SMB_MRADDACK:
        AA = 0;                       // NACK sent on acknowledge cycle.
        break;
 
      // Master Receiver: Slave address + READ transmitted.  NACK received.
      // Slave not responding.  Send repeated start to try again.
      case SMB_MRADDNACK:
         //STO = 0;
         STA = 1;
         break;

      // Data byte received.  ACK transmitted.
      // State should not occur because AA is set to zero in previous state.
      // Send STOP if state does occur.
      case SMB_MRDBACK:
         WORDR = SMB0DAT;
		     STO = 1;
         SM_BUSY = 0;
         break;

      // Data byte received.  NACK transmitted.
      // Read operation has completed.  Read data register and send STOP.
      case SMB_MRDBNACK:
      	  WORDR = SMB0DAT;
      	  STO = 1;
          SM_BUSY = 0;                  // Free SMBus
         break;

      // All other status codes meaningless in this application. Reset communication.
//      default:
//       //SFRPAGE   = SMB0_PAGE;
//	     STO = 1;                      // Reset communication.
//       SM_BUSY = 0;
//       break;
      }

   SI=0;                               // clear interrupt flag
}
//------------------------------------------------------------------------------
void delay1s(unsigned char U)
{ while (U)
	{delaynms(200);
		delaynms(200);
		delaynms(200);
		delaynms(200);
        delaynms(200);
		U--;} }
//------------------------------------------------------------------------------
void delaynms (unsigned int uu)
{  unsigned int oo,ll;
   for (oo=0;oo<uu;oo++)
   {for(ll=0;ll<1140;ll++);}}
//------------------------------------------------------------------------------
void delayn_ms (unsigned int uu)
{  unsigned int oo,ll;
   for (oo=0;oo<uu;oo++) 
   {for(ll=0;ll<10;ll++);}}
//------------------------------------------------------------------------------
//buzzer 100ms per time
//------------------------------------------------------------------------------
void Buzz100ms(unsigned char u)
{ unsigned char i;
	for(i=0;i<u;i++)
	{	Buzz = 0;
		delaynms(100);
		Buzz = 1;
		delaynms(100);}}
//------------------------------------------------------------------------------
//buzzer 1s per time
//------------------------------------------------------------------------------
void Buzz1s(unsigned char uuu)
{
	unsigned char i;
	for(i=0;i<uuu;i++)
	{	
		Buzz = 0;
		delay1s(1);
		Buzz = 1;
		delay1s(1);
    	}
}
//------------------------------------------------------------------------------
////-----------------------------------------------------------------------------
void dac0_12()
{   //DAC0L = 0x00;
    //DAC0H = 0x00;
    DAC0L = 0x55;
    DAC0H = 0x02; 
  }
//-----------------------------------------------------------------------------
void Dac0Rst()
{
    DAC0L = 0x00;                   // DAC0 Low Byte Register    0xa00=1.5v
    DAC0H = 0x00;                   // DAC0 High Byte Register   0xd55=2.0v 
}

//-----------------------------------------------------------------------------
// DAC Configuration
//-----------------------------------------------------------------------------
void DacInit()
{   DAC0CN = 0x80;                    // DAC0 Control Register
    DAC1CN = 0x80;                    // DAC1 Control Register
     DAC0L = 0x00;                    // DAC0 Low Byte Register    0xa00=1.5v
     DAC0H = 0x00;                    // DAC0 High Byte Register   0xd55=2.0v
     //DA1硬件放大倍数=21
     DAC1L = 0x00;                    // DAC1 Low Byte Register
     DAC1H = 0x00;                    // DAC1 High Byte Register
}
//-----------------------------------------------------------------------------
void RefInit()
{REF0CN = 0x03;                     // Reference Control Register
}
//-----------------------------------------------------------------------------
void init_smbus_read()
{
	SM_Send(CHIP_A,PCA9555A_Configport0,0x0000);   //A输出 
  	delaynms(10);
	  SM_Send(CHIP_B,PCA9555A_Configport0,0x0000);   //A输出 
  	delaynms(10);
  	SM_Send(CHIP_C,PCA9555A_Configport0,0x0000);   //A输出 
  	delaynms(10);
  	SM_Send(CHIP_D,PCA9555A_Configport0,0x0000);   //A输出 
  	delaynms(10);
  	SM_Send(CHIP_E,PCA9555A_Configport0,0x0000);   //A输出 
  	delaynms(10);
  	SM_Send(CHIP_F,PCA9555A_Configport0,0x0000);   //A输出 
  	delaynms(10);
  	SM_Send(CHIP_G,PCA9555A_Configport0,0x0000);   //A输出 
  	delaynms(10);
  	SM_Send(CHIP_H,PCA9555A_Configport0,0x0000);   //A输出 
  	delaynms(10);
	  
	  SM_Send(CHIP_A,PCA9555A_OutPutPort0,0x0000);   //写1恢复A片全1
    delaynms(5);
    SM_Send(CHIP_B,PCA9555A_OutPutPort0,0x0000);   //写1恢复A片全1
    delaynms(5);
    SM_Send(CHIP_C,PCA9555A_OutPutPort0,0x0000);   //写1恢复A片全1
    delaynms(5);
  	SM_Send(CHIP_D,PCA9555A_OutPutPort0,0x0000);   //写1恢复A片全1
    delaynms(5);
    SM_Send(CHIP_E,PCA9555A_OutPutPort0,0x0000);   //写1恢复A片全1
    delaynms(5);
    SM_Send(CHIP_F,PCA9555A_OutPutPort0,0x0000);   //写1恢复A片全1
    delaynms(5);
    SM_Send(CHIP_G,PCA9555A_OutPutPort0,0x0000);   //写1恢复A片全1
    delaynms(5);
    SM_Send(CHIP_H,PCA9555A_OutPutPort0,0x0000);   //写1恢复A片全1
    delaynms(5);
    
    SM_Send(CHIP_A,PCA9555A_OutPutPort0,0xffff);   //写1恢复A片全1
    delaynms(5);
    SM_Send(CHIP_B,PCA9555A_OutPutPort0,0xffff);   //写1恢复A片全1
    delaynms(5);
    SM_Send(CHIP_C,PCA9555A_OutPutPort0,0xffff);   //写1恢复A片全1
    delaynms(5);
  	SM_Send(CHIP_D,PCA9555A_OutPutPort0,0xffff);   //写1恢复A片全1
    delaynms(5);
    SM_Send(CHIP_E,PCA9555A_OutPutPort0,0xffff);   //写1恢复A片全1
    delaynms(5);
    SM_Send(CHIP_F,PCA9555A_OutPutPort0,0xffff);   //写1恢复A片全1
    delaynms(5);
    SM_Send(CHIP_G,PCA9555A_OutPutPort0,0xffff);   //写1恢复A片全1
    delaynms(5);
    SM_Send(CHIP_H,PCA9555A_OutPutPort0,0xffff);   //写1恢复A片全1
    delaynms(5);
	
	  SM_Send(CHIP_A,PCA9555A_Configport0,0xffff);   //A输入 
  	delaynms(10);
  	SM_Send(CHIP_B,PCA9555A_Configport0,0xffff);   //A输入 
  	delaynms(10);
  	SM_Send(CHIP_C,PCA9555A_Configport0,0xffff);   //A输入  
  	delaynms(10);	
  	SM_Send(CHIP_D,PCA9555A_Configport0,0xffff);   //A输入  
  	delaynms(10);
  	SM_Send(CHIP_E,PCA9555A_Configport0,0xffff);   //A输入  
  	delaynms(10);
  	SM_Send(CHIP_F,PCA9555A_Configport0,0xffff);   //A输入  
  	delaynms(10);
  	SM_Send(CHIP_G,PCA9555A_Configport0,0xffff);   //A输入  
  	delaynms(10);
  	SM_Send(CHIP_H,PCA9555A_Configport0,0xffff);   //A输入  
  	delaynms(10);
	}
//-----------------------------------------------------------------------------
void tst()
{
	   SM_Receive (CHIP_A, PCA9555A_InPutPort0);
     WORDR = WORDR;
     if(!(WORDR==0x00))
     	{
     		flag_a0_pass++;
     	}
     if(WORDR&0X01)
     	{
     		flag_a0_1 = 1;
     		out_a = out_a|0x0100;
     	  }
     	
     if(WORDR&0X02)
     	{
     		flag_a0_2 = 1;
     		out_a = out_a|0x0200;
     	  } 
     	  
     if(WORDR&0X04)
     	{
     		flag_a0_3 = 1;
     		out_a = out_a|0x0400;
     	  }
     	  
     if(WORDR&0X08)
     	{
     		flag_a0_4 = 1;
     		out_a = out_a|0x0800;
     	  }
     	  
     if(WORDR&0X10)
     	{
     		flag_a0_5 = 1;
     		out_a = out_a|0x1000;
     	  }
     	  
     if(WORDR&0X20)
     	 {
     		flag_a0_6 = 1;
     		out_a = out_a|0x2000;
     	  }
     	  
     if(WORDR&0X40)
     	{
     		flag_a0_7 = 1;
     		out_a = out_a|0x4000;
     	  }
     	  
     if(WORDR&0X80)
     	 {
     		flag_a0_8 = 1;
     		out_a = out_a|0x8000;
     	  }
     
          
     SM_Receive (CHIP_A, PCA9555A_InPutPort1);
     WORDR = WORDR;
     if(!(WORDR==0x00))
     	{
     		flag_a1_pass++;
     	}
     if(WORDR&0X01)
     	{
     		flag_a1_1 = 1;
     		out_a = out_a|0x0001;
     	  }
     	
     if(WORDR&0X02)
     	{
     		flag_a1_2 = 1;
     		out_a = out_a|0x0002;
     	  }
     	  
     if(WORDR&0X04)
     	{
     		flag_a1_3 = 1;
     		out_a = out_a|0x0004;
     	  }
     	  
     if(WORDR&0X08)
     	{
     		flag_a1_4 = 1;
     		out_a = out_a|0x0008;
     	  }
     	  
     if(WORDR&0X10)
     	{
     		flag_a1_5 = 1;
     		out_a = out_a|0x0010;
     	  }
     	  
     if(WORDR&0X20)
     	 {
     		flag_a1_6 = 1;
     		out_a = out_a|0x0020;
     	  }
     	  
     if(WORDR&0X40)
     	{
     		flag_a1_7 = 1;
     		out_a = out_a|0x0040;
     	  }
     	  
     if(WORDR&0X80)
     	 {
     		flag_a1_8 = 1;
     		out_a = out_a|0x0080;
     	  }
     
     
     SM_Receive (CHIP_B, PCA9555A_InPutPort0);
     WORDR = WORDR;
     if(!(WORDR==0x00))
     	{
     		flag_b0_pass++;
     	}
     if(WORDR&0X01)
     	{
     		flag_b0_1 = 1;
     		out_b = out_b|0x0100;
     	  }
     	
     if(WORDR&0X02)
     	{
     		flag_b0_2 = 1;
     		out_b = out_b|0x0200;
     	  }
     	  
     if(WORDR&0X04)
     	{
     		flag_b0_3 = 1;
     		out_b = out_b|0x0400;
     	  }
     	  
     if(WORDR&0X08)
     	{
     		flag_b0_4 = 1;
     		out_b = out_b|0x0800;
     	  }
     	  
     if(WORDR&0X10)
     	{
     		flag_b0_5 = 1;
     		out_b = out_b|0x1000;
     	  }
     	  
     if(WORDR&0X20)
     	 {
     		flag_b0_6 = 1;
     		out_b = out_b|0x2000;
     	  }
     	  
     if(WORDR&0X40)
     	{
     		flag_b0_7 = 1;
     		out_b = out_b|0x4000;
     	  }
     	  
     if(WORDR&0X80)
     	 {
     		flag_b0_8 = 1;
     		out_b = out_b|0x8000;
     	  }
     
          
     SM_Receive (CHIP_B, PCA9555A_InPutPort1);
     WORDR = WORDR;
     if(!(WORDR==0x00))
     	{
     		flag_b1_pass++;
     	}
     if(WORDR&0X01)
     	{
     		flag_b1_1 = 1;
     		out_b = out_b|0x0001;
     	  }
     	
     if(WORDR&0X02)
     	{
     		flag_b1_2 = 1;
     		out_b = out_b|0x0002;
     	  }
     	  
     if(WORDR&0X04)
     	{
     		flag_b1_3 = 1;
     		out_b = out_b|0x0004;
     	  }
     	  
     if(WORDR&0X08)
     	{
     		flag_b1_4 = 1;
     		out_b = out_b|0x0008;
     	  }
     	  
     if(WORDR&0X10)
     	{
     		flag_b1_5 = 1;
     		out_b = out_b|0x0010;
     	  }
     	  
     if(WORDR&0X20)
     	 {
     		flag_b1_6 = 1;
     		out_b = out_b|0x0020;
     	  }
     	  
     if(WORDR&0X40)
     	{
     		flag_b1_7 = 1;
     		out_b = out_b|0x0040;
     	  }
     	  
     if(WORDR&0X80)
     	 {
     		flag_b1_8 = 1;
     		out_b = out_b|0x0080;
     	  }
  	
     SM_Receive (CHIP_C, PCA9555A_InPutPort0);
     WORDR = WORDR;
     if(!(WORDR==0x00))
     	{
     		flag_c0_pass++;
     	}
     if(WORDR&0X01)
     	{
     		flag_c0_1 = 1;
     		out_c = out_c|0x0100;
     	  }
     	
     if(WORDR&0X02)
     	{
     		flag_c0_2 = 1;
     		out_c = out_c|0x0200;
     	  }
     	  
     if(WORDR&0X04)
     	{
     		flag_c0_3 = 1;
     		out_c = out_c|0x0400;
     	  }
     	  
     if(WORDR&0X08)
     	{
     		flag_c0_4 = 1;
     		out_c = out_c|0x0800;
     	  }
     	  
     if(WORDR&0X10)
     	{
     		flag_c0_5 = 1;
     		out_c = out_c|0x1000;
     	  }
     	  
     if(WORDR&0X20)
     	 {
     		flag_c0_6 = 1;
     		out_c = out_c|0x2000;
     	  }
     	  
     if(WORDR&0X40)
     	{
     		flag_c0_7 = 1;
     		out_c = out_c|0x4000;
     	  }
     	  
     if(WORDR&0X80)
     	 {
     		flag_c0_8 = 1;
     		out_c = out_c|0x8000;
     	  }
     	
     	
     SM_Receive (CHIP_C, PCA9555A_InPutPort1);
     WORDR = WORDR;
     if(!(WORDR==0x00))
     	{
     		flag_c1_pass++;
     	}
     if(WORDR&0X01)
     	{
     		flag_c1_1 = 1;
     		out_c = out_c|0x0001;
     	  }
     	
     if(WORDR&0X02)
     	{
     		flag_c1_2 = 1;
     		out_c = out_c|0x0002;
     	  }
     	  
     if(WORDR&0X04)
     	{
     		flag_c1_3 = 1;
     		out_c = out_c|0x0004;
     	  }
     	  
     if(WORDR&0X08)
     	{
     		flag_c1_4 = 1;
     		out_c = out_c|0x0008;
     	  }
     	  
     if(WORDR&0X10)
     	{
     		flag_c1_5 = 1;
     		out_c = out_c|0x0010;
     	  }
     	  
     if(WORDR&0X20)
     	 {
     		flag_c1_6 = 1;
     		out_c = out_c|0x0020;
     	  }
     	  
     if(WORDR&0X40)
     	{
     		flag_c1_7 = 1;
     		out_c = out_c|0x0040;
     	  }
     	  
     if(WORDR&0X80)
     	 {
     		flag_c1_8 = 1;
     		out_c = out_c|0x0080;
     	  }
     
     	
     SM_Receive (CHIP_D, PCA9555A_InPutPort0);
     WORDR = WORDR;
     if(!(WORDR==0x00))
     	{
     		flag_d0_pass++;
     	}
     if(WORDR&0X01)
     	{
     		flag_d0_1 = 1;
     		out_d = out_d|0x0100;
     	  }
     	
     if(WORDR&0X02)
     	{
     		flag_d0_2 = 1;
     		out_d = out_d|0x0200;
     	  }
     	  
     if(WORDR&0X04)
     	{
     		flag_d0_3 = 1;
     		out_d = out_d|0x0400;
     	  }
     	  
     if(WORDR&0X08)
     	{
     		flag_d0_4 = 1;
     		out_d = out_d|0x0800;
     	  }
     	  
     if(WORDR&0X10)
     	{
     		flag_d0_5 = 1;
     		out_d = out_d|0x1000;
     	  }
     	  
     if(WORDR&0X20)
     	 {
     		flag_d0_6 = 1;
     		out_d = out_d|0x2000;
     	  }
     	  
     if(WORDR&0X40)
     	{
     		flag_d0_7 = 1;
     		out_d = out_d|0x4000;
     	  }
     	  
     if(WORDR&0X80)
     	 {
     		flag_d0_8 = 1;
     		out_d = out_d|0x8000;
     	  }
     	
     SM_Receive (CHIP_D, PCA9555A_InPutPort1);
     WORDR = WORDR;
     if(!(WORDR==0x00))
     	{
     		flag_d1_pass++;
     	}
     if(WORDR&0X01)
     	{
     		flag_d1_1 = 1;
     		out_d = out_d|0x0001;
     	  }
     	
     if(WORDR&0X02)
     	{
     		flag_d1_2 = 1;
     		out_d = out_d|0x0002;
     	  }
     	  
     if(WORDR&0X04)
     	{
     		flag_d1_3 = 1;
     		out_d = out_d|0x0004;
     	  }
     	  
     if(WORDR&0X08)
     	{
     		flag_d1_4 = 1;
     		out_d = out_d|0x0008;
     	  }
     	  
     if(WORDR&0X10)
     	{
     		flag_d1_5 = 1;
     		out_d = out_d|0x0010;
     	  }
     	  
     if(WORDR&0X20)
     	 {
     		flag_d1_6 = 1;
     		out_d = out_d|0x0020;
     	  }
     	  
     if(WORDR&0X40)
     	{
     		flag_d1_7 = 1;
     		out_d = out_d|0x0040;
     	  }
     	  
     if(WORDR&0X80)
     	 {
     		flag_d1_8 = 1;
     		out_d = out_d|0x0080;
     	  }
     	
     	
     SM_Receive (CHIP_E, PCA9555A_InPutPort0);
     WORDR = WORDR;
     if(!(WORDR==0x00))
     	{
     		flag_e0_pass++;
     	}
     if(WORDR&0X01)
     	{
     		flag_e0_1 = 1;
     		out_e = out_e|0x0100;
     	  }
     	
     if(WORDR&0X02)
     	{
     		flag_e0_2 = 1;
     		out_e = out_e|0x0200;
     	  }
     	  
     if(WORDR&0X04)
     	{
     		flag_e0_3 = 1;
     		out_e = out_e|0x0400;
     	  }
     	  
     if(WORDR&0X08)
     	{
     		flag_e0_4 = 1;
     		out_e = out_e|0x0800;
     	  }
     	  
     if(WORDR&0X10)
     	{
     		flag_e0_5 = 1;
     		out_e = out_e|0x1000;
     	  }
     	  
     if(WORDR&0X20)
     	 {
     		flag_e0_6 = 1;
     		out_e = out_e|0x2000;
     	  }
     	  
     if(WORDR&0X40)
     	{
     		flag_e0_7 = 1;
     		out_e = out_e|0x4000;
     	  }
     	  
     if(WORDR&0X80)
     	 {
     		flag_e0_8 = 1;
     		out_e = out_e|0x8000;
     	  }
     	
     	
     SM_Receive (CHIP_E, PCA9555A_InPutPort1);
     WORDR = WORDR;
     if(!(WORDR==0x00))
     	{
     		flag_e1_pass++;
     	}
     if(WORDR&0X01)
     	{
     		flag_e1_1 = 1;
     		out_e = out_e|0x0001;
     	  }
     	
     if(WORDR&0X02)
     	{
     		flag_e1_2 = 1;
     		out_e = out_e|0x0002;
     	  }
     	  
     if(WORDR&0X04)
     	{
     		flag_e1_3 = 1;
     		out_e = out_e|0x0004;
     	  }
     	  
     if(WORDR&0X08)
     	{
     		flag_e1_4 = 1;
     		out_e = out_e|0x0008;
     	  }
     	  
     if(WORDR&0X10)
     	{
     		flag_e1_5 = 1;
     		out_e = out_e|0x0010;
     	  }
     	  
     if(WORDR&0X20)
     	 {
     		flag_e1_6 = 1;
     		out_e = out_e|0x0020;
     	  }
     	  
     if(WORDR&0X40)
     	{
     		flag_e1_7 = 1;
     		out_e = out_e|0x0040;
     	  }
     	  
     if(WORDR&0X80)
     	 {
     		flag_e1_8 = 1;
     		out_e = out_e|0x0080;
     	  }
     	
     	
     SM_Receive (CHIP_F, PCA9555A_InPutPort0);
     WORDR = WORDR;
     if(!(WORDR==0x00))
     	{
     		flag_f0_pass++;
     	}
     if(WORDR&0X01)
     	{
     		flag_f0_1 = 1;
     		out_f = out_f|0x0100;
     	  }
     	
     if(WORDR&0X02)
     	{
     		flag_f0_2 = 1;
     		out_f = out_f|0x0200;
     	  }
     	  
     if(WORDR&0X04)
     	{
     		flag_f0_3 = 1;
     		out_f = out_f|0x0400;
     	  }
     	  
     if(WORDR&0X08)
     	{
     		flag_f0_4 = 1;
     		out_f = out_f|0x0800;
     	  }
     	  
     if(WORDR&0X10)
     	{
     		flag_f0_5 = 1;
     		out_f = out_f|0x1000;
     	  }
     	  
     if(WORDR&0X20)
     	 {
     		flag_f0_6 = 1;
     		out_f = out_f|0x2000;
     	  }
     	  
     if(WORDR&0X40)
     	{
     		flag_f0_7 = 1;
     		out_f = out_f|0x4000;
     	  }
     	  
     if(WORDR&0X80)
     	 {
     		flag_f0_8 = 1;
     		out_f = out_f|0x8000;
     	  }
     
     SM_Receive (CHIP_F, PCA9555A_InPutPort1);
     WORDR = WORDR;
     if(!(WORDR==0x00))
     	{
     		flag_f1_pass++;
     	}
     if(WORDR&0X01)
     	{
     		flag_f1_1 = 1;
     		out_f = out_f|0x0001;
     	  }
     	
     if(WORDR&0X02)
     	{
     		flag_f1_2 = 1;
     		out_f = out_f|0x0002;
     	  }
     	  
     if(WORDR&0X04)
     	{
     		flag_f1_3 = 1;
     		out_f = out_f|0x0004;
     	  }
     	  
     if(WORDR&0X08)
     	{
     		flag_f1_4 = 1;
     		out_f = out_f|0x0008;
     	  }
     	  
     if(WORDR&0X10)
     	{
     		flag_f1_5 = 1;
     		out_f = out_f|0x0010;
     	  }
     	  
     if(WORDR&0X20)
     	 {
     		flag_f1_6 = 1;
     		out_f = out_f|0x0020;
     	  }
     	  
     if(WORDR&0X40)
     	{
     		flag_f1_7 = 1;
     		out_f = out_f|0x0040;
     	  }
     	  
     if(WORDR&0X80)
     	 {
     		flag_f1_8 = 1;
     		out_f = out_f|0x0080;
     	  }
     	
     	
     SM_Receive (CHIP_G, PCA9555A_InPutPort0);
     WORDR = WORDR;
     if(!(WORDR==0xf0))
     	{
     		flag_g0_pass++;
     	}
     if(WORDR&0X01)
     	{
     		flag_g0_1 = 1;
     		out_g = out_g|0x0100;
     	  }
     	
     if(WORDR&0X02)
     	{
     		flag_g0_2 = 1;
     		out_g = out_g|0x0200;
     		}
     	  
     if(WORDR&0X04)
     	{
     		flag_g0_3 = 1;
     		out_g = out_g|0x0400;
     	  }
     	  
     if(WORDR&0X08)
     	{
     		flag_g0_4 = 1;
     		out_g = out_g|0x0800;
     	  }
     	  
//     if(WORDR&&0X10)
//     	{
//     		flag_g0_5 = 1;
//     	  }
//     	  
//     if(WORDR&&0X20)
//     	 {
//     		flag_g0_6 = 1;
//     	  }
//     	  
//     if(WORDR&&0X40)
//     	{
//     		flag_g0_7 = 1;
//     	  }
//     	  
//     if(WORDR&&0X80)
//     	 {
//     		flag_g0_8 = 1;
//     	  }
     
     
//     SM_Receive (CHIP_G, PCA9555A_InPutPort1);
//     WORDR = WORDR;
//     if(WORDR==0x00)
//     	{
//     		flag_g1_pass = 1;
//     	}
//     if(WORDR&&0X01)
//     	{
//     		flag_g1_1 = 1;
//     	  }
//     	
//     if(WORDR&&0X02)
//     	{
//     		flag_g1_2 = 1;
//     	  }
//     	  
//     if(WORDR&&0X04)
//     	{
//     		flag_g1_3 = 1;
//     	  }
//     	  
//     if(WORDR&&0X08)
//     	{
//     		flag_g1_4 = 1;
//     	  }
     	  
//     if(WORDR&&0X10)
//     	{
//     		flag_f1_5 = 1;
//     	  }
//     	  
//     if(WORDR&&0X20)
//     	 {
//     		flag_f1_6 = 1;
//     	  }
//     	  
//     if(WORDR&&0X40)
//     	{
//     		flag_f1_7 = 1;
//     	  }
//     	  
//     if(WORDR&&0X80)
//     	 {
//     		flag_f1_8 = 1;
//     	  }
     	
	}
//-----------------------------------------------------------------------------
// ADC 
//-----------------------------------------------------------------------------
//void ADC (unsigned char Channel)
//    {
//    	
//    	float  AD_VAL_TEST,AD_VAL;
//      unsigned char xdata YYY;
//      unsigned int xdata ad_arr[20];//,TempData;
//		Channel_Select = Channel; 
//		  AMX0SL = Channel_Select;	// AMUX Channel Select Register
//	   //delaynms(5);
//    AD_VAL=0;
//    AD_VAL_TEST = 0;
//    for( YYY = 0;YYY<20;YYY++) 
//    {
//    ad_arr[YYY]=0;
//    } 
//    YYY=0; 
//    ADCINT = 0; 
//    
//    for(YYY= 0;YYY<20;YYY++)     //AD_TIMES=50  采50次
//    {  
//    ADBUSY = 1;   
// 
//    delayn_ms(20);  ////////////---------------------------------------------------采样等待
//    while (!ADCINT); 
// 	  ADCINT = 0;	
//    _nop_();
//    _nop_();
//    _nop_();	           //*AD_POINTER = ADC0
//    ad_arr[YYY]=ADC0;		 
//     }
//    //ADC0 = 0; 
//    for(YYY=0;YYY<20;YYY++) //  for(i = 0;i <AD_TIMES;i++)
//   	{ 
//		AD_VAL_TEST += ad_arr[YYY];
//     }
// 
// 	AD_VAL = (AD_VAL_TEST/20);     
// 	AD_VAL = AD_VAL/4096;
// 	AD_VAL = AD_VAL*2.43; 
// 	adtemp = AD_VAL;
//	 } 

//-----------------------------------------------------------------------------
void Clear()
{ 
	   Dac0Rst();
	   Mos = 1;
	   Led_red = 1;
  	 Led_yellow = 1;
  	 Led_green = 1;
  	 rst_smbus();
  	 
  	 STO = 1;                      // Reset communication.
     SI=0;                               // clear interrupt flag
	   
	   flag_a0_pass=0;flag_a1_pass=0;flag_b0_pass=0;flag_b1_pass=0;flag_c0_pass=0;flag_c1_pass=0;flag_d0_pass=0;flag_d1_pass=0;flag_e0_pass=0;flag_e1_pass=0;flag_f0_pass=0;flag_f1_pass=0;flag_g0_pass=0;
     flag_a0_1=0;flag_a0_2=0;flag_a0_3=0;flag_a0_4=0;flag_a0_5=0;flag_a0_6=0;flag_a0_7=0;flag_a0_8=0;
     flag_a1_1=0;flag_a1_2=0;flag_a1_3=0;flag_a1_4=0;flag_a1_5=0;flag_a1_6=0;flag_a1_7=0;flag_a1_8=0;
     flag_b0_1=0;flag_b0_2=0;flag_b0_3=0;flag_b0_4=0;flag_b0_5=0;flag_b0_6=0;flag_b0_7=0;flag_b0_8=0;
     flag_b1_1=0;flag_b1_2=0;flag_b1_3=0;flag_b1_4=0;flag_b1_5=0;flag_b1_6=0;flag_b1_7=0;flag_b1_8=0;
     flag_c0_1=0;flag_c0_2=0;flag_c0_3=0;flag_c0_4=0;flag_c0_5=0;flag_c0_6=0;flag_c0_7=0;flag_c0_8=0;
     flag_c1_1=0;flag_c1_2=0;flag_c1_3=0;flag_c1_4=0;flag_c1_5=0;flag_c1_6=0;flag_c1_7=0;flag_c1_8=0;
     flag_d0_1=0;flag_d0_2=0;flag_d0_3=0;flag_d0_4=0;flag_d0_5=0;flag_d0_6=0;flag_d0_7=0;flag_d0_8=0;
     flag_d1_1=0;flag_d1_2=0;flag_d1_3=0;flag_d1_4=0;flag_d1_5=0;flag_d1_6=0;flag_d1_7=0;flag_d1_8=0;
     flag_e0_1=0;flag_e0_2=0;flag_e0_3=0;flag_e0_4=0;flag_e0_5=0;flag_e0_6=0;flag_e0_7=0;flag_e0_8=0;
     flag_e1_1=0;flag_e1_2=0;flag_e1_3=0;flag_e1_4=0;flag_e1_5=0;flag_e1_6=0;flag_e1_7=0;flag_e1_8=0;
     flag_f0_1=0;flag_f0_2=0;flag_f0_3=0;flag_f0_4=0;flag_f0_5=0;flag_f0_6=0;flag_f0_7=0;flag_f0_8=0;
     flag_f1_1=0;flag_f1_2=0;flag_f1_3=0;flag_f1_4=0;flag_f1_5=0;flag_f1_6=0;flag_f1_7=0;flag_f1_8=0;
     flag_g0_1=0;flag_g0_2=0;flag_g0_3=0;flag_g0_4=0;
     
     out_a = 0x0000; out_b = 0x0000; out_c = 0x0000; out_d = 0x0000; out_e = 0x0000; out_f = 0x0000; out_g = 0x0000;
	   
	   
	   
  	}
//-----------------------------------------------------------------------------
// void output_final()
//-----------------------------------------------------------------------------
void output_final()
{
	SM_Send(CHIP_A,PCA9555A_Configport0,0x0000);   //A输出 
  	delaynms(10);
	  SM_Send(CHIP_B,PCA9555A_Configport0,0x0000);   //A输出 
  	delaynms(10);
  	SM_Send(CHIP_C,PCA9555A_Configport0,0x0000);   //A输出 
  	delaynms(10);
  	SM_Send(CHIP_D,PCA9555A_Configport0,0x0000);   //A输出 
  	delaynms(10);
  	SM_Send(CHIP_E,PCA9555A_Configport0,0x0000);   //A输出 
  	delaynms(10);
  	SM_Send(CHIP_F,PCA9555A_Configport0,0x0000);   //A输出 
  	delaynms(10);
  	SM_Send(CHIP_G,PCA9555A_Configport0,0x0000);   //A输出 
  	delaynms(10);
  	
	  SM_Send(CHIP_A,PCA9555A_Configport1,0x0000);   //A输出 
  	delaynms(10);
	  SM_Send(CHIP_B,PCA9555A_Configport1,0x0000);   //A输出 
  	delaynms(10);
  	SM_Send(CHIP_C,PCA9555A_Configport1,0x0000);   //A输出 
  	delaynms(10);
  	SM_Send(CHIP_D,PCA9555A_Configport1,0x0000);   //A输出 
  	delaynms(10);
  	SM_Send(CHIP_E,PCA9555A_Configport1,0x0000);   //A输出 
  	delaynms(10);
  	SM_Send(CHIP_F,PCA9555A_Configport1,0x0000);   //A输出 
  	delaynms(10);
  	
  	
	  //输出时，先配置好0口和1口，16位一起写
	  
	  SM_Send(CHIP_A,PCA9555A_OutPutPort0,out_a);   //写1恢复A片全1
    delaynms(5);
    SM_Send(CHIP_B,PCA9555A_OutPutPort0,out_b);   //写1恢复A片全1
    delaynms(5);
    SM_Send(CHIP_C,PCA9555A_OutPutPort0,out_c);   //写1恢复A片全1
    delaynms(5);
  	SM_Send(CHIP_D,PCA9555A_OutPutPort0,out_d);   //写1恢复A片全1
    delaynms(5);
    SM_Send(CHIP_E,PCA9555A_OutPutPort0,out_e);   //写1恢复A片全1
    delaynms(5);
    SM_Send(CHIP_F,PCA9555A_OutPutPort0,out_f);   //写1恢复A片全1
    delaynms(5);
    SM_Send(CHIP_G,PCA9555A_OutPutPort0,out_g);   //写1恢复A片全1
    delaynms(5);
	
	}
//-----------------------------------------------------------------------------
// Timer3_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Timer3 configured for use by the SMBus low timeout detect feature as
// follows:
// - SYSCLK/12 as Timer3 clock source
// - Timer3 reload registers loaded for a 25ms overflow period
// - Timer3 pre-loaded to overflow after 25ms
// - Timer3 enabled
//
//void Timer3_Init (void)
//{
//   TMR3CN = 0x00;                      // Timer3 uses SYSCLK/12
//
//   TMR3RL = -(SYSCLK/12/40);           // Timer3 configured to overflow after
//   TMR3 = TMR3RL;                      // ~25ms (for SMBus low timeout detect)
//
//   EIE2 |= 0x01;                       // Timer3 interrupt enable
//   TMR3CN |= 0x04;                     // Start Timer3
//}

//-----------------------------------------------------------------------------
// Timer3 Interrupt Service Routine (ISR)
//-----------------------------------------------------------------------------
//
// A Timer3 interrupt indicates an SMBus SCL low timeout.
// The SMBus is disabled and re-enabled if a timeout occurs.
//
//void Timer3_ISR (void) interrupt 14
//{
//   SMB0CN &= ~0x40;                    // Disable SMBus
//   SMB0CN |= 0x40;                     // Re-enable SMBus
//   TMR3CN &= ~0x80;                    // Clear Timer3 interrupt-pending flag
//   SM_BUSY = 0;                       // Free bus
//}
//-----------------------------------------------------------------------------
