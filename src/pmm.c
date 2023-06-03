/**
 * @file pmm.c
 * @author bg117 (bg117@sooosilly.xyz)
 * @brief Physical memory manager
 * @version 0.1
 * @date 2023-06-03
 *
 * This file contains the implementation for the physical memory manager.
 */

#include "pmm.h"
#include "terminal.h"

#include "limine.h"

// linked-list allocator
struct block
{
  struct block *next;
  uint64_t      addr;
};

struct block *head = NULL;
struct block *tail = NULL;

uint64_t free_base;
uint64_t free_len;
uint64_t free_amount;
uint64_t free_limit;

void pmm_init(struct limine_memmap_entry **mmap, size_t mmap_count)
{
  uint64_t largest_free_len  = 0;
  uint64_t largest_free_addr = 0;
  for (size_t i = 0; i < mmap_count; i++)
  {
    // skip non-usable memory
    if (mmap[i]->type != LIMINE_MEMMAP_USABLE)
    {
      continue;
    }

    if (mmap[i]->length > largest_free_len)
    {
      largest_free_len  = mmap[i]->length;
      largest_free_addr = mmap[i]->base;
    }
  }

  // round down to nearest page
  largest_free_len &= ~(PMM_PAGE_SIZE - 1);

  // reserve some pages for the PMM
  int pmm_pages = ((largest_free_len / PMM_PAGE_SIZE * sizeof(struct block)) + PMM_PAGE_SIZE - 1) & ~(PMM_PAGE_SIZE - 1);

  // set the free base
  free_base = largest_free_addr + pmm_pages;
  free_len  = largest_free_len - pmm_pages;

  // set the free amount
  free_amount = free_len / PMM_PAGE_SIZE;
  free_limit  = free_amount;

  // initialize the head
  head = (struct block *)largest_free_addr;
  head->next = NULL;
  head->addr = free_base;

  // initialize the tail
  tail = head;

  // make all the blocks free
  for (uint64_t i = 1; i < free_amount; i++)
  {
    // set the next block
    tail->next = (struct block *)(free_base + i * PMM_PAGE_SIZE);
    tail->next->next = NULL;
    tail->next->addr = free_base + i * PMM_PAGE_SIZE;

    // set the tail
    tail = tail->next;
  }
}

void *pmm_alloc(void)
{
  // check if there are any free blocks
  if (free_amount == 0)
  {
    return NULL;
  }

  // get the first block
  struct block *block = head;

  // set the head to the next block
  head = head->next;
  free_amount--;

  // return the block
  return (void *)block->addr;
}

void pmm_free(void *ptr)
{
  if (ptr == NULL)
  {
    return;
  }

  if (free_amount == free_limit)
  {
    return;
  }

  // get the block
  struct block *block = (struct block *)head - 1;
  block->addr = (uint64_t)ptr;

  // set the next block
  block->next = head;
  head = block;
}