#ifndef ODYSSEY_GDT_H
#define ODYSSEY_GDT_H

#include <stdint.h>

struct gdt
{
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t base_middle;
  enum
  {
    ACCESS_PRESENT          = 1 << 7,
    ACCESS_PRIVILEGE_KERNEL = 0 << 5,
    ACCESS_PRIVILEGE_USER   = 3 << 5,
    ACCESS_CODE_DATA        = 1 << 4,
    ACCESS_EXECUTABLE       = 1 << 3,
    ACCESS_DIRECTION        = 1 << 2,
    ACCESS_READWRITE        = 1 << 1,
    ACCESS_ACCESSED         = 1 << 0
  } access : 8;
  uint8_t limit_high : 4;
  enum
  {
    FLAGS_GRANULARITY = 1 << 3,
    FLAGS_SIZE        = 1 << 2,
    FLAGS_LONG        = 1 << 1
  } flags : 4;
  uint8_t base_high;
} __attribute__((packed));

struct gdtr
{
  uint16_t limit;
  uint64_t base;
} __attribute__((packed));



void gdt_load(struct gdtr *gdtr, uint16_t code_selector, uint16_t data_selector);

#endif