#ifndef FM24CL64_H
#define FM24CL64_H

#include "IIC.h"



extern unsigned char read_B_FRAM(unsigned int src);

extern void read_FRAM(unsigned int src, unsigned char *dst, unsigned char n);

extern void write_B_FRAM(unsigned char dat, unsigned int addr);

extern void write_FRAM(unsigned char *src, unsigned int dst, unsigned char n);


#endif	//#define FM24CL64_H