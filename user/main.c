
#include <stdio.h>
#include "ov2640api.h"
#include "BC26/BC26.h"
#include "BASE64/cbase64.h"
#include "JSON/cjson.h"
#include "common.h"
#include "main.h"


static char MYMICCID[42] = "99900000000000000000";
static char IMEI[] = "866971030432384";// "866971030432384";
static char RSSI[] = "00000000000000000000000000000000";

static int make_json_data(char *oustr)
{
	
	char * p = 0;
	cJSON * pJsonRoot = NULL;
	char tmpstr[128];
 
	pJsonRoot = cJSON_CreateObject();
	if(NULL == pJsonRoot){return -1;}
	uint32_t RTC_buff = RTC_GetCounter();	
	cJSON_AddNumberToObject(pJsonRoot, "VERSION", 1);
	cJSON_AddNumberToObject(pJsonRoot, "TIME SEED ", RTC_buff);
	cJSON_AddStringToObject(pJsonRoot, "DEVID",IMEI);
	cJSON_AddStringToObject(pJsonRoot, "MICCID", MYMICCID);
	cJSON_AddStringToObject(pJsonRoot, "RSSI", RSSI);
	cJSON_AddStringToObject(pJsonRoot, "INFO","2600KPa");
	cJSON_AddStringToObject(pJsonRoot, "NET", "China Telecom");
	cJSON_AddStringToObject(pJsonRoot, "STATUS", "live");
	cJSON_AddStringToObject(pJsonRoot, "Auth ","Qitas");
	cJSON_AddStringToObject(pJsonRoot, "TIME","2018.11.14");	

	snprintf(tmpstr,64,"Current Battery %d mV",read_vdd_voltage());
	cJSON_AddStringToObject(pJsonRoot, "BODY", tmpstr );
	
	p = cJSON_Print(pJsonRoot);
	
	if(NULL == p)
	{
		cJSON_Delete(pJsonRoot);
		return -1;
	}
	cJSON_Delete(pJsonRoot);
	
	sprintf(oustr,"%s",p);
	
	printf("JSON:%s\r\n",oustr);
	
	free(p);
	return 0;
}

static int push_data_func(unsigned char * push_data , int push_length)
{
	
	int i = 0;
	int ret;
	int ret1 = -1;
	int length;
	int recvlen;

	
	char *sbuffer; //存储二进制裸数据
	char *hexbuffer;
	
	sbuffer = malloc(1024 + 32);
	hexbuffer = malloc(1024 + 32);
	
	conv_hex_2_string((unsigned char*)push_data,push_length,(char*)hexbuffer);

	sprintf((char*)sbuffer,"AT+QLWDATASEND=19,0,0,512,");
	strcat((char*)sbuffer,(char*)hexbuffer);
	
	if(push_length < 512)
	{
		for(i=0;i<(512 - push_length);i++)
		{
			strcat(sbuffer,"00");
		}
	}
	strcat(sbuffer,",0x0000\r\n");
	
	printf("SEND TO BC26 STR: %s ]\r\n",sbuffer);
	
	uart_data_write(sbuffer, strlen(sbuffer), 0);
	
	free(sbuffer);
	free(hexbuffer);
	
	return ret;
}

/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
* Attention		   : None
*******************************************************************************/
int main(void)
{
	NVIC_SetVectorTable(NVIC_VectTab_FLASH,(0x8000000+4));
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	SysTick_Config(SystemCoreClock / 100);
	
	init_uart1();
	init_uart2();
	RTC_Init();
	SET_BOOTLOADER_STATUS(2);
	WKUP_Pin_Init();
	
	init_utimer();
	init_task();
	init_mem();
	init_uart2_buffer();
	
	modem_poweron();	
	vdd_5v_out(1);
	vdd_3v3_out(1);
	led0_on();
	uint32_t startcnt = RTC_GetCounter();
	//utimer_sleep(1000);
	while(neul_bc26_get_netstat()<0)
	{
		utimer_sleep(400);
	}//等待连接上网络
	
	{
		/*
		 * 分配内存
		 */
		#define RECV_BUF_LEN 1024
		char *recvbuf = malloc(RECV_BUF_LEN);
		char *atbuf = malloc(1024);
		char *jsonbuf = malloc(512);
		
		int ret=0,PTR=0;
		
		
		
		/*
		 * 发送ATI指令
		 */
//		memset(recvbuf,0x0,RECV_BUF_LEN);
//		uart_data_write("ATI\r\n", strlen("ATI\r\n"), 0);
//		uart_data_read(recvbuf, RECV_BUF_LEN, 0, 200);
//		
//		memset(recvbuf,0x0,RECV_BUF_LEN);
//		uart_data_write("AT+QBAND?\r\n", strlen("AT+QBAND?\r\n"), 0);
//		uart_data_read(recvbuf, RECV_BUF_LEN, 0, 200);
				
		/*
		 * 打开PSM
		 */
		memset(recvbuf,0x0,RECV_BUF_LEN);
		uart_data_write("AT+CPSMS=1\r\n", strlen("AT+CPSMS=1\r\n"), 0);
		uart_data_read(recvbuf, RECV_BUF_LEN, 0, 200);
		
		/*
		 * 关闭回显
		 */
		memset(recvbuf,0x0,RECV_BUF_LEN);
		uart_data_write("ATE0\r\n", strlen("ATE0\r\n"), 0);
		uart_data_read(recvbuf, RECV_BUF_LEN, 0, 200);
		/*
		 * 获取设备IP
		 */
//		memset(recvbuf,0x0,RECV_BUF_LEN);
//		uart_data_write("AT+CGPADDR=1\r\n", strlen("AT+CGPADDR=1\r\n"), 0);
//		uart_data_read(recvbuf, RECV_BUF_LEN, 0, 200);	
//		if(strstr(recvbuf,"OK"))
//		{
//			memcpy(IMEI,uart2_rx_buffer+9,strlen(IMEI));	
//			printf("IMEI : %s\r\n",IMEI);
//		}		
//		
		/*
		 * 获取设备IMEI
		 */
		
		memset(recvbuf,0x0,RECV_BUF_LEN);
		uart_data_write("AT+CGSN=1\r\n", strlen("AT+CGSN=1\r\n"), 0);
		ret = uart_data_read(recvbuf, RECV_BUF_LEN, 0, 200);	
		if(strstr(recvbuf,"OK"))
		{
			memcpy(IMEI,uart2_rx_buffer+9,strlen(IMEI));	
			printf("IMEI : %s\r\n",IMEI);
		}		

		memset(recvbuf,0x0,RECV_BUF_LEN);
		uart_data_write("AT*MICCID\r\n", strlen("AT*MICCID\r\n"), 0);
		uart_data_read(recvbuf, RECV_BUF_LEN, 0, 200);
		{
			char * __tmp = strstr(recvbuf,"OK");
			if (__tmp > 0)
			{
				__tmp -= 24;
				int i=0;
				for(i=0;i<20;i++)
				{
					MYMICCID[i] = __tmp[i];
				}
				MYMICCID[20] = 0x0;
				printf("MYMICCID : [%s\r\n",MYMICCID);
			}
		}

		/*
		 * 链接电信云服务器
		 */
		{
			char *tmpstr;
			tmpstr = (char*)malloc(128);
			snprintf(tmpstr,128,"%s","AT+QLWSERV=\"180.101.147.115\",5683\r\n");
			memset(recvbuf,0x0,RECV_BUF_LEN);
			uart_data_write(tmpstr,strlen(tmpstr),0);
			uart_data_read(recvbuf, RECV_BUF_LEN, 0, 200);
			free(tmpstr);
		}
		
		{
			char *tmpstr;
			tmpstr = (char*)malloc(128);
			snprintf(tmpstr,128,"AT+QLWCONF=\"%s\"\r\n",IMEI);
			memset(recvbuf,0x0,RECV_BUF_LEN);
			uart_data_write(tmpstr,strlen(tmpstr),0);
			uart_data_read(recvbuf, RECV_BUF_LEN, 0, 200);
			free(tmpstr);
		}
		
		{
			//AT+QLWCONF?\r\n
//			char *tmpstr;
//			tmpstr = (char*)malloc(128);
//			snprintf(tmpstr,128,"%s","AT+QLWCONF?\r\n");
//			memset(recvbuf,0x0,RECV_BUF_LEN);
//			uart_data_write(tmpstr,strlen(tmpstr),0);
//			uart_data_read(recvbuf, RECV_BUF_LEN, 0, 200);
//			free(tmpstr);
		}
		{
			char *tmpstr;
			tmpstr = (char*)malloc(128);
			snprintf(tmpstr,128,"%s","AT+QLWADDOBJ=19,0,1,\"0\"\r\n");
			memset(recvbuf,0x0,RECV_BUF_LEN);
			uart_data_write(tmpstr,strlen(tmpstr),0);
			uart_data_read(recvbuf, RECV_BUF_LEN, 0, 200);
			free(tmpstr);
		}	
		{
			char *tmpstr;
			tmpstr = (char*)malloc(128);
			snprintf(tmpstr,128,"%s","AT+QLWADDOBJ=19,1,1,\"0\"\r\n");
			memset(recvbuf,0x0,RECV_BUF_LEN);
			uart_data_write(tmpstr,strlen(tmpstr),0);
			uart_data_read(recvbuf, RECV_BUF_LEN, 0, 200);
			free(tmpstr);
		}	

		{
			char *tmpstr;
			tmpstr = (char*)malloc(128);
			snprintf(tmpstr,128,"%s","AT+QLWOPEN=0\r\n");
			memset(recvbuf,0x0,RECV_BUF_LEN);
			uart_data_write(tmpstr,strlen(tmpstr),0);
			uart_data_read(recvbuf, RECV_BUF_LEN, 0, 200);		
			utimer_sleep(2000);			
			free(tmpstr);			
		}	
			
		{
			char *tmpstr;
			tmpstr = (char*)malloc(128);
			snprintf(tmpstr,128,"%s","AT+QLWCFG=\"dataformat\",0,1\r\n");
			memset(recvbuf,0x0,RECV_BUF_LEN);
			uart_data_write(tmpstr,strlen(tmpstr),0);
			uart_data_read(recvbuf, RECV_BUF_LEN, 0, 200);	
			free(tmpstr);
		}
		int cnt=0;
		do{
				/*
				 * 获取信号值CSQ
				 */
				memset(recvbuf,0x0,RECV_BUF_LEN);
				uart_data_write("AT+CSQ\r\n", strlen("AT+CSQ\r\n"), 0);
				ret = uart_data_read(recvbuf, RECV_BUF_LEN, 0, 200);	
				if(strstr(recvbuf,"OK"))
				{
					memcpy(RSSI,uart2_rx_buffer+8,ret-16);	
					PTR=ret-16;
					RSSI[PTR++] =' ';
				}
				cnt++;
				if(cnt>1) utimer_sleep(1000);
			}while(ret==21 && cnt<5);
		    //if(cnt>4) RTC_SetAlarm(RTC_GetCounter()+ 60);
			/*
			 * 获取信号值CESQ
			 */
			memset(recvbuf,0x0,RECV_BUF_LEN);
			uart_data_write("AT+CESQ\r\n", strlen("AT+CESQ\r\n"), 0);
			ret = uart_data_read(recvbuf, RECV_BUF_LEN, 0, 200);
			
			if(strstr(recvbuf,"OK"))
			{	
				memcpy(RSSI+PTR,uart2_rx_buffer+9,ret-17);		
				PTR+=ret-17;
				memset(RSSI+PTR,'.',27-PTR);					
			}
			char *p;
			while((p = strchr(RSSI,','))!=NULL)
			{
				*p=':';
			}
			printf("RSSI: %s\r\n",RSSI);	
			
		
		
		/*
		 * 发送数据
		 */		
		{
			char *tmpstr;
			char tmp[4];
			tmpstr = (char*)malloc(128);
			uint32_t endcnt = RTC_GetCounter() - startcnt;
			sprintf(tmp,"%04X",endcnt);
			printf("time cnt: %s\r\n",tmp);	
			//strcpy(RSSI+28,tmp);
			//strcat(RSSI,tmp);
			RSSI[30]=tmp[2];
			RSSI[31]=tmp[3];
//			RSSI[33]=tmp[2];
//			RSSI[34]=tmp[3];
			sprintf((char*)tmpstr,"AT+QLWDATASEND=19,0,0,32,%32s,0x0000\r\n", RSSI);
			uart_data_write(tmpstr,strlen(tmpstr),0);			
			memset(recvbuf,0x0,RECV_BUF_LEN);
			uart_data_read(recvbuf, RECV_BUF_LEN, 0, 200);
			free(tmpstr);
		}
		
//		{
//			char *tmp = malloc(512);
//			make_json_data(tmp);
//			memset(recvbuf,0x0,RECV_BUF_LEN);
//			push_data_func((unsigned char*)tmp,strlen(tmp));
//			
//			for(;;)
//			{
//				uart_data_read(recvbuf, RECV_BUF_LEN, 0, 2000);
//			}
//			free(tmp);
//		}


		
		printf("连接建立完毕，waiting 1 second...\r\n");
		utimer_sleep(1000);
		
//		memset(recvbuf,0x0,RECV_BUF_LEN);
//		uart_data_write("AT+CPSMS=0\r\n", strlen("AT+CPSMS=0\r\n"), 0);
//		uart_data_read(recvbuf, RECV_BUF_LEN, 0, 200);						
		/*
		释放内存
		*/
		free(recvbuf);
		free(atbuf);
		free(jsonbuf);
	}
	RTC_SetAlarm(RTC_GetCounter()+ 116);
	printf("Sys_Enter_Standby CurrentTim %d\r\n",RTC_GetCounter());
	modem_poweroff();
	led0_off();
	//进入休眠
	utimer_sleep(20);
	Sys_Enter_Standby();
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
	
  USART_SendData(USART1, (uint8_t) ch);
  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
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

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

