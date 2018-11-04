#ifndef __alarm__
#define __alarm__

void init_alarm_gpio(void);

#define READ_ALARM (GPIOA->IDR & GPIO_Pin_7)

#endif