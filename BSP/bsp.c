#include "bsp.h"
#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_adc.h>
#include <stm32f10x_dma.h>
#include <misc.h>

void gpio_all_ain(void)
{
}

/*******************************************************************************
* Function Name  : USART_Configuration
* Description    : Configure USART1 
* Input          : None
* Output         : None
* Return         : None
* Attention		 	: None
*******************************************************************************/
void init_uart1(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure; 

  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);
  /*
  *  USART1_TX -> PA9 , USART1_RX ->	PA10
  */				
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);		   

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART1, &USART_InitStructure); 
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  //USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
  USART_ClearFlag(USART1,USART_FLAG_TC);
  USART_Cmd(USART1, ENABLE);	
}

void init_uart2(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA ,ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2 ,ENABLE);
  /*
  *  USART1_TX -> PA2 , USART1_RX ->	PA3
  */				
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);		   

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART2, &USART_InitStructure); 
  USART_Cmd(USART2, ENABLE);
	
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	//USART_ITConfig(USART2, USART_IT_TXE, ENABLE);   
 
	USART_Cmd(USART2, ENABLE);//ê1?ü'??ú1
}

void uart1_putchar(unsigned char data)
{
	USART_SendData(USART1, (uint8_t) data);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}
}

void uart2_putchar(unsigned char data)
{
	USART_SendData(USART2, (uint8_t) data);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}
}

void modem_poweron(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

  MODEM_POWER_RCC_TYPE( MODEM_POEWR_RCC , ENABLE); 						 				 
  GPIO_InitStructure.GPIO_Pin =  MODEM_POEWR_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 
  GPIO_Init(MODEM_POEWR_GPIO, &GPIO_InitStructure);
	                             
	GPIO_ResetBits(MODEM_POEWR_GPIO,MODEM_POEWR_PIN);
}
void modem_poweroff(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

  MODEM_POWER_RCC_TYPE( MODEM_POEWR_RCC , ENABLE); 						 
  /**
  *  LED1 -> PF6 , LED2 -> PF7 , LED3 -> PF8 , LED4 -> PF9
  */					 
  GPIO_InitStructure.GPIO_Pin =  MODEM_POEWR_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 
  GPIO_Init(MODEM_POEWR_GPIO, &GPIO_InitStructure);
	
	GPIO_ResetBits(MODEM_POEWR_GPIO,MODEM_POEWR_PIN);
}

void modem_reseton(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

  MODEM_RST_RCC_TYPE( MODEM_RST_RCC , ENABLE); 						 
  /**
  *  LED1 -> PF6 , LED2 -> PF7 , LED3 -> PF8 , LED4 -> PF9
  */					 
  GPIO_InitStructure.GPIO_Pin =  MODEM_RST_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 
  GPIO_Init(MODEM_RST_GPIO, &GPIO_InitStructure);
	                             
	GPIO_ResetBits(MODEM_RST_GPIO,MODEM_RST_PIN);
	//
}
void modem_resetoff(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

  MODEM_POWER_RCC_TYPE( MODEM_RST_RCC , ENABLE); 						 
  /**
  *  LED1 -> PF6 , LED2 -> PF7 , LED3 -> PF8 , LED4 -> PF9
  */					 
  GPIO_InitStructure.GPIO_Pin =  MODEM_RST_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 
  GPIO_Init(MODEM_RST_GPIO, &GPIO_InitStructure);
	
	GPIO_ResetBits(MODEM_RST_GPIO,MODEM_RST_PIN);
	//
}

void ov_poweron(void)
{
}
void ov_poweroff(void)
{
}

void led0_on(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

  LED0_RCC_TYPE( LED0_RCC , ENABLE); 						 
  /**
  *  LED1 -> PF6 , LED2 -> PF7 , LED3 -> PF8 , LED4 -> PF9
  */					 
  GPIO_InitStructure.GPIO_Pin =  LED0_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(LED0_GPIO, &GPIO_InitStructure);
	
	GPIO_SetBits(LED0_GPIO,LED0_PIN);
	
}
void led0_off(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

  LED0_RCC_TYPE( LED0_RCC , ENABLE); 						 
  /**
  *  LED1 -> PF6 , LED2 -> PF7 , LED3 -> PF8 , LED4 -> PF9
  */					 
  GPIO_InitStructure.GPIO_Pin =  LED0_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(LED0_GPIO, &GPIO_InitStructure);

	GPIO_ResetBits(LED0_GPIO,LED0_PIN);
}

void usben_on(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

  USBEN_RCC_TYPE( USBEN_RCC , ENABLE); 						 
  /**
  *  LED1 -> PF6 , LED2 -> PF7 , LED3 -> PF8 , LED4 -> PF9
  */					 
  GPIO_InitStructure.GPIO_Pin =  USBEN_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(USBEN_GPIO, &GPIO_InitStructure);
	
	GPIO_SetBits(USBEN_GPIO,USBEN_PIN);
	
}
void usben_off(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

  USBEN_RCC_TYPE( USBEN_RCC , ENABLE); 						 
  /**
  *  LED1 -> PF6 , LED2 -> PF7 , LED3 -> PF8 , LED4 -> PF9
  */					 
  GPIO_InitStructure.GPIO_Pin =  USBEN_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 
  GPIO_Init(USBEN_GPIO, &GPIO_InitStructure);
	
	//GPIO_ResetBits(LED0_GPIO,LED0_PIN);
}

void vdd_5v_out(unsigned char on)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

  V50_RCC_TYPE( V50_RCC , ENABLE); 						 
  /**
  *  LED1 -> PF6 , LED2 -> PF7 , LED3 -> PF8 , LED4 -> PF9
  */					 
  GPIO_InitStructure.GPIO_Pin =  V50_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(V50_GPIO, &GPIO_InitStructure);
	
	if(!on)
		GPIO_SetBits(V50_GPIO,V50_PIN);
	else
		GPIO_ResetBits(V50_GPIO,V50_PIN);
	
}
void vdd_3v3_out(unsigned char on)
{
		GPIO_InitTypeDef GPIO_InitStructure;

  V33_RCC_TYPE( V33_RCC , ENABLE); 						 
  /**
  *  LED1 -> PF6 , LED2 -> PF7 , LED3 -> PF8 , LED4 -> PF9
  */					 
  GPIO_InitStructure.GPIO_Pin =  V33_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(V33_GPIO, &GPIO_InitStructure);
	
	if(!on)
		GPIO_SetBits(V33_GPIO,V33_PIN);
	else
		GPIO_ResetBits(V33_GPIO,V33_PIN);
}


void bc26_powerpin(unsigned char status)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	memset(&GPIO_InitStructure,0x0,sizeof(GPIO_InitStructure));
	
  RCC_APB2PeriphClockCmd( USBEN_RCC , ENABLE); 						 
  /**
  *  LED1 -> PF6 , LED2 -> PF7 , LED3 -> PF8 , LED4 -> PF9
  */
	
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	
	return ;
	
	if (status == 0)
	{
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	}else{
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	}
	
}

void led0_fanzhuan(void)
{
	LED0_GPIO->ODR ^= LED0_PIN;
}

void rtc_init(void)
{
	//
}
void watchdog_init(void)
{
	//
}

void led_ctrl(unsigned char index , unsigned char status)
{

	//
}

void setfrq(unsigned char mod)
{
	//
}

int read_usb_status(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

  CHECK_USB_RCC_TYPE( CHECK_USB_RCC , ENABLE); 						 
  /**
  *  LED1 -> PF6 , LED2 -> PF7 , LED3 -> PF8 , LED4 -> PF9
  */					 
  GPIO_InitStructure.GPIO_Pin =  CHECK_USB_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
  GPIO_Init(CHECK_USB_GPIO, &GPIO_InitStructure);
	
	//printf("PIN 11: %d\r\n",GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11));
	//printf("PIN 12: %d\r\n",GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12));
	
	return GPIO_ReadInputDataBit(CHECK_USB_GPIO,CHECK_USB_PIN);
	//
}




