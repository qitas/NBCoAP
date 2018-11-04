#ifndef __bkpreg_h__
#define __bkpreg_h__

#include <stm32f10x_bkp.h>
#include <stm32f10x_rtc.h>

/*
#define BKP_DR1			RTC 初始化记录
#define BKP_DR2			系统重启计数器
#define BKP_DR3			
#define BKP_DR4			最后一次报警时间，32位

#define BKP_DR5			系统状态                           ((uint16_t)0x0014)
#define BKP_DR6			                           ((uint16_t)0x0018)
#define BKP_DR7			6,7存储下一次启动时间                           ((uint16_t)0x001C)
#define BKP_DR8			静止超时状态                           ((uint16_t)0x0020)
#define BKP_DR9			BOOTLOADER 状态                           ((uint16_t)0x0024)
#define BKP_DR10		AUTOWAKEUP                          ((uint16_t)0x0028)
#define BKP_DR11		AUTOWAKEUP                          ((uint16_t)0x0040)
#define BKP_DR12                          ((uint16_t)0x0044)
#define BKP_DR13                          ((uint16_t)0x0048)
#define BKP_DR14                          ((uint16_t)0x004C)
#define BKP_DR15                          ((uint16_t)0x0050)
#define BKP_DR16                          ((uint16_t)0x0054)
#define BKP_DR17                          ((uint16_t)0x0058)
#define BKP_DR18                          ((uint16_t)0x005C)
#define BKP_DR19                          ((uint16_t)0x0060)
#define BKP_DR20                          ((uint16_t)0x0064)
#define BKP_DR21                          ((uint16_t)0x0068)
#define BKP_DR22                          ((uint16_t)0x006C)
#define BKP_DR23                          ((uint16_t)0x0070)
#define BKP_DR24                          ((uint16_t)0x0074)
#define BKP_DR25                          ((uint16_t)0x0078)
#define BKP_DR26                          ((uint16_t)0x007C)
#define BKP_DR27                          ((uint16_t)0x0080)
#define BKP_DR28                          ((uint16_t)0x0084)
#define BKP_DR29                          ((uint16_t)0x0088)
#define BKP_DR30                          ((uint16_t)0x008C)
#define BKP_DR31                          ((uint16_t)0x0090)
#define BKP_DR32                          ((uint16_t)0x0094)
#define BKP_DR33                          ((uint16_t)0x0098)
#define BKP_DR34                          ((uint16_t)0x009C)
#define BKP_DR35                          ((uint16_t)0x00A0)
#define BKP_DR36                          ((uint16_t)0x00A4)
#define BKP_DR37                          ((uint16_t)0x00A8)
#define BKP_DR38                          ((uint16_t)0x00AC)
#define BKP_DR39                          ((uint16_t)0x00B0)
#define BKP_DR40                          ((uint16_t)0x00B4)
#define BKP_DR41                          ((uint16_t)0x00B8)
#define BKP_DR42                          ((uint16_t)0x00BC)
*/


unsigned int bkp32bit_read(unsigned short a , unsigned short b);
void bkp32bit_write(unsigned int val , unsigned short *a , unsigned short *b);
void __set_last_alarm_time(void);
unsigned int __get_last_alarm_time(void);
void __set_next_wakeup_time(unsigned int time);
unsigned int __get_next_wakeup_time(void);

#define GET_RTCINIT_FLAG		read_bkp_buffer(((unsigned char*)(&BKPREG)))->RTC_INIT_FLAG
#define SET_RTCINIT_FLAG		read_bkp_buffer(((unsigned char*)(&BKPREG)))->RTC_INIT_FLAG = 1;WRITE_BKP_REG;//BKP_WriteBackupRegister(BKP_DR2,GET_SYSTEM_COUNTER+1)

#define GET_SYSTEM_COUNTER  read_bkp_buffer(((unsigned char*)(&BKPREG)))->SYSTEM_RESET_COUNTE
#define SET_SYSTEM_COUNTER	read_bkp_buffer(((unsigned char*)(&BKPREG)))->SYSTEM_RESET_COUNTE = GET_SYSTEM_COUNTER + 1;WRITE_BKP_REG;//BKP_WriteBackupRegister(BKP_DR2,GET_SYSTEM_COUNTER+1)

#define GET_LAST_ALARM_TIME	read_bkp_buffer(((unsigned char*)(&BKPREG)))->LAST_ALARM_TIME
#define SET_LAST_ALARM_TIME	read_bkp_buffer(((unsigned char*)(&BKPREG)))->LAST_ALARM_TIME = RTC_GetCounter();WRITE_BKP_REG;

#define GET_SYSTEM_STATUS 	read_bkp_buffer(((unsigned char*)(&BKPREG)))->SYSTEM_STATUS
#define SET_SYSTEM_STATUS(X)		read_bkp_buffer(((unsigned char*)(&BKPREG)))->SYSTEM_STATUS = X;WRITE_BKP_REG;

#define GET_NEXT_WAKEUP_TIME	read_bkp_buffer(((unsigned char*)(&BKPREG)))->WAKE_UP_TIME1
#define SET_NEXT_WAKEUP_TIME(X)	read_bkp_buffer(((unsigned char*)(&BKPREG)))->WAKE_UP_TIME1 = X;WRITE_BKP_REG;

#define GET_AUTOWAKEUP_TIM			read_bkp_buffer(((unsigned char*)(&BKPREG)))->WAKE_UP_TIME2
#define SET_AUTOWAKEUP_TIM(X)		read_bkp_buffer(((unsigned char*)(&BKPREG)))->WAKE_UP_TIME2 = X;WRITE_BKP_REG;

#define GET_MOTIONLESS_STATUS	read_bkp_buffer(((unsigned char*)(&BKPREG)))->MOTIONLESS_STATUS
#define SET_MOTIONLESS_STATUS(X)	read_bkp_buffer(((unsigned char*)(&BKPREG)))->MOTIONLESS_STATUS = X;WRITE_BKP_REG;



/*
	BKP_DR9
	记录BOOTLOADER状态，每次启动APP之前设置为1，APP成功运行设置为2，如果bootloader中检测到
	重复为1那么说明APP执行失败，也就是固件下载错误，一般是直接进入配置模式
*/	
#define GET_BOOTLOADER_STATUS	BKP_ReadBackupRegister(BKP_DR9)
#define SET_BOOTLOADER_STATUS(X)	BKP_WriteBackupRegister(BKP_DR9,X)




#pragma pack(push)
#pragma  pack(1)
struct BKP_REG {
	unsigned char RTC_INIT_FLAG:1;
	unsigned char MOTIONLESS_STATUS:1;
	
	unsigned char SYSTEM_RESET_COUNTE;
	unsigned char SYSTEM_STATUS;
	
	unsigned int LAST_ALARM_TIME;
	unsigned int WAKE_UP_TIME1;
	unsigned int WAKE_UP_TIME2;
};
#pragma pack(pop)

#define DEBUG_BKP_REG \
printf("RTC_INIT_FLAG :%d \r\n",BKPREG.RTC_INIT_FLAG); \
printf("MOTIONLESS_STATUS :%d \r\n",BKPREG.MOTIONLESS_STATUS); \
printf("SYSTEM_RESET_COUNTE :%d \r\n",BKPREG.SYSTEM_RESET_COUNTE); \
printf("SYSTEM_STATUS :%d \r\n",BKPREG.SYSTEM_STATUS); \
printf("WAKE_UP_TIME1 :%d \r\n",BKPREG.WAKE_UP_TIME1); \
printf("WAKE_UP_TIME2 :%d \r\n",BKPREG.WAKE_UP_TIME2);


extern struct BKP_REG BKPREG;

struct BKP_REG *read_bkp_buffer(unsigned char*buf);
#define READ_BKP_REG read_bkp_buffer(((unsigned char*)(&BKPREG)));
void write_bkp_buffer(unsigned char*buf);
#define WRITE_BKP_REG write_bkp_buffer(((unsigned char*)(&BKPREG)));
	
void _set_auto_wakeup_tim(unsigned int time);
unsigned int _get_auto_wakeup_tim(void);

#endif

