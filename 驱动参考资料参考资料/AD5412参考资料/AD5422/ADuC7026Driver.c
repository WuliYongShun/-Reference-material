/********************************************************************************
 Author : CAC (CustomerApplications Center, Asia) 

 Date : 2012-07-28

 File name : ADuC7026Driver.c

 Description : AD5422 write and read

 Hardware plateform : ADuC7026_DEMO_V1.2 + EVAL-AD5422EBZ   	
********************************************************************************/
#include "ADuC7026.h"
#include "ADuC7026Driver.h"
#include "AD5422.h"
#include "stdio.h"

extern unsigned char buf[3];


/*    Rewrite the serial port function  */
/* 	  To use the pfrintf() in ADuC702x in Keil UV4, the sendchar() must be rewrite */
int SendChar (int ch)  {                 
      while(!(0x020==(COMSTA0 & 0x020)))	{}
 
 	COMTX = ch;

	while(!(0x020==(COMSTA0 & 0x020)))	{}

	return ch;
}


struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;
FILE __stderr;

int fputc (int ch, FILE *f) {
  return (SendChar(ch));
}

int fgetc (FILE *fp)  {
  return (0);
}


int fclose (FILE* f) {
  return 0;
}

int ferror(FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}

int fseek (FILE *fp, long nPos, int nMode)  {
  return (0);
}

int fflush (FILE *pStream)  {
  return (0);
}

void _ttywrch (int ch) {
  SendChar(ch);
}

void _sys_exit (int return_code) {
label:  goto label;  /* endless loop */
}
/*    Rewrite the serial port function  */

unsigned char ADuC7026InputBit(unsigned char GPIONum)
{
	unsigned long int Temp;
	unsigned char Data;

	Temp=0xFFFFFFFF-(1<<((GPIONum&0x0F)+24));

	switch(GPIONum>>4)
	{
		case	0:
			GP0DAT&=Temp;
			Data=GP0DAT;		
			break;
		case	1:
			GP1DAT&=Temp;
			Data=GP1DAT;
			break;
		case	2:
			GP2DAT&=Temp;
			Data=GP2DAT;
			break;
		case	3:
			GP3DAT&=Temp;
			Data=GP3DAT;
			break;
		case	4:
			GP4DAT&=Temp;
			Data=GP4DAT;
			break;
	}
	if((Data&(1<<(GPIONum&0x0F)))==(1<<(GPIONum&0x0F)))
	{
		Data=1;
	}
	else
	{
		Data=0;
	}

	return Data;
}

void ADuC7026OutputBit(unsigned char GPIONum, unsigned char Data)
{
	unsigned long int Temp;

	Temp=1<<(GPIONum&0x0F);

	switch(GPIONum>>4)
	{
		case	0:
			GP0DAT|=(Temp<<24);
			if(Data==0)
			{
				GP0CLR=(Temp<<16);
			}
			else
			{
				GP0SET=(Temp<<16);	
			}
			break;
		case	1:
			GP1DAT|=(Temp<<24);
			if(Data==0)
			{
				GP1CLR=(Temp<<16);
			}
			else
			{
				GP1SET=(Temp<<16);	
			}
			break;
		case	2:
			GP2DAT|=(Temp<<24);
			if(Data==0)
			{
				GP2CLR=(Temp<<16);
			}
			else
			{
				GP2SET=(Temp<<16);	
			}
			break;
		case	3:
			GP3DAT|=(Temp<<24);
			if(Data==0)
			{
				GP3CLR=(Temp<<16);
			}
			else
			{
				GP3SET=(Temp<<16);	
			}
			break;
		case	4:
			GP4DAT|=(Temp<<24);
			if(Data==0)
			{
				GP4CLR=(Temp<<16);
			}
			else
			{
				GP4SET=(Temp<<16);	
			}
			break;
	}
}

void ADuC7026SetSystemClock(unsigned char SystemClock)
{
	POWKEY1 = 0x01;
	POWCON = SystemClock;	
	POWKEY2 = 0xF4;
}

void ADuC7026InitializeUart(void)
{
	// Setup tx & rx pins on SPM 0 and SPM 1
	GP1CON |= 0x11;

    // Start setting up UART at 115200 baud rate, assume that system clock is 41.78MHz
	COMCON0 = 0x80;					// Setting DLAB
   	COMDIV0 = 0x0B;					// Setting DIV0 and DIV1 to DL calculated
	COMDIV1 = 0x00;
   	COMCON0 = 0x07;					// Clearing DLAB
	// fractional divider
  	COMDIV2 = 0x883E;			  	// M=1,N=01101010101  =853,M+N/2048	 =1.4165
	
	
	/*
	// Start setting up UART at 921600 baud rate, 
	// Note that, this high speed is available only when there is a PL2302(USB->UART) on board, PL2302 can support this baud rate
	// Still, this high speed is not a guarantee of ADuC7026 hardware, Just for demo, don't use it in final products. 
	COMCON0 = 0x80;					// Setting DLAB
   	COMDIV0 = 0x01;					// Setting DIV0 and DIV1 to DL calculated
	COMDIV1 = 0x00;
   	COMCON0 = 0x07;					// Clearing DLAB
	// fractional divider
  	COMDIV2 = 0x8B55;			  	// M=1,N=853
	*/
		
}

void Delay(int Length)
{
   while(Length>0)
         Length--;
}
/*    Function Pointers for Interrupts  */
// Copied from irq_arm.c in Keil uV4, required 
tyVctHndlr    IRQ     = (tyVctHndlr)0x0;
tyVctHndlr    SWI     = (tyVctHndlr)0x0;
tyVctHndlr    FIQ     = (tyVctHndlr)0x0;
tyVctHndlr    UNDEF   = (tyVctHndlr)0x0;
tyVctHndlr    PABORT  = (tyVctHndlr)0x0;
tyVctHndlr    DABORT  = (tyVctHndlr)0x0;

void	IRQ_Handler   (void) __irq;
void	SWI_Handler   (void) __irq;
void	FIQ_Handler   (void) __irq;
void	Undef_Handler (void) __irq;
void	PAbt_Handler  (void) __irq;
void	DAbt_Handler  (void) __irq;

void	IRQ_Handler(void) __irq
{
//	if ( *IRQ !=0x00)
//	{
//		IRQ();
//	}

//    unsigned short int  SampaleData[2] = {0,0};

/*    if((IRQSTA&GP_TIMER_BIT)!=0)
	{
		GP4DAT ^= 0x00040000;

		T1CLRI = 0;

		buf[2] = 0x55;				//Control Register 
  //	buf[1] = 0x1f;              //Enable Slew Rate and the Slew Rate Time is 20s while selecting the current mode
  //	buf[0] = 0x15;
  //	buf[1] = 0x1d;              //Enable Slew Rate and the Slew Rate Time is 12s while selecting the current mode
  //	buf[0] = 0x15;
  //	buf[1] = 0x1c;              //Enable Slew Rate and the Slew Rate Time is 4.8s while selecting the current mode
  //	buf[0] = 0x35;

	    buf[1] = 0x10;              //Disable Slew Rate	
	    buf[0] = 0x00;				//Selecting the Voltage Mode(0V - 5V)
   //	buf[0] = 0x01;				//Selecting the Voltage Mode(0V - 10V)
  //	buf[0] = 0x02;				//Selecting the Voltage Mode(-5V - 5V)
  //	buf[0] = 0x03;				//Selecting the Voltage Mode(-10V - 10V)
  //	buf[0] = 0x05;				//Selecting the Current Mode(4mA - 20mA)
  //   	buf[0] = 0x06;				//Selecting the Current Mode(0mA - 20mA)
  //	buf[0] = 0x07;				//Selecting the Current Mode(0mA - 24mA)
						 
	   WriteToAD5422(3,buf);		//Write 551000 to SHIFT REGISTER  to write 1005 to control register


	   buf[2] = 0x01;              //DATA REGISTER
	   buf[1] = 0x80;
	   buf[0] = 0x00;
	   WriteToAD5422(3,buf);		//Write 019966H to SHIFT REGISTER  to write 9966H to DATA REGISTER
	   
	   buf[2] = 0x02;
	   buf[1] = 0x00;
	   buf[0] = 0x01;			    //Read data register
	   WriteToAD5422(3,buf);

	   ReadFromAD5422(3,buf);		//Read data REGISTER

	   printf("aa=%02x\n",buf[2]);
	   printf("bb=%02x\n",buf[1]);
	   printf("cc=%02x\n",buf[0]);	

	}	   
	return;*/	 
}

void	FIQ_Handler(void) __irq
{
	if ( *FIQ !=0x00)
	{
		FIQ();
	}
}

void	SWI_Handler(void) __irq
{
	if ( *SWI !=0x00)
	{
		SWI();
	}
}

void	Undef_Handler(void)__irq 
{
	if ( *UNDEF !=0x00)
	{
		UNDEF();
	}
}

void	PAbt_Handler(void) __irq
{
	if ( *PABORT !=0x00)
	{
		PABORT();
	}
}

void	DAbt_Handler(void) __irq
{
	if ( *DABORT !=0x00)
	{
		DABORT();
	}
}
