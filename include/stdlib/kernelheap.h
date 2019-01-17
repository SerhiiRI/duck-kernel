#ifndef _KERNEL_HEAP
#define _KERNEL_HEAP

#include "types.h"



/* normal */
u32 kmalloc(u32 size);

/* page aligned */
u32 kmalloc_a(u32 sz);

/* return of physical address */
u32 kmalloc_p(u32 sz, u32 *phys);

/* page aligned and returns */
/* a physical address       */
u32 kmalloc_ap(u32 sz, u32 *phys);

#endif
