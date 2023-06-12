#pragma once

#include "types.hpp"

namespace graphics
{
void initialize();
void set_pixel(offset x, offset y, dword bpp32);
dword get_pixel(offset x, offset y);

void print(char c);
__attribute__((format(printf, 1, 2))) void print(const char *s, ...);
} // namespace graphics
