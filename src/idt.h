#ifndef ODYSSEY_IDT_H
#define ODYSSEY_IDT_H

#include <stdint.h>

struct idt
{
  uint16_t isr_low;
  uint16_t code_selector;
  uint8_t ist : 3;
  uint8_t reserved_0 : 5;
  enum
  {
    IDT_PRESENT          = 1 << 7,
    IDT_PRIVILEGE_KERNEL = 0 << 5,
    IDT_PRIVILEGE_USER   = 3 << 5,
    IDT_GATE_INT         = 14,
    IDT_GATE_TRAP        = 15
  } attributes : 8;
  uint64_t isr_high : 48;
  uint32_t reserved_1;
} __attribute__((packed));

struct idtr
{
  uint16_t limit;
  uint64_t base;
} __attribute__((packed));

#endif