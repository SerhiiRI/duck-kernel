#ifndef _PAGING
#define _PAGING


#include "types.h"
#include "isr.h"


typedef struct page_struct
{
  u32 present   :1;             /* page present in mamory */
  u32 rw        :1;             /* read-only if clear, readwrite if set */
  u32 user      :1;             /* supervisor level only if clear */
  u32 accessed  :1;             /* has the page been accessed since last refresh? */
  u32 dirty     :1;             /* has the page been written to since last refresh? */
  u32 unused    :7;             /* amalgamation(lączenia) of unused and resernved bits */
  u32 frame     :20;            /* frame address (shifted right 12 bits)*/
} page;



typedef struct page_table_struct
{
  page pages[1024];
} page_table;


typdef struct page_directory_struct
{
  /**
     Array of pointer to pagetables;
   **/
  page_table *tables[1024];
  /**
     Array of pointers to the pagetables above, but gives their *physical*
     location, for loading into the CR3 register.
  **/
  u32 tablesPhysical[1024];
  /**
     The physical address of tablesPhysical.This comes into play
     when we get our kernel heap allocated and the directory may
     be in a different location in virtual memory.
  **/
  u32 physicalAddr;
}page_directory;

/**
   Sets up the invironment, page directories etc and
   enables pagging.
**/
void initialise_paging();

/**
   Couses the specified page direcotry to be loaded into the
   CR3 register.
**/
void switch_page_direcotory(page_directory_t *new);

/**
   Retrieves a pointer ot the page required.
   if make == 1, if the page-table in which this page should
   reside isn't created, create it!
**/
page_t *get_page(u32 address, int make, page_directory_t *dir);


/**
   Handler for page faults
**/
void page_fault(registers_table regs)


#endif
