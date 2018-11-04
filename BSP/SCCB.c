#include "SCCB.h"
#include <string.h>

typedef unsigned char u8;

static void delay_us(unsigned int us)
{
	int len;
	for (;us > 0; us --)
     for (len = 0; len < 20; len++ );
	//
}

#define PORT_TYPE GPIO_Mode_Out_PP



#define SDA_PORT GPIOB
#define SCL_PORT GPIOB
#define SDA_PORT_PIN GPIO_Pin_14
#define SCL_PORT_PIN GPIO_Pin_13



//IO方向设置
#define SDA_IN()  SDAPortIN()
#define SDA_OUT() SDAPortOUT();



#define SDA_H  GPIO_SetBits(SDA_PORT, SDA_PORT_PIN)
#define SDA_L  GPIO_ResetBits(SDA_PORT, SDA_PORT_PIN)

#define SCL_H  GPIO_SetBits(SCL_PORT, SCL_PORT_PIN)
#define SCL_L  GPIO_ResetBits(SCL_PORT, SCL_PORT_PIN)

#define READ_SDA GPIO_ReadInputDataBit(SDA_PORT, SDA_PORT_PIN)


void InitSCCB(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =  SDA_PORT_PIN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = PORT_TYPE; 
	GPIO_Init(SDA_PORT, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin =  SCL_PORT_PIN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = PORT_TYPE; 
	GPIO_Init(SCL_PORT, &GPIO_InitStructure); 
	
	SDA_H;
	SCL_H;
}




static void SDAPortIN(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = SDA_PORT_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;         
	GPIO_Init(SDA_PORT,&GPIO_InitStructure);
}

static void SDAPortOUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = SDA_PORT_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode =  PORT_TYPE;         
	GPIO_Init(SDA_PORT,&GPIO_InitStructure);
}

#define SIO_D_SET			SDA_H
#define SIO_C_SET			SCL_H
#define SIO_D_CLR			SDA_L
#define SIO_C_CLR			SCL_L

#define SIO_D_IN			SDAPortIN()
#define SIO_D_OUT			SDAPortOUT()
#define SIO_D_STATE		READ_SDA


//SCCB起始信号
//当时钟为高的时候,数据线的高到低,为SCCB起始信号
//在激活状态下,SDA和SCL均为低电平
void SCCB_Start(void)
{
	//SDA_H;     //数据线高电平	   
	//SCL_H;	    //在时钟线高的时候数据线由高至低
	SDA_H;
	SCL_H;
	delay_us(50);  
	//SDA_L;
	SDA_L;
	delay_us(50);	 
	//SCL_L;	    //数据线恢复低电平，单操作函数必要	  
	SCL_L;
}

//SCCB停止信号
//当时钟为高的时候,数据线的低到高,为SCCB停止信号
//空闲状况下,SDA,SCL均为高电平
void SCCB_Stop(void)
{
	//SDA_L;
	SDA_L;
	delay_us(50);	 
	//SCL_H;	
	SCL_H;
	delay_us(50); 
	//SDA_H;	
	SDA_H;
	delay_us(50);
}  
//产生NA信号
void SCCB_No_Ack(void)
{
	delay_us(50);
	//SDA_H;	
	//SCL_H;
	SDA_H;
	SCL_H;
	delay_us(50);
	//SCL_L;
	SCL_L;
	delay_us(50);
	//SDA_L;	
	SDA_L;
	delay_us(50);
}
//SCCB,写入一个字节
//返回值:0,成功;1,失败. 
u8 SCCB_WR_Byte(u8 dat)
{
	u8 j,res;	 
	for(j=0;j<8;j++) //循环8次发送数据
	{
		if(dat&0x80)
		{
			SDA_H;
		}
		else 
		{
			SDA_L;
		}
		dat<<=1;
		delay_us(50);
		SCL_H;	
		delay_us(50);
		SCL_L;		   
	}			 
	SDAPortIN();		//设置SDA为输入 
	delay_us(50);
	SCL_H;			//接收第九位,以判断是否发送成功
	delay_us(50);
	if(READ_SDA)res=1;  //SDA=1发送失败，返回1
	else res=0;         //SDA=0发送成功，返回0
	SCL_L;		 
	SDAPortOUT();		//设置SDA为输出    
	return res;  
}	 
//SCCB 读取一个字节
//在SCL的上升沿,数据锁存
//返回值:读到的数据
u8 SCCB_RD_Byte(void)
{
	u8 temp=0,j;    
	SDA_IN();		//设置SDA为输入  
	for(j=8;j>0;j--) 	//循环8次接收数据
	{		     	  
		delay_us(50);
		SCL_H;
		temp=temp<<1;
		if(READ_SDA)temp++;   
		delay_us(50);
		SCL_L;
	}	
	SDA_OUT();		//设置SDA为输出    
	return temp;
} 							    
//写寄存器
//返回值:0,成功;1,失败.
u8 sccbw(u8 reg,u8 data)
{
	u8 res=0;
	SCCB_Start(); 					//启动SCCB传输
	if(SCCB_WR_Byte(SCCB_ID))res=1;	//写器件ID	  
	delay_us(100);
  if(SCCB_WR_Byte(reg))res=1;		//写寄存器地址	  
	delay_us(100);
  if(SCCB_WR_Byte(data))res=1; 	//写数据	 
  SCCB_Stop();	  
  return	res;
}		  					    
//读寄存器
//返回值:读到的寄存器值
u8 sccbr(u8 reg)
{
	u8 val=0;
	SCCB_Start(); 				//启动SCCB传输
	SCCB_WR_Byte(SCCB_ID);		//写器件ID	  
	delay_us(100);	 
  SCCB_WR_Byte(reg);			//写寄存器地址	  
	delay_us(100);	  
	SCCB_Stop();   
	delay_us(100);	   
	//设置寄存器地址后，才是读
	SCCB_Start();
	SCCB_WR_Byte(SCCB_ID|0X01);	//发送读命令	  
	delay_us(100);
  val=SCCB_RD_Byte();		 	//读取数据
  SCCB_No_Ack();
  SCCB_Stop();
  return val;
}



