/**
 * @file main.c
 * @author bg117 (bg117@sooosilly.xyz)
 * @brief Kernel entry point
 * @version 0.1
 * @date 2023-06-03
 *
 * This file contains the entry point of the Odyssey operating
 * system kernel.
 */

#include "limine.h"

#include <stddef.h>

/* extern */
uint64_t KERNEL_PHYS;
uint64_t KERNEL_SIZE;
struct limine_framebuffer *FRAMEBUFFER;
uint64_t FRAMEBUFFER_SIZE;
uint64_t HIGHER_HALF_START;
struct limine_memmap_entry **MEMMAP;
uint64_t MEMMAP_COUNT;

void odyssey(void);

static volatile struct limine_framebuffer_request fb_request = {
    LIMINE_FRAMEBUFFER_REQUEST, 0};
static volatile struct limine_memmap_request mmap_request = {
    LIMINE_MEMMAP_REQUEST, 0};
static volatile struct limine_hhdm_request hhdm_request = {LIMINE_HHDM_REQUEST,
                                                           0};

__attribute__((noreturn)) static void halt(void)
{
  asm("cli");
  // in case of NMIs, put HLT in infinite loop
  while (1)
  {
    asm("hlt");
  }
}

__attribute__((noreturn)) void _start(void)
{
  if (fb_request.response == NULL || fb_request.response->framebuffer_count < 1)
  {
    halt();
  }

  // get framebuffer from response
  FRAMEBUFFER = fb_request.response->framebuffers[0];

  // get higher half start from response
  HIGHER_HALF_START = hhdm_request.response->offset;

  // get memory map from response
  MEMMAP       = mmap_request.response->entries;
  MEMMAP_COUNT = mmap_request.response->entry_count;

  // get start of kernel and framebuffer size
  for (uint64_t i = 0; i < MEMMAP_COUNT; i++)
  {
    switch (MEMMAP[i]->type)
    {
    case LIMINE_MEMMAP_KERNEL_AND_MODULES:
      KERNEL_PHYS = MEMMAP[i]->base;
      KERNEL_SIZE = MEMMAP[i]->length;
      break;
    case LIMINE_MEMMAP_FRAMEBUFFER:
      FRAMEBUFFER_SIZE = MEMMAP[i]->length;
      break;
    }
  }

  // call init
  odyssey();

  // die
  halt();
}