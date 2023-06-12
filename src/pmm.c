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

#include "limine.h"
#include "log.h"
#include "terminal.h"

#include <string.h>

uint64_t *BITMAP, BITMAP_SIZE;

static uint64_t top_address;
static uint64_t free_base;
static uint64_t free_len;
static uint64_t free_amount;
static uint64_t free_limit;

extern struct limine_memmap_entry **MEMMAP;
extern uint64_t MEMMAP_COUNT, HIGHER_HALF_START;

void pmm_init(void)
{
  free_base = 0;
  free_len  = 0;

  LOG("Finding all free regions of memory...");
  for (size_t i = 0; i < MEMMAP_COUNT; i++)
  {
    // skip non-usable memory
    if (MEMMAP[i]->type != LIMINE_MEMMAP_USABLE)
    {
      continue;
    }

    uint64_t base = MEMMAP[i]->base, len = MEMMAP[i]->length;

    if (free_base == 0)
    {
      free_base = base;
    }

    LOG("Found %016llp-%016llp", base, len);
    uint64_t top = base + len;
    free_len += len;

    if (top > top_address)
    {
      LOG("Setting new top address for free memory %016llp", top);
      top_address = top;
    }
  }

  // get amount of pages needed for the bitmap
  BITMAP_SIZE = ((top_address - free_base) / 4096 / 8 + PMM_PAGE_SIZE - 1) &
                ~(PMM_PAGE_SIZE - 1);
  LOG("Bitmap size is %llp bytes", BITMAP_SIZE);

  // find a large enough free area for the bitmap that doesn't overlap with
  // anything
  for (size_t i = 0; i < MEMMAP_COUNT; i++)
  {
    // skip non-usable memory
    if (MEMMAP[i]->type != LIMINE_MEMMAP_USABLE)
    {
      continue;
    }

    // check if the memory overlaps with the bitmap
    if (MEMMAP[i]->length >= BITMAP_SIZE)
    {
      LOG("Found region for storing bitmap %016llp", MEMMAP[i]->base);

      BITMAP = (uint64_t *)(MEMMAP[i]->base + HIGHER_HALF_START);
      break;
    }
  }

  // set all pages as used
  LOG("Clearing bitmap...", BITMAP_SIZE);

  memset(BITMAP, 0xFF, BITMAP_SIZE);
  free_amount = 0;

  LOG("Setting free regions as free...");
  // iterate over the memory map again to set the usable pages as free
  for (size_t i = 0; i < MEMMAP_COUNT; i++)
  {
    // skip non-usable memory
    if (MEMMAP[i]->type != LIMINE_MEMMAP_USABLE)
    {
      continue;
    }

    // iterate through the pages
    for (uint64_t j = 0; j < MEMMAP[i]->length / PMM_PAGE_SIZE; j++)
    {
      // get the address of the page
      uint64_t addr = MEMMAP[i]->base + j * PMM_PAGE_SIZE;

      // get the index of the page
      uint64_t idx = (addr - free_base) / PMM_PAGE_SIZE;

      // set the page as free
      BITMAP[idx / 64] &= ~(1 << (idx % 64));
      free_amount++;
    }
  }

  uint64_t bitmap_pages = BITMAP_SIZE / PMM_PAGE_SIZE;

  LOG("Setting bitmap pages as used...");
  // set bitmap pages as used
  for (uint64_t i = 0; i < bitmap_pages; i++)
  {
    uint64_t idx =
        ((uint64_t)BITMAP - HIGHER_HALF_START + i * PMM_PAGE_SIZE - free_base) /
        PMM_PAGE_SIZE;
    BITMAP[idx / 64] |= (1 << (idx % 64));
  }

  free_amount -= bitmap_pages;
  free_limit = free_amount;
}

void *pmm_alloc(void)
{
  // check if there are any free blocks
  if (free_amount == 0)
  {
    LOG("Critical: no more free physical pages");
    return NULL;
  }

  // get first free block
  uint64_t i = 0;
  for (; i < free_limit; i++)
  {
    if (!(BITMAP[i / 64] & (1 << (i % 64))))
    {
      break;
    }
  }

  // set the block as used
  BITMAP[i / 64] |= (1 << (i % 64));
  free_amount--;

  void *addr = (void *)(free_base + i * PMM_PAGE_SIZE);
  LOG("Allocated block %016llp", addr);

  // return the address of the block
  return addr;
}

void pmm_free(void *ptr)
{
  if (ptr == NULL)
  {
    LOG("Error: cannot free null pointer");
    return;
  }

  if (free_amount == free_limit)
  {
    LOG("Error: cannot free more pages than %llu", free_limit);
    return;
  }

  // get the index of the block
  uint64_t i = ((uint64_t)ptr - free_base) / PMM_PAGE_SIZE;

  // set the block as free
  BITMAP[i / 64] &= ~(1 << (i % 64));

  LOG("Freed %016llp", ptr);
}