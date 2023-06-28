#include "memory/pmm.hpp"

#include "graphics/framebuffer.hpp"
#include "kernel/info.hpp"
#include "limine.h"
#include "misc/flag.hpp"
#include "misc/log.hpp"
#include "misc/round.hpp"

#include <cstring>

extern kernel::info INFO;

namespace
{
uint64_t *bitmap, bitmap_size;

uint64_t free_amount, free_limit;
uintptr_t free_start;
} // namespace

namespace memory
{
namespace pmm
{
void initialize()
{
  uintptr_t top_address{};
  limine_memmap_entry **mmap = INFO.memory_map.map;

  for (uint64_t i = 0; i < INFO.memory_map.count; i++)
  {
    // skip non-usable memory
    if (mmap[i]->type != LIMINE_MEMMAP_USABLE)
    {
      continue;
    }

    const auto base = mmap[i]->base, len = mmap[i]->length;

    if (free_start == 0)
    {
      free_start = base;
    }

    LOG("found free region 0x%016lX-0x%016lX", base, len);

    if (const auto top = base + len; top > top_address)
    {
      LOG("new top address for free memory: 0x%016lX", top);
      top_address = top;
    }
  }

  // get amount of pages needed for the bitmap
  bitmap_size =
      round::up((top_address - free_start) / PAGE_SIZE / 8, PAGE_SIZE);
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
  for (uint64_t i = 0; i < INFO.memory_map.count; i++)
  {
    // skip non-usable memory
    if (mmap[i]->type != LIMINE_MEMMAP_USABLE)
    {
      continue;
    }

    // iterate through the pages
    for (uint64_t j = 0; j < mmap[i]->length / PAGE_SIZE; j++)
    {
      // get the address of the page
      const uintptr_t addr = mmap[i]->base + j * PAGE_SIZE;

      // get the index of the page
      const auto idx = (addr - free_start) / PAGE_SIZE;

      // set the page as free
      flag::unset(bitmap[idx / 64], 1 << (idx % 64));
      free_amount++;
    }
  }

  const uint64_t bitmap_pages = bitmap_size / PAGE_SIZE;

  LOG("setting pages that bitmap is occupying");
  // set bitmap pages as used
  for (uint64_t i = 0; i < bitmap_pages; i++)
  {
    const auto idx = (reinterpret_cast<uint64_t>(bitmap) -
                      INFO.higher_half_direct_offset + i * PAGE_SIZE - free_start) /
                     PAGE_SIZE;

    flag::set(bitmap[idx / 64], 1 << (idx % 64));
  }

  free_amount -= bitmap_pages;
  free_limit = free_amount;

  LOG("updating kernel info structure");

  INFO.bitmap.location = reinterpret_cast<uintptr_t>(bitmap) -
                         INFO.higher_half_direct_offset;
  INFO.bitmap.size = bitmap_size;
}

void *allocate()
{
  // check if there are any free blocks
  if (free_amount == 0)
  {
    LOG("critical: no more free physical pages");
    return nullptr;
  }

  // get first free block
  uint64_t i = 0;
  for (; i < free_limit; i++)
  {
    if (!flag::is_set(bitmap[i / 64], 1 << (i % 64)))
    {
      break;
    }
  }

  // set the block as used
  flag::set(bitmap[i / 64], 1 << (i % 64));
  free_amount--;

  const auto block = reinterpret_cast<void *>(free_start + i * PAGE_SIZE);
  // LOG("allocated block %p", block);

  // return the address of the block
  return block;
}

void deallocate(void *page)
{
  if (page == nullptr)
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
  const auto i = (reinterpret_cast<uintptr_t>(page) - free_start) / PAGE_SIZE;

  // set the block as free
  flag::unset(bitmap[i / 64], 1 << (i % 64));

  // LOG("freed %p", page);
}
} // namespace pmm
} // namespace memory
