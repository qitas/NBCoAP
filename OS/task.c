#include "task.h"
#include <string.h>

struct TASK_DATA {
  TASKP task;
  void *arg;
  char enable;
};

#define TASKLIST_LENGTH 4

struct TASK_DATA task_list[TASKLIST_LENGTH];

void init_task(void)
{
  memset(task_list,0x0,sizeof(task_list));
  //
}

void run_task(void)
{
  int i=0;
  for(i=0;i<TASKLIST_LENGTH;i++)
  {
    if (task_list[i].enable == 1)
    {
      task_list[i].task(task_list[i].arg);
      task_list[i].enable = 0;
      
    }
  }
  //
}

char post_task(TASKP tp , void *arg)
{
  int i=0;
  for(i=0;i<TASKLIST_LENGTH;i++)
  {
    if (task_list[i].enable == 0)
    {
      task_list[i].task = tp;
      task_list[i].arg = arg;
      task_list[i].enable = 1;
      return 1;
    }
  }
  
  return 0;
  //
}

