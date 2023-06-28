#include "low_level/gdt.hpp"

#include "misc/log.hpp"

namespace low_level
{
namespace gdt
{
void load(descriptor &desc, uint16_t code_selector, uint16_t data_selector)
{
  asm volatile("lgdt [%0]\n"  // load GDT descriptor to GDTR
               "mov ds, %1\n" // reload all data segment registers
               "mov es, %1\n"
               "mov fs, %1\n"
               "mov gs, %1\n"
               "mov ss, %1\n"
               "push %2\n"
               "push offset L1\n"
               "lretq\n" // reload code segment
               "L1:" ::"r"(&desc),
               "r"(data_selector), "r"(static_cast<uint64_t>(code_selector))
               : "rax");
  LOG("loaded %p to GDTR (CS=0x%hX, DS=ES=FS=GS=SS=0x%hX)", &desc,
      code_selector, data_selector);
}
} // namespace gdt

} // namespace low_level
