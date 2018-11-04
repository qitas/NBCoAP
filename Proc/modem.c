#include "modem.h"
#include "bsp.h"

void gprs_modem_power_on(void)
{
	modem_poweron();
}

void gprs_modem_power_off(void)
{
	modem_poweroff();
}


void modem_power_pin_set(void)
{
}

void modem_power_pin_reset(void)
{
}

char read_modem_status(void)
{
	return 0;
}

void config_ring(void)
{

}

void config_resetPin(void)
{

}
