#include "udp_proto.h"
#include <string.h>
#include "common.h"
#include "at.h"
#include "adc.h"
#include "rtc.h"
#include "mem.h"
#include "setting.h"
#include "mainloop.h"
#include <string.h>
#include "bsp.h"

#define DEVICE_TYPE_VAL 1

static unsigned short message_id = 0;

void update_message_id(void)
{
	SET_SYSTEM_COUNTER;
	message_id = GET_SYSTEM_COUNTER;
}

static void set_device_number(unsigned char *devnum)
{
	int ret;
	//10 00 00 05 07 88  73 1f
	//10000005???0
	
	//IMEI_CODE
	char *tmp1 = (char*)alloc_mem(__FILE__,__LINE__,32);
	
	snprintf(tmp1,32,"%sf",IMEI_CODE);
	
	ret = conv_string_2_hex(tmp1,devnum);
	
	printf("DEVICE CONV SIZE %d [%02x %02x %02x %02x %02x %02x %02x %02x]\r\n",ret,devnum[0],devnum[1],devnum[2],devnum[3],devnum[4],devnum[5],devnum[6],devnum[7]);
	
	#define xDEVBUM_TEST
	#ifdef DEVBUM_TEST
	
	devnum[0] = 0x10;
	devnum[1] = 0x00;
	devnum[2] = 0x00;
	devnum[3] = 0x06;
	#endif
	
	free_mem(__FILE__,__LINE__,(unsigned char*)tmp1);
	//devnum[0] = 0x01;
	
	
}

int make_0x1091(unsigned char *data , unsigned char *in_data , short in_len)
{
		//10 91 02 01 01 01 00 23 00 00 00 01 00 01 40 00 6a 7b 86 42 70 03 47 88 73 1f 00 01 00 14 00 14 01 00 16
		unsigned char i;
    //unsigned char data[78];
    unsigned short verification = 0;
    struct UDP_PROTO_1091 body;

    //memset(data,0x0,sizeof(data));

    body.hdr.cmdcode = 0x1091;
		transfer16((unsigned short*)&body.hdr.cmdcode);
	
		//body.hdr.fw_ver;//0x0201;
		
		((unsigned char*)(&body.hdr.fw_ver))[0] = MAJOR_VERSION_NUMBER;
		((unsigned char*)(&body.hdr.fw_ver))[1] = MINOR_VERSION_NUMBER;
	
		//transfer16((unsigned short*)&body.hdr.fw_ver);
	
	
		body.hdr.pwd_ver = 0;//0x0101;
		transfer16((unsigned short*)&body.hdr.pwd_ver);
	
		body.hdr.messageLength = sizeof(struct UDP_PROTO_1091) + in_len; //0x27;
    transfer16((unsigned short*)&body.hdr.messageLength);
	
    body.hdr.messageID = message_id;
		update_message_id();
		
    transfer16((unsigned short*)&body.hdr.messageID);

    
    body.hdr.messageCount = 1;
    transfer16((unsigned short*)&body.hdr.messageCount);

    body.hdr.messageSeq = 1;
    transfer16((unsigned short*)&body.hdr.messageSeq);
		
		
		
		

    body.hdr.jie_ru_chuan_ma = 0;
		transfer16((unsigned short*)&body.hdr.jie_ru_chuan_ma);
		
		set_device_number(body.hdr.device_number);
		
		//------------------
		
		body.dianliang = mdata.device_voltage;
		transfer16((unsigned short*)&body.dianliang);
		

		body.signal_val = gsm_signal;
		body.device_type = DEVICE_TYPE_VAL;
		transfer16(&body.device_type);
		
		
		for(i = sizeof(struct UDP_PROTO_HDR);i<sizeof(struct UDP_PROTO_1091);i++)
    {
        verification+=((unsigned char*)(&body))[i];
    }
		
		for(i = 0;i<in_len;i++)
    {
        verification+=((unsigned char*)(in_data))[i];
    }
		
    body.hdr.verification = verification;
		transfer16((unsigned short*)&body.hdr.verification);
		
    memcpy(data,(unsigned char*)&body,sizeof(body));
		memcpy(data+sizeof(body),in_data,in_len);
		
		return sizeof(body)+in_len;

}



int make_0x1092(unsigned short imgl , unsigned char *imgdata ,unsigned short frq_count , unsigned char imgtype ,  unsigned short img_frq ,unsigned char *data)
{
	
		int i;
		unsigned short verification = 0;
    //static unsigned char data[1024];
    struct UDP_PROTO_1092 *img;
	
    img = (struct UDP_PROTO_1092 *)data;
	
		img->hdr.cmdcode = 0x1092;
		transfer16((unsigned short*)&img->hdr.cmdcode);

    img->hdr.messageID = message_id;
    transfer16((unsigned short*)&img->hdr.messageID);
	
		set_device_number(img->hdr.device_number);

    img->hdr.messageCount = frq_count;
    transfer16((unsigned short*)&img->hdr.messageCount);

    img->hdr.messageSeq = 0;
    transfer16((unsigned short*)&img->hdr.messageSeq);
		
		//设置图片类型
		img->pic_type = imgtype;
		
		//设置拍照时间
		img->time_paizhao_time = RTC_GetCounter() - mdata.paizhao_time;
		transfer32(&img->time_paizhao_time);
		
		DEBUG_VALUE(RTC_GetCounter());
		DEBUG_VALUE(mdata.paizhao_time);
		DEBUG_VALUE(img->time_paizhao_time);


		memset(data+sizeof(struct UDP_PROTO_1092)-1,0x0,sizeof(data) - (sizeof(struct UDP_PROTO_1092) - 1));
		memcpy(img->imagebody,imgdata,imgl);
		
		img->hdr.messageLength = imgl + sizeof(struct UDP_PROTO_1092) - 1;
		transfer16((unsigned short*)&img->hdr.messageLength);
		
		img->hdr.messageSeq = img_frq;
		transfer16((unsigned short*)&img->hdr.messageSeq);
		
		set_device_number(img->hdr.device_number);
		
		
		for(i = sizeof(struct UDP_PROTO_HDR);i<(imgl + sizeof(struct UDP_PROTO_1092) - 1);i++)
    {
        verification+=((unsigned char*)(img))[i];//data[i];
    }
		
		
		
		
		
		//verification = 0;
		
		img->hdr.verification = verification;
		transfer16((unsigned short*)&(img->hdr.verification));
		
		debug_buf("VERIFI",(unsigned char*)&(img->hdr.verification),2);
		
		return imgl + sizeof(struct UDP_PROTO_1092) - 1;

}



int make_0x10A0(unsigned char *data , c_u16 alarmtype , c_u32 alarmnum , c_u32 alarmtime)
{
		//10 91 02 01 01 01 00 23 00 00 00 01 00 01 40 00 6a 7b 86 42 70 03 47 88 73 1f 00 01 00 14 00 14 01 00 16
		unsigned char i;
    //unsigned char data[78];
    unsigned short verification = 0;
    struct UDP_PROTO_10A0_DATA body;

    //memset(data,0x0,sizeof(data));

    body.hdr.cmdcode = 0x10A0;
		transfer16((unsigned short*)&body.hdr.cmdcode);
	
		body.hdr.fw_ver = 0;//0x0201;
		transfer16((unsigned short*)&body.hdr.fw_ver);
		body.hdr.pwd_ver = 0;//0x0101;
		transfer16((unsigned short*)&body.hdr.pwd_ver);
	
		body.hdr.messageLength = sizeof(struct UDP_PROTO_10A0_DATA); //0x27;
    transfer16((unsigned short*)&body.hdr.messageLength);
	
    body.hdr.messageID = message_id;
		update_message_id();
    transfer16((unsigned short*)&body.hdr.messageID);

    
    body.hdr.messageCount = 1;
    transfer16((unsigned short*)&body.hdr.messageCount);

    body.hdr.messageSeq = 1;
    transfer16((unsigned short*)&body.hdr.messageSeq);
		
		
		
		

    body.hdr.jie_ru_chuan_ma = 0;
		transfer16((unsigned short*)&body.hdr.jie_ru_chuan_ma);
		
		set_device_number(body.hdr.device_number);
		
		
		body.alarmnum = alarmnum;
		transfer32((c_u32*)&body.alarmnum);
		body.alarmtime = alarmtime;
		transfer32((c_u32*)&body.alarmtime);
		
		body.alarmtype = alarmtype;
		transfer16((c_u16*)&body.alarmtype);
		
		body.gsm_lac = GSM_LAC;
		body.gsm_ci = GSM_CI;
		
		transfer32(&body.gsm_lac);
		transfer32(&body.gsm_ci);
		
		for(i = sizeof(struct UDP_PROTO_HDR);i<sizeof(body);i++)
    {
        verification+=((unsigned char*)(&body))[i];
    }
    body.hdr.verification = verification;
		transfer16((unsigned short*)&body.hdr.verification);
		
    memcpy(data,(unsigned char*)&body,sizeof(body));
		
		return sizeof(body);

}


int make_0x10B0(unsigned char *data , char *json)
{
	int i;
	unsigned short verification = 0;
	struct UDP_PROTO_HDR *hdr;
	hdr = (struct UDP_PROTO_HDR *)data;
	
	hdr->cmdcode = 0x10b0;
	transfer16((unsigned short*)&hdr->cmdcode);
	
	((unsigned char*)(&hdr->fw_ver))[0] = MAJOR_VERSION_NUMBER;
	((unsigned char*)(&hdr->fw_ver))[1] = MINOR_VERSION_NUMBER;
	
	hdr->pwd_ver = 0;//0x0101;
	transfer16((unsigned short*)&hdr->pwd_ver);
	
	hdr->messageLength = sizeof(struct UDP_PROTO_HDR) + strlen(json) +1; //0x27;
  transfer16((unsigned short*)&hdr->messageLength);
	
	
	for(i = 0; i<strlen(json); i++)
	{
			verification+=((unsigned char*)(json))[i];
	}
	
	hdr->verification = verification;
	transfer16((unsigned short*)&hdr->verification);
	
	
	//memcpy(data,(unsigned char*)&body,sizeof(body));
	memcpy(data+sizeof(struct UDP_PROTO_HDR),json,strlen(json)+1);
		
	return sizeof(struct UDP_PROTO_HDR) + strlen(json) + 1;

}


unsigned char check_pkg(unsigned char *buf , int buflen)
{
	
	//校验数据包是否合法的函数，返回0不合法，返回1合法
	int i;
	unsigned short verification,verification2,messageLength;
	struct UDP_PROTO_HDR *hdr;
	hdr = (struct UDP_PROTO_HDR *)buf;
	
	verification = 0;
	verification2 = 0;
	messageLength = 0;
	
	messageLength = hdr->messageLength;
	transfer16((unsigned short*)&messageLength);
	
	if (buflen <= 4)
		return 0;
	
	
//	if ((buf[buflen-2] == 0x0D) && (buf[buflen-1] == 0x0A))
//	{
//		printf("PKG CHECK 0D0A SUCCESS!\r\n");
//	}else{
//		printf("PKG CHECK 0D0A ERROR!\r\n");
//		return 0;
//	}
	
	//进行累加校验
	printf("CHECK VERFI:\r\n");
	for(i = sizeof(struct UDP_PROTO_HDR);i<messageLength;i++)
	{
			verification+=((unsigned char*)(buf))[i];
			printf(" %02X",buf[i]);
	}
	printf("\r\n");
	
	//把服务端送过来的转一下
	verification2 = hdr->verification;
	transfer16((unsigned short*)&verification2);
	
	
	printf("verification NUM : %d,%d ,%d , %d \r\n",verification2,verification,messageLength,sizeof(struct UDP_PROTO_2091_DATA));
	if (verification == verification2)
	{
		printf("PKG CHECK VERFI NUM SUCCESS!\r\n");
	}else{
		printf("PKG CHECK VERFI NUM ERROR!\r\n");
		return 0;
	}
	
	
	return 1;
	
	
	//
}

char * make_alarm(char *devid)
{
	char * p = 0;
	cJSON * pJsonRoot = NULL;
	
	pJsonRoot = cJSON_CreateObject();
	if(NULL == pJsonRoot){return 0;}
	
	cJSON_AddNumberToObject(pJsonRoot, "VER", 0);
	cJSON_AddStringToObject(pJsonRoot, "DEVID", devid);
	cJSON_AddNumberToObject(pJsonRoot, "MSGID",0);
	cJSON_AddNumberToObject(pJsonRoot, "VOL",0);
	cJSON_AddNumberToObject(pJsonRoot, "SIG",0);
	cJSON_AddNumberToObject(pJsonRoot, "CMD",1);
	
	p = cJSON_Print(pJsonRoot);

	if(NULL == p)
	{
			cJSON_Delete(pJsonRoot);
			return 0;
	}

	cJSON_Delete(pJsonRoot);
	
	return p;
}