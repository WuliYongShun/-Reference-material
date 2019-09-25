/********************************************************************************
 Author : CAC (CustomerApplications Center, Asia) 

 Date : 2012-07-28

 File name : AD5422Test.c

 Description : AD5422 write and read

 Hardware plateform : ADuC7026_DEMO_V1.2 + EVAL-AD5422EBZ   	
********************************************************************************/

#include "ADuC7026.h"
#include "ADuC7026Driver.h"
#include "AD5422.h"

unsigned	char	buf[3] = {0,0,0};

int main(void)
{	

   	ADuC7026SetSystemClock(SYSTEM_CLOCK_41780K);
	ADuC7026InitializeUart();


/*	T1LD = 0x3300;                //the interrupt time is about 5ms
	T1LD = 0x20000;			      //the interrupt time is about 50ms
	T1CON = 0xC4;

	IRQEN = GP_TIMER_BIT;        //enable the timer interrupt

	*/

	Delay(0xA00000);

	buf[2] = 0x55;				//Control Register 
//	buf[1] = 0x1f;              //Enable Slew Rate and the Slew Rate Time is 20s while selecting the current mode
//	buf[0] = 0x15;
//	buf[1] = 0x1d;              //Enable Slew Rate and the Slew Rate Time is 12s while selecting the current mode
//	buf[0] = 0x15;
//	buf[1] = 0x1c;              //Enable Slew Rate and the Slew Rate Time is 4.8s while selecting the current mode
//	buf[0] = 0x35;
	buf[1] = 0x11;              //Enable Slew Rate and the Slew Rate Time is 10ms while selecting the current mode	
//	buf[0] = 0xB0;				//Selecting the Voltage Mode(0V - 5V)
	buf[0] = 0xB2;				//Selecting the Voltage Mode(-5V - 5V)

//	buf[1] = 0x10;              //Disable Slew Rate	
//	buf[0] = 0x00;				//Selecting the Voltage Mode(0V - 5V)
//	buf[0] = 0x01;				//Selecting the Voltage Mode(0V - 10V)
//	buf[0] = 0x02;				//Selecting the Voltage Mode(-5V - 5V)
//	buf[0] = 0x03;				//Selecting the Voltage Mode(-10V - 10V)
//	buf[0] = 0x05;				//Selecting the Current Mode(4mA - 20mA)
//	buf[0] = 0x06;				//Selecting the Current Mode(0mA - 20mA)
//	buf[0] = 0x07;				//Selecting the Current Mode(0mA - 24mA)
						 
	WriteToAD5422(3,buf);		//Write 0x551000 to SHIFT REGISTER  to write 1005 to control register


	buf[2] = 0x01;              //Data register
	buf[1] = 0x00;
	buf[0] = 0x00;

	WriteToAD5422(3,buf);		//Write 0x010000 to SHIFT REGISTER  to write 0x018000 to DATA REGISTER

	Delay(0xA00000);
	Delay(0xA00000);
	Delay(0xA00000);

	buf[2] = 0x01;              //Data register
	buf[1] = 0xff;
	buf[0] = 0xff;				//Write 0x010001 to SHIFT REGISTER  to write 0x018000 to DATA REGISTER

	WriteToAD5422(3,buf);		//Write 0x018000 to SHIFT REGISTER  to write 0x018000 to DATA REGISTER
	

	buf[2] = 0x02;
	buf[1] = 0x00;
	buf[0] = 0x01;			    //Read data register
	WriteToAD5422(3,buf);

	ReadFromAD5422(3,buf);		//Read data REGISTER

	printf("a=%02x\n",buf[2]);
	printf("b=%02x\n",buf[1]);
	printf("c=%02x\n",buf[0]);	

	while (1)
	{
	};
	return 0;
}
