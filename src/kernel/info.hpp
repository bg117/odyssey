#pragma once

#include "../limine.h"
#include "../low_level/rsdp.hpp"

#include <cstdint>

constexpr uint64_t PAGE_SIZE = 0x1000; // 4 KiB

template <typename InfoT = std::nullptr_t> struct physical_memory_region
{
  uintptr_t location;
  uint64_t size;
  InfoT info;
};

namespace kernel
{
struct info
{
  struct
  {
    limine_memmap_entry **map;
    uint64_t count;
  } memory_map;

  physical_memory_region<> kernel;
  physical_memory_region<> stack;
  physical_memory_region<limine_framebuffer> framebuffer;
  physical_memory_region<> bitmap;
  physical_memory_region<low_level::rsdp> rsdp;

  uint64_t higher_half_direct_offset;
  uint64_t higher_half_kernel_offset;
};
} // namespace kernel
