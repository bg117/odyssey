#include "gdt.h"

struct gdt gdt_make_entry(uint32_t base, uint32_t limit, uint8_t access,
                          uint8_t flags)
{
  struct gdt entry = {
      .limit_low   = limit & 0xFFFF,
      .base_low    = base & 0xFFFF,
      .base_middle = (base >> 16) & 0xFF,
      .access      = access,
      .limit_high  = (limit >> 16) & 0xF,
      .flags       = flags,
      .base_high   = (base >> 24) & 0xFF,
  };

  return entry;
}

void gdt_make_descriptor(struct gdtr *gdtr, struct gdt *gdt, uint16_t size)
{
  gdtr->limit = sizeof(struct gdt) * size - 1;
  gdtr->base  = (uint64_t)gdt;
}

void gdt_load(struct gdtr *gdtr, uint16_t code_selector,
                                     uint16_t data_selector)
{
  asm volatile("lgdt (%0);"
               "movw %1, %%ax;"
               "movw %%ax, %%ds;"
               "movw %%ax, %%es;"
               "movw %%ax, %%fs;"
               "movw %%ax, %%gs;"
               "movw %%ax, %%ss;"
               "xorq %%rax, %%rax;"
               "movw %2, %%ax;"
               "push %%rax;"
               "push $1f;"
               "lretq;"
               "1:" ::"r"(gdtr),
               "r"(data_selector), "r"(code_selector)
               : "rax");
}