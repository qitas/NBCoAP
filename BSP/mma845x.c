#include "IOI2C.h"

#include <stdio.h>

#define m_define_ic_id 0x2A
#define m_define_device_address_write 0x3A
#define m_define_device_address_read 0x3B

#define m_define_ctl_reg_1 0x2A
#define m_define_ctl_reg_2 0x2B
#define m_define_ctl_reg_3 0x2C
#define m_define_ctl_reg_4 0x2D
#define m_define_ctl_reg_5 0x2E

#define m_define_ctl_interrupt_status 0x0C
#define m_define_trans_config 0x1D
#define m_define_trans_source 0x1E
#define m_define_trans_threshold 0x1F
#define m_define_trans_count 0x20

#define m_define_xyz_date_config 0x0E
#define m_define_hp_filter_cut_off_config 0x0F

#define m_define_value_ctl_reg_1_standby 0x1C
#define m_define_value_ctl_reg_1_activity 0x1D

#define m_define_value_ctl_reg_2  0x01
#define m_define_value_ctl_reg_3_interrupt_high 0x02
#define m_define_value_ctl_reg_4_trans_mode 0x20
#define m_define_value_ctl_reg_5_pin2 0x00

#define m_define_value_how_much_byte_1 1
#define m_define_value_setting 0x24

#define m_define_value_trans_xyz_date_config 0x10
#define m_define_value_trans_hp_filter_cut_off_config 0x00

#define m_define_value_trans_config 0x0E
#define m_define_value_trans_threshold 0x01

#define m_define_value_trans_count  0x05

unsigned int mma8452_moving_time = 0;
unsigned int mma8452_stay_time = 0;

typedef unsigned char u8;


static void write_i2c_dev(u8 devid , u8 reg , u8 value , u8 num , u8 vs)
{
	IICwriteBytes(devid,reg,1,&value);
}

static void read_i2c_dev(u8 devid , u8 reg , u8 value , u8 num , u8 vs)
{
	IICreadBytes(devid,reg,1,&value);
}
//IICreadBytes
/**
	???MMA8452??
*/
char init_mma845x(void)
{

	unsigned char testvalue;
	extern unsigned short I2C_Erorr_Count;
	
	
	IOI2C_Init();
	
	printf("Init MMA8452Q Failed [%d][%d]! \r\n",I2C_Erorr_Count,__LINE__);
	
	write_i2c_dev(m_define_device_address_write, 
			m_define_ctl_reg_1, 
			m_define_value_ctl_reg_1_standby, 
			m_define_value_how_much_byte_1, 
			m_define_value_setting);
	

	write_i2c_dev(m_define_device_address_write,
			m_define_ctl_reg_2,
			m_define_value_ctl_reg_2,
			m_define_value_how_much_byte_1,
			m_define_value_setting);

	write_i2c_dev(m_define_device_address_write,
			m_define_ctl_reg_3,
			m_define_value_ctl_reg_3_interrupt_high,
			m_define_value_how_much_byte_1,
			m_define_value_setting);

	write_i2c_dev(m_define_device_address_write,
			m_define_ctl_reg_4,
			m_define_value_ctl_reg_4_trans_mode,
			m_define_value_how_much_byte_1,
			m_define_value_setting);

	write_i2c_dev(m_define_device_address_write,
			m_define_ctl_reg_5,
			m_define_value_ctl_reg_5_pin2,
			m_define_value_how_much_byte_1,
			m_define_value_setting);

	write_i2c_dev(m_define_device_address_write,
			m_define_xyz_date_config,
			m_define_value_trans_xyz_date_config,
			m_define_value_how_much_byte_1,
			m_define_value_setting);

	write_i2c_dev(m_define_device_address_write,
			m_define_hp_filter_cut_off_config,
			m_define_value_trans_hp_filter_cut_off_config,
			m_define_value_how_much_byte_1,
			m_define_value_setting);

	write_i2c_dev(m_define_device_address_write,
			m_define_trans_config,
			m_define_value_trans_config,
			m_define_value_how_much_byte_1,
			m_define_value_setting);

	write_i2c_dev(m_define_device_address_write,
			m_define_trans_threshold,
			m_define_value_trans_threshold,
			m_define_value_how_much_byte_1,
			m_define_value_setting);


	write_i2c_dev(m_define_device_address_write,
			m_define_trans_count,
			m_define_value_trans_count,
			m_define_value_how_much_byte_1,
			m_define_value_setting);
			

	write_i2c_dev(m_define_device_address_write, 
			m_define_ctl_reg_1, 
			m_define_value_ctl_reg_1_standby, 
			m_define_value_how_much_byte_1, 
			m_define_value_setting);

	write_i2c_dev(m_define_device_address_write, 
			m_define_ctl_reg_1, 
			m_define_value_ctl_reg_1_activity, 
			m_define_value_how_much_byte_1, 
			m_define_value_setting);


	write_i2c_dev(m_define_device_address_write, 
			m_define_ctl_reg_1, 
			m_define_value_ctl_reg_1_activity, 
			m_define_value_how_much_byte_1, 
			m_define_value_setting);

			
	testvalue = 0;
	//static void read_i2c_dev(u8 devid , u8 reg , u8 value , u8 num , u8 vs)
	IICreadBytes(m_define_device_address_write,m_define_trans_threshold,1,&testvalue);
			
	if ((I2C_Erorr_Count == 0) &&  (testvalue == 1))
	{
		printf("Init MMA8452Q SUCCESS ! \r\n");
		return 1;
	}
	else
	{
		printf("Init MMA8452Q Failed [%d][%d]! \r\n",I2C_Erorr_Count,__LINE__);
		return 0;
	}
	
	

	
	


}
