#include "bkpreg.h"
#include <stm32f10x.h>
#include <stm32f10x_bkp.h>
#include <stm32f10x_rtc.h>
#include "common.h"

struct BKP_REG BKPREG;

unsigned int bkp32bit_read(unsigned short a , unsigned short b)
{
	
	unsigned int ret;
	
	ret = a;
	
	ret = ret << 16;
	
	ret += b;
	
	return ret;
	//
}

void bkp32bit_write(unsigned int val , unsigned short *a , unsigned short *b)
{
	unsigned int t1,t2;
	t1 = val;
	t2 = val;
	*a = t1 >> 16;
	*b = t2 & 0x0000FFFF;
	
}

void __set_last_alarm_time(void)
{
	
	unsigned short reg1,reg2;
	unsigned int current_time = RTC_GetCounter();
	bkp32bit_write(current_time,&reg1,&reg2);
	
	BKP_WriteBackupRegister(BKP_DR3,reg1);
	BKP_WriteBackupRegister(BKP_DR4,reg2);
	//
}
unsigned int __get_last_alarm_time(void)
{
	return bkp32bit_read(BKP_ReadBackupRegister(BKP_DR3),BKP_ReadBackupRegister(BKP_DR4));
	//
}

void __set_next_wakeup_time(unsigned int time)
{
	unsigned short reg1,reg2;
	bkp32bit_write(time,&reg1,&reg2);
	
	BKP_WriteBackupRegister(BKP_DR6,reg1);
	BKP_WriteBackupRegister(BKP_DR7,reg2);
	//
}
unsigned int __get_next_wakeup_time(void)
{
	return bkp32bit_read(BKP_ReadBackupRegister(BKP_DR6),BKP_ReadBackupRegister(BKP_DR7));
}

struct BKP_REG *read_bkp_buffer(unsigned char*buf)
{
	int i=0;
	unsigned short *p;
	p = (unsigned short *)buf;
	i = 0;
	p[i++] = BKP_ReadBackupRegister(BKP_DR1);
	p[i++] = BKP_ReadBackupRegister(BKP_DR2);
	p[i++] = BKP_ReadBackupRegister(BKP_DR3);
	p[i++] = BKP_ReadBackupRegister(BKP_DR4);
	p[i++] = BKP_ReadBackupRegister(BKP_DR5);
	p[i++] = BKP_ReadBackupRegister(BKP_DR6);
	p[i++] = BKP_ReadBackupRegister(BKP_DR7);
	p[i++] = BKP_ReadBackupRegister(BKP_DR8);
	p[i++] = BKP_ReadBackupRegister(BKP_DR10);

	
//	printf("BKP REG BUF:\r\n");
//	for(i=0;i<sizeof(BKPREG);i++)
//	{
//		printf("%x ",((unsigned char*)(&BKPREG))[i]);
//	}
//	printf("\r\n");
	
	return &BKPREG;

	//
}
void write_bkp_buffer(unsigned char*buf)
{
	unsigned char i=0;
	unsigned short *p;
	p = (unsigned short *)buf;
	
	BKP_WriteBackupRegister(BKP_DR1,p[i++]);
	BKP_WriteBackupRegister(BKP_DR2,p[i++]);
	BKP_WriteBackupRegister(BKP_DR3,p[i++]);
	BKP_WriteBackupRegister(BKP_DR4,p[i++]);
	BKP_WriteBackupRegister(BKP_DR5,p[i++]);
	BKP_WriteBackupRegister(BKP_DR6,p[i++]);
	BKP_WriteBackupRegister(BKP_DR7,p[i++]);
	BKP_WriteBackupRegister(BKP_DR8,p[i++]);
	BKP_WriteBackupRegister(BKP_DR10,p[i++]);

	
}

void _set_auto_wakeup_tim(unsigned int time)
{
	unsigned short *p;
	p = (unsigned short *)(&time);
	
	BKP_WriteBackupRegister(BKP_DR10,p[0]);
	BKP_WriteBackupRegister(BKP_DR12,p[1]);
	
	printf("@@@ %X %X \r\n",p[0],p[1]);
	
}

unsigned int _get_auto_wakeup_tim(void)
{
	unsigned int time;
	unsigned short *p;
	p = (unsigned short *)(&time);
	
	p[0] = BKP_ReadBackupRegister(BKP_DR10);
	p[1] = BKP_ReadBackupRegister(BKP_DR12);
	
	printf("@@@ %X %X \r\n",p[0],p[1]);
	
	return time;
}