#include "memory/pmm.hpp"

#include "graphics/framebuffer.hpp"
#include "kernel/info.hpp"
#include "limine.h"
#include "misc/log.hpp"

#include <cstring>

extern kernel::info INFO;

namespace
{
uint64_t *bitmap, bitmap_size;

uint64_t free_amount, free_limit;
physical_address free_start;
} // namespace

namespace memory
{
namespace pmm
{
void initialize()
{
  physical_address top_address{};
  uint64_t free_len{};
  limine_memmap_entry **mmap = INFO.memory_map.map;

  for (uint64_t i = 0; i < INFO.memory_map.count; i++)
  {
    // skip non-usable memory
    if (mmap[i]->type != LIMINE_MEMMAP_USABLE)
    {
      continue;
    }

    auto base = mmap[i]->base, len = mmap[i]->length;

    if (free_start == 0)
    {
      free_start = base;
    }

    LOG("found free region 0x%016lX-0x%016lX", base, len);
    auto top = base + len;
    free_len += len;

    if (top > top_address)
    {
      LOG("new top address for free memory: 0x%016lX", top);
      top_address = top;
    }
  }

  // get amount of pages needed for the bitmap
  bitmap_size =
      ((top_address - free_start) / PAGE_SIZE / 8 + PAGE_SIZE - 1) & -PAGE_SIZE;
  LOG("bitmap size is %lu bytes", bitmap_size);

  // find a large enough free area for the bitmap that doesn't overlap with
  // anything
  for (uint64_t i = 0; i < INFO.memory_map.count; i++)
  {
    // skip non-usable memory
    if (mmap[i]->type != LIMINE_MEMMAP_USABLE)
    {
      continue;
    }

    // check if the memory overlaps with the bitmap
    if (mmap[i]->length >= bitmap_size)
    {
      LOG("found region for storing bitmap: 0x%016lX", mmap[i]->base);

      bitmap = reinterpret_cast<uint64_t *>(mmap[i]->base +
                                            INFO.higher_half_direct_offset);
      break;
    }
  }

  // set all pages as used
  LOG("setting whole bitmap as used");

  memset(bitmap, 0xFF, bitmap_size);
  free_amount = 0;

  LOG("unsetting free regions");
  // iterate over the memory map again to set the usable pages as free
  for (counter i = 0; i < INFO.memory_map.count; i++)
  {
    // skip non-usable memory
    if (mmap[i]->type != LIMINE_MEMMAP_USABLE)
    {
      continue;
    }

    // iterate through the pages
    for (counter j = 0; j < mmap[i]->length / PAGE_SIZE; j++)
    {
      // get the address of the page
      physical_address addr = mmap[i]->base + j * PAGE_SIZE;

      // get the index of the page
      auto idx = (addr - free_start) / PAGE_SIZE;

      // set the page as free
      bitmap[idx / 64] &= ~(1 << (idx % 64));
      free_amount++;
    }
  }

  uint64_t bitmap_pages = bitmap_size / PAGE_SIZE;

  LOG("setting pages that bitmap is occupying");
  // set bitmap pages as used
  for (uint64_t i = 0; i < bitmap_pages; i++)
  {
    auto idx = (reinterpret_cast<uint64_t>(bitmap) -
                INFO.higher_half_direct_offset + i * PAGE_SIZE - free_start) /
               PAGE_SIZE;
    bitmap[idx / 64] |= (1 << (idx % 64));
  }

  free_amount -= bitmap_pages;
  free_limit = free_amount;

  LOG("updating kernel info structure");

  INFO.bitmap.location = reinterpret_cast<physical_address>(bitmap) -
                         INFO.higher_half_direct_offset;
  INFO.bitmap.size = bitmap_size;
}

void *allocate()
{
  // check if there are any free blocks
  if (free_amount == 0)
  {
    LOG("critical: no more free physical pages");
    return NULL;
  }

  // get first free block
  counter i = 0;
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

  auto block = reinterpret_cast<void *>(free_start + i * PAGE_SIZE);
  //LOG("allocated block %p", block);

  // return the address of the block
  return block;
}

void deallocate(void *page)
{
  if (page == NULL)
  {
    LOG("error: cannot free null pointer");
    return;
  }

  if (free_amount == free_limit)
  {
    LOG("error: cannot free more pages than %lu", free_limit);
    return;
  }

  // get the index of the block
  auto i = (reinterpret_cast<physical_address>(page) - free_start) / PAGE_SIZE;

  // set the block as free
  bitmap[i / 64] &= ~(1 << (i % 64));

  //LOG("freed %p", page);
}
} // namespace pmm
} // namespace memory