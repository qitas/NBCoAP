#include "rtc.h"

#include <stdio.h>
#include <stdlib.h>
#include <stm32f0xx.h>
#include <string.h>
#include <time.h>

static void test(void)
{
	//time_t mktime(struct tm * timeptr);

}

__IO uint32_t CaptureNumber = 0, PeriodValue = 0;

/**
  * @brief  Configure the RTC peripheral by selecting the clock source.
  * @param  None
  * @retval None
  */
void RTC_Config(void)
{
  RTC_TimeTypeDef RTC_TimeStructure;
//  NVIC_InitTypeDef NVIC_InitStructure; 
//  EXTI_InitTypeDef EXTI_InitStructure;
//  RTC_AlarmTypeDef RTC_AlarmStructure;
	RTC_InitTypeDef   RTC_InitStructure;
	
  /* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  
  /* Allow access to RTC */
  PWR_BackupAccessCmd(ENABLE);
	RCC_LSICmd(ENABLE);
	
	if (__READ_BKP_SET_RTC_HISTORY != 0x55)
	{
		

		/* LSI used as RTC source clock */
		/* The RTC Clock may varies due to LSI frequency dispersion. */   
		/* Enable the LSI OSC */ 


		/* Wait till LSI is ready */  
		while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
		{
		}

		/* Select the RTC Clock Source */
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
		 
		/* Enable the RTC Clock */
		RCC_RTCCLKCmd(ENABLE);

		/* Wait for RTC APB registers synchronisation */
		RTC_WaitForSynchro();

		/* Calendar Configuration */
		RTC_InitStructure.RTC_AsynchPrediv = 99;
		RTC_InitStructure.RTC_SynchPrediv	=  399; /* (40KHz / 100) - 1 = 399*/
		RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
		RTC_Init(&RTC_InitStructure);
		
		
		/* Set the time to 00h 00mn 00s AM */
		RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
		RTC_TimeStructure.RTC_Hours   = 0x00;
		RTC_TimeStructure.RTC_Minutes = 0x00;
		RTC_TimeStructure.RTC_Seconds = 0x00;  
		
		RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);
		
		__WRITE_BKP_SET_RTC_HISTORY;
	
	}else{

	}
	
	return ;
  
}

/**
  * @brief  Configures TIM14 to measure the LSI oscillator frequency. 
  * @param  None
  * @retval LSI Frequency
  */
uint32_t GetLSIFrequency(void)
{
  NVIC_InitTypeDef   NVIC_InitStructure;
  TIM_ICInitTypeDef  TIM_ICInitStructure;
  RCC_ClocksTypeDef  RCC_ClockFreq;

  /* TIM14 configuration *******************************************************/ 
  /* Enable TIM14 clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
  
  /* Reset TIM14 registers */
  TIM_DeInit(TIM14);

  /* Configure TIM14 prescaler */
  TIM_PrescalerConfig(TIM14, 0, TIM_PSCReloadMode_Immediate);

  /* Connect internally the TIM14_CH1 to the RTC clock output */
  TIM_RemapConfig(TIM14, TIM14_RTC_CLK);

  /* TIM14 configuration: Input Capture mode ---------------------
     The reference clock(LSE or external) is connected to TIM14 CH1
     The Rising edge is used as active edge,
     The TIM14 CCR1 is used to compute the frequency value 
  ------------------------------------------------------------ */
  TIM_ICInitStructure.TIM_Channel     = TIM_Channel_1;
  TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV8;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_ICInit(TIM14, &TIM_ICInitStructure);

  /* Enable the TIM14 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable TIM14 counter */
  TIM_Cmd(TIM14, ENABLE);

  /* Reset the flags */
  TIM14->SR = 0;
    
  /* Enable the CC1 Interrupt Request */  
  TIM_ITConfig(TIM14, TIM_IT_CC1, ENABLE);
	

  /* Wait until the TIM14 get 2 LSI edges (refer to TIM14_IRQHandler() in 
    stm32F0xx_it.c file) ******************************************************/
  while(CaptureNumber != 2)
  {
  }
  /* Deinitialize the TIM14 peripheral registers to their default reset values */
  TIM_DeInit(TIM14);


  /* Compute the LSI frequency, depending on TIM14 input clock frequency (PCLK1)*/
  /* Get SYSCLK, HCLK and PCLKx frequency */
  RCC_GetClocksFreq(&RCC_ClockFreq);
  
  /* PCLK1 prescaler equal to 1 => TIMCLK = PCLK1 */
  return ((RCC_ClockFreq.PCLK_Frequency / PeriodValue) * 8);
}

uint32_t RTC_GetCounter(void)
{
	#define RTC_GetCounter_NO_DEBUG
	
	RTC_TimeTypeDef  RTC_TimeStampStructure;
	RTC_DateTypeDef  RTC_TimeStampDateStructure;
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
	
	time_t t;
	struct tm time;
	RC_CALLBACK;
	remk:
	
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	
	#ifndef RTC_GetCounter_NO_DEBUG
	printf("\n\r============== Current Time Display ============================\n\r");
	printf("\n\r  The current time (Hour-Minute-Second) is :  %0.2d:%0.2d:%0.2d \n\r", RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
	#endif
	
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
	
	#ifndef RTC_GetCounter_NO_DEBUG
  printf("\n\r============== Current Date Display ============================\n\r");
  printf("\n\r  The current date (WeekDay-Date-Month-Year) is :  %0.2d-%0.2d-%0.2d-%0.2d \n\r", RTC_DateStructure.RTC_WeekDay, RTC_DateStructure.RTC_Date, RTC_DateStructure.RTC_Month, RTC_DateStructure.RTC_Year);
	#endif

	memset(&time,0x0,sizeof(time));
	
	
	time.tm_hour = RTC_TimeStructure.RTC_Hours;
	time.tm_sec = RTC_TimeStructure.RTC_Seconds;
	time.tm_min = RTC_TimeStructure.RTC_Minutes;
	
	time.tm_year = RTC_DateStructure.RTC_Year;
	time.tm_mon = RTC_DateStructure.RTC_Month;
	time.tm_mday = RTC_DateStructure.RTC_Date;
	
	//printf("GMT is: %s", asctime(&time));
	
	t = mktime(&time);
	
	#if 0
	{
		struct tm *p;  
		t=1384936600;  
		p=gmtime(&t);
		
		printf("GMT is: %s", asctime(p));
	}		
	#endif
	
	
	
	return rt();
}


void RTC_Alarm_IRQHandler(void) 
{ 
    if(RTC_GetFlagStatus(RTC_FLAG_ALRAF)==SET) 
    { 
        printf("alarm come !\r\n"); 
        PWR_BackupAccessCmd(ENABLE);//???,??RTC_ClearITPendingBit ???? 
        RTC_ClearITPendingBit(RTC_IT_ALRA); 
        PWR_BackupAccessCmd(DISABLE); 
        EXTI_ClearITPendingBit(EXTI_Line17);//???????,?????????,????????? 
    } 
}

void setAlarm(unsigned int timep)
{

	RTC_AlarmTypeDef RTC_AlarmStructure; 
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	struct tm *p;  
	p=gmtime(&timep);
	//printf("setAlarm GMT is: %s", asctime(p));
	//
	
	RTC_TimeStructure.RTC_Hours = p->tm_hour;
	RTC_TimeStructure.RTC_Seconds = p->tm_sec;
	RTC_TimeStructure.RTC_Minutes = p->tm_min;
	
	RTC_DateStructure.RTC_Year = p->tm_year;
	RTC_DateStructure.RTC_Month = p->tm_mon;
	RTC_DateStructure.RTC_Date = p->tm_mday;
	
	
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours=p->tm_hour; 
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes=p->tm_sec; 
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds=5; 
	RTC_AlarmStructure.RTC_AlarmTime.RTC_H12=RTC_H12_AM;
	
	//RTC_AlarmStructure.RTC_AlarmDateWeekDay
	
	RTC_AlarmStructure.RTC_AlarmMask=RTC_AlarmMask_None;//?????????????? 
	//RTC_AlarmStructure.RTC_AlarmMask=RTC_AlarmMask_DateWeekDay| 
	//RTC_AlarmMask_Hours|RTC_AlarmMask_Minutes;


  RTC_SetAlarm(RTC_Format_BIN,RTC_Alarm_A,&RTC_AlarmStructure); 
  RTC_AlarmCmd(RTC_Alarm_A,ENABLE);
	RTC_ITConfig(RTC_IT_ALRA,ENABLE); 
  //????????nvic.c 
  PWR_BackupAccessCmd(DISABLE);
	
	
	 //Alarm????EXTI_Line17?? 
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//??? 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; 
	EXTI_InitStructure.EXTI_Line = EXTI_Line17; 
	EXTI_Init(&EXTI_InitStructure); 
	
	
}


///**
//  * @brief  Display the current time on the Hyperterminal.
//  * @param  None
//  * @retval None
//  */
//void RTC_TimeShow(void)
//{
//  /* Get the current Time and Date */
//  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
//  printf("\n\r============== Current Time Display ============================\n\r");
//  printf("\n\r  The current time (Hour-Minute-Second) is :  %0.2d:%0.2d:%0.2d \n\r", RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
//  /* Unfreeze the RTC DR Register */
//  (void)RTC->DR;
//}

///**
//  * @brief  Display the current date on the Hyperterminal.
//  * @param  None
//  * @retval None
//  */
//void RTC_DateShow(void)
//{
//  /* Get the current Date */
//  RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
//  printf("\n\r============== Current Date Display ============================\n\r");
//  printf("\n\r  The current date (WeekDay-Date-Month-Year) is :  %0.2d-%0.2d-%0.2d-%0.2d \n\r", RTC_DateStructure.RTC_WeekDay, RTC_DateStructure.RTC_Date, RTC_DateStructure.RTC_Month, RTC_DateStructure.RTC_Year);
//}


///**
//  * @brief  Display the current TimeStamp (time and date) on the Hyperterminal.
//  * @param  None
//  * @retval None
//  */
//void RTC_TimeStampShow(void)
//{
//  /* Get the current TimeStamp */
//  RTC_GetTimeStamp(RTC_Format_BIN, &RTC_TimeStampStructure, &RTC_TimeStampDateStructure);
//  printf("\n\r==============TimeStamp Display (Time and Date)=================\n\r");
//  printf("\n\r  The current time stamp time (Hour-Minute-Second) is :  %0.2d:%0.2d:%0.2d \n\r", RTC_TimeStampStructure.RTC_Hours, RTC_TimeStampStructure.RTC_Minutes, RTC_TimeStampStructure.RTC_Seconds);
//  printf("\n\r  The current timestamp date (WeekDay-Date-Month) is :  %0.2d-%0.2d-%0.2d \n\r", RTC_TimeStampDateStructure.RTC_WeekDay, RTC_TimeStampDateStructure.RTC_Date, RTC_TimeStampDateStructure.RTC_Month);
//}

void __set_time_counter(void)
{
	RTC_WriteBackupRegister(RTC_BKP_DR1,RTC_ReadBackupRegister(RTC_BKP_DR1) + 20);
}

unsigned char get_bkp_regx(uint32_t RTC_BKP_DR,char index)
{
	unsigned char *p;
	uint32_t ret = RTC_ReadBackupRegister(RTC_BKP_DR);
	//
	p = (unsigned char*)(&ret);
	
	return p[index];
}

unsigned char get_bkp_reg_4bit(uint32_t RTC_BKP_DR,char index)
{
	unsigned char x = index % 2;
	unsigned char y = index / 2;
	uint32_t val = RTC_ReadBackupRegister(RTC_BKP_DR);
	unsigned char *p = (unsigned char*)(&val);
	unsigned char ret;
	
	if (x == 0)
	{
		ret = p[y] >> 4; //高4bit
	}else
	if (x == 1)
	{
		ret = p[y]&0x0f; //低4bit
	}
	
	return ret;
}

unsigned char set_bkp_reg_4bit(uint32_t RTC_BKP_DR,char index,unsigned char z)
{
	unsigned char x = index % 2;
	unsigned char y = index / 2;
	uint32_t val = RTC_ReadBackupRegister(RTC_BKP_DR);
	unsigned char *p = (unsigned char*)(&val);
	unsigned char ret;
	
	if (x == 0)
	{
		
		p[y] = p[y] & 0x0f; //清空高4bit
		
		z = z << 4;  //xxxx1111
		z |= 0x0f;
		
		p[y] = p[y] | z;
		
	}else
	if (x == 1)
	{
		p[y] = p[y] & 0xf0; //清空低4bit
		z &= 0x0f;	//0000xxxx
		p[y] = p[y] | z;
	}
	
	RTC_WriteBackupRegister(RTC_BKP_DR,val);
}




unsigned char get_bkp_reg(uint32_t RTC_BKP_DR,char index)
{
	unsigned char x = index % 2;
	unsigned char y = index / 2;
	uint32_t val = RTC_ReadBackupRegister(RTC_BKP_DR);
	unsigned char *p = (unsigned char*)(&val);
	unsigned char ret;
	
	ret = p[index];
	
	return ret;
}

unsigned char set_bkp_reg(uint32_t RTC_BKP_DR,char index,unsigned char z)
{
	unsigned char x = index % 2;
	unsigned char y = index / 2;
	uint32_t val = RTC_ReadBackupRegister(RTC_BKP_DR);
	unsigned char *p = (unsigned char*)(&val);
	unsigned char ret;
	
	p[index] = z;
	
	RTC_WriteBackupRegister(RTC_BKP_DR,val);
}

void set_next_wakeup_tim(unsigned int timep)
{
	if (timep > RTC_GetCounter())
	{
		__SET_NEXT_WAKEUP_TIM(timep - RTC_GetCounter());
	}else{
		__SET_NEXT_WAKEUP_TIM(0);
	}
}