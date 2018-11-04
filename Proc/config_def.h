#ifndef config_def_h__
#define config_def_h__

#define CONFNIG_RAW_DATA_SIZE 4

enum OTA_FLAG_ENUM {
	OTA_FLAG_INIT,
	OTA_FLAG_DOWNLOAD,
	OTA_FLAG_DOWNLOAD_FINISH,
};

struct CONFIG_DATA {
	char version;
	
	/*
	工作模式:
	0,GPRS
	1,GSM
	*/
	char workmode;
	
	
	char ipaddress[32];
	char ipport[8];
	
	char phonenum[13][3];
	
	/*
	电话模式
	1，短信
	2，电话
	3，短信+电话
	*/
	char phonemode[3];
	
	//
};




#endif



