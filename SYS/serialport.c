#include "serialport.h"
#include "bsp.h"
#include "common.h"
#include "mem.h"
#include "utimer.h"
#include <string.h>

char *uart2_rx_buffer;
int uart2_rx_buffer_index;

void clear_uart2_buffer(void)
{
	memset(uart2_rx_buffer,0x0,UART2_RX_BUF_LEN);
	uart2_rx_buffer_index = 0;
}

void init_uart2_buffer(void)
{
	uart2_rx_buffer = (char*)alloc_mem(__FILE__,__LINE__,UART2_RX_BUF_LEN);
}


int uart_data_read(char *buf, int maxrlen, int mode, int timeout)
{
	int timeo = 0;
	char * back;
	int history_rx_len = 0;
	uart2_rx_buffer_index = 0;
	#define DELAY_MS 20
	for(;;)
	{
		
		utimer_sleep(DELAY_MS);
		timeo += DELAY_MS;
		
		if (uart2_rx_buffer_index > 0)
		{
			if (uart2_rx_buffer_index > history_rx_len)
			{
				history_rx_len = uart2_rx_buffer_index;
				timeo = 0;
			}else{
				
				timeout = 40;
			}
			
			if (uart2_rx_buffer_index >= maxrlen)
			{
				memcpy(buf,uart2_rx_buffer,uart2_rx_buffer_index);
				goto ret;
			}
		}
		
		if (timeo >= timeout)
		{
			memcpy(buf,uart2_rx_buffer,uart2_rx_buffer_index);
			goto ret;
		}	
	}
	
	ret:
	printf("BC->ST : [%d] [%d] %s\r\n",uart2_rx_buffer_index,timeout,buf);
//	back = strstr(buf,"+CSQ:");
//	if(back !=NULL) printf("CSQ BACK: %s\r\n",back);
//	if(strstr(buf,"OK")!=NULL) return 255;	
	return uart2_rx_buffer_index;
}
//static int uart_data_write(const char *buf, int writelen, int mode)
int uart_data_write(char *buf, int writelen, int mode)
{
	int i=0;
	
	printf("ST(uart)->BC : %s\r\n",buf);
	clear_uart2_buffer();
	for(i=0;i<writelen;i++)
	{
		uart2_putchar(buf[i]);
	}
  return 0;
}