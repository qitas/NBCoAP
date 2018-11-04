#include "wd.h"
#include <stm32f10x_iwdg.h>

void watch_dog_config(void)
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

void feed_watchdog(void)
{
	IWDG_ReloadCounter();
	//
}
