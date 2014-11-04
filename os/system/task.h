//
// thread (task) array for QSID OS
//

#define QSID_OS_MAX_TASK 32
#define QSID_OS_MAX_TASK_NAME_LEN 128

#define PRIO_VERYHIGH99 99
#define PRIO_VERYHIGH98 98
#define PRIO_VERYHIGH97 97
#define PRIO_VERYHIGH96 96
#define PRIO_VERYHIGH95 95
#define PRIO_VERYHIGH94 94
#define PRIO_VERYHIGH93 93
#define PRIO_VERYHIGH92 92
#define PRIO_VERYHIGH91 91
#define PRIO_VERYHIGH90 90

#define PRIO_HIGH89 89
#define PRIO_HIGH88 88
#define PRIO_HIGH87 87
#define PRIO_HIGH86 86
#define PRIO_HIGH85 85
#define PRIO_HIGH84 84
#define PRIO_HIGH83 83
#define PRIO_HIGH82 82
#define PRIO_HIGH81 81
#define PRIO_HIGH80 80

#define PRIO_NORMAL59 59
#define PRIO_NORMAL58 58
#define PRIO_NORMAL57 57
#define PRIO_NORMAL56 56
#define PRIO_NORMAL55 55
#define PRIO_NORMAL54 54
#define PRIO_NORMAL53 53
#define PRIO_NORMAL52 52
#define PRIO_NORMAL51 51
#define PRIO_NORMAL50 50

#define PRIO_LOW29 29
#define PRIO_LOW28 28
#define PRIO_LOW27 27
#define PRIO_LOW26 26
#define PRIO_LOW25 25
#define PRIO_LOW24 24
#define PRIO_LOW23 23
#define PRIO_LOW22 22
#define PRIO_LOW21 21
#define PRIO_LOW20 20


typedef struct _QSID_task_t {
  char task_name[QSID_OS_MAX_TASK_NAME_LEN];
  pthread_t task_id;
 } QSID_task_t;

QSID_task_t G_QSID_tasks[QSID_OS_MAX_TASK];

uint8_t G_QSID_task_count;

