#include "utimer.h"
#include "task.h"
#include <string.h>
#include "bsp.h"


unsigned int system_second = 0;

struct UTIMER timerlist[TIMERLIST_LEN];

void init_utimer(void)
{
  memset(&timerlist,0x0,sizeof(timerlist));
  memset(__time_100ms_cnt,0x0,sizeof(__time_100ms_cnt));
  //
}

void run_utimer(void *p)
{
  
  int i=0;
  for(i=0;i<TIMERLIST_LEN;i++)
  {
    if (timerlist[i].enable == 1)
    {
      timerlist[i].__dangqian ++;
      if (timerlist[i].__dangqian >= timerlist[i].__zhouqi)
      {
        timerlist[i].func(timerlist[i].arg);
        timerlist[i].__dangqian = 0;
        
        //如果类型是1那么，执行完一次后，就关闭
        if (timerlist[i].type == 1)
        {
          timerlist[i].enable = 0;
          //
        }
      }
    }
    //
  }
	
	//100ms计数器
	for(i=0;i<TIMER_100MS_CNT_LEN;i++)
	{
		__time_100ms_cnt[i] ++;
	}
  
  //
}

unsigned int __time_10ms_cnt[TIMER_10MS_CNT_LEN];
unsigned int __time_100ms_cnt[TIMER_100MS_CNT_LEN];

void systick_handle(void)
{
	int i;
	static unsigned char systick_cnt = 0;
	
	for(i=0;i<TIMER_10MS_CNT_LEN;i++)
	{
		__time_10ms_cnt[i] ++;
	}
	
	systick_cnt ++;
	if (systick_cnt >= 10)
	{
		systick_cnt = 0;
		post_task(run_utimer,0);
		for(i=0;i<TIMER_100MS_CNT_LEN;i++)
		{
			__time_100ms_cnt[i] ++;
		}
	}
	
}

void utimer_sleep(int ms)
{

	__time_10ms_cnt[0] = 0;
	
	for(;;)
	{
		//printf("__time_10ms_cnt[0] == %d\r\n",__time_10ms_cnt[0]);
		__disable_irq();
		if ((__time_10ms_cnt[0] * 10) > ms)
		{
			break;
		}
		__enable_irq();
	}
	__enable_irq();
}
