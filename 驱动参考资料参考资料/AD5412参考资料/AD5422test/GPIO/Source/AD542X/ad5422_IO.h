/*----------------------------------------------------------------------
  File Name          : AD5422_IO.h 
  Author             : FZ - CAST
  Version            : V1.0
  Date               : 12/06/2012
  Description        : AD5422 Driver Declaration

  The AD5412/AD5422 is a low-cost, precision, fully integrated
  12/16-bit converter offering a programmable current source and
  programmable voltage output designed to meet the
  requirements of industrial process control applications.


----------------------------------------------------------------------

  The present firmware which is for guidance only aims at providing
  customers with coding information regarding their products in order
  for them to save time.  As a result, Analog Devices shall not be
  held liable for any direct, indirect or consequential damages with
  respect to any claims arising from the content of such firmware and/or
  the use made by customers of the coding information contained herein
  in connection with their products.
 ----------------------------------------------------------------------*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AD5422_H
#define __AD5422_H

  #define AD5412 1
//#define AD5422 1   //不可同时定义

#define IOUT_4_20 1
#define IOUT_0_20 2
#define IOUT_0_24 3

//---------------------------------
//void WriteToAD5422(unsigned char count,unsigned char *buf);
//---------------------------------
//Function that writes to the AD5422 via the SPI port. 
//--------------------------------------------------------------------------------
extern void WriteToAD5422(unsigned char count,unsigned char *buf);
//---------------------------------
//void ReadFromAD5422(unsigned char count,unsigned char *buf);
//---------------------------------
//Function that reads from the AD5422 via the SPI port. 
//--------------------------------------------------------------------------------
extern void ReadFromAD5422(unsigned char count, unsigned char *buf);
extern void ad54x2_delay (int length);
extern uint16_t CurrentToData(unsigned char current_type, unsigned char output_current);
#endif /* __AD5422_H */
