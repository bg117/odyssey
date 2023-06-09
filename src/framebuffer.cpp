#include "graphics/framebuffer.hpp"

#include "graphics/psf.hpp"
#include "kernel/info.hpp"
#include "misc/convert.hpp"
#include "misc/round.hpp"

#include <cstdarg>
#include <cstring>

extern kernel::info INFO;
extern char __fb_fonts_start, __fb_fonts_end;

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
graphics::psf font;
uint64_t line, column;
limine_framebuffer *fb;
/* const */
uint64_t MAX_LINES, MAX_COLUMNS;

// draw bare character
void print_bare(char c);
void printf_receive_arg_signed(printf_length len, std::va_list ap, char *buf,
                               int radix);
void printf_receive_arg_unsigned(printf_length len, std::va_list ap, char *buf,
                                 int radix);
void printf_pad(printf_pad_type pad, uint64_t len, const char *buf);

void scroll_line();
} // namespace

namespace graphics
{
namespace framebuffer
{
void initialize(const char *font_name)
{
  line = column = 0;
  fb            = &INFO.framebuffer.info;

  set_font(font_name);
}

void set_font(const char *font_name)
{
  font        = psf(font_name);
  MAX_COLUMNS = fb->width / font.width;
  MAX_LINES   = fb->height / font.height;
}

void set_pixel(const uint64_t x, const uint64_t y, const uint32_t bpp32)
{
  const auto loc     = static_cast<uint32_t *>(fb->address);
  const uint64_t off = y * fb->width + x;
  loc[off]           = bpp32;
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
    column = round::up(column, 8); // round to nearest 8
    if (column >= MAX_COLUMNS)
    {
      const uint64_t over = column - MAX_COLUMNS;
      column              = over;
      line++;
    }
    break;
  case '\0':
    break;
  default:
    print_bare(c);
    column++;
    break;
  }

  if (column >= MAX_COLUMNS)
  {
    column = 0;
    line++;
  }
  if (line >= MAX_LINES)
  {
    scroll_line();
    line--;
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
        print('%');
        break;
      case 's': {
        const char *str = va_arg(ap, char *);
        printf_pad(pad, pad_len, str);
        print(str);
      }
      break;
      case 'i':
      case 'd':
        printf_receive_arg_signed(length, ap, num, 10);
        printf_pad(pad, pad_len, num);
        print(num);
        break;
      case 'u':
        printf_receive_arg_unsigned(length, ap, num, 10);
        printf_pad(pad, pad_len, num);
        print(num);
        break;
      case 'x':
        printf_receive_arg_unsigned(length, ap, num, 16);
        printf_pad(pad, pad_len, num);
        print(num);
        break;
      case 'X':
        printf_receive_arg_unsigned(length, ap, num, 16);
        printf_pad(pad, pad_len, num);
        print(convert::string_to_upper(num));
        break;
      case 'p':
        printf("0x");
        printf_receive_arg_unsigned(printf_length::ll, ap, num, 16);
        printf_pad(printf_pad_type::zero, 16, num);
        print(convert::string_to_upper(num));
        break;
      case 'o':
        printf_receive_arg_unsigned(length, ap, num, 16);
        printf_pad(pad, pad_len, num);
        print(num);
        break;
      default:
        print('%');
        print(*fmt);
        break;
      }
    }
    else
    {
      print(*fmt);
    }

    status  = printf_status::normal;
    length  = printf_length::normal;
    pad     = printf_pad_type::none;
    pad_len = 0;

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
  const auto bytes_per_row = round::up(font.width, 8) / 8;
  const auto glyph_idx     = c * font.bytes_per_glyph;
  const auto glyph         = font.glyphs + glyph_idx;

  for (uint64_t y = 0; y < font.height; y++)
  {
    const auto row = reinterpret_cast<uint16_t *>(glyph + y * bytes_per_row);

    for (uint64_t x = 0; x < font.width; x++)
    {
      graphics::framebuffer::set_pixel(
          column * font.width + x, line * font.height + y,
          *row & 1 << (font.width - x - 1) ? 0xb0acac : 0x0);
    }
  }
}

void printf_receive_arg_signed(const printf_length len, std::va_list ap,
                               char *buf, const int radix)
{
  switch (len)
  {
  case printf_length::hh:
    convert::int_to_string(static_cast<char>(va_arg(ap, int)), buf, radix);
    break;
  case printf_length::h:
    convert::int_to_string(static_cast<short>(va_arg(ap, int)), buf, radix);
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

void printf_receive_arg_unsigned(const printf_length len, std::va_list ap,
                                 char *buf, const int radix)
{
  switch (len)
  {
  case printf_length::hh:
    convert::uint_to_string(
        static_cast<unsigned char>(va_arg(ap, unsigned int)), buf, radix);
    break;
  case printf_length::h:
    convert::uint_to_string(
        static_cast<unsigned short>(va_arg(ap, unsigned int)), buf, radix);
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

void printf_pad(const printf_pad_type pad, const uint64_t len, const char *buf)
{
  if (len == 0)
  {
    return;
  }

  for (size_t i = 0; i < len - strlen(buf); i++)
  {
    switch (pad)
    {
    case printf_pad_type::zero:
      graphics::framebuffer::print('0');
      break;
    case printf_pad_type::space:
      graphics::framebuffer::print(' ');
      break;
    default:
      break;
    }
  }
}

void scroll_line()
{
  // get how many pixels for one row (width of font * MAX_COLUMNS * height of
  // font)
  const auto row_size    = fb->pitch * font.height;
  const auto fb_size     = fb->pitch * fb->height;
  const auto address_chr = static_cast<uint8_t *>(fb->address);
  // memmove
  memmove(address_chr, address_chr + row_size, fb_size - row_size);
  // clear current line
  memset(address_chr + fb_size - row_size, 0, row_size);
}
} // namespace
