#include "common.h"
#include "defs.h"
#include "task.h"

//
// thread scheduling policies: SCHED_FIFO, SCHED_RR, SCHED_BATCH, SCHED_IDLE, SCHED_OTHER
// priority values: 1 (low) to 99 (high) only for SCHED_OTHER and SCHED_RR, for others _must_ be 0
//


pthread_t SYS_start_task(char *task_name, void (*task_function)(), int scheduling_policy, int priority)
 {

   pthread_t new_task;
   pthread_attr_t new_task_attr;
   struct sched_param new_task_sched_param;

   pthread_attr_init(&new_task_attr);
   pthread_attr_setdetachstate(&new_task_attr, PTHREAD_CREATE_DETACHED);

   if(pthread_create(&new_task, &new_task_attr, task_function, NULL))
    {
     SYS_debug(DEBUG_NORMAL,"SYS_start_task: cannot start task %x",task_function);
     return -1;
    }

   new_task_sched_param.sched_priority = priority;

   if(pthread_setschedparam(new_task, scheduling_policy, &new_task_sched_param))
    SYS_debug(DEBUG_NORMAL,"SYS_start_task: warning: cannot set scheduling policy for task %x",task_function);

   G_QSID_task_count++;

   strncpy(G_QSID_tasks[G_QSID_task_count].task_name, task_name, QSID_OS_MAX_TASK_NAME_LEN);
   G_QSID_tasks[G_QSID_task_count].task_id = new_task;
 
   return new_task;

 }
