#pragma once

#include "../misc/types.hpp"

namespace graphics
{
namespace framebuffer
{
void initialize();
void set_pixel(offset x, offset y, dword bpp32);
dword get_pixel(offset x, offset y);

void print(char c);
void print(const char *s);
__attribute__((format(printf, 1, 2))) void printf(const char *fmt, ...);
} // namespace framebuffer
} // namespace graphics
