#include "graphics/psf.hpp"

#include <cstring>

extern char __fb_fonts_start, __fb_fonts_end;

namespace graphics
{
psf::psf(const char *font_name)
{
  auto current = &__fb_fonts_start; // .fb_fonts section start
  // search for font in fb_fonts
  bool found = false;
  while (!found && current < &__fb_fonts_end)
  {
    // if not matching
    if (memcmp(font_name, current, strlen(font_name)) != 0)
    {
      current += strlen(current) + 1; // null terminator
      // get size of "file"
      const auto size = *reinterpret_cast<uint64_t *>(current);
      current += size;
      continue;
    }

    // go to start of header
    found = true;
  }

  auto font   = reinterpret_cast<psf *>(current + strlen(font_name) + 1 + 8);
  auto glyphs = reinterpret_cast<uint8_t *>(font) + font->header_size;

  *this        = *font;
  this->glyphs = glyphs;
}
} // namespace graphics