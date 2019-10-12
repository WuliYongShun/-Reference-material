/**
  ******************************************************************************
  *               Copyright(C) 2014-2024 GDKY  All Rights Reserved
  *
  * @file    : max538.h
  * @author  : ZouZH
  * @version : V1.02
  * @date    : 05-Dec-2014
  * @brief   : 12 Bit DAC.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion --------------------------------------*/
#ifndef __SPI538_H
#define __SPI538_H

#ifdef __cplusplus
 extern "C" {
#endif  /* __cplusplus */

/* INCLUDES ------------------------------------------------------------------- */
#include "stm32f10x.h"

	 /* MACROS  -------------------------------------------------------------------- */
#define GPIO_PORT_MOSI  GPIOA
#define GPIO_PORT_SCLK  GPIOA
#define GPIO_PORT_NCS1  GPIOC


#define GPIO_PIN_MOSI   GPIO_Pin_12
#define GPIO_PIN_SCLK	  GPIO_Pin_5
#define GPIO_PIN_NCS1	  GPIO_Pin_12


#define GPIO_CLK_MOSI	  RCC_APB2Periph_GPIOA
#define GPIO_CLK_SCLK	  RCC_APB2Periph_GPIOA
#define GPIO_CLK_NCS1	  RCC_APB2Periph_GPIOC


#define MOSI_L()       GPIO_ResetBits(GPIO_PORT_MOSI, GPIO_PIN_MOSI)
#define MOSI_H()       GPIO_SetBits(GPIO_PORT_MOSI, GPIO_PIN_MOSI)

#define SCLK_L()       GPIO_ResetBits(GPIO_PORT_SCLK, GPIO_PIN_SCLK)
#define SCLK_H()       GPIO_SetBits(GPIO_PORT_SCLK, GPIO_PIN_SCLK)

#define NCS1_L()        GPIO_ResetBits(GPIO_PORT_NCS1, GPIO_PIN_NCS1)
#define NCS1_H()        GPIO_SetBits(GPIO_PORT_NCS1, GPIO_PIN_NCS1)


#define GPIO_PORT_NCS2  GPIOA
#define GPIO_PIN_NCS2	  GPIO_Pin_6

#define GPIO_CLK_NCS2	  RCC_APB2Periph_GPIOA

#define NCS2_L()        GPIO_ResetBits(GPIO_PORT_NCS2, GPIO_PIN_NCS2)
#define NCS2_H()        GPIO_SetBits(GPIO_PORT_NCS2, GPIO_PIN_NCS2)


/* TYPEDEFS ------------------------------------------------------------------- */

/* MACROS  -------------------------------------------------------------------- */

/* CONSTANTS  ----------------------------------------------------------------- */

/* GLOBAL VARIABLES ----------------------------------------------------------- */

/* GLOBAL FUNCTIONS ----------------------------------------------------------- */
void MAX538_Init(void);
void MAX538_WriteValue(uint8_t DAx, uint16_t daout);




/* LOCAL VARIABLES ------------------------------------------------------------ */

/* LOCAL FUNCTIONS ------------------------------------------------------------ */


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* __MAX538_H */


