#ifndef __task_h__
#define __task_h__

typedef void (*TASKP)(void *p);

void init_task(void);
char post_task(TASKP tp , void *arg);
void run_task(void);



#endif

