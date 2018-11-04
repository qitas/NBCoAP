#ifndef __utimer_h__
#define __utimer_h__

extern unsigned int system_time_10ms;

struct UTIMER {
  void (*func)(void *p);		//超时的回调函数
  void *arg;								//回调函数参数
  
  char enable;							//是否打开定时器
  char type;								//类型 1,为循环，0为单词
  
  char __zhouqi;
  char __dangqian;
  
};

#define TIMERLIST_LEN 4

extern struct UTIMER timerlist[TIMERLIST_LEN];

void init_utimer(void);
void run_utimer(void *p); //定时器中断提交的任务

void systick_handle(void);

//100ms 定时器计数器
#define TIMER_100MS_CNT_LEN 5
extern unsigned int __time_100ms_cnt[TIMER_100MS_CNT_LEN];

#define TIMER_100MS_STATUS_MASTER				0
#define TIMER_100MS_MAINLOOP_DELAY			1
#define TIMER_100MS_AT_PROCESS_DELAY		2
#define TIMER_100MS_PRINTF_DELAY				3
#define TIMER_100MS_SYSTEMTIME					4


//10ms 定时器计数器
#define TIMER_10MS_CNT_LEN 4
extern unsigned int __time_10ms_cnt[TIMER_10MS_CNT_LEN];

void utimer_sleep(int ms);




#endif

