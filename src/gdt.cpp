#include "low_level/gdt.hpp"

#include "misc/log.hpp"
#include "misc/types.hpp"

namespace low_level
{
namespace gdt
{
void make_descriptor(descriptor &desc, entry *gdt, uint16_t size)
{
  desc.limit = sizeof(entry) * size - 1;
  desc.base  = reinterpret_cast<virtual_address>(gdt);
  LOG("created GDT descriptor: limit=0x%hX, base=0x%016lX", desc.limit,
      desc.base);
}

void load(descriptor &desc, uint16_t code_selector, uint16_t data_selector)
{
  asm volatile("lgdt (%0);" // load GDT descriptor to GDTR
               "movw %1, %%ax;"
               "movw %%ax, %%ds;" // reload all data segment registers
               "movw %%ax, %%es;"
               "movw %%ax, %%fs;"
               "movw %%ax, %%gs;"
               "movw %%ax, %%ss;"
               "xorq %%rax, %%rax;"
               "movw %2, %%ax;"
               "push %%rax;"
               "push $1f;"
               "lretq;" // reload code segment
               "1:" ::"r"(&desc),
               "r"(data_selector), "r"(code_selector)
               : "rax");
  LOG("loaded %p to GDTR (CS=0x%hX, DS=ES=FS=GS=SS=0x%hX)", &desc,
      code_selector, data_selector);
}
} // namespace gdt

} // namespace low_level