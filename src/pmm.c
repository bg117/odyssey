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
#include "terminal.h"

#include <string.h>

uint64_t *bitmap;

uint64_t top_address;
uint64_t free_base;
uint64_t free_len;
uint64_t free_amount;
uint64_t free_limit;

void pmm_init(struct limine_memmap_entry **mmap, size_t mmap_count)
{
  free_base = 0;
  free_len  = 0;

  for (size_t i = 0; i < mmap_count; i++)
  {
    // skip non-usable memory
    if (mmap[i]->type != LIMINE_MEMMAP_USABLE)
    {
      continue;
    }

    if (free_base == 0)
    {
      free_base = mmap[i]->base;
    }

    uint64_t top = mmap[i]->base + mmap[i]->length;
    free_len += mmap[i]->length;

    if (top > top_address)
    {
      top_address = top;
    }
  }

  free_limit = free_len / PMM_PAGE_SIZE;

  // get amount of pages needed for the bitmap
  uint64_t bitmap_size = ((top_address - free_base) / 4096 / 8 + PMM_PAGE_SIZE - 1) &
                         ~(PMM_PAGE_SIZE - 1);

  // find a large enough free area for the bitmap that doesn't overlap with
  // anything
  for (size_t i = 0; i < mmap_count; i++)
  {
    // skip non-usable memory
    if (mmap[i]->type != LIMINE_MEMMAP_USABLE)
    {
      continue;
    }

    // check if the memory overlaps with the bitmap
    if (mmap[i]->length >= bitmap_size)
    {
      bitmap = (uint64_t *)mmap[i]->base;
      break;
    }
  }

  // set all pages as used
  memset(bitmap, 0xFF, bitmap_size);
  free_amount = 0;

  // iterate over the memory map again to set the usable pages as free
  for (size_t i = 0; i < mmap_count; i++)
  {
    // skip non-usable memory
    if (mmap[i]->type != LIMINE_MEMMAP_USABLE)
    {
      continue;
    }

    // iterate through the pages
    for (uint64_t j = 0; j < mmap[i]->length / PMM_PAGE_SIZE; j++)
    {
      // get the address of the page
      uint64_t addr = mmap[i]->base + j * PMM_PAGE_SIZE;

      // get the index of the page
      uint64_t idx = (addr - free_base) / PMM_PAGE_SIZE;

      // set the page as free
      bitmap[idx / 64] &= ~(1 << (idx % 64));
      free_amount++;
    }
  }

  uint64_t bitmap_pages = bitmap_size / PMM_PAGE_SIZE;

  // set bitmap pages as used
  for (uint64_t i = 0; i < bitmap_pages; i++)
  {
    uint64_t idx =
        ((uint64_t)bitmap + i * PMM_PAGE_SIZE - free_base) / PMM_PAGE_SIZE;
    bitmap[idx / 64] |= (1 << (idx % 64));
  }

  free_amount -= bitmap_pages;

// print statistics
#if 1
  terminal_printf("PMM: %u pages free\n", free_amount);
  terminal_printf("PMM: %u pages used\n", free_limit - free_amount);
  terminal_printf("PMM: %u pages total\n", free_limit);
  terminal_printf("PMM: bitmap size: %llu pages, %llu bytes\n", bitmap_pages,
                  bitmap_size);
  terminal_printf("PMM: bitmap address: %llp\n", bitmap);
  terminal_printf("PMM: free base: %llp\n", free_base);
  terminal_printf("PMM: free len: %llp\n", free_len);
  terminal_printf("PMM: top address: %llp\n", top_address);
#endif
}

void *pmm_alloc(void)
{
  // check if there are any free blocks
  if (free_amount == 0)
  {
    return NULL;
  }

  // get first free block
  uint64_t i = 0;
  for (; i < free_limit; i++)
  {
    if (!(bitmap[i / 64] & (1 << (i % 64))))
    {
      break;
    }
  }

  // set the block as used
  bitmap[i / 64] |= (1 << (i % 64));
  free_amount--;

  // return the address of the block
  return (void *)(free_base + i * PMM_PAGE_SIZE);
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

  // get the index of the block
  uint64_t i = ((uint64_t)ptr - free_base) / PMM_PAGE_SIZE;

  // set the block as free
  bitmap[i / 64] &= ~(1 << (i % 64));
}