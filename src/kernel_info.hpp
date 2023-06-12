#pragma once

#include "limine.h"
#include "types.hpp"

template <typename InfoT = std::nullptr_t> struct physical_memory_region
{
  physical_address location;
  uint64_t size;
  InfoT info;
};

struct kernel_info
{
  struct
  {
    limine_memmap_entry **map;
    uint64_t count;
  } memory_map;

  physical_memory_region<> kernel;
  physical_memory_region<> stack;
  physical_memory_region<limine_framebuffer> framebuffer;

  offset higher_half_offset;
};
