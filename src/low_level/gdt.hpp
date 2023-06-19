#pragma once

#include <cstdint>

namespace low_level
{
namespace gdt
{
struct entry
{
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t base_middle;
  uint8_t access : 8;
  uint8_t limit_high : 4;
  uint8_t flags : 4;
  uint8_t base_high;
} __attribute__((packed));

struct descriptor
{
  uint16_t limit;
  entry *base;
} __attribute__((packed));

void load(descriptor &desc, uint16_t code_selector, uint16_t data_selector);
} // namespace gdt

} // namespace low_level
