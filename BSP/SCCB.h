#ifndef _SCCB_H
#define _SCCB_H

# include <stm32f10x.h>

#define SCCB_ID 0x60

#define uchar unsigned char

void InitSCCB(void);
unsigned char sccbw(unsigned char reg,unsigned char data);
unsigned char sccbr(unsigned char reg);


unsigned char xsccbw(unsigned char reg,unsigned char data);
unsigned char xsccbr(unsigned char reg);


#endif /* _SCCB_H */
