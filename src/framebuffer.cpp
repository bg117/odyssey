#include "graphics/framebuffer.hpp"

#include "graphics/psf.hpp"
#include "kernel/info.hpp"
#include "misc/config.hpp"
#include "misc/convert.hpp"
#include "misc/types.hpp"

#include <cstdarg>
#include <cstring>

extern kernel::info INFO;
extern char FONT;

enum class printf_status
{
  normal,
  specifier
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
graphics::psf *font;
offset line, column;
limine_framebuffer *fb;
/* const */
offset MAX_LINES, MAX_COLUMNS;

// draw bare character
void print_bare(char c);
void printf_receive_arg_signed(printf_length len, std::va_list ap, char *buf,
                               int radix);
void printf_receive_arg_unsigned(printf_length len, std::va_list ap, char *buf,
                                 int radix);
void printf_pad(printf_pad_type pad, uint64_t len, char *buf);
} // namespace

namespace graphics
{
namespace framebuffer
{
void initialize()
{
  line = column = 0;
  fb            = &INFO.framebuffer.info;
  font          = reinterpret_cast<psf *>(&FONT);

  MAX_COLUMNS = fb->width / font->width;
  MAX_LINES   = fb->height / font->height;
}

void set_pixel(const offset x, const offset y, const uint32_t bpp32)
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
    column++;
    if (column >= MAX_COLUMNS)
    {
      column = 0;
      line++;
    }
    break;
  }
}

void print(const char *s)
{
  while (*s)
  {
    print(*s);
    s++;
  }
}

__attribute__((format(printf, 1, 2))) void printf(const char *fmt, ...)
{
  std::va_list ap;
  va_start(ap, fmt);

  int i       = 0;
  auto status = printf_status::normal;
  auto length = printf_length::normal;
  auto pad    = printf_pad_type::none;

  int pad_len = 0;

  char num[128];

  while (*fmt != '\0')
  {
    memset(num, 0, sizeof(num));
    if (*fmt == '%')
    {
      status = printf_status::specifier;
      ++fmt;
    }

    if (status == printf_status::specifier)
    {
      switch (*fmt)
      {
      case '0':
        pad = printf_pad_type::zero;
        ++fmt;
        while (*fmt >= '0' && *fmt <= '9')
        {
          pad_len *= 10;
          pad_len += *fmt - '0';
          ++fmt;
        }
        continue;
      case ' ':
        pad = printf_pad_type::space;
        ++fmt;
        while (*fmt >= '0' && *fmt <= '9')
        {
          pad_len *= 10;
          pad_len += *fmt - '0';
          ++fmt;
        }
        continue;
      case 'h':
        if (length == printf_length::normal)
          length = printf_length::h;
        else if (length == printf_length::h)
          length = printf_length::hh;
        ++fmt;
        continue;
      case 'l':
        if (length == printf_length::normal)
          length = printf_length::l;
        else if (length == printf_length::h)
          length = printf_length::ll;
        ++fmt;
        continue;
      case '%':
        graphics::framebuffer::print('%');
        break;
      case 's': {
        char *str = va_arg(ap, char *);
        printf_pad(pad, pad_len, str);
        graphics::framebuffer::print(str);
      }
      break;
      case 'i':
      case 'd':
        printf_receive_arg_signed(length, ap, num, 10);
        printf_pad(pad, pad_len, num);
        graphics::framebuffer::print(num);
        break;
      case 'u':
        printf_receive_arg_unsigned(length, ap, num, 10);
        printf_pad(pad, pad_len, num);
        graphics::framebuffer::print(num);
        break;
      case 'x':
        printf_receive_arg_unsigned(length, ap, num, 16);
        printf_pad(pad, pad_len, num);
        graphics::framebuffer::print(num);
        break;
      case 'X':
        printf_receive_arg_unsigned(length, ap, num, 16);
        printf_pad(pad, pad_len, num);
        graphics::framebuffer::print(convert::string_to_upper(num));
        break;
      case 'p':
        graphics::framebuffer::printf("0x");
        printf_receive_arg_unsigned(length, ap, num, 16);
        printf_pad(pad, pad_len, num);
        graphics::framebuffer::print(num);
        break;
      case 'o':
        printf_receive_arg_unsigned(length, ap, num, 16);
        printf_pad(pad, pad_len, num);
        graphics::framebuffer::print(num);
        break;
      default:
        graphics::framebuffer::print('%');
        graphics::framebuffer::print(*fmt);
        break;
      }
    }
    else
    {
      graphics::framebuffer::print(*fmt);
    }

    status  = printf_status::normal;
    length  = printf_length::normal;
    pad     = printf_pad_type::none;
    pad_len = 0;

    ++i;
    ++fmt;
  }

  va_end(ap);
}
} // namespace framebuffer
} // namespace graphics

namespace
{
void print_bare(const char c)
{
  offset bytes_per_row = ((font->width + 8 - 1) & -8) / 8;
  offset glyph_offset  = font->header_size + c * font->bytes_per_glyph;
  auto glyph           = reinterpret_cast<uint8_t *>(font) + glyph_offset;

  for (counter y = 0; y < font->height; y++)
  {
    auto row = glyph + y * bytes_per_row;

    for (counter x = 0; x < font->width; x++)
    {
      graphics::framebuffer::set_pixel(
          column * font->width + x, line * font->height + y,
          row[x / 8] & (0x80 >> (x & 7)) ? 0xFFFFFF : 0x0);
    }
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

void printf_pad(printf_pad_type pad, uint64_t len, char *buf)
{
  switch (pad)
  {
  case printf_pad_type::zero:
    for (int i = 0; i < len - strlen(buf); ++i)
    {
      graphics::framebuffer::print('0');
    }
    break;
  case printf_pad_type::space:
    for (int i = 0; i < len - strlen(buf); ++i)
    {
      graphics::framebuffer::print(' ');
    }
    break;
  }
}
} // namespace
