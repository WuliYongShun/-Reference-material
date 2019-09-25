/********************************************************************************
 Author : CAC (CustomerApplications Center, Asia) 

 Date : 2012-07-28

 File name : AD5422.h

 Description : AD5422 write and read

 Hardware plateform : ADuC7026_DEMO_V1.2 + EVAL-AD5422EBZ   	
********************************************************************************/

#ifndef AD5422_H
#define AD5422_H

#define AD5422_CLEAR        0x37      //P3.7 = CLEAR
#define AD5422_SCLK         0x16      //P1.6 = SCLK
#define AD5422_LATCH        0x02      //P0.2 = LATCH
#define AD5422_SDIN         0x24      //P2.4 = SDIN
#define AD5422_SDO          0x46      //P4.6 = SDO


//AD5422 control

void WriteToAD5422(unsigned char count,unsigned char *buf);

void ReadFromAD5422(unsigned char count,unsigned char *buf);

#endif
