#pragma once

#include <cstdint>

namespace memory
{
struct __attribute__((packed)) paging_structure_entry
{
  uint16_t flags : 9;
  uint8_t available_0 : 3; // store upper 3 bits of number of consecutive pages
  uint64_t base_addr : 40; // do not store lower 12 bits (all 0)
  uint16_t
      available_1 : 11; // store lower 15 bits of number of consecutive pages
  uint8_t no_execute : 1;
};
} // namespace memory
