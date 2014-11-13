#include "common.h"
#include "defs.h"
#include "task.h"

//
// thread scheduling policies: SCHED_FIFO, SCHED_RR, SCHED_BATCH, SCHED_IDLE, SCHED_OTHER
// priority values: 1 (low) to 99 (high) only for SCHED_OTHER and SCHED_RR, for others _must_ be 0
//


pthread_t SYS_start_task(uint8_t task_slot_id, void (*task_function)(), int scheduling_policy, int priority)
 {

   pthread_t new_task;
   pthread_attr_t new_task_attr;
   struct sched_param new_task_sched_param;

   pthread_attr_init(&new_task_attr);
   pthread_attr_setdetachstate(&new_task_attr, PTHREAD_CREATE_DETACHED);

   if(pipe(G_QSID_tasks[task_slot_id].input_pipe) == -1)
     SYS_debug(DEBUG_NORMAL,"SYS_start_task: warning: cannot create create input pipe for task %x",task_function);

   if(pthread_create(&new_task, &new_task_attr, task_function, NULL))
    {
     SYS_debug(DEBUG_NORMAL,"SYS_start_task: cannot start task %x",task_function);
     return -1;
    }

   new_task_sched_param.sched_priority = priority;

   if(pthread_setschedparam(new_task, scheduling_policy, &new_task_sched_param))
    SYS_debug(DEBUG_NORMAL,"SYS_start_task: warning: cannot set scheduling policy for task %x",task_function);

   G_QSID_task_count++;

   G_QSID_tasks[task_slot_id].task_id = new_task;

   return new_task;

 }
