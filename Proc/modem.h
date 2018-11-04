#ifndef __modem_h__
#define __modem_h__

void gprs_modem_power_on(void);
void gprs_modem_power_off(void);

void modem_power_pin_set(void);
void modem_power_pin_reset(void);
char read_modem_status(void);
void config_ring(void);
void config_resetPin(void);



#endif
