#include "types.h"
#include "isr.h"
#include "stdlib/kernelheap.h"
#include "stdlib/paging.h"
#include "printf.h"

page_directory *kernel_directory=0;
page_directory *current_directory=0;

u32 *frames;
u32 number_of_frames;

extern u32 placement_address;
extern heap_t *kheap;

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))


// Static function to set a bit in the frames bitset
static void set_frame(u32 frame_addr)
{
  u32 frame = frame_addr/0x1000;
  u32 idx = INDEX_FROM_BIT(frame);
  u32 off = OFFSET_FROM_BIT(frame);
  frames[idx] |= (0x1 << off);
}

// Static function to clear a bit in the frames bitset
static void clear_frame(u32 frame_addr)
{
  u32 frame = frame_addr/0x1000;
  u32 idx = INDEX_FROM_BIT(frame);
  u32 off = OFFSET_FROM_BIT(frame);
  frames[idx] &= ~(0x1 << off);
}


// Static function to test if a bit is set.
static u32 test_frame(u32 frame_addr)
{
  u32 frame = frame_addr/0x1000;
  u32 idx = INDEX_FROM_BIT(frame);
  u32 off = OFFSET_FROM_BIT(frame);
  return (frames[idx] & (0x1 << off));
}

// Static function to find the first free frame.
static u32 first_frame()
{
  u32 i, j;
  for (i = 0; i < INDEX_FROM_BIT(number_of_frames); i++)
    {
      if (frames[i] != 0xFFFFFFFF) // nothing free, exit early.
        {
          // at least one bit is free here.
          for (j = 0; j < 32; j++)
            {
              u32 toTest = 0x1 << j;
              if ( !(frames[i]&toTest) )
                {
                  return i*4*8+j;
                }
            }
        }
    }
}



// Function to allocate a frame.
void alloc_frame(page *page, int is_kernel, int is_writeable)
{
   if (page->frame != 0)
   {
       return; // Frame was already allocated, return straight away.
   }
   else
   {
       u32 idx = first_frame(); // idx is now the index of the first free frame.
       if (idx == (u32)-1)
       {
           // PANIC is just a macro that prints a message to the screen then hits an infinite loop.
           PANIC("No free frames!");
       }
       set_frame(idx*0x1000); // this frame is now ours!
       page->present = 1; // Mark it as present.
       page->rw = (is_writeable)?1:0; // Should the page be writeable?
       page->user = (is_kernel)?0:1; // Should the page be user-mode?
       page->frame = idx;
   }
}

// Function to deallocate a frame.
void free_frame(page *page)
{
   u32 frame;
   if (!(frame=page->frame))
   {
       return; // The given page didn't actually have an allocated frame!
   }
   else
   {
       clear_frame(frame); // Frame is now free again.
       page->frame = 0x0; //Page now doesn't have a frame.
   }
}



void initialise_paging(){
  /* the size of physical memeory */
  u32 memory_end_page = 0x1000000; /* 16 MB */

  /* make a page directory */
  number_of_frames = memory_end_page / 0x1000;                         /* count of 4KB blocks */
  frames = (u32*)kmalloc(INDEX_FROM_BIT(number_of_frames));             /*  */
  memset(frames, 0, INDEX_FROM_BIT(number_of_frames));

  u32 phys;
  kernel_directory = (page_directory*)kmalloc_a(sizeof(page_directory));
  memset(kernel_directory, 0 , sizeof(page_directory));
  kernel_directory->physicalAddr = (u32) kernel_directory->tablesPhysical;

  // Map some pages in the kernel heap area.
  // Here we call get_page but not alloc_frame. This causes page_table_t's
  // to be created where necessary. We can't allocate frames yet because they
  // they need to be identity mapped first below, and yet we can't increase
  // placement_address between identity mapping and enabling the heap!
  int i = 0;
  for (i = KHEAP_START; i < KHEAP_START+KHEAP_INITIAL_SIZE; i += 0x1000)
    get_page(i, 1, kernel_directory);
  // We need to identity map (phys addr = virt addr) from
  // 0x0 to the end of used memory, so we can access this
  // transparently, as if paging wasn't enabled.
  // NOTE that we use a while loop here deliberately.
  // inside the loop body we actually change placement_address
  // by calling kmalloc(). A while loop causes this to be
  // computed on-the-fly rather than once at the start.
  i = 0;
  while (i < placement_address+0x1000)
    {
      alloc_frame( get_page(i, 1, kernel_directory), 0, 0);
      i += 0x1000;
    }

  // Now allocate those pages we mapped earlier.
  for (i = KHEAP_START; i < KHEAP_START+KHEAP_INITIAL_SIZE; i += 0x1000)
    alloc_frame( get_page(i, 1, kernel_directory), 0, 0);

  register_interrupt_handler(14, page_fault);
  switch_page_directory(kernel_directory);
  kheap = create_heap(KHEAP_START, KHEAP_START+KHEAP_INITIAL_SIZE, 0xCFFFF000, 0, 0);
  //print_memory_statistics(kheap);
  current_directory = clone_directory(kernel_directory);
  switch_page_directory(current_directory);
}

void print_memory_statistics(heap_t * tempheap){
  printf("\tHEAP:\n \t\tSTART: %X\n \t\tEND:  %X\n \t\tholes/blocks header:\n",  tempheap->start_address, tempheap->end_address);
  int counter;
  for(counter = 0; counter <=(((ordered_array_t)(tempheap->index)).size) -1; counter++){
  printf("\t\t\t%d:\n\t\t\t  Magic:%X\n \t\t\t  Is Hole: %X\n \t\t\t  Size: %X\n",
         counter,
         ((header_t*)(((ordered_array_t)(tempheap->index)).array[counter]))->magic,
         ((header_t*)(((ordered_array_t)(tempheap->index)).array[counter]))->is_hole,
         ((header_t*)(((ordered_array_t)(tempheap->index)).array[counter]))->size);
  }
}

void print_kernel_heap_statistics(){
  printf("\tHEAP:\n \t\tSTART: %X\n \t\tEND:   %X\n \t\tholes/blocks header:\n",  kheap->start_address, kheap->end_address);
  int counter;
  for(counter = 0; counter <=(((ordered_array_t)(kheap->index)).size) -1; counter++){
    printf("\t\t\t%d block in address %X \n\t\t\t  Magic:%X  Is Hole: %X  Size: %X\n",
           counter,
           ((u32)(((ordered_array_t)(kheap->index)).array[counter])), 
           ((header_t*)(((ordered_array_t)(kheap->index)).array[counter]))->magic,
           ((header_t*)(((ordered_array_t)(kheap->index)).array[counter]))->is_hole,
           ((header_t*)(((ordered_array_t)(kheap->index)).array[counter]))->size);
  }
}

void switch_page_directory (page_directory * dir)
{
  current_directory = dir;
  asm volatile("mov %0, %%cr3" :: "r"(dir->physicalAddr));
  u32 cr0;
  asm volatile("mov %%cr0, %0": "=r"(cr0));
  cr0 |= 0x80000000;
  asm volatile("mov %0, %%cr0" :: "r"(cr0));
}


page *get_page(u32 address, int make, page_directory *dir)
{
  /* Turn the address into an index. */
  address /= 0x1000;
  /* find the page table */
  u32 table_idx = address / 1024;
  if(dir->tables[table_idx])
    {
      return &dir->tables[table_idx]->pages[address%1024];
    }
  else if(make)
    {
      u32 tmp;
      dir->tables[table_idx] = (page_table*)kmalloc_ap(sizeof(page_table), &tmp);
      memset(dir->tables[table_idx], 0, 0x1000);
      dir->tablesPhysical[table_idx] = tmp | 0x7; /* present, rw, us */
      return &dir->tables[table_idx]->pages[address%1024];
    }
  else
    {
      return 0;
    }
}



void page_fault(registers_table regs)
{
  /* a page fautl has occured. */
  /* the faulting address is stored in the CR2 register */
  u32 faulting_address;
  asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

  /* the error code gives us details of what happened */
  int present   = !(regs.error_code & 0x1);
  int rw = regs.error_code & 0x2;
  int us = regs.error_code & 0x4;
  int reserved = regs.error_code & 0x8;
  int id = regs.error_code & 0x10;

  printf("Page fault! ( ");
  if (present) {printf("present");}
  if (rw) {printf("read-only");}
  if (us) {printf("user-mode");}
  if (reserved) {printf("reserwed");}
  printf(" ) at 0x%X\n", faulting_address);
  PANIC("Page fault");
}

static page_table *clone_table(page_table *original, u32 * physAddr)
{
  page_table * table = (page_table*)kmalloc_ap(sizeof(page_table), physAddr);
  memset (table, 0, sizeof(page_table));

  int i;
  for (i = 0; i < 1024; i++)
    {
      if(!original->pages[i].frame)
        {
          alloc_frame(&table->pages[i], 0, 0);
          if(original->pages[i].present)   table->pages[i].present = 1;
          if(original->pages[i].rw)        table->pages[i].rw = 1;
          if(original->pages[i].user)      table->pages[i].user = 1;
          if(original->pages[i].accessed)  table->pages[i].accessed = 1;
          if(original->pages[i].dirty)     table->pages[i].dirty = 1;
          copy_page_physical(original->pages[i].frame * 0x1000, table->pages[i].frame*0x1000);
        }
    }
  return table;
}


page_directory * clone_directory(page_directory *original)
{
  u32 phys;
  page_directory * dir = (page_directory *)kmalloc_ap(sizeof(page_directory), &phys);
  memset(dir, 0, sizeof(page_directory));


  u32 offset = (u32)dir->tablesPhysical  - (u32)dir;
  dir->physicalAddr = phys + offset;

  int i;
  for (i = 0; i< 1024; i++)
    {
      if(!original->tables[i])
        continue;

      if(kernel_directory->tables[i] == original->tables[i])
        {
          dir->tables[i]          = original->tables[i];
          dir->tablesPhysical[i]  = original->tablesPhysical[i];
        }
      else
        {
          u32 phys;
          dir->tables[i]          = clone_table(original->tables[i], &phys);
          dir->tablesPhysical[i]  = phys | 0x07;
        }
    }
  return dir;
}

