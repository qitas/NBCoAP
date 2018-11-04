#include "signalled.h"
#include "mem.h"
#include <stm32f0xx.h>

void init_sig_led_gpio(void)
{
	GPIO_InitTypeDef        GPIO_InitStructure;
	
	/* GPIOC Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

  /* Configure PC10 and PC11 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_10);
}

void sig_led_routing(unsigned char ms)
{
	if (mem > 0)
	{
		mem->sig_led_counter += ms;
		
		if (mem->sig_led_counter > 500)
		{
			mem->sig_led_counter = 0;
			
			if (mem->sig_led_flag == 0)
			{
				GPIO_ResetBits(GPIOB,GPIO_Pin_10);
				mem->sig_led_flag = 1;
			}else{
				GPIO_SetBits(GPIOB,GPIO_Pin_10);
				mem->sig_led_flag = 0;
			}
			
		}
		
		
	}
}