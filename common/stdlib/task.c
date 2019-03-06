#include "stdlib/task.h"
#include "types.h"
#include "stdlib/paging.h"




volatile task_t *current_task;
volatile task_t *ready_queue;




extern page_directory *kernel_directory;
extern page_directory *current_directory;
extern void alloc_frame(page * , int , int);
extern u32 stack_esp;
extern u32 read_eip();




u32 next_pid = 1;


int get_pid()
{
  return current_task->id;
}



void initialise_tasking()
{
  /* diable interruption */
  asm volatile("cli");
  move_stack((void*) 0xE0000000, 0x2000);

  /* create first(kernel system) task */
  current_task = ready_queue = (task_t*) kmalloc (sizeof(task_t));
  current_task->id        = next_pid++;
  current_task->esp       = current_task->ebp   = 0;
  current_task->eip       = 0;
  current_task->directory = current_directory;
  current_task->next      = 0;


  /* again enable interruption */
  asm volatile("sti");
}

void print_task_info()
{
  task_t * temp_task = (task_t*)ready_queue;

  printf("\n===================================[ TASKS ]====================================PID\t\tESP\t\tEBP\t\tEIP\t\tPage directory\n");
  while(temp_task != 0)
    {
      if(temp_task->esp == 0){
      printf("  %d\t   %s\t     %s\t     %s\t\t  0x%X\n",
             temp_task->id,
             "UNDEFINED",
             "UNDEFINED",
             "UNDEFINED",
             temp_task->directory
             );
      }else{
      printf("  %d\t   0x%X\t     0x%X\t     0x%X\t\t  0x%X\n",
             temp_task->id,
             temp_task->esp,
             temp_task->ebp,
             temp_task->eip,
             temp_task->directory
             );
      }
      temp_task = temp_task->next;
    }
  printf("================================================================================");
}


int fork()
{
  asm volatile ("cli");

  /* back up pointer */
  task_t *parent_task = (task_t*) current_task;

  page_directory *directory = clone_directory(current_directory);

  task_t *new_task  = (task_t*) kmalloc(sizeof(task_t));
  new_task->id            = next_pid++;
  new_task->esp           = new_task->ebp       = 0;
  new_task->eip           = 0;
  new_task->directory     = directory;
  new_task->next          = 0;
  task_t *temp_task = (task_t*) ready_queue;
  /* run to end of queue */
  while (temp_task->next)
    temp_task = temp_task->next;
  temp_task->next   = new_task;

  u32 eip   = read_eip();

  if(current_task == parent_task)
    {
      u32 esp; asm volatile("mov %%esp, %0": "=r"(esp));
      u32 ebp; asm volatile("mov %%ebp, %0": "=r"(ebp));
      new_task->esp   = esp;
      new_task->ebp   = ebp;
      new_task->eip   = eip;
      asm volatile ("sti");
      return new_task->id;
    }
  else
    {
      return 0;
    }
}



void task_switch()
{
  /* if null */
  if(!current_task)
    return;

  u32 esp, ebp, eip;
  asm volatile("mov %%esp, %0": "=r"(esp));
  asm volatile("mov %%ebp, %0": "=r"(ebp));
  eip = read_eip();

  /* have we just switch tasks? */
  if(eip == 0x12345)
    return;

  current_task->eip   = eip;
  current_task->esp   = esp;
  current_task->ebp   = ebp;


  /* if end of list reload queue to start */
  current_task        = current_task->next;
  if(!current_task)   current_task = ready_queue;

  eip                 = current_task->eip;
  esp                 = current_task->esp;
  ebp                 = current_task->ebp;

  current_directory   = current_task->directory;

  asm volatile("         \
     cli;                 \
     mov %0, %%ecx;       \
     mov %1, %%esp;       \
     mov %2, %%ebp;       \
     mov %3, %%cr3;       \
     mov $0x12345, %%eax; \
     sti;                 \
     jmp *%%ecx           "
               : : "r"(eip), "r"(esp), "r"(ebp), "r"(current_directory->physicalAddr));
}

void move_stack(void * stack_pointer, u32 stack_size)
{
  u32 i;
  for (i = (u32)stack_pointer;
       i >= (u32)stack_pointer - stack_size;
       i -= 0x1000)
    {
      alloc_frame(get_page(i, 1, current_directory), 0 /* user_mode for page */ , 1 /* is writable */ );
    }

  /**
     FLUSHING TLB
  **/
  u32 tlb_address;
  asm volatile("mov %%cr3, %0" : "=r" (tlb_address));
  asm volatile("mov %0, %%cr3" : : "r" (tlb_address));

  u32 old_stack_pointer;  asm volatile("mov %%esp, %0":"=r"(old_stack_pointer));
  u32 old_base_pointer;   asm volatile("mov %%ebp, %0":"=r"(old_base_pointer));

  u32 offset = (u32)stack_pointer - stack_esp;

  u32 new_stack_pointer = old_stack_pointer + offset;
  u32 new_base_pointer  = old_base_pointer + offset;

  /* just copy old stack */
  memcpy((void*)new_stack_pointer, (void*)old_stack_pointer, stack_esp-old_stack_pointer);

  for (i = (u32)stack_pointer;
       i > (u32)stack_pointer - stack_size;
       i -= 4)
    {
      u32 tmp = * (u32*) i;

      /* remaping pointer from old stack to new stack  */
      if((old_stack_pointer < tmp) && (tmp < stack_esp))
        {
          tmp = tmp + offset;
          u32 *tmp2 = (u32*)i;
          *tmp2 = tmp;
        }
    }

  asm volatile("mov %0, %%esp" : : "r" (new_stack_pointer));
  asm volatile("mov %0, %%ebp" : : "r" (new_base_pointer));
}

