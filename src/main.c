/**
 * @file main.c
 * @author bg117 (bg117@sooosilly.xyz)
 * @brief Kernel initialization
 * @version 0.1
 * @date 2023-06-03
 *
 * This file contains the code needed for initializing the Odyssey operating
 * system.
 */

#include "gdt.h"
#include "limine.h"
#include "pmm.h"
#include "terminal.h"

#include <stddef.h>
#include <stdint.h>

struct gdt gdt[] = {
    {0, 0, 0, 0, 0, 0, 0},
    {0xffff, 0, 0, 0x9a, 0, 0x8, 0},
    {0xffff, 0, 0, 0x92, 0, 0x8, 0},
    {0xffff, 0, 0, 0x9a, 0xf, FLAGS_GRANULARITY | FLAGS_SIZE, 0},
    {0xffff, 0, 0, 0x92, 0xf, FLAGS_GRANULARITY | FLAGS_SIZE, 0},
    {0, 0, 0, 0x9a, 0x2, 0xa, 0},
    {0, 0, 0, 0x92, 0, 0xa, 0},
    {0, 0, 0, 0xf2, 0, 0, 0},
    {0, 0, 0, 0xfa, 0, 2, 0},
};
struct gdtr gdtr;

void odyssey(struct limine_framebuffer *fb, struct limine_memmap_entry **mmap,
             size_t mmap_count, uint64_t hh_offset)
{
  terminal_init(fb);
  terminal_printf("Initializing Odyssey kernel...\n");
  terminal_printf("==============================\n");

  // initialize GDT
  gdt_make_descriptor(&gdtr, gdt, sizeof(gdt) / sizeof(struct gdt));
  gdt_load(&gdtr, 0x28, 0x30);

  // initialize PMM
  pmm_init(mmap, mmap_count);
}