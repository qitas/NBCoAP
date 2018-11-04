#ifndef __ota_h__
#define __ota_h__

#pragma pack(push)
#pragma  pack(1)

struct OTA_GET_INFO {
	unsigned char cmd;
	int fwsize;
	int fwcrc;
};

struct OTA_GET_PKG {
	unsigned char cmd;
	int offset;
	int length;
};

struct OTA_RESP_PKG {
	unsigned char cmd;
	int offset;
	int payload_length;
	unsigned short crc;
	unsigned char patload[1];
};

#pragma pack(pop)

typedef unsigned int u32;
typedef unsigned int vu32;
typedef unsigned short u16;

int JumpToApp(u32 appAddr);
void VectorTable_Set(void);

#endif

