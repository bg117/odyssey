#include "low_level/idt.hpp"

namespace low_level
{
namespace idt
{
void make_descriptor(descriptor &desc, entry *idt, const uint16_t size)
{
  desc.limit = sizeof(entry) * size - 1;
  desc.base  = idt;
}

void load(descriptor &desc)
{
  asm volatile("lidt [%0]\n"
               "sti\n" ::"r"(&desc)
               : "memory");
}
} // namespace idt
} // namespace low_level
