/**
 * @file terminal.c
 * @author bg117 (bg117@sooosilly.xyz)
 * @brief terminal.h implementation
 * @version 0.1
 * @date 2023-06-03
 * 
 * This file contains the implementation for terminal.h.
 */

#include "terminal.h"
#include "psf.h"

#include <stddef.h>
#include <stdbool.h>

// 10x20 font
static int TERM_WIDTH;
static int TERM_HEIGHT;

static struct limine_framebuffer *term;
static struct psf_header *font;

static int current_x = 0, current_y = 0;

// embedded font
extern char _binary_src_10x20_psf_start, _binary_src_10x20_psf_end;

void terminal_init(struct limine_framebuffer *fb)
{
    term = fb;
    font = &_binary_src_10x20_psf_start;

    current_x = 0;
    current_y = 0;

    TERM_WIDTH = term->width / font->width;
    TERM_HEIGHT = term->height / font->height;
}

void terminal_print_char(char c)
{
    // do not print newline
    if (c == '\n')
    {
        current_x = 0;
        current_y += 1;
        return;
    }

    // carriage return, go to start of line
    if (c == '\r')
    {
        current_x = 0;
        return;
    }

    if (c == '\t')
    {
        current_x = (current_x + 8) & ~7; // align to 8
        if (current_x >= TERM_WIDTH)
        {
            current_x = 0;
            current_y += 1;
        }
        return;
    }

    // backspace
    if (c == '\b')
    {
        current_x -= 1;
        if (current_x < 0)
        {
            current_x = 0;
        }
        return;
    }

    // get bytes per row
    size_t bytes_per_row = ((font->width + 7) & ~7) / 8;

    // get glyph offset
    size_t glyph_offset = font->header_size + c * font->bytes_per_glyph;

    // get glyph data
    uint8_t *glyph = (uint8_t *)&_binary_src_10x20_psf_start + glyph_offset;
    int off = current_y * font->height * term->pitch + current_x * font->width * 4;
    int line;

    for (int y = 0; y < font->height; y++)
    {
        line = off;

        for (int x = 0; x < font->width; x++)
        {
            // set pixel; 0xFFFFFF for white, 0x000000 for black
            *(uint32_t *)(term->address + line) = glyph[x / 8] & (0x80 >> (x & 7)) ? 0xFFFFFF : 0x000000;
            line += 4;
        }

        glyph += bytes_per_row;
        off += term->pitch;
    }

    current_x += 1;
    if (current_x >= TERM_WIDTH)
    {
        current_x = 0;
        current_y += 1;
    }
}

void terminal_print_string(const char *str)
{
    while (*str)
    {
        terminal_print_char(*str++);
    }
}
