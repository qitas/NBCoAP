#include "ledctl.h"
#include "bsp.h"

struct LED_CTRL_INFO LEDCTL[LEDCTL_COUNT];

static void led0_ctrl_func(unsigned char status)
{
	switch(status)
	{
		case 0:
			led0_off();
			break;
		case 1:
			led0_on();
			break;
		default:
			break;
	}
}



void init_led(void)
{
	memset(&LEDCTL,0x0,sizeof(LEDCTL));
	
	led0_ctrl_func(0);
	
	LEDCTL[0].ctrl = led0_ctrl_func;
	
	LEDCTL[0].ON_MS = 0;
	LEDCTL[0].OFF_MS = 0;
	
}


void led_routing_func(void)
{
	int i=0;
	unsigned short timestep = 10;
	for(i=0;i<LEDCTL_COUNT;i++)
	{
		switch(LEDCTL[i]._ONOFF)
		{
			case 0:
				if (LEDCTL[i]._TIME >= LEDCTL[i].OFF_MS)
				{
					//打开LED灯
					LEDCTL[i].ctrl(1);
					LEDCTL[i]._ONOFF = 1;
					LEDCTL[i]._TIME = 0;
				}
				break;
			case 1:
			default:
				if (LEDCTL[i]._TIME >= LEDCTL[i].ON_MS)
				{
					//打开LED灯
					LEDCTL[i].ctrl(0);
					LEDCTL[i]._ONOFF = 0;
					LEDCTL[i]._TIME = 0;
				}
				break;
		}
		
		LEDCTL[i]._TIME+=timestep;
	}
	

	//
}