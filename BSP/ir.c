#include "ir.h"

#include <stm32f0xx.h>
#include "utimer.h"
#include "common.h"


static void TIM_ConfigPin(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
	GPIO_ResetBits(GPIOA, GPIO_Pin_11);
	
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  //GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
   // GPIO_InitStructure.GPIO_PuPd =GPIO_PuPd_DOWN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_2);
	
}


static void configpwm_ir(void)
{
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	uint16_t TimerPeriod = 0;
	uint16_t Channel1Pulse = 0, Channel2Pulse = 0, Channel3Pulse = 0, Channel4Pulse = 0;
	
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file
     */ 

  /* TIM Configuration */
  TIM_ConfigPin();
  
  /* TIM1 Configuration ---------------------------------------------------
   Generate 7 PWM signals with 4 different duty cycles:
   TIM1 input clock (TIM1CLK) is set to APB2 clock (PCLK2)    
    => TIM1CLK = PCLK2 = SystemCoreClock
   TIM1CLK = SystemCoreClock, Prescaler = 0, TIM1 counter clock = SystemCoreClock
   SystemCoreClock is set to 48 MHz for STM32F0xx devices
   
   The objective is to generate 7 PWM signal at 17.57 KHz:
     - TIM1_Period = (SystemCoreClock / 17570) - 1
   The channel 1 and channel 1N duty cycle is set to 50%
   The channel 2 and channel 2N duty cycle is set to 37.5%
   The channel 3 and channel 3N duty cycle is set to 25%
   The channel 4 duty cycle is set to 12.5%
   The Timer pulse is calculated as follows:
     - ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100
   
   Note: 
    SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f0xx.c file.
    Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
    function to update SystemCoreClock variable value. Otherwise, any configuration
    based on this variable will be incorrect. 
  ----------------------------------------------------------------------- */
  /* Compute the value to be set in ARR regiter to generate signal frequency at 17.57 Khz */
  TimerPeriod = (SystemCoreClock / 38400 ) - 1;
	
  /* Compute CCR1 value to generate a duty cycle at 50% for channel 1 and 1N */
  Channel1Pulse = (uint16_t) (((uint32_t) 5 * (TimerPeriod - 1)) / 10);

  /* TIM1 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
  
  /* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  /* Channel 1, 2,3 and 4 Configuration in PWM mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
 // TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  //TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;

  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
  
  TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

  TIM_OC4Init(TIM1, &TIM_OCInitStructure);

  /* TIM1 counter enable */
  TIM_Cmd(TIM1, ENABLE);

  /* TIM1 Main Output Enable */
  TIM_CtrlPWMOutputs(TIM1, ENABLE);


}

////////////////////////////////////////////////////////////////////////
static void PWMDISABLE(void)
{
  /* TIM1 Main Output Enable */
  TIM_CtrlPWMOutputs(TIM1, DISABLE);
}

////////////////////////////////////////////////////////////////////////
static void PWMDENABLE(void)
{
  /* TIM1 Main Output Enable */
	configpwm_ir();
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
}
/////////////////////////////////////////////////////////////////////////


static void delay_usXXX(unsigned int us)
{
	int len;
	u8 len1;
	
	for (len1 = 0; len1 < us; len1++)
		{
     for (len = 0; len < 540; len++ );
		}
}




static void TIM_Config111(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
	GPIO_SetBits(GPIOA, GPIO_Pin_11);
	
	
}


static void TIM_Config222(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
	GPIO_ResetBits(GPIOA, GPIO_Pin_11);
}


static void HONGWAI111(void)
{
	//configpwm();////////////////////////////////////////////////////////////////////////////////////
	PWMDENABLE();/////////////////////////////////////////////////////////////////////////////////
	//utimer_sleep(10);///////////////////////////////////////////////////////////////////////////////
	//utimer_sleep01ms(6);
	//utimer_sleephalfms(1);
	delay_usXXX(6);
	PWMDISABLE();//////////////////////////////////////////////////////////////////////////////////
	//utimer_sleep(10);///////////////////////////////////////////////////////////////////////////////
	//utimer_sleep01ms(17);
	//utimer_sleephalfms(1);
	TIM_Config222();
	delay_usXXX(17);
}



static void HONGWAI000(void)
{
	//configpwm();////////////////////////////////////////////////////////////////////////////////////
	PWMDENABLE();/////////////////////////////////////////////////////////////////////////////////
	//utimer_sleep(10);///////////////////////////////////////////////////////////////////////////////
	//utimer_sleep01ms(6);
	//utimer_sleephalfms(1);
	delay_usXXX(6);
	PWMDISABLE();//////////////////////////////////////////////////////////////////////////////////
	//utimer_sleep(10);///////////////////////////////////////////////////////////////////////////////
	//utimer_sleep01ms(17);
	//utimer_sleephalfms(1);
	TIM_Config222();
	delay_usXXX(6);
}


void PWMHONGWAI(void)
{
	u16 XXX=0;
	configpwm_ir();////////////////////////////////////////////////////////////////////////////////////
	
	//for(;;){};
	
	for(XXX=0;XXX<0xF;XXX++)
	{

		//configpwm();////////////////////////////////////////////////////////////////////////////////////
		PWMDENABLE();/////////////////////////////////////////////////////////////////////////////////
		//utimer_sleep(10);///////////////////////////////////////////////////////////////////////////////
		//utimer_sleep01ms(6);
		//utimer_sleephalfms(1);
		delay_usXXX(90);
		PWMDISABLE();//////////////////////////////////////////////////////////////////////////////////
		//utimer_sleep(10);///////////////////////////////////////////////////////////////////////////////
		//utimer_sleep01ms(17);
		//utimer_sleephalfms(1);
		TIM_Config222();
		delay_usXXX(45);

		//continue

		HONGWAI111();
		HONGWAI000();
		HONGWAI000();
		HONGWAI111();

		HONGWAI111();
		HONGWAI000();
		HONGWAI111();
		HONGWAI000();

		HONGWAI000();
		HONGWAI111();
		HONGWAI000();
		HONGWAI111();

		HONGWAI000();
		HONGWAI111();
		HONGWAI000();
		HONGWAI111();

		HONGWAI000();
		HONGWAI111();
		HONGWAI111();
		HONGWAI000();

		HONGWAI111();
		HONGWAI000();
		HONGWAI000();
		HONGWAI000();

		HONGWAI000();
		HONGWAI111();
		HONGWAI111();
		HONGWAI000();

		HONGWAI111();
		HONGWAI000();
		HONGWAI000();
		HONGWAI000();

	}
}













///////////////////////////////////////////////////////////////////////////////////

void init_ir_gpio(void)
{
	configpwm_ir();
	//
}

void openIR(void)
{
	__send_ir_wakeup_cmd();
}

void closeIR(void)
{
	//
}


/*******************************************************************************
*******************************************************************************/




#define TEST1_RCC				RCC_AHBPeriph_GPIOA
#define TEST1_GPIO			GPIOA
#define TEST1_PIN				GPIO_Pin_15

#define TEST3_RCC				RCC_AHBPeriph_GPIOB
#define TEST3_GPIO			GPIOB
#define TEST3_PIN				GPIO_Pin_9

#define TEST4_RCC				RCC_AHBPeriph_GPIOA
#define TEST4_GPIO			GPIOA
#define TEST4_PIN				GPIO_Pin_12

#define TEST5_RCC				RCC_AHBPeriph_GPIOA
#define TEST5_GPIO			GPIOA
#define TEST5_PIN				GPIO_Pin_5


static void init_testpoint_pin2(void)
{
	GPIO_InitTypeDef        GPIO_InitStructure;
	
	/* GPIOC Periph clock enable */
  RCC_AHBPeriphClockCmd(TEST1_RCC, ENABLE);
  GPIO_InitStructure.GPIO_Pin = TEST1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(TEST1_GPIO, &GPIO_InitStructure);
	GPIO_ResetBits(TEST1_GPIO,TEST1_PIN);
	
	RCC_AHBPeriphClockCmd(TEST3_RCC, ENABLE);
  GPIO_InitStructure.GPIO_Pin = TEST3_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(TEST3_GPIO, &GPIO_InitStructure);
	GPIO_ResetBits(TEST3_GPIO,TEST3_PIN);
	
	RCC_AHBPeriphClockCmd(TEST4_RCC, ENABLE);
  GPIO_InitStructure.GPIO_Pin = TEST4_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(TEST4_GPIO, &GPIO_InitStructure);
	GPIO_ResetBits(TEST4_GPIO,TEST4_PIN);
	
	RCC_AHBPeriphClockCmd(TEST5_RCC, ENABLE);
  GPIO_InitStructure.GPIO_Pin = TEST5_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(TEST5_GPIO, &GPIO_InitStructure);
	GPIO_ResetBits(TEST5_GPIO,TEST5_PIN);
}


//1200 == 833us

static char config_uart_pin(void)
{
	GPIO_InitTypeDef        GPIO_InitStructure;
	
	/* GPIOC Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	memset(&GPIO_InitStructure,0x0,sizeof(GPIO_InitStructure));
  /* Configure PC10 and PC11 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	return (GPIOA->IDR & GPIO_Pin_12);
	
	
}

static char config_uart_pin_out(void)
{
	GPIO_InitTypeDef        GPIO_InitStructure;
	
	/* GPIOC Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	memset(&GPIO_InitStructure,0x0,sizeof(GPIO_InitStructure));
  /* Configure PC10 and PC11 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	//return (GPIOA->IDR & GPIO_Pin_12);
	
	
}


#include <stdlib.h>

#define IRL	TIM_CtrlPWMOutputs(TIM1, ENABLE)
#define IRH	TIM_CtrlPWMOutputs(TIM1, DISABLE)

//#define IRL	TIM_CtrlPWMOutputs(TIM1, ENABLE)
//#define IRH	TIM_CtrlPWMOutputs(TIM1, DISABLE)

void _read_uart_data(void)
{
	for(;;)
	{
		if ((GPIOA->IDR & GPIO_Pin_12) == 0)
		{
			TIM_CtrlPWMOutputs(TIM1, ENABLE);
		}else{
			TIM_CtrlPWMOutputs(TIM1, DISABLE);
		}
		
	}
	
	
}

void IO_TXD(u8 Data)
{
	u8 i = 0;
	
	u8 Data2;
	u8 jiou = 0;
	Data2 = Data;
	for(i = 0; i < 8; i++)
	{
		if(Data2&0x01)
		{
		}			
		else
		{
			jiou ++;
		}
		
		Data2 = Data2>>1;
	}
	 
	
	//OI_TXD = 0;
	IRL;
	delayUS(DELAY_IR);
	for(i = 0; i < 8; i++)
	{
		if(Data&0x01)
			IRH;  
		else
			IRL;	
		
		delayUS(DELAY_IR);
		Data = Data>>1;
	}
	
	if ((jiou % 2) == 0)
	{
		IRL;
	}else{
		IRH;
	}
	delayUS(DELAY_IR);
	
	IRH;
	delayUS(DELAY_IR);
	
	
	
		
}

static void test_delay(void)
{
	//delayUS
	config_uart_pin_out();
	for(;;)
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_12);
		delayUS(10700);
		GPIO_SetBits(GPIOA,GPIO_Pin_12);
		delayUS(10700);
		//
	}
}

void __send_ir_wakeup_cmd(void)
{
	int i=0;
	unsigned char data[] = {0xFE,0xFE,0xFE,0xFE,0x68,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0x68,0x13,0x00,0xDF,0x16};
	//config_uart_pin();
	configpwm_ir();
	
	//test_delay();
	//config_uart_pin();
	//_read_uart_data();
	//config_uart_pin_out();
	
	for(i=0;i<5;i++)
	{
		
		for(i=0;i<sizeof(data);i++)
		{
			IO_TXD(data[i]);
		}
		utimer_sleep(15);
		
	}
	
}

