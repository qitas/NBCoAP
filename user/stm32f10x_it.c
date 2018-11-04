/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    10/15/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "utimer.h"
#include "serialport.h"
#include "common.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	extern void led_routing_func(void);
	//led_routing_func();
	systick_handle();
	
}


unsigned char __debug_uart_flag = 0;
unsigned char *__debug_uart_buffer = 0;
unsigned int __debug_uart_buffer_index = 0;
#include "common.h"
#include "mem.h"

void start_uart_debug(void)
{
	#ifdef ENABLE_UART_DEBUG
	__debug_uart_flag = 1;
	__debug_uart_buffer = alloc_mem(__FILE__,__LINE__,1024);
	__debug_uart_buffer_index = 0;
	__debug_uart_buffer[0] = 0x0;
	#endif
}

void stop_uart_debug(void)
{
	#ifdef ENABLE_UART_DEBUG
	__debug_uart_flag = 0;
	printf("\r\n\r\n\r\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\r\n");
	printf("MODEM_UART DEBUG STR:[%s]\r\n",__debug_uart_buffer);
	debug_buf("MODEM_UART DEBUG HEX ",__debug_uart_buffer,__debug_uart_buffer_index);
	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\r\n\r\n\r\n");
	free_mem(__FILE__,__LINE__,__debug_uart_buffer);
	#endif
	//
}

void USART2_IRQHandler(void)
{
	
	//Ω” ’ ˝æ›
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		if (uart2_rx_buffer_index < UART2_RX_BUF_LEN)
		{
			//unsigned char dat = USART_ReceiveData(USART2);
			uart2_rx_buffer[uart2_rx_buffer_index++] = USART_ReceiveData(USART2);
			#if 1
			#ifdef ENABLE_UART_DEBUG
			if (__debug_uart_flag == 1)
			{
				__debug_uart_buffer[__debug_uart_buffer_index ++] = uart2_rx_buffer[uart2_rx_buffer_index-1];
			}
			#endif
			#endif
		}else{
			USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
			USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
			USART_Cmd(USART2, DISABLE);
			USART_Cmd(USART2, ENABLE);//Í1?¸'??˙1
		}
		
		
		
  }else{
		USART_ClearITPendingBit(USART2, USART_IT_ORE);
	}
	//»Áπ˚ «“Á≥ˆ÷–∂œ£¨‘Ú«Â≥˝“Á≥ˆ
	
	
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
