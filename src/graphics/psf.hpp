#pragma once

#include <cstdint>

namespace graphics
{
struct __attribute__((packed)) psf
{
  uint32_t magic;
  uint32_t version;
  uint32_t header_size;
  uint32_t flags;
  uint32_t num_glyph;
  uint32_t bytes_per_glyph;
  uint32_t height;
  uint32_t width;
  uint8_t *glyphs;

  psf() = default;

  psf(const char *font_name);
};
} // namespace graphics