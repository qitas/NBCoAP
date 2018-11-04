#include "wwdg.h"
#include <stm32f0xx.h>

void enable_iwdg(void)
{
	/* Enable write access to IWDG_PR and IWDG_RLR registers */
 	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);	
	/* 
		IWDG counter clock: 32KHz(LSI) / 256 = 0.125KHz  == 125Hz 8ms
												40         / 256 = 							156Hz 6ms
	*/
	IWDG_SetPrescaler(IWDG_Prescaler_256);	
	/* Set counter reload value T=(fn(????)/4)*0.1*RLR(?????)  */
	// 0 ~ 4095
	// 166/125 1s
	IWDG_SetReload(0xFFF);	  //24s
	/* Reload IWDG counter?? */
	IWDG_ReloadCounter();	
	IWDG_Enable();
}

void feed_iwdg(void)
{
	IWDG_ReloadCounter();
	//
}

char __feeddog_flag = 0;
void __indog_10ms(void)
{
	static unsigned int __10mscnt = 0;
	
	__10mscnt += 10;
	if (__10mscnt > (1000*60))
	{
		if (__feeddog_flag > 0)
			
		__10mscnt = 0;
	}
	//
}
	
