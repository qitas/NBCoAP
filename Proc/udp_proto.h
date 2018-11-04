#ifndef __udp_proto_h__
#define __udp_proto_h__

#include "common.h"
#include "JSON/cJSON.h"

#define VER_2_1_6_a



enum {
	UDP_CLIENT_STATUS_WAIT_NETWORK_READY = 0,
	UDP_CLIENT_STATUS_INIT,
	UDP_CLIENT_STATUS_BIND,
	UDP_CLIENT_STATUS_BIND_SUCCESS,
	UDP_CLIENT_STATUS_RST,
};

enum {
	UPLOAD_PIC_STATUS_INIT,
	UPLOAD_PIC_STATUS_START,
	UPLOAD_PIC_STATUS_UPLAODING,
	UPLOAD_PIC_STATUS_FINISH,
	UPLOAD_PIC_STATUS_TIMEOUT,
};

struct UPLOADPIC_DATA {
	unsigned char uplaod_pic_status;
	unsigned int img_length;
	unsigned char *img_data;
	unsigned int img_send_len;
	unsigned char img_frq_count;
	unsigned char img_frq_index;
	unsigned int img_pkg_ack; //SETBIT
	unsigned short current_frq;
	
	//
	unsigned int waitack_len;
	
};





#pragma pack(push)
#pragma  pack(1)

struct UDP_PROTO_HDR {
    c_u16 cmdcode;					//指令代码
		c_u16 fw_ver;							//固件版本
		c_u16 pwd_ver;						//秘钥版本    
    c_u16 messageLength;			//消息长度
		c_u16 messageID;					//消息ID
    c_u16 messageCount;				//消息总数
    c_u16 messageSeq;					//消息序列号
    c_u16 verification;				//verification
	
		c_u16 jie_ru_chuan_ma;		//接入串码
    c_u8 device_number[8];				//设备编号

};

//1091
struct UDP_PROTO_1091 {
    struct UDP_PROTO_HDR hdr;
    c_u16 dianliang;
    c_u8 signal_val;
		c_u16 device_type;
    //
	
};

struct UDP_PROTO_1092 {
    struct UDP_PROTO_HDR hdr;
    c_u32 time_paizhao_time;
    c_u8 pic_type;
    c_u8 imagebody[1];
};

struct UDP_PROTO_2091_DATA {
	struct UDP_PROTO_HDR hdr;
	c_u16 dianliangfazhi;
	c_u8  time1[4];
	c_u8  time2[4];
	c_u16 baoguangzhi;
	c_u16 shanguangdeng;
	c_u8  heibai;
	c_u8  resolution;
	c_u16 new_fw_number;
	c_u8 new_fw_pkg_count;
	c_u16 new_fw_pwd_ver;
	#ifdef VER_2_1_6_a
	c_u8 IP[7+5];
	#endif
};

struct UDP_PROTO_2092_DATA {
	struct UDP_PROTO_HDR hdr;
};


struct UDP_PROTO_10A0_DATA {
	struct UDP_PROTO_HDR hdr;
	c_u16 alarmtype;
	c_u32 alarmnum;
	c_u32 alarmtime;
	c_u32 gsm_lac;
	c_u32 gsm_ci;
};

struct UDP_PROTO_20A0_DATA {
	struct UDP_PROTO_HDR hdr;
	c_u32 alarm_resp;
};

#pragma pack(pop)


struct UDP_PROTO_DATA {
	
	c_u8 status;
	c_u16 serialcode;
	c_u16 message_id;
	c_u32 device_number;
	c_u8 image_type;
};

extern struct UDP_PROTO_DATA upd;
extern struct UPLOADPIC_DATA uploadpic_data;

int make_0x1091(unsigned char *data , unsigned char *in_data , short in_len);
int make_0x1092(unsigned short imgl , unsigned char *imgdata ,unsigned short frq_count ,unsigned char imgtype , unsigned short img_frq , unsigned char *data);
int make_0x10A0(unsigned char *data , c_u16 alarmtype , c_u32 alarmnum , c_u32 alarmtime);
int make_0x10B0(unsigned char *data , char *json);
unsigned char check_pkg(unsigned char *buf , int buflen);
void update_message_id(void); //更新messageID，每次发送的数据包的message不一致，但是图片包，一幅图片拆分的包不计算在内

char *make_alarm(char *devid);

#endif

