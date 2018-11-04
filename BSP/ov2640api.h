#ifndef __ov2640_api_h__
#define __ov2640_api_h__

#include "flash.h"

//JPEG_BUFFER_LENGTH

extern unsigned char *JpegBuffer;			//图片缓冲区
extern unsigned int JpegDataCnt;				//图片缓冲区长度

void alloc_jpegbuffer(void);
void free_jpegbuffer(void);

void ov2640_poweron(void);								//打开OV2640摄像头
void ov2640_poweroff(void);							//关闭OV2640摄像头

int ov2640_init(void);									//初始化OV2640摄像头,返回 0 代表成功，返回 -1代表失败

int ov2640_read(void);							//拍照采集图像，采集后的图像存储于图片缓冲区中

void _config_mco(void);

int read_photo_ov2640(int index);
unsigned char* read_imgbuffer(int index  , int *imglen , unsigned int *paizhaotime);
void __write_img_2_flash(int index , unsigned char *buffer , int buflen , unsigned int paizhaoshijian);
unsigned char* read_imgbuffer(int index  , int *imglen , unsigned int *paizhaotime);

/*
example : 

int main(int argc , char ** argv)
{
	ov2640_poweron(); //打开摄像头
	ov2640_init();	//初始化摄像头
	
	ov2640_read();	// 读取图片
	
	ov2640_poweroff(); //关闭摄像头电源
	//
	
	//对图片进行处理
	JpegBuffer
	JpegDataCnt
}

*/

#endif

