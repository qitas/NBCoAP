#include "at.h"
#include "modem.h"
#include "serialport.h"
#include "utimer.h"
#include "common.h"
#include "utimer.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define AT_PROCESS_DELAY __time_100ms_cnt[TIMER_100MS_AT_PROCESS_DELAY]

unsigned short gsm_signal = 0;
unsigned int GSM_LAC,GSM_CI;
float GSM_LAT,GSM_LON;

int at_cmd_wait(char *at , int(*at_cb)(char*resp,int resplen) , int(*at_wait)(char*resp,int resplen) , int timeout)
{
	int ret = -1;
	
	AT_PROCESS_DELAY = 0;
	
	clear_uart2_buffer();
	
	//只有当AT指令有效的时候才会去发送
	if (at > 0)
	{
		printf("--> [TIM:%d] %s",AT_PROCESS_DELAY,at);
		send_at(at);
	}
	
	for(;;)
	{
		if ((AT_PROCESS_DELAY * 100) > timeout)
		{
			printf("<-- %s",uart2_rx_buffer);
			ret = at_cb(uart2_rx_buffer,uart2_rx_buffer_index);
			goto exit;
		}else{
			
			if (at_wait > 0)
			{
				if (at_wait(uart2_rx_buffer,uart2_rx_buffer_index) == 1)
				{
					printf("<## %s",uart2_rx_buffer);
					ret = at_cb(uart2_rx_buffer,uart2_rx_buffer_index);
					goto exit;
				}
			}
			
		}
	}
	printf("AT TIMEOUT !\r\n");
	exit:
	return ret;
}

static char wait_str[32];
static char check_str[32];
static int AT_WAIT_STR(char *resp , int len)
{
	char *tmp;
	tmp = strstr(resp,wait_str);
	//+MIPRUDP: 183.131.17.185,29100,1,0,209000207F00001A000100010001000010910000000000000000
	if(tmp > 0)
	{
		
		return 1;
		
	}
	return 0;
}
static int AT_CHECK_STR(char *resp , int len)
{
	char *tmp;

	tmp = strstr(resp,check_str);
	//+MIPRUDP: 183.131.17.185,29100,1,0,209000207F00001A000100010001000010910000000000000000
	if(tmp > 0)
	{

		return AT_RESP_OK;
		
	}
	return AT_RESP_UNKONW;
}



int at_cmd_wait_str(char *at , int(*at_cb)(char*resp,int resplen) , char *waitstr , int timeout)
{
	snprintf(wait_str,sizeof(wait_str),"%s",waitstr);
	return at_cmd_wait(at,at_cb,AT_WAIT_STR,timeout);
	//
}

int at_cmd_wait_str_str(char *at , char *checkstr , char *waitstr , int timeout)
{
	snprintf(wait_str,sizeof(wait_str),"%s",waitstr);
	snprintf(check_str,sizeof(check_str),"%s",checkstr);
	return at_cmd_wait(at,AT_CHECK_STR,AT_WAIT_STR,timeout);
	//
}


int ___at_cmd_wait(char *at , int(*at_cb)(char*resp,int resplen) , int(*at_wait)(char*resp,int resplen) , int timeout)
{
	int ret = -1;
	
	AT_PROCESS_DELAY = 0;
	
	clear_uart2_buffer();
	
	//printf("==> %s",at);
	
	send_at(at);
	
	for(;;)
	{
		if ((AT_PROCESS_DELAY * 100) > timeout)
		{
			//printf("<== %s",uart2_rx_buffer);
			ret = at_cb(uart2_rx_buffer,uart2_rx_buffer_index);
			goto exit;
		}else{
			
			if (at_wait > 0)
			{
				if (at_wait(uart2_rx_buffer,uart2_rx_buffer_index) == 1)
				{
					//printf("<== %s",uart2_rx_buffer);
					ret = at_cb(uart2_rx_buffer,uart2_rx_buffer_index);
					goto exit;
				}
			}
			
		}
	}
	printf("AT TIMEOUT !\r\n");
	exit:
	return ret;
}

void send_at (char *at)
{
	int i=0;
	while(at[i] != 0)
	{
		uart2_putchar(at[i++]);
	}
	//
}

void send_data(unsigned char *data , int length)
{
	int i=0;
	for(i=0;i<length;i++)
	{
		uart2_putchar(data[i]);
	}
	
}

int AT_AT(char *resp , int len)
{
	if (strstr(resp,"OK"))
	{
		return AT_RESP_OK;
	}else{
		return AT_RESP_ERROR;
	}
}

int AT_CSQ(char *resp , int len)
{
	int tmp;
	if (strstr(resp,"OK"))
	{
		/*
		char *h = strstr(resp,"+CSQ: ");
		char *a = strstr(h,",");
		
		h += 6;
		a[0] = 0x0;
		
		printf("@@@@@@@@@@@@@@@@@@@@@@ csq %s\r\n",h);
		
		sscanf(h,"%d",&tmp);
		
		gsm_signal = tmp;
		*/
		return AT_RESP_OK;
	}
	else{
		gsm_signal = 99;
		return AT_RESP_ERROR;
	}
	//
}

char IMEI_CODE[16] = {0x0};
int AT_GSN(char *resp , int len)
{
	int i=0;
	char *f = strstr(resp,"\r\n");
	char *f_gsn = strstr(resp,"+GSN");
	printf("@@@@@@@@@@@@@@@@@@@@@@1xxx GSN RESP %s\r\n",resp);
	//return AT_RESP_OK;
	if (f_gsn)
	{
		//printf("@@@@@@@@@@@@@@@@@@@@@@1IMEI_CODE %s | %s\r\n",IMEI_CODE,resp);
		for(i=0;i<15;i++)
		{
			IMEI_CODE[i] = f_gsn[i+6];
		}
		IMEI_CODE[i] = 0x0;
		
		printf("@@@@@@@@@@@@@@@@@@@@@@2IMEI_CODE %s \r\n",IMEI_CODE);
		
		return AT_RESP_OK;
	}else
	if (f)
	{
		//printf("@@@@@@@@@@@@@@@@@@@@@@1IMEI_CODE %s | %s\r\n",IMEI_CODE,resp);
		for(i=0;i<15;i++)
		{
			IMEI_CODE[i] = f[i+2];
		}
		IMEI_CODE[i] = 0x0;
		
		printf("@@@@@@@@@@@@@@@@@@@@@@2IMEI_CODE %s \r\n",IMEI_CODE);
		
		return AT_RESP_OK;
		
	}
	else{
		return AT_RESP_ERROR;
	}
	//
}


//

int AT_CGREG(char *resp , int len)
{
	if (strstr(resp,"+CGREG: 2,1,") || strstr(resp,"+CGREG: 2,5,"))
	{
		
		char *p1;
		char *p2;
		char *p3;
		char *p4;
		
		GSM_LAC = 0;
		GSM_CI = 0;
		
		p1 = strstr(resp,"\"");
		p2 = strstr(p1+1,"\"");
		p3 = strstr(p2+1,"\"");
		p4 = strstr(p3+1,"\"");
		
		if ((p4>p3)&(p3>p2)&(p2>p1)){
			p2[0] = 0x0;
			p4[0] = 0x0;
			printf("PARSE CGREG LAC %s \r\n",p1+1);
			printf("PARSE CGREG CI  %s \r\n",p3+1);
			
			sscanf(p1+1,"%x",&GSM_LAC);
			sscanf(p3+1,"%x",&GSM_CI);
			
			printf("LAC: %x CI: %x \r\n",GSM_LAC,GSM_CI);
			
		}else{
			printf("PARSE CGREG ERRPR \r\n");
		}
		
		return AT_RESP_CGREGOK;
		
	}
	else{
		return AT_RESP_CGREGNOTREADY;
	}
	//
}

int AT_AMGSMLOC(char *resp , int len)
{
	if (strstr(resp,"+AMGSMLOC: 0"))
	{
		char *p1;
		char *p2;
		char *p3;
		char *p4;
		char *p5;
		
		p1 = strstr(resp,",");
		p2 = strstr(p1+1,",");
		p3 = strstr(p2+1,",");
		printf("GET LOC : %s \r\n",resp);
		if(((unsigned int)p1*(unsigned int)p2*(unsigned int)p3) > 0)
		{
			
			p2[0] = 0x0;
			p3[0] = 0x0;
			sscanf(p1+1,"%f",&GSM_LAT);
			sscanf(p2+1,"%f",&GSM_LON);
			printf("LAT %s %f\r\n",p1+1,GSM_LAT);
			printf("LON %s %f\r\n",p2+1,GSM_LON);
		}
		
		
		
		return AT_RESP_OK;
	}
	else{
		return AT_RESP_ERROR;
	}
	//
}

int AT_MIPCALL(char *resp , int len)
{
	//Operation not allowed
	if (strstr(resp,"+MIPCALL:") || (strstr(resp,"Operation not allowed")))
	{
		return AT_RESP_MIPCALL_OK;
	}
	else{
		return AT_RESP_MIPCALL_ERROR;
	}
}

int AT_MIPOPEN(char *resp , int len)
{
	
	if (strstr(resp,"OK") || (strstr(resp,"TCPIP socket used")))
	{
		return AT_RESP_OK;
	}else{
		return AT_RESP_ERROR;
	}
	//
}

int AT_CGMR_CHECK6312(char *resp , int len)
{
	if (strstr(resp,"M6312"))
	{
		return AT_RESP_MODEM_TYPE_M6312;
	}
	
	if (strstr(resp,"G510"))
	{
		return AT_RESP_MODEM_TYPE_G510;
	}

	return AT_RESP_ERROR;
}

int AT_CMRD(char *resp , int len)
{
	if (strstr(resp,"+CMRD"))
	{
		return AT_RESP_OK;
	}else{
		return AT_RESP_ERROR;
	}
	
}

int AT_NULL(char *resp , int len)
{
	return AT_RESP_OK;
}

int AT_CGDCONT(char *resp , int len)
{
	if (strstr(resp,"CME ERROR"))
	{
		return AT_RESP_ERROR;
	}
	
	if (strstr(resp,"OK"))
	{
		return AT_RESP_OK;
	}else{
		return AT_RESP_ERROR;
	}
	//
}
int AT_CGACT(char *resp , int len)
{
	if (strstr(resp,"CME ERROR"))
	{
		return AT_RESP_ERROR;
	}
	
	if (strstr(resp,"OK"))
	{
		return AT_RESP_OK;
	}else{
		return AT_RESP_ERROR;
	}
	//
}
int AT_CGATT(char *resp , int len)
{
	
	if (strstr(resp,"CME ERROR"))
	{
		return AT_RESP_ERROR;
	}
	
	if (strstr(resp,"CGATT: 1"))
	{
		return AT_RESP_OK;
	}else{
		return AT_RESP_ERROR;
	}
	//
}
int AT_CMMUX(char *resp , int len)
{
	if (strstr(resp,"OK"))
	{
		return AT_RESP_OK;
	}else{
		return AT_RESP_ERROR;
	}
	//
}

int AT_IPSTART(char *resp , int len)
{
	if (strstr(resp,"CONNECT") || strstr(resp,"OK"))
	{
		return AT_RESP_OK;
	}else{
		return AT_RESP_ERROR;
	}
	//
}

int AT_MIPPUSH(char *resp , int len)
{
	
	if (strstr(resp,"+MIPRUDP:"))
	{
		printf("+MIPRUDP DEBUG , %s",resp);
		return AT_RESP_OK;
	}else{
		return AT_RESP_ERROR;
	}
}

int AT_MIPPUSH_WAIT(char *resp , int len)
{
	char *tmp;
	tmp = strstr(resp,"+MIPRUDP");
	//+MIPRUDP: 183.131.17.185,29100,1,0,209000207F00001A000100010001000010910000000000000000
	if(tmp > 0)
	{
		if (strstr(tmp,"\r\n"))
		{
			return 1;
		}
		
	}
	return 0;
}

int AT_WAIT(char *resp , int len)
{
	char *tmp1,*tmp2,*tmp;
	tmp1 = strstr(resp,"OK");
	tmp2 = strstr(resp,"ERROR");
	//+MIPRUDP: 183.131.17.185,29100,1,0,209000207F00001A000100010001000010910000000000000000
	if((tmp1 > 0) || (tmp2>0))
	{
		if (tmp1>0)
			tmp = tmp1;
		else if (tmp2>0)
			tmp = tmp2;
		
		if (strstr(tmp,"\r\n"))
		{
			return 1;
		}
		
	}
	return 0;
}

int parser_recv_g510_data(char *str , unsigned char *data)
{
	int i;
	char *p;
	char parser_status = 0;
	char comma_cnt = 0;
	int len  = strlen (str);
	
	
	///+MIPRUDP: 183.131.17.185,29100,1,0,209000203E00001A000000010001000010910000000000000000
	
	for(i=0;i<len;i++)
	{
		
		switch(parser_status)
		{
			case 0:
			{
				if (str[i] == ',')
				{
					comma_cnt ++;
					//printf("### %s",str+i);
				}
				
				if (comma_cnt == 5)
				{
					parser_status = 1;
					p = str + i + 1;
					//(str + i + 1)
					//
				}
				
				break;
			}
			case 1:
			
				if (str[i] == '\r')
				{
					str[i]  = 0x0;
					parser_status = 2;
				}
			
				break;
				
			case 2:
				//printf("=== recv %s \r\n",p);
				return conv_string_2_hex(p,data);
			  parser_status = 3;
				break;
			case 3:
				break;
			default:
				break;
		}
		
	}
	
	
	return 0;
}

int parser_recv_m6312_data(char *str , unsigned char *data)
{
	
	char i;
	
	clear_uart2_buffer();
	send_at("AT+CMRD=0,1024\r\n");
	utimer_sleep(100);
	
	if (strstr("AT+CMRD=0,1024\r\n",uart2_rx_buffer) == uart2_rx_buffer)
	{
		printf("############### RECV ERROR RX BUFFER \r\n");
	}
	
	debug_buf("RECV DATA",(unsigned char*)uart2_rx_buffer,uart2_rx_buffer_index);
	memcpy(data,uart2_rx_buffer,uart2_rx_buffer_index);
	
	return uart2_rx_buffer_index;
	//
}

