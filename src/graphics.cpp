#include "graphics.hpp"

#include "config.hpp"
#include "int.hpp"
#include "kernel_info.hpp"
#include "psf.hpp"
#include "types.hpp"

#include <cstdarg>

extern kernel_info INFO;
extern char FONT;

enum class printf_status
{
  normal,
  specifier,
  length
};

enum class printf_length
{
  normal,
  hh,
  h,
  ll,
  l
};

enum class printf_pad_type
{
  none,
  zero,
  space
};

namespace
{
offset line, column;
psf *font;
limine_framebuffer *fb;
/* const */
offset MAX_LINES, MAX_COLUMNS;

// draw bare character
void print_bare(char c);
void print_bare(const char *s);
void printf_receive_arg_signed(printf_length len, std::va_list ap, char *buf,
                               int radix);
void printf_receive_arg_unsigned(printf_length len, std::va_list ap, char *buf,
                                 int radix);
void printf_pad(printf_pad_type pad, size len);
} // namespace

namespace graphics
{
void initialize()
{
  line = column = 0;
  fb            = &INFO.framebuffer.info;
  font          = reinterpret_cast<psf *>(&FONT);

  MAX_LINES   = fb->width / font->width;
  MAX_COLUMNS = fb->height / font->height;
}

void set_pixel(const offset x, const offset y, const dword bpp32)
{
  auto loc   = reinterpret_cast<uint32_t *>(fb->address);
  offset off = y * (fb->pitch / (fb->bpp / 8)) + x;
  loc[off]   = bpp32;
}

void print(const char c)
{
  switch (c)
  {
  case '\r':
    column = 0;
    break;
  case '\n':
    line++;
    column = 0;
    break;
  case '\b':
    if (column > 0)
    {
      column--;
    }
    break;
  case '\t':
    column = (column + 8 - 1) & -8; // round to nearest 8
    if (column >= MAX_COLUMNS)
    {
      offset over = column - MAX_COLUMNS;
      column      = over;
      line++;
    }
    break;
  case '\0':
    break;
  default:
    print_bare(c);
    if (column >= MAX_COLUMNS)
    {
      column = 0;
      line++;
    }
    else
    {
      column++;
    }
    break;
  }
}

__attribute__((format(printf, 1, 2))) void print(const char *fmt, ...)
{
  print_bare(fmt);
}
} // namespace graphics

namespace
{
void print_bare(const char c)
{
  offset bytes_per_row = ((font->width + 8 - 1) & -8) / 8;
  offset glyph_offset  = font->header_size + c * font->bytes_per_glyph;
  auto glyph           = reinterpret_cast<byte *>(font) + glyph_offset;

  for (counter y = 0; y < font->height; y++)
  {
    auto row = glyph + y * bytes_per_row;

    for (counter x = 0; x < font->width; x++)
    {
      graphics::set_pixel(column * font->width + x, line * font->height + y,
                          row[x / 8] & (0x80 >> (x & 7)) ? 0xFFFFFF : 0x0);
    }
  }
}

void print_bare(const char *s)
{
  while (*s)
  {
    graphics::print(*s);
    s++;
  }
}

void printf_receive_arg_signed(printf_length len, std::va_list ap, char *buf,
                               int radix)
{
  switch (len)
  {
  case printf_length::hh:
    convert::int_to_string((char)va_arg(ap, int), buf, radix);
    break;
  case printf_length::h:
    convert::int_to_string((short)va_arg(ap, int), buf, radix);
    break;
  case printf_length::normal:
    convert::int_to_string(va_arg(ap, int), buf, radix);
    break;
  case printf_length::l:
    convert::int_to_string(va_arg(ap, long), buf, radix);
    break;
  case printf_length::ll:
    convert::int_to_string(va_arg(ap, long long), buf, radix);
    break;
  }
}

void printf_receive_arg_unsigned(printf_length len, std::va_list ap, char *buf,
                                 int radix)
{
  switch (len)
  {
  case printf_length::hh:
    convert::uint_to_string((unsigned char)va_arg(ap, unsigned int), buf,
                            radix);
    break;
  case printf_length::h:
    convert::uint_to_string((unsigned short)va_arg(ap, unsigned int), buf,
                            radix);
    break;
  case printf_length::normal:
    convert::uint_to_string(va_arg(ap, unsigned int), buf, radix);
    break;
  case printf_length::l:
    convert::uint_to_string(va_arg(ap, unsigned long), buf, radix);
    break;
  case printf_length::ll:
    convert::uint_to_string(va_arg(ap, unsigned long long), buf, radix);
    break;
  }
}

void printf_pad(printf_pad_type pad, size len)
{
  switch (pad)
  {
    
  }
  if (zero_pad)                                     \
    {                                                 \
      for (int i = 0; i < pad_len - strlen(buf); ++i) \
      {                                               \
        terminal_print_char('0');                     \
      }                                               \
    }
}
} // namespace
