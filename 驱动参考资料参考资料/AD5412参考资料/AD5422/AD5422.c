/********************************************************************************
 Author : CAC (CustomerApplications Center, Asia) 

 Date : 2012-07-28

 File name : AD5422.c

 Description : AD5422 write and read

 Hardware plateform : ADuC7026_DEMO_V1.2 + EVAL-AD5422EBZ   	
********************************************************************************/

#include "ADuC7026.h"
#include "ADuC7026Driver.h"
#include "AD5422.h"
#include "stdio.h"


//the function write data to the AD5422's register 
void WriteToAD5422(unsigned char count,unsigned char *buf)
{
	unsigned	char	ValueToWrite = 0;
    unsigned	char	i = 0;
	unsigned	char	j = 0;
	

	ADuC7026OutputBit(AD5422_LATCH, 0);

	for ( i=count;i>0;i-- )
 	{
	 	ValueToWrite =	*(buf+i-1);
		for (j=0; j<8; j++)
		{
			ADuC7026OutputBit(AD5422_SCLK, 0);
			if(0x80 == (ValueToWrite & 0x80))
			{
				ADuC7026OutputBit(AD5422_SDIN, 1);	    //Send one to SDIN pin of AD5422
			}
			else
			{
				ADuC7026OutputBit(AD5422_SDIN, 0);	    //Send zero to SDIN pin of AD5422
			}
										 
			Delay(1);
			ADuC7026OutputBit(AD5422_SCLK, 1);
			Delay(1);
			ValueToWrite <<= 1;	                      //Rotate data
		}
	}

	ADuC7026OutputBit(AD5422_SCLK, 0);
	Delay(1);

	ADuC7026OutputBit(AD5422_LATCH, 1);
	Delay(20);
   
}


//the function read the data register from AD5422 
void ReadFromAD5422(unsigned char count,unsigned char *buf)
{
    unsigned	char	i = 0;
	unsigned	char	j = 0;
	unsigned	char  	iTemp = 0;
	unsigned	char  	RotateData = 0;

	ADuC7026OutputBit(AD5422_LATCH, 0);

	for(j=count; j>0; j--)
	{
		for(i=0; i<8; i++)
		{
		    ADuC7026OutputBit(AD5422_SCLK, 0);
			RotateData <<= 1;		                            //Rotate data
			Delay(1);

			ADuC7026OutputBit(AD5422_SDIN, 0);			       //Write a nop condition when read the data. 
			iTemp = ADuC7026InputBit(AD5422_SDO);			   //Read SDO of AD5422

			ADuC7026OutputBit(AD5422_SCLK, 1);	
			if(iTemp == 1)
			{
				RotateData |= 1;	
			}
			Delay(1);
		}
		*(buf+j-1)= RotateData;
	}

	ADuC7026OutputBit(AD5422_SCLK, 0);
	Delay(1);
		 
	ADuC7026OutputBit(AD5422_LATCH, 1);
	Delay(20);
   
}
