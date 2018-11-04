/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "stm320518_eval.h"
#include "common.h"
#include "udp_proto.h"
#include "rtc.h"
#include "flash.h"
#include "config.h"
#include "dcmi_ov2640.h"
#include <stdlib.h>
#include "ota.h"
#include "flash.h"

#define DEBUG_BOOTLOADER

int main(void)
{
	
	int val;
	
	//引导程序，以最高的效率和最低的功耗完成执行任务
	unsigned char * __config_hdr;
	
	enable_iwdg(); //引导程序务必打开开门狗
	
	__config_hdr = (unsigned char *)CONFIG_DATA_ADDR;
	
	if ((__config_hdr[0] == 0xFA) && (__config_hdr[1] == 0xFB) && (__config_hdr[2] == 0xFC))
	{
		switch(__config_hdr[3])
		{
			case 0:
				goto RUN_APP;
				break;
			case 1:
				goto UPDATE_APP;
				break;
			case 2:
				break;
			default:
				break;
		}
		//
	}else{
		goto RUN_APP;
	}
	
	UPDATE_APP:
	SysTick_Config(SystemCoreClock / 100);
	debugport_init();
	
	
	RUN_APP:
	
	#ifdef DEBUG_BOOTLOADER
	SysTick_Config(SystemCoreClock / 100);
	debugport_init();
	#endif
	
	#ifdef DEBUG_BOOTLOADER
	printf("JumpToApp %08x \r\n" , APPLICATION_ADDRESS);
	#endif
	
	val = JumpToApp(APPLICATION_ADDRESS);
	
	printf("App Ret %d \r\n" , val);
	
	return 0;
}

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(EVAL_COM1, (uint8_t) ch);

  /* Loop until transmit data register is empty */
  while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TXE) == RESET)
  {}

  return ch;
}



#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
