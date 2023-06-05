/**
 * @file vmm.h
 * @author bg117 (bg117@sooosilly.xyz)
 * @brief Virtual memory manager
 * @version 0.1
 * @date 2023-06-05
 *
 * This file contains code for managing virtual memory. This includes
 * mapping and unmapping pages of memory in the virtual address space.
 */

#ifndef ODYSSEY_VMM_H
#define ODYSSEY_VMM_H

#include <stdint.h>

struct page_table_entry
{
  enum
  {
    PT_PRESENT        = 1 << 0,
    PT_WRITABLE       = 1 << 1,
    PT_USER           = 1 << 2,
    PT_WRITE_THROUGH  = 1 << 3,
    PT_CACHE_DISABLED = 1 << 4,
    PT_ACCESSED       = 1 << 5,
    PT_DIRTY          = 1 << 6,
    PT_HUGE           = 1 << 7,
    PT_GLOBAL         = 1 << 8,
  } flags : 9;
  uint8_t used : 1;
  uint8_t available_0 : 2;
  uint64_t base_addr : 40; // do not store lower 12 bits (all 0)
  uint16_t available_1 : 11;
  uint8_t no_execute : 1;
} __attribute__((packed));

/**
 * @brief Initialize the virtual memory manager.
 */
void vmm_init();

#endif