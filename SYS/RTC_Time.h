/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               RTC_Time.h
** Descriptions:            None
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-10-30
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/

#ifndef _RTC_TIME_H_
#define _RTC_TIME_H_

/* Includes ------------------------------------------------------------------*/
#include <stm32f10x.h>
#include <stm32f10x_rtc.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

/* Private function prototypes -----------------------------------------------*/
int RTC_Init(void);
void Time_Display(void);
void Time_Regulate(void);

#define CURRENT_RTC_TIM RTC_GetCounter()

unsigned char get_bkp_reg(uint32_t RTC_BKP_DR,char index);
unsigned char set_bkp_reg(uint32_t RTC_BKP_DR,char index,unsigned char val);
unsigned char get_bkp_reg_4bit(uint32_t RTC_BKP_DR,char index);
unsigned char set_bkp_reg_4bit(uint32_t RTC_BKP_DR,char index,unsigned char val);

/* Private variables ---------------------------------------------------------*/
extern FunctionalState TimeDisplay;

#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
