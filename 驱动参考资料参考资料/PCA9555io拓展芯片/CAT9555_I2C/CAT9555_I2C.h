#ifndef _CAT9555_I2C_ 
#define _CAT9555_I2C_

#include <msp430G2553.h>
#include"datatype.h"

#define SDA (1<<7)  //->CAT9555 SDA
#define SCL (1<<6)  //->CAT9555 SCL

//�ⲿ���� 
void  ucb0_config(uchar addr);//�ӻ���ַ
void  IIC_P0DIR(uchar dir);   //CAT9555 P0�ڷ�������
void  IIC_P1DIR(uchar dir);   //CAT9555 P1�ڷ�������
void  IIC_P0OUT(uchar data);  //�� CAT9555 P0�� д����
void  IIC_P1OUT(uchar data);  //�� CAT9555 P1�� д����
uchar IIC_P0IN();             //��ѯCAT9555 P0�� ����
uchar IIC_P1IN();             //��ѯCAT9555 P1�� ����
#endif

