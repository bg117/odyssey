#pragma once

#include <cstdint>

namespace graphics
{
namespace framebuffer
{
void initialize(const char *font_name = "default");
void set_font(const char *font_name);
void set_pixel(uint64_t x, uint64_t y, uint32_t bpp32);
uint32_t get_pixel(uint64_t x, uint64_t y);

void print(char c);
void print(const char *s);
__attribute__((format(printf, 1, 2))) void printf(const char *fmt, ...);
} // namespace framebuffer
} // namespace graphics
