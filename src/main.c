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

#include "limine.h"
#include "terminal.h"

#include <stddef.h>
#include <stdint.h>

void odyssey(struct limine_framebuffer *fb, struct limine_memmap_entry **mmap, size_t mmap_count)
{
    terminal_init(fb);
    terminal_printf("Initializing Odyssey kernel...\n");
    terminal_printf("==============================\n");
    terminal_printf("Framebuffer: \t%dx%d\n", fb->width, fb->height);
    terminal_printf("Memory map:\n");
    for (size_t i = 0; i < mmap_count; i++)
    {
        terminal_printf("\t\t%08x - %08x:\t%s\n", mmap[i]->base, mmap[i]->base + mmap[i]->length, mmap[i]->type == 1 ? "Usable" : "Reserved");
    }
}