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

void odyssey(struct limine_framebuffer *fb, struct limine_memmap_entry **mmap,
             size_t mmap_count, uint64_t hh_offset)
{
  terminal_init(fb);
  terminal_printf("Initializing Odyssey kernel...\n");
  terminal_printf("==============================\n");
  terminal_printf("Framebuffer: \t%dx%d\n", fb->width, fb->height);
  terminal_printf("HHDM offset: \t%016llp\n", hh_offset);
  terminal_printf("Address of main: \t%016llp\n", &odyssey);
  terminal_printf("Usable memory:\n");

  for (size_t i = 0; i < mmap_count; i++)
  {
    const char *type = "Usable";
    switch (mmap[i]->type)
    {
    case LIMINE_MEMMAP_RESERVED:
      type = "Reserved";
      break;
    case LIMINE_MEMMAP_ACPI_RECLAIMABLE:
      type = "ACPI Reclaimable";
      break;
    case LIMINE_MEMMAP_ACPI_NVS:
      type = "ACPI NVS";
      break;
    case LIMINE_MEMMAP_BAD_MEMORY:
      type = "Bad memory";
      break;
    case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
      type = "Bootloader reclaimable";
      break;
    case LIMINE_MEMMAP_KERNEL_AND_MODULES:
      type = "Kernel and modules";
      break;
    case LIMINE_MEMMAP_FRAMEBUFFER:
      type = "Framebuffer";
      break;
    }

    terminal_printf("\t%016llp - %016llp (%016llp) %s\n", mmap[i]->base,
                    mmap[i]->base + mmap[i]->length - 1, mmap[i]->length, type);
  }

  // initialize GDT
  // initialize PMM
  pmm_init(mmap, mmap_count);

#if 1
  // PMM test
  void *p1 = pmm_alloc();
  void *p2 = pmm_alloc();

  int passed = p2 == p1 + 0x1000;
  terminal_printf("PMM test: 2: %016llp, 1: %016llp (%d)\n", p2, p1, passed);

  pmm_free(p1);
  void *p3 = pmm_alloc();

  passed &= p1 == p3;
  terminal_printf("PMM test: 1: %016llp, 3: %016llp (%d)\n", p1, p3, passed);

  void *p4 = pmm_alloc();

  passed &= p4 == p2 + 0x1000;
  terminal_printf("PMM test: 4: %016llp, 2: %016llp (%d)\n", p4, p2, passed);

  pmm_free(p2);
  pmm_free(p3);
  pmm_free(p4);

  if (passed)
  {
    terminal_printf("PMM test passed!\n");
  }
  else
  {
    terminal_printf("PMM test failed!\n");
  }
#endif
}