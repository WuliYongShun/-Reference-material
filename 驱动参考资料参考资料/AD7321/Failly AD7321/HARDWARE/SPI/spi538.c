/**
  ******************************************************************************
  *               Copyright(C) 2014-2024 GDKY  All Rights Reserved
  *
  * @file    : max538.c
  * @author  : ZouZH
  * @version : V1.02
  * @date    : 05-Dec-2014
  * @brief   : 12 Bit DAC .
  ******************************************************************************
  */
  
/* INCLUDES ------------------------------------------------------------------- */
#include "spi538.h"
/* TYPEDEFS ------------------------------------------------------------------- */





/* CONSTANTS  ----------------------------------------------------------------- */

/* GLOBAL VARIABLES ----------------------------------------------------------- */

/* GLOBAL FUNCTIONS ----------------------------------------------------------- */

/* LOCAL VARIABLES ------------------------------------------------------------ */

/* LOCAL FUNCTIONS ------------------------------------------------------------ */

void MAX538_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 

  RCC_APB2PeriphClockCmd(GPIO_CLK_MOSI | GPIO_CLK_SCLK| GPIO_CLK_NCS1, ENABLE);
  RCC_APB2PeriphClockCmd( GPIO_CLK_NCS2, ENABLE);

  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_InitStructure.GPIO_Pin =  GPIO_PIN_MOSI;
  GPIO_Init(GPIO_PORT_MOSI, &GPIO_InitStructure);
  MOSI_H();

  GPIO_InitStructure.GPIO_Pin =  GPIO_PIN_SCLK;
  GPIO_Init(GPIO_PORT_SCLK, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  GPIO_PIN_NCS1;
  GPIO_Init(GPIO_PORT_NCS1, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  GPIO_PIN_NCS2;
  GPIO_Init(GPIO_PORT_NCS2, &GPIO_InitStructure);
}


/**
 * @brief : Write DA out value to MAX538
 *
 * @param : DAx : DA2(2) or DA4(4)
 * @param : dat : 12 bit DA out value,max value: 0x0FFF
 *
 * @retval: none.
 */
void MAX538_WriteValue(uint8_t DAx, uint16_t daout)
{
  uint8_t i = 0;
  uint8_t j = 0;
  uint16_t mask = 0;
	
//	daout =(float) daout/250;
	
  daout = (float)daout/250*163.8;//(4095/25)

  daout = daout & 0x0FFFF;
    
	if (1 == DAx)
  {
    SCLK_L();
    NCS1_L();
    for (i=16; i>0; i--) 
    {
      mask = 1 << (i - 1);    
      if (daout & mask)
      {
        MOSI_H();
      }    
      else
      {
        MOSI_L();
      } 
      for (j = 20; j > 0; j--);
      SCLK_H(); 
      for (j = 20; j > 0; j--);
      SCLK_L();
    }
    NCS1_H();
  }
  if (2 == DAx)
  {
    SCLK_L();
    NCS2_L();
    for (i=16; i>0; i--) 
    {
      mask = 1 << (i - 1);    
      if (daout & mask)
      {
        MOSI_H();
      }    
      else
      {
        MOSI_L();
      } 
      for (j = 20; j > 0; j--);
      SCLK_H(); 
      for (j = 20; j > 0; j--);
      SCLK_L();
    }
    NCS2_H();
  }

}
