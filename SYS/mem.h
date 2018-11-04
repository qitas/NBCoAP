#ifndef __mem__
#define __mem__

#include "config_def.h"

struct MEM_DATA {
	unsigned char led_cnt;
	unsigned fangchaiflag; //这个变量保证仅仅被读取一次
	unsigned int sig_led_counter;
	unsigned char sig_led_flag;
	//adccache
	
	unsigned short adccache[10];
	
	//config:
	unsigned char config_write_flag;
	struct CONFIG_DATA cfg_data;
	unsigned char cfg_rawdata[CONFNIG_RAW_DATA_SIZE];
	
};



void init_mem(void);
void trace_mem(void);
unsigned char* alloc_mem(char *file , int line , int size);
void free_mem(char *file , int line , unsigned char *mem);

extern struct MEM_DATA *mem;


#endif



