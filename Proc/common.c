
#include <stdlib.h>
#include "config_def.h"
#include "mem.h"
#include "common.h"

void delayUS(unsigned int us)
{
	unsigned int i =0;
	for (i = 0; i < us; i++)
	{
	}
}


char xtod(char c) {
 if (c>='0' && c<='9') return c-'0';
 if (c>='A' && c<='F') return c-'A'+10;
 if (c>='a' && c<='f') return c-'a'+10;
 return c=0; // not Hex digit
}
 
	
int conv_string_2_hex(char *str , unsigned char *data)
{
	int i;
	int strl = strlen(str);
	
	if ((strl % 2) != 0)
		return -1;
	
	for(i=0;i<(strl/2);i++)
	{
		
		unsigned char H = (unsigned char)xtod(str[i*2 + 0]);
		unsigned char L = (unsigned char)xtod(str[i*2 + 1]);
		unsigned char RES = H*16 + L;
	
		
		data[i] = RES;
		//
	}
	
	return i;
	//
}


int conv_hex_2_string(unsigned char *data , int datalen , char *str)
{
	int i=0;
	char tmp[4];
	memset(str,0x0,datalen*2+4);
	//debug_buf("xx111",data,datalen);
	for(i=0;i<datalen;i++)
	{
		snprintf(tmp,sizeof(tmp),"%02X",data[i]);
		strcat(str,tmp);
		//printf("[%s]",tmp);
	}
	
	return datalen*2+1;
}

void debug_buf(char *name , unsigned char *buf , int len)
{
	int i=0;
	printf("DEBUG BUFFER (%s):\r\n",name);
	for(i=0;i<len;i++)
	{
		printf("%02X ",buf[i]);
	}
	printf("\r\n");
}


void transfer16(c_u16 *val)
{

    unsigned char a,b;
    unsigned char *x = (unsigned char*)val;
    a = x[0];
    b = x[1];

    x[0] = b;
    x[1] = a;

}

void transfer32(c_u32 *val)
{

    unsigned char a,b,c,d;
    unsigned char *x = (unsigned char*)val;
    a = x[0];
    b = x[1];
    c = x[2];
    d = x[3];

    x[0] = d;
    x[1] = c;
    x[2] = b;
    x[3] = a;

}

void settime(c_u8 *buf , c_u32 year , c_u32 mon , c_u32 day , c_u32 hour , c_u32 min , c_u32 sec)
{

		unsigned char *tmp;
		c_u32 nDstTimeTmp;
		c_u32 nDstDateTmp;
    c_u32 nDstDate = 0;
    c_u32 nDstTime = 0;

    c_u32 nYear = year - 2000;
    c_u32 nMon  = mon;
    c_u32 nDay  = day;
    c_u32 nHour = hour;
    c_u32 nMin  = min;
    c_u32 nSec  = sec;


    nDstDate = (nYear << 9) & 0x0000fe00;
    nDstDate = ((nMon << 5) & 0x000001e0) | nDstDate;
    nDstDate = (nDay & 0x0000001f) | nDstDate;
    nDstDateTmp = (nDstDate >> 8) & 0x000000ff;
    nDstDate = (((nDstDate & 0x000000ff) << 8 ) & 0x0000ff00) | nDstDateTmp;
    nDstTime = (nHour << 11) & 0x0000f800;
    nDstTime = ((nMin << 5) & 0x000007e0) | nDstTime;
    nDstTime = (nSec & 0x0000001f) | nDstTime;

    nDstTimeTmp = (nDstTime >> 8) & 0x000000ff;
    nDstTime = (((nDstTime & 0x000000ff) << 8 ) & 0x0000ff00) | nDstTimeTmp;

    tmp = (unsigned char*)nDstTime;


    buf[0] = (((c_u8*)(&nDstDate))[1]);
    buf[1] = (((c_u8*)(&nDstDate))[0]);
    buf[2] = (((c_u8*)(&nDstTime))[1]);
    buf[3] = (((c_u8*)(&nDstTime))[0]);


}

#include <stm32f10x.h>
#include "core_cm3.h"
void sys_shutdown(void)
{
	NVIC_SystemReset();
}


struct GLOBAL_DATA *GD;


