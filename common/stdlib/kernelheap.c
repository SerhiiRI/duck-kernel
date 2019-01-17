#include "types.h"
#include "stdlib/kernelheap.h"

extern u32 end;
u32 placement_address = (u32)&end;


u32 kmalloc_internal(u32 size, int align, u32 *phys){
  if(align == 1 && (placement_address & 0xFFFFF000))
    {
      placement_address &= 0xFFFFF000;
      placement_address += 0x1000;
    }
  if(phys)
    {
      *phys = placement_address;
    }
  u32 temp = placement_address;
  placement_address += size;
  return temp;
}

u32 kmalloc_a (u32 size)            { return kmalloc_internal(size, 1, 0);    }
u32 kmalloc_p (u32 size, u32 *phys) { return kmalloc_internal(size, 0, phys); }
u32 kmalloc_ap(u32 size, u32 *phys) { return kmalloc_internal(size, 1, phys); }
u32 kmalloc   (u32 size)            { return kmalloc_internal(size, 0, 0);    }
