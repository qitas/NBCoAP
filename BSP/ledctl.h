#ifndef __ledctl_h__
#define __ledctl_h__

struct LED_CTRL_INFO {
	unsigned short ON_MS;
	unsigned short OFF_MS;
	unsigned short _TIME;
	unsigned short _ONOFF;
	void (*ctrl)(unsigned char status);
};

#define LEDCTL_COUNT 1

extern struct LED_CTRL_INFO LEDCTL[LEDCTL_COUNT];

void init_led(void);
void led_routing_func(void);

#define LED_NETWORK_REGISTER_STATUS LEDCTL[0].ON_MS=500;LEDCTL[0].OFF_MS = 500;
#define LED_NETWORK_ACTIVE_STATUS LEDCTL[0].ON_MS=300;LEDCTL[0].OFF_MS = 1000;

#endif