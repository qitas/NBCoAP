#ifndef __rtc_h__
#define __rtc_h__

#include <stm32f10x.h>

void RTC_Config(void); //≈‰÷√RTC
unsigned int RTC_GetCounter(void);

unsigned char get_bkp_reg(uint32_t RTC_BKP_DR,char index);
unsigned char set_bkp_reg(uint32_t RTC_BKP_DR,char index,unsigned char val);
unsigned char get_bkp_reg_4bit(uint32_t RTC_BKP_DR,char index);
unsigned char set_bkp_reg_4bit(uint32_t RTC_BKP_DR,char index,unsigned char val);

void setAlarm(unsigned int timep);

#define __CURRENT_TIME RTC_GetCounter()

#define __READ_BKP_SET_RTC_HISTORY get_bkp_reg(RTC_BKP_DR0,0)
#define __WRITE_BKP_SET_RTC_HISTORY set_bkp_reg(RTC_BKP_DR0,0,0x55)

#define __READ_RESET_COUNT get_bkp_reg(RTC_BKP_DR0,1)
#define __WRITE_RESET_COUNT set_bkp_reg(RTC_BKP_DR0,1,get_bkp_reg(RTC_BKP_DR0,1)+1)

#define __READ_STANDBY_STATUS get_bkp_reg(RTC_BKP_DR0,2)
#define __WRITE_STANDBY_STATUS(X) set_bkp_reg(RTC_BKP_DR0,2,X)

#define __READ_COUNTER get_bkp_reg(RTC_BKP_DR0,3)
#define __WRITE_COUNTER(X) set_bkp_reg(RTC_BKP_DR0,3,X)
#define __ADD_COUNTER __WRITE_COUNTER(__READ_COUNTER+1)


#define __SET_NEXT_WAKEUP_TIM(X) RTC_WriteBackupRegister(RTC_BKP_DR1,RTC_GetCounter() + X)
#define __READ_NEXT_WAKEUP_TIM RTC_ReadBackupRegister(RTC_BKP_DR1)
#define __CLEAR_NEXT_WAKEUP_TIM RTC_WriteBackupRegister(RTC_BKP_DR1,0)


//#define __SET_TIM1(X) RTC_WriteBackupRegister(RTC_BKP_DR2,X)
//#define __GET_TIM1 RTC_ReadBackupRegister(RTC_BKP_DR2)

//#define __SET_TIM2(X) RTC_WriteBackupRegister(RTC_BKP_DR3,X)
//#define __GET_TIM2 RTC_ReadBackupRegister(RTC_BKP_DR3)

#define __SET_UPLOAD_TIM(X) RTC_WriteBackupRegister(RTC_BKP_DR2,X)
#define __GET_UPLOAD_TIM RTC_ReadBackupRegister(RTC_BKP_DR2)

#define __SET_LAST_PAIZHAOTIM(X) RTC_WriteBackupRegister(RTC_BKP_DR3,X)
#define __GET_LAST_PAIZHAOTIM RTC_ReadBackupRegister(RTC_BKP_DR3)


//#define __SET_NEXT_WAKEUP_TIM_TIM1 RTC_WriteBackupRegister(RTC_BKP_DR1,RTC_GetCounter() + __GET_TIM1)
//#define __SET_NEXT_WAKEUP_TIM_TIM2 RTC_WriteBackupRegister(RTC_BKP_DR1,RTC_GetCounter() + __GET_TIM2 - __GET_TIM1)

#define __READ_ALARM_FLAG				get_bkp_reg_4bit(RTC_BKP_DR4,0)
#define __WRITE_ALARM_FLAG(X) 	set_bkp_reg_4bit(RTC_BKP_DR4,0,X)

extern void set_next_wakeup_tim(unsigned int timep);

#endif
