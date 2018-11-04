/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               RTC_Time.c
** Descriptions:            The RTC application function
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

/*******************************************************************************
* ±¾ÎÄ¼şÊµÏÖ»ùÓÚRTCµÄÈÕÆÚ¹¦ÄÜ£¬Ìá¹©ÄêÔÂÈÕµÄ¶ÁĞ´¡££¨»ùÓÚANSI-CµÄtime.h£©
* 

* RTCÖĞ±£´æµÄÊ±¼ä¸ñÊ½£¬ÊÇUNIXÊ±¼ä´Á¸ñÊ½µÄ¡£¼´Ò»¸ö32bitµÄtime_t±äÁ¿£¨ÊµÎªu32£©
*
* ANSI-CµÄ±ê×¼¿âÖĞ£¬Ìá¹©ÁËÁ½ÖÖ±íÊ¾Ê±¼äµÄÊı¾İ  ĞÍ£º
* time_t:   	UNIXÊ±¼ä´Á£¨´Ó1970-1-1Æğµ½Ä³Ê±¼ä¾­¹ıµÄÃëÊı£©
* 	typedef unsigned int time_t;
* 
* struct tm:	Calendar¸ñÊ½£¨ÄêÔÂÈÕĞÎÊ½£©
*   tm½á¹¹ÈçÏÂ£º
*   struct tm {
*   	int tm_sec;   // Ãë seconds after the minute, 0 to 60
*   					 (0 - 60 allows for the occasional leap second)
*   	int tm_min;   // ·Ö minutes after the hour, 0 to 59
*		int tm_hour;  // Ê± hours since midnight, 0 to 23
*		int tm_mday;  // ÈÕ day of the month, 1 to 31
*		int tm_mon;   // ÔÂ months since January, 0 to 11
*		int tm_year;  // Äê years since 1900
*		int tm_wday;  // ĞÇÆÚ days since Sunday, 0 to 6
*		int tm_yday;  // ´ÓÔªµ©ÆğµÄÌìÊı days since January 1, 0 to 365
* 		int tm_isdst; // ÏÄÁîÊ±£¿£¿Daylight Savings Time flag
* 		...
* 	}
* 	ÆäÖĞwday£¬yday¿ÉÒÔ×Ô¶¯²úÉú£¬Èí¼şÖ±½Ó¶ÁÈ¡
* 	monµÄÈ¡ÖµÎª0-11
*	***×¢Òâ***£º
*	tm_year:ÔÚtime.h¿âÖĞ¶¨ÒåÎª1900ÄêÆğµÄÄê·İ£¬¼´2008ÄêÓ¦±íÊ¾Îª2008-1900=108
* 	ÕâÖÖ±íÊ¾·½·¨¶ÔÓÃ»§À´Ëµ²»ÊÇÊ®·ÖÓÑºÃ£¬ÓëÏÖÊµÓĞ½Ï´ó²îÒì¡£
* 	ËùÒÔÔÚ±¾ÎÄ¼şÖĞ£¬ÆÁ±ÎÁËÕâÖÖ²îÒì¡£
* 	¼´Íâ²¿µ÷ÓÃ±¾ÎÄ¼şµÄº¯ÊıÊ±£¬tm½á¹¹ÌåÀàĞÍµÄÈÕÆÚ£¬tm_year¼´Îª2008
* 	×¢Òâ£ºÈôÒªµ÷ÓÃÏµÍ³¿âtime.cÖĞµÄº¯Êı£¬ĞèÒª×ÔĞĞ½«tm_year-=1900
* 
* ³ÉÔ±º¯ÊıËµÃ÷£º
* struct tm Time_ConvUnixToCalendar(time_t t);
* 	ÊäÈëÒ»¸öUnixÊ±¼ä´Á£¨time_t£©£¬·µ»ØCalendar¸ñÊ½ÈÕÆÚ
* time_t Time_ConvCalendarToUnix(struct tm t);
* 	ÊäÈëÒ»¸öCalendar¸ñÊ½ÈÕÆÚ£¬·µ»ØUnixÊ±¼ä´Á£¨time_t£©
* time_t Time_GetUnixTime(void);
* 	´ÓRTCÈ¡µ±Ç°Ê±¼äµÄUnixÊ±¼ä´ÁÖµ
* struct tm Time_GetCalendarTime(void);
* 	´ÓRTCÈ¡µ±Ç°Ê±¼äµÄÈÕÀúÊ±¼ä
* void Time_SetUnixTime(time_t);
* 	ÊäÈëUNIXÊ±¼ä´Á¸ñÊ½Ê±¼ä£¬ÉèÖÃÎªµ±Ç°RTCÊ±¼ä
* void Time_SetCalendarTime(struct tm t);
* 	ÊäÈëCalendar¸ñÊ½Ê±¼ä£¬ÉèÖÃÎªµ±Ç°RTCÊ±¼ä
* 
* Íâ²¿µ÷ÓÃÊµÀı£º
* ¶¨ÒåÒ»¸öCalendar¸ñÊ½µÄÈÕÆÚ±äÁ¿£º
* struct tm now;
* now.tm_year = 2008;
* now.tm_mon = 11;		//12ÔÂ
* now.tm_mday = 20;
* now.tm_hour = 20;
* now.tm_min = 12;
* now.tm_sec = 30;
* 
* »ñÈ¡µ±Ç°ÈÕÆÚÊ±¼ä£º
* tm_now = Time_GetCalendarTime();
* È»ºó¿ÉÒÔÖ±½Ó¶Átm_now.tm_wday»ñÈ¡ĞÇÆÚÊı
* 
* ÉèÖÃÊ±¼ä£º
* Step1. tm_now.xxx = xxxxxxxxx;
* Step2. Time_SetCalendarTime(tm_now);
* 
* ¼ÆËãÁ½¸öÊ±¼äµÄ²î
* struct tm t1,t2;
* t1_t = Time_ConvCalendarToUnix(t1);
* t2_t = Time_ConvCalendarToUnix(t2);
* dt = t1_t - t2_t;
* dt¾ÍÊÇÁ½¸öÊ±¼ä²îµÄÃëÊı
* dt_tm = mktime(dt);	//×¢ÒâdtµÄÄê·İÆ¥Åä£¬ansi¿âÖĞº¯ÊıÎªÏà¶ÔÄê·İ£¬×¢Òâ³¬ÏŞ
* Áí¿ÉÒÔ²Î¿¼Ïà¹Ø×ÊÁÏ£¬µ÷ÓÃansi-c¿âµÄ¸ñÊ½»¯Êä³öµÈ¹¦ÄÜ£¬ctime£¬strftimeµÈ
* 
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "RTC_Time.h"
#include <stm32f10x_bkp.h>
#include <stm32f10x_pwr.h>
#include <stm32f10x_rtc.h>
#include <stm32f10x_rcc.h>
#include "misc.h"
#include "common.h"
#include "bsp.h"

/* Private define ------------------------------------------------------------*/
//#define RTCClockOutput_Enable  /* RTC Clock/64 is output on tamper pin(PC.13) */  

/* Private function prototypes -----------------------------------------------*/
void Time_Set(u32 t);

/*******************************************************************************
* Function Name  : Time_ConvUnixToCalendar
* Description    : ×ª»»UNIXÊ±¼ä´ÁÎªÈÕÀúÊ±¼ä
* Input          : - t: µ±Ç°Ê±¼äµÄUNIXÊ±¼ä´Á
* Output         : None
* Return         : struct tm
* Attention		 : None
*******************************************************************************/
struct tm Time_ConvUnixToCalendar(time_t t)
{
	struct tm *t_tm;
	t_tm = localtime(&t);
	t_tm->tm_year += 1900;	/* localtime×ª»»½á¹ûµÄtm_yearÊÇÏà¶ÔÖµ£¬ĞèÒª×ª³É¾ø¶ÔÖµ */
	return *t_tm;
}

/*******************************************************************************
* Function Name  : Time_ConvCalendarToUnix
* Description    : Ğ´ÈëRTCÊ±ÖÓµ±Ç°Ê±¼ä
* Input          : - t: struct tm
* Output         : None
* Return         : time_t
* Attention		 : None
*******************************************************************************/
time_t Time_ConvCalendarToUnix(struct tm t)
{
	t.tm_year -= 1900;  /* Íâ²¿tm½á¹¹Ìå´æ´¢µÄÄê·İÎª2008¸ñÊ½	*/
						/* ¶øtime.hÖĞ¶¨ÒåµÄÄê·İ¸ñÊ½Îª1900Äê¿ªÊ¼µÄÄê·İ */
						/* ËùÒÔ£¬ÔÚÈÕÆÚ×ª»»Ê±Òª¿¼ÂÇµ½Õâ¸öÒòËØ¡£*/
	return mktime(&t);
}


/*******************************************************************************
* Function Name  : Time_GetUnixTime
* Description    : ´ÓRTCÈ¡µ±Ç°Ê±¼äµÄUnixÊ±¼ä´ÁÖµ
* Input          : None
* Output         : None
* Return         : time_t
* Attention		 : None
*******************************************************************************/
time_t Time_GetUnixTime(void)
{
	return (time_t)RTC_GetCounter();
}

/*******************************************************************************
* Function Name  : Time_GetCalendarTime
* Description    : ´ÓRTCÈ¡µ±Ç°Ê±¼äµÄÈÕÀúÊ±¼ä£¨struct tm£©
* Input          : None
* Output         : None
* Return         : struct tm
* Attention		 : None
*******************************************************************************/
struct tm Time_GetCalendarTime(void)
{
	time_t t_t;
	struct tm t_tm;

	t_t = (time_t)RTC_GetCounter();
	t_tm = Time_ConvUnixToCalendar(t_t);
	return t_tm;
}

/*******************************************************************************
* Function Name  : Time_SetUnixTime
* Description    : ½«¸ø¶¨µÄUnixÊ±¼ä´ÁĞ´ÈëRTC
* Input          : - t: time_t 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Time_SetUnixTime(time_t t)
{
	RTC_WaitForLastTask();
	RTC_SetCounter((u32)t);
	RTC_WaitForLastTask();
	return;
}

/*******************************************************************************
* Function Name  : Time_SetCalendarTime
* Description    : ½«¸ø¶¨µÄCalendar¸ñÊ½Ê±¼ä×ª»»³ÉUNIXÊ±¼ä´ÁĞ´ÈëRTC
* Input          : - t: struct tm
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Time_SetCalendarTime(struct tm t)
{
	Time_SetUnixTime(Time_ConvCalendarToUnix(t));
	return;
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures the nested vectored interrupt controller.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  /* Enable the RTC Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures the RTC.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void RTC_Configuration(void)
{
  /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);

  /* Reset Backup Domain */
  BKP_DeInit();

  RCC_LSICmd(ENABLE);
  while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);  

  /* Select LSE as RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

  /* Enable RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC registers synchronization */
  RTC_WaitForSynchro();

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Enable the RTC Second */
  RTC_ITConfig(RTC_IT_SEC, ENABLE);

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Set RTC prescaler: set RTC period to 1sec */
  RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
}

/*******************************************************************************
* Function Name  : USART_Scanf
* Description    : USART Receive
* Input          : - min_value: 
*                  - max_value:
*                  - lenght:
* Output         : None
* Return         : uint8_t
* Attention		 : None
*******************************************************************************/
static uint16_t USART_Scanf(uint32_t min_value,uint32_t max_value,uint8_t lenght)
{
  uint16_t index = 0;
  uint32_t tmp[4] = {0, 0, 0, 0};


  return index;
}

/*******************************************************************************
* Function Name  : Time_Regulate
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Time_Regulate(void)
{
  struct tm time;

  memset(&time, 0 , sizeof(time) );	/* Çå¿Õ½á¹¹Ìå */

  printf("=======================Time Settings==========================\r\n");
  printf("Please Set Years between 1970 to 2037\r\n");

  while ( time.tm_year>2037  || time.tm_year<1970)
  {   
    time.tm_year = USART_Scanf(1970,2037,4);
  }
  printf("Set Years:  %d\r\n", time.tm_year);

  printf("Please Set Months between 01 to 12\r\n");
  while (time.tm_mon >12 || time.tm_mon < 1 )
  {
    time.tm_mon= USART_Scanf(1,12,2)-1;
  }
  printf("Set Months:  %d\r\n", time.tm_mon);

  printf("Please Set Days between 01 to 31\r\n");
  while (time.tm_mday >31 ||time.tm_mday <1 )
  {
    time.tm_mday = USART_Scanf(1,31,2);
  }
  printf("Set Days:  %d\r\n", time.tm_mday);

  printf("Please Set Hours between 01 to 23\r\n");
  while (time.tm_hour >23 ||time.tm_hour <1 )
  {
    time.tm_hour = USART_Scanf(1,23,2);
  }
  printf("Set Hours:  %d\r\n", time.tm_hour);

  printf("Please Set Minutes between 01 to 59\r\n");
  while (time.tm_min >59 || time.tm_min <1 )
  {
    time.tm_min = USART_Scanf(1,59,2);
  }
  printf("Set Minutes:  %d\r\n", time.tm_min);

  printf("Please Set Seconds between 01 to 59\r\n");
  while (time.tm_sec >59 || time.tm_sec <1 )
  {
    time.tm_sec = USART_Scanf(1,59,2);
  }
  printf("Set Seconds:  %d\r\n", time.tm_sec);
  /* Return the value to store in RTC counter register */
  Time_SetCalendarTime(time);  
}

/*******************************************************************************
* Function Name  : RTC_Init
* Description    : RTC Initialization
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None	
*******************************************************************************/



static void RTC_NVIC_Config(void)
{	
  NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		//RTCÈ«¾ÖÖĞ¶Ï
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//ÏÈÕ¼ÓÅÏÈ¼¶1Î»,´ÓÓÅÏÈ¼¶3Î»
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//ÏÈÕ¼ÓÅÏÈ¼¶0Î»,´ÓÓÅÏÈ¼¶4Î»
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//Ê¹ÄÜ¸ÃÍ¨µÀÖĞ¶Ï
	NVIC_Init(&NVIC_InitStructure);		//¸ù¾İNVIC_InitStructÖĞÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯ÍâÉèNVIC¼Ä´æÆ÷
}


int RTC_Init(void)
{
	//¼ì²éÊÇ²»ÊÇµÚÒ»´ÎÅäÖÃÊ±ÖÓ
	u8 temp=0;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//Ê¹ÄÜPWRºÍBKPÍâÉèÊ±ÖÓ   
	PWR_BackupAccessCmd(ENABLE);	//Ê¹ÄÜºó±¸¼Ä´æÆ÷·ÃÎÊ  
	RCC_LSICmd(ENABLE);
	if (GET_RTCINIT_FLAG != 1)		//´ÓÖ¸¶¨µÄºó±¸¼Ä´æÆ÷ÖĞ¶Á³öÊı¾İ:¶Á³öÁËÓëĞ´ÈëµÄÖ¸¶¨Êı¾İ²»Ïàºõ
	{
		printf("RTC  ÖØĞÂ³õÊ¼»¯ %d  \r\n",GET_RTCINIT_FLAG);
		BKP_DeInit();	//¸´Î»±¸·İÇøÓò 	
		//RCC_LSEConfig(RCC_LSE_OFF);	//ÉèÖÃÍâ²¿µÍËÙ¾§Õñ(LSE),Ê¹ÓÃÍâÉèµÍËÙ¾§Õñ
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
		while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET&&temp<250)	//¼ì²éÖ¸¶¨µÄRCC±êÖ¾Î»ÉèÖÃÓë·ñ,µÈ´ıµÍËÙ¾§Õñ¾ÍĞ÷
		{
			temp++;
			//rt_thread_sleep(1);
		}
		if(temp>=250)return 1;//³õÊ¼»¯Ê±ÖÓÊ§°Ü,¾§ÕñÓĞÎÊÌâ	    
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);		//ÉèÖÃRTCÊ±ÖÓ(RTCCLK),Ñ¡ÔñLSE×÷ÎªRTCÊ±ÖÓ    
		RCC_RTCCLKCmd(ENABLE);	//Ê¹ÄÜRTCÊ±ÖÓ  
		RTC_WaitForLastTask();	//µÈ´ı×î½üÒ»´Î¶ÔRTC¼Ä´æÆ÷µÄĞ´²Ù×÷Íê³É
		RTC_WaitForSynchro();		//µÈ´ıRTC¼Ä´æÆ÷Í¬²½  
		//RTC_ITConfig(RTC_IT_SEC, ENABLE);		//Ê¹ÄÜRTCÃëÖĞ¶Ï
		RTC_WaitForLastTask();	//µÈ´ı×î½üÒ»´Î¶ÔRTC¼Ä´æÆ÷µÄĞ´²Ù×÷Íê³É
		RTC_EnterConfigMode();/// ÔÊĞíÅäÖÃ	
		RTC_SetPrescaler(40000); //ÉèÖÃRTCÔ¤·ÖÆµµÄÖµ
		RTC_WaitForLastTask();	//µÈ´ı×î½üÒ»´Î¶ÔRTC¼Ä´æÆ÷µÄĞ´²Ù×÷Íê³É
		//RTC_Set(2015,1,14,17,42,55);  //ÉèÖÃÊ±¼ä	
		
		RTC_ITConfig(RTC_IT_ALR, ENABLE);	//Ê¹ÄÜRTCÃëÖĞ¶Ï
		RTC_NVIC_Config();//RCTÖĞ¶Ï·Ö×éÉèÖÃ	
		
		RTC_ExitConfigMode(); //ÍË³öÅäÖÃÄ£Ê
		SET_RTCINIT_FLAG;  
		//BKP_WriteBackupRegister(BKP_DR1, 0X5050);	//ÏòÖ¸¶¨µÄºó±¸¼Ä´æÆ÷ÖĞĞ´ÈëÓÃ»§³ÌĞòÊı¾İ
	}
	else//ÏµÍ³¼ÌĞø¼ÆÊ±
	{

		RTC_WaitForSynchro();	//µÈ´ı×î½üÒ»´Î¶ÔRTC¼Ä´æÆ÷µÄĞ´²Ù×÷Íê³É
		RTC_WaitForLastTask();	//µÈ´ı×î½üÒ»´Î¶ÔRTC¼Ä´æÆ÷µÄĞ´²Ù×÷Íê³É
	}
		    				     	
	return 0; //ok

}		 				    
//RTCÊ±ÖÓÖĞ¶Ï
//Ã¿Ãë´¥·¢Ò»´Î  
//extern u16 tcnt; 
void RTC_IRQHandler(void)
{		 
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)//ÃëÖÓÖĞ¶Ï
	{
	}
	if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)//ÄÖÖÓÖĞ¶Ï
	{
		printf("alarm .\r\n");
	} 				  								 

	RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW|RTC_IT_ALR);		//ÇåÄÖÖÓÖĞ¶Ï
		  	    						 	   	 
}

////
//void RTC_Init(void)
//{

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP | RCC_APB1Periph_PWR , ENABLE);
//	PWR_BackupAccessCmd(ENABLE);//???????,??????????


//	if(BKP_ReadBackupRegister(BKP_DR1) != 0xAA00)//Vbat?VDD??
//	{
//		BKP_DeInit();  //??????,?BKP?????
//		RCC_LSICmd(ENABLE);//????????LSI
//		while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == 0) 
//		{
//		}


//		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
//		//RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//????????LSE?RTC???

//		RCC_RTCCLKCmd(ENABLE);//????RTC??


//		RTC_WaitForLastTask();//??????RTC????(??,?????RTC?????,????????????)
//		RTC_WaitForSynchro();//???APB1???,???RTC???


//		///////////////// ////(???????)////////////////////////////

//		RTC_EnterConfigMode();//??RTC??
//		RTC_WaitForLastTask();//??????RTC????


//		RTC_SetPrescaler(40000);//????1HZ
//		RTC_WaitForLastTask();//??????RTC????

//		RTC_ITConfig(RTC_IT_ALR , DISABLE);//??????.  ???RTC_IT_SEC

//		RTC_WaitForLastTask();//??????RTC????



//		RTC_WaitForLastTask();//??????RTC????

//		RTC_ExitConfigMode(); //??RTC????


//		BKP_WriteBackupRegister(BKP_DR1, 0xAA00);//???????????

//	}

//	else//????????????
//	{

//		RTC_WaitForSynchro();//???APB1???,???RTC???

//		RTC_EnterConfigMode();//??RTC??

//		RTC_ITConfig(RTC_IT_ALR, DISABLE);

//		RTC_WaitForLastTask();//??????RTC????

//		RTC_ExitConfigMode(); //??RTC????
//		RTC_WaitForLastTask();//??????RTC????

//	}
//  return;
//}

/*******************************************************************************
* Function Name  : Time_Display
* Description    : Printf Time
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Time_Display(void)
{
   struct tm time;
   time = Time_GetCalendarTime();
   printf("Time: %d-%d-%d   %02d:%02d:%02d \r\n", time.tm_year, \
                   time.tm_mon+1, time.tm_mday,\
                   time.tm_hour, time.tm_min, time.tm_sec);
}


