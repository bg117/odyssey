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

#include <stdbool.h>
#include <stddef.h>

// 10x20 font
static unsigned int TERM_WIDTH;
static unsigned int TERM_HEIGHT;

extern struct limine_framebuffer *FRAMEBUFFER;

static struct psf_header *font;

static unsigned int current_x = 0;
static unsigned int current_y = 0;

void terminal_init(void *font_addr)
{
  font = (struct psf_header *)font_addr;

  current_x = 0;
  current_y = 0;

  TERM_WIDTH  = FRAMEBUFFER->width / font->width;
  TERM_HEIGHT = FRAMEBUFFER->height / font->height;
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
  uint32_t bytes_per_row = ((font->width + 7) & ~7) / 8;

  // get glyph offset
  uint64_t glyph_offset = font->header_size + c * font->bytes_per_glyph;

  // get glyph data
  uint8_t *glyph = (uint8_t *)font + glyph_offset;
  int off =
      current_y * font->height * FRAMEBUFFER->pitch + current_x * font->width * 4;
  int line;

  for (uint32_t y = 0; y < font->height; y++)
  {
    line = off;

    for (uint32_t x = 0; x < font->width; x++)
    {
      // set pixel; 0xFFFFFF for white, 0x000000 for black
      *(uint32_t *)(FRAMEBUFFER->address + line) =
          glyph[x / 8] & (0x80 >> (x & 7)) ? 0xffffff : 0x000000;
      line += 4;
    }

    glyph += bytes_per_row;
    off += FRAMEBUFFER->pitch;
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
