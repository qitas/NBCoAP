#include "common.h"
#include "at.h"
#include "adc.h"
#include "mainloop.h"
#include "sccb.h"
#include "flash.h"
#include "config.h"
#include "mem.h"
#include "dcmi_ov2640.h"
#include <stdio.h>

static unsigned int random;

static unsigned int auth_flag = 0; //
static unsigned int systick_hdl_cnt = 0;

void _DISABLE_DEBUG_FUNC(void)
{
	auth_flag = 1;
}

extern int ___at_cmd_wait(char *at , int(*at_cb)(char*resp,int resplen) , int(*at_wait)(char*resp,int resplen) , int timeout);

extern void STick_Handler(void);
extern unsigned char _sccbw(unsigned char reg,unsigned char data);
extern unsigned char _sccbr(unsigned char reg);
extern unsigned int _RTC_GetCounter(void);
extern void __uart_flow(void);
extern void __write_img_2_flash(int index , unsigned char *buffer , int buflen);
	
extern unsigned int system_second;

static unsigned int __reg_buf[16];

static void init_reg(void)
{
	int i=0;
	static unsigned char init_flag = 0;
	if (init_flag == 0)
	{
		init_flag = 1;
		
		__reg_buf[__SCCB_REGW] = (unsigned int)_sccbw;									//SCCB
		__reg_buf[__SCCB_REGR] = (unsigned int)_sccbr;
		__reg_buf[__SYS_RC] = (unsigned int)_RTC_GetCounter;
		__reg_buf[__UART_FLOW] = (unsigned int)__uart_flow;
		__reg_buf[__ST] = (unsigned int )(&system_second);
		__reg_buf[__WRITE2FLASH] = (unsigned int )__write_img_2_flash;
		__reg_buf[__RCFG] = (unsigned int )read_config;
		__reg_buf[__WCFG] = (unsigned int )write_config;
		
		//DEBUG_VALUE(__reg_buf[__SCCB_REGW]);
		
		//¼ì²éÐÅÏ¢ÊÇ·ñÕýÈ·£¬Èç¹û²»ÕýÈ·Ôò½øÐÐÉÕÂ¼
		for(i=0;i<sizeof(__reg_buf);i++)
		{
			if (__reg_buf[i] != ((unsigned int*)REG_ADDR)[i])
			{
				FLASH_ProgramStart(REG_ADDR,REG_SIZE);
				FLASH_AppendBuffer((unsigned char*)__reg_buf,sizeof(__reg_buf));
				FLASH_AppendEnd();
				FLASH_ProgramDone();
				
				break;
			}
		}
		
		
		//write to flash
		
		
		#if 0
		void FLASH_ProgramStart(u32 addr , u32 size);
		u32 FLASH_AppendOneByte(u8 Data);
		u32 FLASH_AppendBuffer(u8 *Data , u32 size);
		void FLASH_AppendEnd(void);
		u32 FLASH_WriteBank(u8 *pData, u32 addr, u16 size);
		void FLASH_ProgramDone(void);
		#endif
		
	}
		
}
//extern void SysTick_Handler(void);
void SysTick_Handler(void)
{
	systick_handle();
	init_reg();
	STick_Handler();
	
	#if 0
	if (mdata.status > MODEM_GPRS_MIPOPEN_SUCCESS)
	{
		systick_hdl_cnt ++;
		if (systick_hdl_cnt >= (100)*10)
		{
			if (auth_flag<=0)
			{
				printf("\r\nT\r\n");
				for(;;){}
			};
		}
	}
	#endif
}


void EXTI4_15_IRQHandler(void)
{
	#define READ_VSYNC			(GPIOB->IDR & GPIO_Pin_8)  //GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)			//GPIO_Pin_8	????????
	#define READ_PICLK			(GPIOB->IDR & GPIO_Pin_15) //GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)		//1			//GPIO_Pin_15 ?????GPIO_Pin_15
	#define READ_HREF				(GPIOC->IDR & GPIO_Pin_13) //GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)		//1			//GPIO_Pin_15 ?????GPIO_Pin_15

	#if 0
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource8);		//VSYNC
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource15);	//PCLK
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);	//HREF
	#endif
	
	if ((READ_PICLK != 0) && (READ_HREF != 0))
	{
		JpegBuffer[JpegDataCnt] = (unsigned char)(GPIOB->IDR);
		if (JpegDataCnt < (JPEG_BUFFER_LENGTH - 4))
		{
			JpegDataCnt ++ ;
		}
	}
	
	EXTI_ClearITPendingBit(EXTI_Line15|EXTI_Line8|EXTI_Line13);
	
}


#if 1

extern unsigned char _sccbw(unsigned char reg,unsigned char data);
extern unsigned char _sccbr(unsigned char reg);


void system_clk_config_HSI(void)
{
	RCC_DeInit();
	RCC_HSICmd(ENABLE);
	/* enable HSI */
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)
	{}
	
	/* Enable Prefetch Buffer */
	FLASH_PrefetchBufferCmd(ENABLE);
	/* set Flash Latency, 0-24M: FLASH_Latency_0, 24M < sysclk <48M: FLASH_Latency_1*/
	FLASH_SetLatency(FLASH_Latency_1);
	
	/* Configures the AHB clock (HCLK)  */
	RCC_HCLKConfig(RCC_CFGR_HPRE_DIV1);
	
	/* Configures the APB clock (PCLK) */
	RCC_PCLKConfig(RCC_CFGR_PPRE_DIV1);
	
	/* Configures PLL source, (8M/2) * 16 = 64M */
	RCC_PLLConfig(RCC_CFGR_PLLSRC_HSI_Div2, RCC_CFGR_PLLMULL16); 
	RCC_PLLCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
	{}
	
	/* Configures system clock source */
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
  /* Wait till PLL is used as system clock source */
  while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL)
  {}
		
	
		
}

void __uart_flow(void)
{
		USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
		USART_Cmd(USART2, DISABLE);
		USART_Cmd(USART2, ENABLE);//ê1?ü'??ú1
}

void __write_img_2_flash(int index , unsigned char *buffer , int buflen)
{
	//Êý¾Ý½á¹¹:
	//0xFA,0XFB,0XFC,0XFD,[LENGTH],[DATA]
	
	extern unsigned int ___paizhaoshijian;
	//
	unsigned int program_address;
	unsigned char hdr[] = {0xFA,0xFB,0xFC,0xFD};
	
	unsigned int paizhaotime = ___paizhaoshijian;//RTC_GetCounter();
	
	
	if (buflen > (IMG_CACHE_SIZE - 8))
	{
		DEBUG_ERROR(buflen);
		return;
	}
	
	/**
	FLASH_ProgramStart(CONFIG_DATA_ADDR,CONFIG_DATA_SIZE);
	FLASH_AppendBuffer(config_raw_data,CONFNIG_RAW_DATA_SIZE);
	FLASH_AppendBuffer((unsigned char*)json,strlen(json));
	FLASH_AppendEnd();
	FLASH_ProgramDone();
	*/
	
	switch(index)
	{
		case 0:
			program_address = IMG_CACHE_ADDR_0;
			break;
		case 1:
			program_address = IMG_CACHE_ADDR_1;
			break;
		case 2:
			program_address = IMG_CACHE_ADDR_2;
			break;
		default:
			break;
			
	}
	
	FLASH_ProgramStart(program_address,IMG_CACHE_SIZE);
	
	FLASH_AppendBuffer(hdr,sizeof(hdr)); // paizhaotime
	FLASH_AppendBuffer((unsigned char*)(&buflen),sizeof(buflen));
	FLASH_AppendBuffer((unsigned char*)(&paizhaotime),sizeof(paizhaotime));
	FLASH_AppendBuffer(buffer,buflen);
	FLASH_AppendEnd();
	FLASH_ProgramDone();
	
}

//sccb private



#endif

