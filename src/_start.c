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

void odyssey(struct limine_framebuffer *fb, struct limine_memmap_entry **mmap,
             uint64_t mmap_count, uint64_t hh_offset);

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
  struct limine_framebuffer *fb = fb_request.response->framebuffers[0];
  // call init
  odyssey(fb, mmap_request.response->entries,
          mmap_request.response->entry_count, hhdm_request.response->offset);

  // die
  halt();
}