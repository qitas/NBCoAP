#include "rtc.h"

#include <stdio.h>
#include <stdlib.h>
#include <stm32f0xx.h>
#include <string.h>
#include <time.h>



uint32_t _RTC_GetCounter(void)
{
	#define RTC_GetCounter_NO_DEBUG
	
	RTC_TimeTypeDef  RTC_TimeStampStructure;
	RTC_DateTypeDef  RTC_TimeStampDateStructure;
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
	
	time_t t;
	struct tm time;
	
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
	
	t = mktime(&time);
	

	if (2117514496 == t)
		goto remk;
	
	//printf("================ %d\r\n",t);
	
  return t;
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

