#include "low_level/isr.hpp"

#include "misc/log.hpp"

extern void (*interrupt_stub_table[256])();

namespace
{
low_level::isr::handler handlers[256] = {};
low_level::idt::entry *idt;

void set_idt_entry_fields(uint8_t vector, void (*stub)(), uint16_t code_segment,
                          uint8_t attributes);
} // namespace

extern "C" void exception_handler(const low_level::isr::interrupt_info *int_info)
{
  if (handlers[int_info->vector] != nullptr)
  {
    handlers[int_info->vector](int_info);
  }
}

namespace low_level
{
namespace isr
{
void initialize(idt::entry idt[256])
{
  ::idt = idt;
  for (counter i = 0; i < 256; i++)
  {
    const uint8_t attributes = i == 3 || i == 4 ? 0x8F : 0x8E;
    set_idt_entry_fields(i, interrupt_stub_table[i], 0x28, attributes);
  }
}

void set_handler(const uint8_t vector, const handler fn)
{
  handlers[vector] = fn;
}
} // namespace isr
} // namespace low_level

namespace
{
void set_idt_entry_fields(const uint8_t vector, void (*stub)(), const uint16_t code_segment,
                          const uint8_t attributes)
{
  const auto addr = reinterpret_cast<virtual_address>(stub);
  const auto ent  = &idt[vector];
  ent->offset_low = addr & 0xFFFF;
  ent->offset_hi  = addr >> 16;
  ent->cs         = code_segment;
  ent->type       = attributes;
}
} // namespace
