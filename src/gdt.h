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

/**
 * @brief Initialize a GDT descriptor using the given values.
 * 
 * @param gdtr The address of the GDT descriptor structure.
 * @param gdt An array of GDT entries, starting with an empty entry at index 0.
 * @param size The size of the GDT entry array.
 */
void gdt_make_descriptor(struct gdtr *gdtr, struct gdt *gdt, uint16_t size);

/**
 * @brief Load the GDT descriptor into the GDTR register.
 * 
 * @param gdtr The address of the GDT descriptor structure.
 * @param code_selector The offset to the code segment in the GDT.
 * @param data_selector The offset to the data segment in the GDT.
 */
void gdt_load(struct gdtr *gdtr, uint16_t code_selector, uint16_t data_selector);

#endif