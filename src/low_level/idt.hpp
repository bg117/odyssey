#pragma once

#include <cstdint>

namespace low_level
{
namespace idt
{
struct __attribute__((packed)) entry
{
  uint16_t offset_low;
  uint16_t cs;
  uint8_t ist : 3;
  uint8_t reserved_0 : 5;
  uint8_t type;
  uint64_t offset_hi : 48;
  uint32_t reserved_1;
};
struct __attribute__((packed)) descriptor
{
  uint16_t limit;
  entry *base;
};

void make_descriptor(descriptor &desc, entry *idt, uint16_t size);
void load(descriptor &desc);
} // namespace idt
} // namespace low_level
