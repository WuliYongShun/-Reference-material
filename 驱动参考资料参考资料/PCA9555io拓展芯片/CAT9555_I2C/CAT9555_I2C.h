#ifndef _CAT9555_I2C_ 
#define _CAT9555_I2C_

#include <msp430G2553.h>
#include"datatype.h"

#define SDA (1<<7)  //->CAT9555 SDA
#define SCL (1<<6)  //->CAT9555 SCL

//外部函数 
void  ucb0_config(uchar addr);//从机地址
void  IIC_P0DIR(uchar dir);   //CAT9555 P0口方向配置
void  IIC_P1DIR(uchar dir);   //CAT9555 P1口方向配置
void  IIC_P0OUT(uchar data);  //向 CAT9555 P0口 写数据
void  IIC_P1OUT(uchar data);  //向 CAT9555 P1口 写数据
uchar IIC_P0IN();             //查询CAT9555 P0口 数据
uchar IIC_P1IN();             //查询CAT9555 P1口 数据
#endif

