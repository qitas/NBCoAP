#ifndef __at_h__
#define __at_h__

extern unsigned short gsm_signal;
extern char IMEI_CODE[16];
extern unsigned int GSM_LAC,GSM_CI;
extern float GSM_LAT,GSM_LON;

enum {
	AT_RESP_OK = 0,
	AT_RESP_ERROR,
	AT_RESP_UNKONW,
	
	AT_RESP_CGREGOK,
	AT_RESP_CGREGNOTREADY,
	
	AT_RESP_MIPCALL_OK,
	AT_RESP_MIPCALL_ERROR,
	
	AT_RESP_MODEM_TYPE_G510,
	AT_RESP_MODEM_TYPE_M6312,
	
};

#define AT_WAIT_LEVEL_1				300
#define AT_WAIT_LEVEL_2				500
#define AT_WAIT_LEVEL_3				500
#define AT_WAIT_LEVEL_4				1000
#define AT_WAIT_LEVEL_5				5000


//int at_cmd(char *at , int(*at_cb)(char*resp,int resplen) , int timeout);
int at_cmd_wait(char *at , int(*at_cb)(char*resp,int resplen) , int(*at_wait)(char*resp,int resplen) , int timeout);
int at_cmd_wait_str(char *at , int(*at_cb)(char*resp,int resplen) , char *waitstr , int timeout);
int at_cmd_wait_str_str(char *at , char *checkstr , char *waitstr , int timeout);
void send_at(char *at);
void send_data(unsigned char *data , int length);

int AT_AT(char *resp , int len);
int AT_CSQ(char *resp , int len);
int AT_GSN(char *resp , int len);
int AT_CGREG(char *resp , int len);
int AT_AMGSMLOC(char *resp , int len);
int AT_MIPCALL(char *resp , int len);
int AT_MIPOPEN(char *resp , int len);
int AT_CGMR_CHECK6312(char *resp , int len); //int CGMR(char *resp , int len);
int AT_CMRD(char *resp , int len);
int AT_NULL(char *resp , int len);

int AT_CGDCONT(char *resp , int len);
int AT_CGACT(char *resp , int len);
int AT_CGATT(char *resp , int len);
int AT_CMMUX(char *resp , int len);
int AT_IPSTART(char *resp , int len);

int AT_MIPPUSH(char *resp , int len);
int AT_MIPPUSH_WAIT(char *resp , int len);
int AT_WAIT(char *resp , int len);

int parser_recv_g510_data(char *str , unsigned char *data);
int parser_recv_m6312_data(char *str , unsigned char *data);

#endif



