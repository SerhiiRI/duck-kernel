#ifndef TASK_H
#define TASK_H
#include "stdlib/paging.h"
#include "types.h"


typedef struct task
{
  int id;
  u32 esp, ebp;
  u32 eip;
  page_directory *directory;
  struct task *next;
} task_t;

void initialise_tasking();

void task_switch();

int fork();

void move_stack(void * stack_pointer, u32 stack_size);

/* return PID of current process */
int get_pid();

#endif
