#ifndef __IOI2C_H
#define __IOI2C_H

extern void IOI2C_Init(void);
extern unsigned char IICwriteBytes(unsigned char dev, unsigned char reg, unsigned char length, unsigned char* data);
extern unsigned char IICreadBytes(unsigned char dev, unsigned char reg, unsigned char length, unsigned char *data);
extern unsigned short I2C_Erorr_Count;

#endif

