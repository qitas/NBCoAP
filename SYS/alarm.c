#include "alarm.h"

#include <stm32f0xx.h>

void init_alarm_gpio(void)
{
	GPIO_InitTypeDef        GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	/* GPIOC Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  /* Configure PC10 and PC11 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = 0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = 0;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, GPIO_PinSource7);	//中断线0连接GPIOA.0

  EXTI_InitStructure.EXTI_Line = EXTI_Line0;	//设置按键所有的外部线路
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//设外外部中断模式:EXTI线路为中断请求
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //上升沿触发
 	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	// 初始化外部中断

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x03;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure); 
	
}

#if 0

//PA0 WKUP唤醒初始化
void WKUP_Pin_Init(void)
{	
  GPIO_InitTypeDef  GPIO_InitStructure;  		  
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_AHBPeriph_GPIOA , ENABLE);//使能GPIOA和复用功能时钟

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;	 //PA.0
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN;//上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//初始化IO
	
    //使用外部中断方式
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, GPIO_PinSource0);	//中断线0连接GPIOA.0

  EXTI_InitStructure.EXTI_Line = EXTI_Line0;	//设置按键所有的外部线路
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//设外外部中断模式:EXTI线路为中断请求
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //上升沿触发
 	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	// 初始化外部中断

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x03;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure); 
	
}

void ledrouttjdsjfdsajfks

#endif
