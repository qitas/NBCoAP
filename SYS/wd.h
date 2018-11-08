#ifndef __wd_h__
#define __wd_h__

#include <stm32f10x_iwdg.h>

void watch_dog_config(void);
void feed_watchdog(void);

#endif
