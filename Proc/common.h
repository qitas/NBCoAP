#ifndef __common_h__
#define __common_h__

#include <stdio.h>
#include <string.h>


#define DEBUG_ERROR(X) printf("ERROR:%s,%s,%d CODE[%d]\r\n",__FILE__,__FUNCTION__,__LINE__,X)
#define DEBUG_VALUE(X) printf("VALUE:%s,%s,%d CODE[%d][%X]\r\n",__FILE__,__FUNCTION__,__LINE__,X,X)
#define DEBUG_VALUE2(Y,X) printf("%s VALUE:%s,%s,%d CODE[%d][%X]\r\n",Y,__FILE__,__FUNCTION__,__LINE__,X,X)

#define GetBit(dat,i) ((dat&((unsigned char)1<<i))?1:0)
#define SetBit(dat,i) ((dat)|=((unsigned char)1<<(i)))
#define ClearBit(dat,i) ((dat)&=(~((unsigned char)0x01<<(i))))

//-------------------------

typedef unsigned short c_u16;
typedef unsigned char c_u8;
typedef unsigned int c_u32;

#define DELAY_IR	10800
#define DELAY_ADC 10000
void delayUS(unsigned int us);

int conv_string_2_hex(char *str , unsigned char *data);
int conv_hex_2_string(unsigned char *data , int datalen , char *str);
void debug_buf(char *name , unsigned char *buf , int len);

void transfer32(c_u32 *val);
void transfer16(c_u16 *val);
void settime(c_u8 *buf , c_u32 year , c_u32 mon , c_u32 day , c_u32 hour , c_u32 min , c_u32 sec);

void sys_shutdown(void);

int SYSCFG_ReInt(void);

extern unsigned char __setbuffer(void);

#define ENABLE_WAKEUP_DBG


#endif
