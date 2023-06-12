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

#define TERM_FONT FONT(6x12)

#include "gdt.h"
#include "limine.h"
#include "log.h"
#include "pmm.h"
#include "terminal.h"
#include "vmm.h"

#include <stddef.h>
#include <stdint.h>

#define FONT(s) _binary_fonts_##s##_psf_start

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

extern char TERM_FONT;
extern uint64_t KERNEL_PHYS, KERNEL_SIZE, MEMMAP_COUNT;
extern struct limine_memmap_entry **MEMMAP;

void odyssey(void)
{
  // initialize terminal
  terminal_init(&TERM_FONT);

  // initialize GDT
  LOG("Initializing GDT...");
  gdt_make_descriptor(&gdtr, gdt, sizeof(gdt) / sizeof(struct gdt));
  gdt_load(&gdtr, 0x28, 0x30);

  // initialize PMM
  LOG("Initializing PMM...");
  pmm_init();

  // initialize VMM
  LOG("Initializing VMM...");
  vmm_init();

  // test VMM
  vmm_alloc();
}