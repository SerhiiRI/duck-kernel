#ifndef _KERNEL_HEAP
#define _KERNEL_HEAP


#define KHEAP_START         0xC0000000
#define KHEAP_INITIAL_SIZE  0x100000
#define HEAP_INDEX_SIZE   0x20000
#define HEAP_MAGIC        0x123890AB
#define HEAP_MIN_SIZE     0x70000


#include "types.h"
#include "ordered_array.h"

typedef struct
{
  u32   magic;
  u8    is_hole;
  u32   size;

} header_t;

typedef struct
{
  u32       magic;
  header_t  *header;
} footer_t;

typedef struct
{
  ordered_array_t   index;
  u32               start_address;
  u32               end_address;
  u32               max_address;
  u8                supervisor;
  u8                readonly;
} heap_t;

/* normal */
u32 kmalloc(u32 size);
/* page aligned */
u32 kmalloc_a(u32 sz);
/* return of physical address */
u32 kmalloc_p(u32 sz, u32 *phys);
/* page aligned and returns */
/* a physical address       */
u32 kmalloc_ap(u32 sz, u32 *phys);
/**
   create a new heap
**/
heap_t * create_heap(u32 start, u32 end, u32 max, u8 supervisor, u8 readonly);
/**
   Allocates a contiguous region of memory 'size' in size, if page_align==1, it creates that block starting
   on a page boundary
**/
void * alloc(u32 size, u8 page_align, heap_t * heap);
/**
   Releases a block allocated with 'alloc'
**/
void free(void *p, heap_t * heap);

void kfree(void *p);
void print_memory_statistics(heap_t*);
void print_kernel_heap_statistics();
#endif
