/********************************************************************************
 Author : CAC (CustomerApplications Center, Asia) 

 Date : 2012-07-28

 File name : ADuC7026Driver.h

 Description : AD5422 write and read

 Hardware plateform : ADuC7026_DEMO_V1.2 + EVAL-AD5422EBZ   	
********************************************************************************/
#ifndef ADUC7026_DRIVER_H
#define ADUC7026_DRIVER_H

#include "stdio.h"

#define SYSTEM_CLOCK_41780K	0x00
#define SYSTEM_CLOCK_20890K	0x01
#define SYSTEM_CLOCK_10440K	0x02
#define SYSTEM_CLOCK_5220K	0x03
#define SYSTEM_CLOCK_2610K	0x04
#define SYSTEM_CLOCK_1310K	0x05
#define SYSTEM_CLOCK_653K	0x06
#define SYSTEM_CLOCK_326K	0x07



/*    Rewrite the serial port function  */
/* 	  To use the pfrintf() in ADuC702x in Keil UV4, the sendchar() must be rewrite */
int SendChar (int Data);

//GPIO Control
//void ADuC7026SwitchBit(unsigned char GPIONum);
unsigned char ADuC7026InputBit(unsigned char GPIONum);
void ADuC7026OutputBit(unsigned char GPIONum, unsigned char Data);

void ADuC7026SetSystemClock(unsigned char SystemClock);
void ADuC7026InitializeUart(void);

void Delay(int Length);

#endif
